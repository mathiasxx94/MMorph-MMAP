#pragma once

#ifndef _WIN32
#error Rly?
#endif

#include "pch.h"

#ifndef NT_FAIL
	#define NT_FAIL(status) (status < 0)
#endif

#ifndef NT_SUCCESS
	#define NT_SUCCESS(status) (status >= 0)
#endif

#define THREAD_CREATE_FLAGS_CREATE_SUSPENDED	0x00000001
#define THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH  0x00000002
#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER	0x00000004

#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

struct UNICODE_STRING
{
	WORD		Length;
	WORD		MaxLength;
	wchar_t *	szBuffer;
};

using f_NtCreateThreadEx			= NTSTATUS	(__stdcall*)(HANDLE * pHandle, ACCESS_MASK DesiredAccess, void * pAttr, HANDLE hTargetProc, void * pFunc, void * pArg,
										ULONG Flags, SIZE_T ZeroBits, SIZE_T StackSize, SIZE_T MaxStackSize, void * pAttrListOut);
