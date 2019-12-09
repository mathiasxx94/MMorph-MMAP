#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <signal.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	using std::wstring;
	//std::string dllname = "Osiris.dll";
	unsigned char dllname[] = {0x0,0x1b,0xc,0x1b,0x6,0x1b,0x4b,0xd,0x3,0x4}; //55 xor'ed with ascii(e) is 0 termination char.
	std::string nokkel = "ohei";

	std::vector<char> vectordllname(10);
	for (int i = 0; i < vectordllname.size(); i++) vectordllname[i] = ("%c",dllname[i]);

	for (int i = 0; i < sizeof(dllname); i++)
		vectordllname[i] ^= nokkel[i % nokkel.length()];
	
	
	wstring str(vectordllname.begin(), vectordllname.end());

	std::wcout << str;

	const wchar_t* endelig = str.c_str();

    std::cout << "Hello World!\n";
	float a = 5;
	static float prev_player_hp[65];
	std::cout << prev_player_hp[5];
	Sleep(1000);

	float* flyt = &a;
	a = 10;
	int fitte = static_cast<int>(*flyt);
	auto aofp = &flyt;
	GetAsyncKeyState(VK_BACK);

	//const wchar_t* argument = L"hei";
	raise(SIGSEGV);

	//for (long long int i = 0; ++i; (&i)[i] = i);  //access violation

	MessageBox(NULL, endelig, L"hallo", MB_OK | MB_SYSTEMMODAL);
	Sleep(3000);
}


