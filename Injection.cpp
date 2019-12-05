#include "pch.h"

#include "Injection.h"
#pragma comment (lib, "Psapi.lib")

DWORD InitErrorStruct(const wchar_t* szDllPath, INJECTIONDATAW* pData, bool bNative, DWORD RetVal);

DWORD InjectDLL(const wchar_t* szDllFile, HANDLE hTargetProc, INJECTION_MODE im, LAUNCH_METHOD Method, DWORD Flags, DWORD& LastError, HINSTANCE& hOut);

DWORD InitErrorStruct(const wchar_t* szDllPath, INJECTIONDATAW* pData, bool bNative, DWORD RetVal)
{
	if (!RetVal)
		return INJ_ERR_SUCCESS;

	ERROR_INFO info{ 0 };
	info.szDllFileName = szDllPath;
	info.szTargetProcessExeFileName = pData->szTargetProcessExeFileName;
	info.TargetProcessId = pData->ProcessID;
	info.InjectionMode = pData->Mode;
	info.LaunchMethod = pData->Method;
	info.Flags = pData->Flags;
	info.ErrorCode = RetVal;
	info.LastWin32Error = pData->LastErrorCode;
	info.HandleValue = pData->hHandleValue;
	info.bNative = bNative;

	//ErrorLog(&info);

	return RetVal;
}

DWORD __stdcall InjectA(INJECTIONDATAA* pData)
{
	if (!pData->szDllPath)
		return InitErrorStruct(nullptr, ReCa<INJECTIONDATAW*>(pData), false, INJ_ERR_INVALID_FILEPATH);
	
	INJECTIONDATAW data{ 0 };
	size_t len_out = 0;
	size_t max_len = sizeof(data.szDllPath) / sizeof(wchar_t);
	StringCchLengthA(pData->szDllPath, max_len, &len_out);
	mbstowcs_s(&len_out, const_cast<wchar_t*>(data.szDllPath), max_len, pData->szDllPath, max_len);

	data.ProcessID = pData->ProcessID;
	data.Mode = pData->Mode;
	data.Method = pData->Method;
	data.Flags = pData->Flags;
	data.hHandleValue = pData->hHandleValue;

	return InjectW(&data);
}

DWORD InjectW(INJECTIONDATAW* pData)
{
	DWORD ErrOut = 0;

	if (!pData->szDllPath)
		return InitErrorStruct(nullptr, pData, false, INJ_ERR_INVALID_FILEPATH);

	const wchar_t* szDllPath = pData->szDllPath;

	if (!pData->ProcessID)
		return InitErrorStruct(szDllPath, pData, false, INJ_ERR_INVALID_PID);

	if (!FileExists(szDllPath))
	{
		pData->LastErrorCode = GetLastError();

		return InitErrorStruct(szDllPath, pData, false, INJ_ERR_FILE_DOESNT_EXIST);
	}

	HANDLE hTargetProc = nullptr;
	DWORD access_mask = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION;
	//if (pData->Method == LM_NtCreateThreadEx)
	access_mask |= PROCESS_CREATE_THREAD;

	hTargetProc = OpenProcess(access_mask, FALSE, pData->ProcessID);
	if (!hTargetProc)
	{
		pData->LastErrorCode = GetLastError();

		return InitErrorStruct(szDllPath, pData, false, INJ_ERR_CANT_OPEN_PROCESS);
	}


	DWORD handle_info = 0;
	if (!hTargetProc || !GetHandleInformation(hTargetProc, &handle_info))
	{
		pData->LastErrorCode = GetLastError();

		return InitErrorStruct(szDllPath, pData, false, INJ_ERR_INVALID_PROC_HANDLE);
	}

	wchar_t target_exe_name[MAX_PATH]{ 0 };
	if (!K32GetModuleBaseNameW(hTargetProc, NULL, target_exe_name, sizeof(target_exe_name) / sizeof(target_exe_name[0])))
	{
		pData->szTargetProcessExeFileName = nullptr;
	}
	else
	{
		pData->szTargetProcessExeFileName = target_exe_name;
	}

	bool native_target = true;
	

	HINSTANCE hOut = NULL;
	DWORD RetVal = INJ_ERR_SUCCESS;

	RetVal = InjectDLL(szDllPath, hTargetProc, pData->Mode, pData->Method, pData->Flags, ErrOut, hOut);

	if (!(pData->Flags & INJ_HIJACK_HANDLE))
		CloseHandle(hTargetProc);

	pData->LastErrorCode = ErrOut;
	pData->hDllOut = hOut;

	return InitErrorStruct(szDllPath, pData, native_target, RetVal);
}

DWORD InjectDLL(const wchar_t* szDllFile, HANDLE hTargetProc, INJECTION_MODE im, LAUNCH_METHOD Method, DWORD Flags, DWORD& LastError, HINSTANCE& hOut)
{
	DWORD Ret = 0;

	Ret = _ManualMap(szDllFile, hTargetProc, Method, Flags, hOut, LastError);

	if (Ret != INJ_ERR_SUCCESS)
	{
		return Ret;
	}

	if (!hOut)
	{
		return INJ_ERR_REMOTE_CODE_FAILED;
	}

	return Ret;
}