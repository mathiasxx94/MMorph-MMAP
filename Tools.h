#pragma once

#include "NT Stuff.h"
#include "Error.h"

//reinterpret_cast = too long to type
#define ReCa reinterpret_cast

#define MPTR(d) (void*)(ULONG_PTR)d
//Macro to convert 32-bit DWORD into void*.

#define MDWD(p) (DWORD)((ULONG_PTR)p & 0xFFFFFFFF)
//Macro to convert void* into 32-bit DWORD.

//Global variable to store the base address of the current image of the injector. Initialized in DllMain.
extern HINSTANCE g_hInjMod;

UINT __forceinline _strlenA(const char * szString)
{
	UINT Ret = 0;
	if (!szString) return Ret;
	for (; *szString++; Ret++);
	return Ret;
}
//Simple function to determine the length of a string. It gets mapped into memory which is why __forceinline is required.
//
//Arguments:
//		szString (const char*)
///			A pointer to an ansi string.
//
//Returnvalue (UINT):
///		The amount of characters in the string

void __forceinline _ZeroMemory(BYTE * pData, UINT Len)
{
	while (Len--)
		*pData++ = 0;
}

void __forceinline _ZeroMemory2(BYTE * pData, UINT Len)
{
	for (UINT i = 0; i != Len; ++i)
	{
		pData[i] = 0;
	}
}
//Simple function to zero memoryg. It gets mapped into memory which is why __forceinline is required.
//
//Arguments:
//		pData (BYTE*):
///			Pointer to the data to zero.
//		Len (UINT):
///			Amount of bytes to overwrite.
//
//Returnvalue (void)

struct ERROR_INFO
	//A structure used to pass information to the error log function.
{
	const wchar_t *	szDllFileName;
	const wchar_t * szTargetProcessExeFileName;
	DWORD			TargetProcessId;
	DWORD			InjectionMode;
	DWORD			LaunchMethod;
	DWORD			Flags;
	DWORD			ErrorCode;
	DWORD			LastWin32Error;
	DWORD			HandleValue;
	bool			bNative;
};

bool FileExists(const wchar_t * szFile);
//A function to quickly check whether a file exists or not.
//
//Arguments:
//		szFile (const wchar_t*):
///			Pointer to a string containing the full path to the file.
//
//Returnvalue (bool):
///		true:	the file exists.
///		false:	the file doesn't exist.


bool GetOwnModulePath(wchar_t * pOut, size_t BufferCchSize);
//A function to get the filepath to the file of this image of the injector.
//
//Arguments:
//		pOut (wchar_t*):
///			Pointer to a widechar buffer to contain the full path.
//		BufferCchSize (size_t):
///			Size of the buffer in characters.
//
//Returnvalue (bool):
///		true:	pOut now contains the path.
///		false:	error enumerating the modules.


void ErrorLog(ERROR_INFO * info);
//A function used to generate an error log file in case shit hit the fan for some reason.
//
//Arguments:
//		info (ERROR_INFO*):
///			A pointer to an ERROR_INFO structure which contains information about what went wrong.
//
//Returnvalue (void)