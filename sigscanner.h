#pragma once
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>


DWORD TargetId;
DWORD GetProcessId(const wchar_t* processName)
{
	time_t now = time(0);

	while (!TargetId){
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32W entry; 
		entry.dwSize = sizeof(entry);

		do
			if (!lstrcmp(entry.szExeFile, processName)) {
				TargetId = entry.th32ProcessID;
				CloseHandle(handle);
				return TargetId;
			}
		while (Process32NextW(handle, &entry));
		
		Sleep(1000);
		time_t end = time(0);
		if ((end - now) > 300) 
		{
			break;
		}
	}
	exit(0);
}