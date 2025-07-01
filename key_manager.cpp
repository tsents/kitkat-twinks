#include "key_manager.h"
#include <stdio.h> //For printf.
#include <stdlib.h>

char* getKeyValue(HKEY rootKey, LPCSTR subKey, LPCSTR value) {
	HKEY keyHandle = NULL;
	LSTATUS lResult = RegOpenKeyExA(rootKey, subKey, 0, KEY_ALL_ACCESS, &keyHandle);
	if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return NULL;
		}
		else {
			printf("Error opening key.\n");
			return NULL;
		}
	}
	char* keyData = (char*)malloc(BUFFER_SIZE);
	DWORD length = BUFFER_SIZE;
	RegGetValueA(keyHandle, NULL, value, RRF_RT_REG_SZ, NULL, keyData, &length);
	printf("%s\n", keyData);
	RegCloseKey(keyHandle);
	return keyData;
}
