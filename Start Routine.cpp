#include "pch.h"

#include "Start Routine.h"
#pragma comment(lib, "wtsapi32.lib")

DWORD SR_NtCreateThreadEx	(HANDLE hTargetProc, f_Routine * pRoutine, void * pArg, DWORD & LastWin32Error, bool CloakThread,		ULONG_PTR & Out);


DWORD StartRoutine(HANDLE hTargetProc, f_Routine * pRoutine, void * pArg, LAUNCH_METHOD Method, bool CloakThread,  DWORD & LastWin32Error, ULONG_PTR & Out)
{
	DWORD Ret = 0;
	Ret = SR_NtCreateThreadEx(hTargetProc, pRoutine, pArg, LastWin32Error, CloakThread, Out);
	
	return Ret;
}

DWORD SR_NtCreateThreadEx(HANDLE hTargetProc, f_Routine * pRoutine, void * pArg, DWORD & LastWin32Error, bool CloakThread, ULONG_PTR & Out)
{
	auto h_nt_dll = GetModuleHandleA("ntdll.dll");
	if (!h_nt_dll)
	{
		LastWin32Error = GetLastError();

		return SR_NTCTE_ERR_GET_MODULE_HANDLE_FAIL;
	}

	auto p_NtCreateThreadEx = ReCa<f_NtCreateThreadEx>(GetProcAddress(h_nt_dll, "NtCreateThreadEx"));
	if (!p_NtCreateThreadEx)
	{
		LastWin32Error = GetLastError();

		return SR_NTCTE_ERR_NTCTE_MISSING;
	}

	void * pEntrypoint = nullptr;

	DWORD Flags		= THREAD_CREATE_FLAGS_CREATE_SUSPENDED | THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER;
	HANDLE hThread	= nullptr;


	
	NTSTATUS ntRet = p_NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, nullptr, hTargetProc, CloakThread ? pEntrypoint : pRoutine, pArg, CloakThread ? Flags : NULL, 0, 0, 0, nullptr);
	
	if (NT_FAIL(ntRet) || !hThread)
	{
		LastWin32Error = ntRet;
		
		return SR_NTCTE_ERR_NTCTE_FAIL;
	}
	
	DWORD dwWaitRet = WaitForSingleObject(hThread, 2000);
	if (dwWaitRet != WAIT_OBJECT_0)
	{
		LastWin32Error = GetLastError();

		TerminateThread(hThread, 0);
		CloseHandle(hThread);

		return SR_NTCTE_ERR_TIMEOUT;
	}

	DWORD dwOut = 0;
	BOOL bRet = GetExitCodeThread(hThread, &dwOut);
	if (!bRet)
	{
		LastWin32Error = GetLastError();
		
		CloseHandle(hThread);

		return SR_NTCTE_ERR_GECT_FAIL;
	}

	Out = dwOut;

	CloseHandle(hThread);

	return SR_ERR_SUCCESS;
}

