#include "key_manager.h"

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";


int main() {
	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	char* autostartEntry = getKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "ENGINEER");
	
	setKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", (const BYTE*)"BLA", "ENGINEER");
	free(autostartEntry);
	return 0;
}
