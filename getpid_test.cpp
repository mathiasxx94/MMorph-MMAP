#include <iostream>
#include "sigscanner.h"
#include "Injection.h"

HINSTANCE g_hInjMod = NULL;

CHAR path[MAX_PATH*2];
WCHAR dllpath[MAX_PATH * 2];

/*
#define $$$ __asm\
{\
mov al, 2\
}
*/

int main()
{
	/*
	__asm {
		mov al, 2
		mov dx, 0xD007
		out dx, al
	}
	$$$
	$$$
	$$$
	*/

	GetProcessId((L"csgo.exe")); // endre til csgo.exe
	//std::cout << TargetId;

	GetModuleFileName(GetModuleHandle(NULL), dllpath, sizeof(dllpath));
	std::wstring path2(dllpath);
	path2.erase(path2.find_last_of(L"\\")+1);
	path2.append(L"Osiris.dll");

	INJECTIONDATAW winjeksjonsdata;

	wcscpy_s(winjeksjonsdata.szDllPath, path2.c_str());
	winjeksjonsdata.ProcessID = TargetId;
	winjeksjonsdata.Method = LM_NtCreateThreadEx;
	winjeksjonsdata.Mode = IM_ManualMap;
	winjeksjonsdata.Flags = INJ_ERASE_HEADER;

	InjectW(&winjeksjonsdata);
}

