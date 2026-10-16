#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <format>

DWORD Assistent(const std::wstring& processName) {
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBoxW(NULL, L"Ошибка HANDLE", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	PROCESSENTRY32 NEVURIS32;
	NEVURIS32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &NEVURIS32)) {
		MessageBoxW(NULL, L"Критическая ошибка!!!", L"CRITICAL ERROR", MB_OK | MB_ICONINFORMATION);
		CloseHandle(hSnapshot);
		return 0;
	}

	do {
		if (processName == NEVURIS32.szExeFile) {
			processId = NEVURIS32.th32ProcessID;	
			break;
		}
	} while (Process32NextW(hSnapshot, &NEVURIS32));

	CloseHandle(hSnapshot);
	return processId;
}


int main() {
	setlocale(LC_ALL, "Russian");
	
	std::wstring name;
	std::wcin >> name;
	if (name != L"") {
		HWND hConsole = GetConsoleWindow();
		SetWindowLong(hConsole, GWL_EXSTYLE, GetWindowLong(hConsole ,GWL_EXSTYLE) | WS_EX_LAYERED);
		for (int opacity = 255; opacity >= 0; opacity -= 15) {
			SetLayeredWindowAttributes(hConsole, 0, opacity, LWA_ALPHA);
			Sleep(40);
		}
		ShowWindow(hConsole, SW_HIDE);
	}
	Sleep(1500);
	DWORD pid = Assistent(name);

	if (pid != NULL) {
		HANDLE hFile = CreateFile(
			L"C:\\Program Files\\log.txt",
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBoxW(NULL, L"Ошибка!", L"ERROR", MB_OK | MB_ICONINFORMATION);
			return 1;
		}

		std::string text = std::format("Process found! Its PID: {}\n", pid);

		DWORD byte = 0;
		WriteFile(
			hFile,
			text.c_str(),
			text.length(),
			&byte,
			NULL
		);
		CloseHandle(hFile);

		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		MessageBoxW(NULL, L"Процес закрыт", L"Complete Process", MB_OK | MB_ICONINFORMATION);
	}

	else {
		HANDLE hFile = CreateFile(
			L"C:\\Program Files\\log.txt",
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile != INVALID_HANDLE_VALUE) {
			std::string text = std::format("Process {} is not running in the system.\n",
				std::string(name.begin(), name.end()));

			DWORD byte = 0;
			WriteFile(
				hFile,
				text.c_str(),
				text.length(),
				&byte,
				NULL
			);
			CloseHandle(hFile);
		}

		MessageBoxW(NULL, L"Процесс не найден", L"Инфо", MB_OK | MB_ICONINFORMATION);

	}


    return 0;
}

