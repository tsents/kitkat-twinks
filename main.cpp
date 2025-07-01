#include <windows.h>
#include <stdio.h> //For printf.
#include <stdlib.h>
#include <ntdef.h>

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";


int main() {
	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	HKEY autorunKey = NULL;
	LSTATUS lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &autorunKey);
	if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		}
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}
	char* engineerKeyData = (char*)malloc(100);
	DWORD length = 100;
	RegGetValueA(autorunKey, NULL, "ENGINEER", RRF_RT_REG_SZ, NULL, engineerKeyData, &length);
	printf("%s\n", engineerKeyData);
	

	
	return 0;
}
