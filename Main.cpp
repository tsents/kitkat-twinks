#include "RegistryKey.h"
#include <iostream>
#include <memory>
#include <synchapi.h>

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";

const LPCSTR MUTEX_NAME = "Global\\Engineer-Mutex";

// Key used to make us run on startup
const LPCSTR TARGET_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

// The field under which we will be named
const LPCWSTR KEY_ENTRY_NAME = L"Engineer";

// In miliseconds.
const int HOUR = 1000 * 60 * 60;

typedef std::unique_ptr<void, decltype(&ReleaseMutex)> Mutex;

/*
 * Uses a mutex to make sure that the program is executed once.
 * Returns a unique_ptr "Mutex" that manages the handle context and memory.
 *
 * name   [IN]  The name of the mutex to obtain
 * return [OUT] A pointer to a unique_ptr for the mutex if obtained, and NULL valued unique_ptr if failed.
 */
Mutex executeOnce(LPCSTR mutexName) {
    Mutex singleProgramMutex(CreateMutex(NULL, FALSE, mutexName), &ReleaseMutex);
    if (singleProgramMutex == NULL) {
        std::cout << "Got error in Mutex creation " << GetLastError() << std::endl;
        return Mutex(NULL, NULL);
    }
    std::cout << "Handle " << singleProgramMutex.get() << " " << std::endl;
    DWORD waitResult = WaitForSingleObject(singleProgramMutex.get(), 0);
    if (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED) {
        return singleProgramMutex;
    }
    std::cout << "Got bad result in Wait for mutex: " << waitResult << std::endl;
    std::cout << GetLastError() << std::endl;
    return Mutex(NULL, NULL); // Releasess the singleProgramMutex because its unique_ptr
}

int main() {
    Mutex heldMutex = executeOnce(MUTEX_NAME);
    if (heldMutex.get() == NULL) {
        return 1; // Another instance is running
    }

    MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

    RegistryKey autostartKey = RegistryKey(HKEY_CURRENT_USER, TARGET_KEY);

    wprintf(L"Old key value: %ls\n", autostartKey.getKeyValue(KEY_ENTRY_NAME));

    WCHAR filename[MAX_PATH];
    GetModuleFileNameW(NULL, filename, MAX_PATH); // NULL=The executable running this.
    if (autostartKey.setKeyValue(filename, KEY_ENTRY_NAME) == false) {
        std::cout << "Failed setKeyValue call when trying to enable autorun on logon" << std::endl;
        return 1;
    }

    Sleep(HOUR);

    return 0;
}
