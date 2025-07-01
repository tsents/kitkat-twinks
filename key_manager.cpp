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

LPSTR KeyContext::getKeyValue(LPCSTR field) {
	DWORD length = 0;
	LSTATUS lResult = RegGetValueA(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, NULL, &length); //To get needed length
	if (lResult != ERROR_SUCCESS && lResult != ERROR_MORE_DATA) {
		std::cout << "Couldn't get key length" << std::endl;
		return NULL;
	}	
	LPSTR keyData = new CHAR[length];
	lResult = RegGetValueA(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, keyData, &length);
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Failed loading key after length check " << length << std::endl;
		return NULL;
	}
	return keyData;
}

int KeyContext::setKeyValue(const BYTE* value, LPCSTR field) {
	LSTATUS lResult = RegSetValueExA(m_keyHandle, field, FALSE, REG_SZ, value, 100); //TODO - add length. 
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Error encountered in setKeyValue " << value << " " << field << " " << lResult << std::endl;
		return FALSE;
	}
	RegCloseKey(m_keyHandle);
	return TRUE;
}
