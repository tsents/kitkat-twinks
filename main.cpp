#include "key_manager.h"
#include <stdio.h>

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";


int main() {
	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	BYTE* autostartEntry = getKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "ENGINEER");

	printf("Got key %s\n", autostartEntry);
	
	setKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", (const BYTE*)"BLA", "ENGINEER");

	delete[] autostartEntry;
	return 0;
}
