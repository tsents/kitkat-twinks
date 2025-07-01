#include "key_manager.h"
#include <iostream>

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";

const LPCSTR MUTEX_NAME = "Global\\Engineer-Mutex";

// Key used to make us run on startup
const LPCSTR TARGET_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

// The field under which we will be named
const LPCWSTR KEY_ENTRY_NAME = L"Engineer"; 

const HMODULE MY_EXECUTABLE = NULL;

int main() {
    HANDLE singleProgramMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
	if (singleProgramMutex == NULL) {
		std::cout << "Got error in Mutex creation " << GetLastError() << std::endl;
	}
	DWORD waitResult = WaitForSingleObject(singleProgramMutex, 0);
	if (waitResult != WAIT_OBJECT_0) {
		std::cout << "Got bad result " << waitResult << std::endl;
		std::cout << GetLastError() << std::endl;
		std::cout << "(I THINK) Program is allready running" << std::endl;
		return 0;
	}

	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	KeyContext *autostartKey = new KeyContext(HKEY_CURRENT_USER, TARGET_KEY);

	LPWSTR filename = new WCHAR[MAX_PATH];
	GetModuleFileNameW(MY_EXECUTABLE, filename, MAX_PATH);
	autostartKey->setKeyValue((BYTE*)filename ,KEY_ENTRY_NAME);

	//std::cout << autostartKey->getKeyValue("ENGINEER") << std::endl;

	delete autostartKey;
	return 0;
}
