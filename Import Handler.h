#pragma once

#include "Tools.h"

HINSTANCE GetModuleHandleEx(HANDLE hTargetProc, const TCHAR * lpModuleName);
//Uses CreateToolHelp32Snapshot and Module32First/Next to retrieve the baseaddress of an image.
//
//Arguments:
//		hTargetProc (HANDLE):
///			A handle to the target process with either PROCESS_QUERY_INFORMATION or PROCESS_QUERY_LIMITED_INFORMATION.
//		szModuleName (const TCHAR*):
///			The name of the module including the file extension.
//
//Returnvalue (HINSTANCE):
///		On success: base address of the image.
///		On failure: NULL.

bool GetProcAddressEx(HANDLE hTargetProc, const TCHAR * szModuleName, const char * szProcName, void * &pOut);
//A function which tries to get the address of a function by parsing the export directory of the specified module.
//
//Arguments:
//		hTargetProc (HANDLE):
///			A handle to the target process with either PROCESS_QUERY_INFORMATION or PROCESS_QUERY_LIMITED_INFORMATION.
//		szModuleName (const TCHAR*):
///			The name of the module including the file extension.
//		szProcName (const char*):
///			The name of the function as an ansi string.
//		pOut (void*&):
///			A reference to a pointer that (on success) will contain the pointer to the function in the specified target process.
//
//Returnvalue (bool):
///		true:	pOut now contains the address of the function.
///		false:	something went wrong.

bool GetProcAddressEx(HANDLE hTargetProc, HINSTANCE hModule, const char * szProcName, void * &pOut);
//Same as other GetProcAddressEx overload but modulebase provided instead of modulename. For performance boost only.