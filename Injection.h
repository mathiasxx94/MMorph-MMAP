#pragma once

#include "Start Routine.h"
#include "Tools.h"

enum INJECTION_MODE
{
	IM_LoadLibrary,
	IM_LdrLoadDll,
	IM_ManualMap
};
//Enum to define the injection mode.

#define INJ_ERASE_HEADER				0x0001
#define INJ_FAKE_HEADER					0x0002
#define INJ_UNLINK_FROM_PEB				0x0004
#define INJ_SHIFT_MODULE				0x0008
#define INJ_CLEAN_DATA_DIR				0x0010
#define INJ_THREAD_CREATE_CLOAKED		0x0020
#define INJ_SCRAMBLE_DLL_NAME			0x0040
#define INJ_LOAD_DLL_COPY				0x0080
#define INJ_HIJACK_HANDLE				0x0100

#define INJ_MAX_FLAGS 0x01FF

//ansi version of the info structure:
struct INJECTIONDATAA
{
	DWORD			LastErrorCode;								//used to store the error code of the injection 
	char			szDllPath[MAX_PATH * 2];					//fullpath to the dll to inject
	DWORD			ProcessID;									//process identifier of the target process
	INJECTION_MODE	Mode;										//injection mode
	LAUNCH_METHOD	Method;										//method to execute the remote shellcode
	DWORD			Flags;										//combination of the flags defined above
	DWORD			hHandleValue;								//optional value to identify a handle in a process
	HINSTANCE		hDllOut;									//returned image base of the injection
};


//unicode version of the info structure (documentation above).
//the additional member szTargetProcessExeFileName should be ignored since it's only used for error logging.
struct INJECTIONDATAW
{
	DWORD			LastErrorCode;
	wchar_t			szDllPath[MAX_PATH * 2];
	wchar_t* szTargetProcessExeFileName;	//exe name of the target process, this value gets set automatically and should be ignored
	DWORD			ProcessID;
	INJECTION_MODE	Mode;
	LAUNCH_METHOD	Method;
	DWORD			Flags;
	DWORD			hHandleValue;
	HINSTANCE		hDllOut;
};

DWORD __stdcall InjectA(INJECTIONDATAA* pData);
DWORD InjectW(INJECTIONDATAW* pData);
//Main injection functions (ansi/unicode).
//
//Arguments:
//		pData (INJECTIONDATAA/INJECTIONDATAW):
///			Pointer to the information for the injection.
//
//Returnvalue (DWORD):
///		On success: INJ_ERR_SUCCESS.
///		On failure: One of the errorcodes defined in Error.h.


DWORD _ManualMap(const wchar_t* szDllFile, HANDLE hTargetProc, LAUNCH_METHOD Method, DWORD Flags, HINSTANCE& hOut, DWORD& LastError);
//Injection methods called by InjectA/InjectW -> InjectDll


#define RELOC_FLAG86(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_HIGHLOW)
#define RELOC_FLAG64(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)
#ifdef _WIN64
#define RELOC_FLAG RELOC_FLAG64
#else
#define RELOC_FLAG RELOC_FLAG86
#endif
//Used for internal stuff when manually mapping.

#define ALIGN_UP(X, A) (X + (A - 1)) & (~(A - 1))
#define ALIGN_IMAGE_BASE_X64(Base) ALIGN_UP(Base, 0x10)
#define ALIGN_IMAGE_BASE_X86(Base) ALIGN_UP(Base, 0x08)
#ifdef _WIN64 
#define ALIGN_IMAGE_BASE(Base) ALIGN_IMAGE_BASE_X64(Base)
#else
#define ALIGN_IMAGE_BASE(Base) ALIGN_IMAGE_BASE_X86(Base)
#endif
//Used for internal stuff when manually mapping.

#define MAXPATH_IN_TCHAR	MAX_PATH
#define MAXPATH_IN_BYTE_A	MAX_PATH * sizeof(char)
#define MAXPATH_IN_BYTE_W	MAX_PATH * sizeof(wchar_t)
#define MAXPATH_IN_BYTE		MAX_PATH * sizeof(TCHAR)
//Used for internal usage.

using f_LoadLibraryExW = decltype(LoadLibraryExW);
using f_LoadLibraryA = decltype(LoadLibraryA);
using f_GetProcAddress = ULONG_PTR(WINAPI*)(HINSTANCE hModule, const char* lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwReason, void* pReserved);
using f_VirtualAlloc = decltype(VirtualAlloc);
using f_VirtualFree = decltype(VirtualFree);
//Function prototypes used when manually mapping.

struct MANUAL_MAPPING_DATA
{
	HINSTANCE				hRet;
	f_LoadLibraryA* pLoadLibraryA;
	f_GetProcAddress		pGetProcAddress;
	BYTE* pModuleBase;
	DWORD					Flags;
};
