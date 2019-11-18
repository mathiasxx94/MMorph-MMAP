#include <iostream>
#include "sigscanner.h"
#include "Injection.h"

HINSTANCE g_hInjMod = NULL;

WCHAR path[MAX_PATH*2];

int main()
{
	
	GetProcessId((L"csgo.exe")); // endre til csgo.exe
	//std::cout << TargetId;

	GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
	//std::cout << path;

	INJECTIONDATAA injeksjonsdata;
	strcpy_s(injeksjonsdata.szDllPath, "C:\\Users\\mathi\\Desktop\\Osiris-master\\Osiris-master\\Release\\Osiris.dll");
	injeksjonsdata.ProcessID = TargetId;
	injeksjonsdata.Method = LM_NtCreateThreadEx;
	injeksjonsdata.Mode = IM_ManualMap;
	injeksjonsdata.Flags = INJ_ERASE_HEADER;

	InjectA(&injeksjonsdata);

	std::cout << injeksjonsdata.LastErrorCode;
	Sleep(10000);
}

