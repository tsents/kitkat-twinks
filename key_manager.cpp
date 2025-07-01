#include "key_manager.h"

BYTE* getKeyValue(HKEY rootKey, LPCSTR subKey, LPCSTR value) {
	HKEY keyHandle = NULL;
	LSTATUS lResult = RegOpenKeyExA(rootKey, subKey, 0, KEY_ALL_ACCESS, &keyHandle);
	if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			//std::cout << "Key not found." << std::endl;
			return NULL;
		}
		else {
			//std::cout << "Error opening key." << std::endl;
			return NULL;
		}
	}
	DWORD length = 0;
	lResult = RegGetValueA(keyHandle, NULL, value, RRF_RT_REG_SZ, NULL, NULL, &length); //To get needed length
	if (lResult != ERROR_SUCCESS && lResult != ERROR_MORE_DATA) {
		//std::cout << "Couldn't get key length" << std::endl;
		return NULL;
	}	
	BYTE* keyData = new BYTE[length];
	lResult = RegGetValueA(keyHandle, NULL, value, RRF_RT_REG_SZ, NULL, keyData, &length);
	if (lResult != ERROR_SUCCESS) {
		//std::cout << "Error trying to obtain key " << subKey << " with len " << length << std::endl;
		return NULL;
	}
	//std::cout << "Obtained key value: " << keyData << std::endl;
	RegCloseKey(keyHandle);
	return keyData;
}

int setKeyValue(HKEY rootKey, LPCSTR subKey, const BYTE* value, LPCSTR valueName) {
	HKEY keyHandle = NULL;
	LSTATUS lResult = RegOpenKeyExA(rootKey, subKey, 0, KEY_ALL_ACCESS, &keyHandle);
	if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			//std::cout << "Key not found." << std::endl;
			return FALSE;
		}
		else {
			//std::cout << "Error opening key." << std::endl;
			return FALSE;
		}
	}

	lResult = RegSetValueExA(keyHandle, valueName, FALSE, REG_SZ, value, strlen((const char*)value) + 1); 
	RegCloseKey(keyHandle);
	return TRUE;
}
