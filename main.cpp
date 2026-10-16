#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <format>

void chechWinRun() {
	HKEY hKey;
	RegOpenKeyExW(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
		0,
		KEY_QUERY_VALUE,
		&hKey
	);

	wchar_t shellByfer[260];
	DWORD shellSize = sizeof(shellByfer);

	RegQueryValueExW(
		hKey,
		L"Shell",
		NULL,
		NULL,
		(LPBYTE)shellByfer,
		&shellSize
	);

	wchar_t UserinitByfer[260];
	DWORD UserinitSize = sizeof(UserinitByfer);

	RegQueryValueExW(
		hKey,
		L"Userinit",
		NULL,
		NULL,
		(LPBYTE)UserinitByfer,
		&UserinitSize
	);
	
	RegCloseKey(hKey);

	wchar_t Path[260];
	GetModuleFileNameW(NULL, Path, 260);

	HKEY hRunKey;
	RegOpenKeyExW(
		HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_SET_VALUE,
		&hRunKey
	);

	RegSetValueExW(
		hRunKey,
		L"Scanner",
		0,
		REG_SZ,
		(BYTE*)Path,
		(lstrlenW(Path) + 1) * sizeof(wchar_t)
	);
	std::wstring report = std::format(L"Shell: {},\n Userinit: {}\n", shellByfer, UserinitByfer);
	MessageBoxW(NULL, report.c_str(), L"Result", MB_OK | MB_ICONINFORMATION);


}

int main() {
	HWND hConsole = GetConsoleWindow();
	ShowWindow(hConsole, SW_HIDE);
	setlocale(LC_ALL, "Russian");
	
	chechWinRun();
	Sleep(5000);


    return 0;
}

