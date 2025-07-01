#include "key_manager.h"
#include <iostream>


KeyContext::KeyContext(HKEY rootKey, LPCSTR subKey) : m_keyHandle(NULL) {
	LSTATUS lResult = RegOpenKeyExA(rootKey, subKey, 0, KEY_ALL_ACCESS, &m_keyHandle);
	if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			std::cout << "Key not found." << std::endl;
			return;
		}
		else {
			std::cout << "Error opening key." << std::endl;
			return;
		}
	}
}

KeyContext::~KeyContext() {
	RegCloseKey(m_keyHandle);
}

BYTE* KeyContext::getKeyValue(LPCSTR field) {
	DWORD length = 0;
	LSTATUS lResult = RegGetValueA(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, NULL, &length); //To get needed length
	if (lResult != ERROR_SUCCESS && lResult != ERROR_MORE_DATA) {
		std::cout << "Couldn't get key length" << std::endl;
		return NULL;
	}	
	BYTE* keyData = new BYTE[length];
	lResult = RegGetValueA(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, keyData, &length);
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Failed loading key after length check " << length << std::endl;
		return NULL;
	}
	std::cout << "Obtained key field: " << keyData << std::endl;
	return keyData;
}

int KeyContext::setKeyValue(const BYTE* value, LPCWSTR field) {
	LSTATUS lResult = RegSetValueExW(m_keyHandle, field, FALSE, REG_SZ, value, 100); //TODO - add length. 
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Error encountered in setKeyValue " << value << " " << field << " " << lResult << std::endl;
		return FALSE;
	}
	RegCloseKey(m_keyHandle);
	return TRUE;
}
