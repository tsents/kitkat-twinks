#include "key_manager.h"
#include <iostream>

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";

const LPCSTR MUTEX_NAME = "Global\\Engineer-Mutex";

// Key used to make us run on startup
const LPCSTR TARGET_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

// The field under which we will be named
const LPCSTR KEY_ENTRY_NAME = "Engineer";

const HMODULE MY_EXECUTABLE = NULL;

/*
 * Uses a mutex to make sure that the program is executed once.
 *
 * name [IN]    The name of the mutex to obtain
 * return [OUT] TRUE if can run, FALSE if another instance is running.
 */
int executeOnce(LPCSTR mutexName) {
	HANDLE singleProgramMutex = CreateMutex(NULL, FALSE, mutexName);
	if (singleProgramMutex == NULL) {
		std::cout << "Got error in Mutex creation " << GetLastError() << std::endl;
		return FALSE;
	}
	DWORD waitResult = WaitForSingleObject(singleProgramMutex, 0);
	if (waitResult != WAIT_OBJECT_0) {
		std::cout << "Got bad result in Wait for mutex" << waitResult << std::endl;
		std::cout << GetLastError() << std::endl;
		return FALSE;
	}
	return TRUE;
}

int main() {
	if (executeOnce(MUTEX_NAME) == FALSE) {
		return 1; //Another instance is running
	}

	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	KeyContext *autostartKey = new KeyContext(HKEY_CURRENT_USER, TARGET_KEY);

	std::cout << "Old key value: " << autostartKey->getKeyValue(KEY_ENTRY_NAME) << std::endl;

	LPSTR filename = new CHAR[MAX_PATH];
	GetModuleFileNameA(MY_EXECUTABLE, filename, MAX_PATH);
	autostartKey->setKeyValue(filename ,KEY_ENTRY_NAME);
	delete[] filename;

	delete autostartKey;
	return 0;
}
