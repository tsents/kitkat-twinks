#include "RegistryKey.h"
#include <iostream>
#include <wchar.h> //for wcslen


RegistryKey::RegistryKey(HKEY rootKey, LPCSTR subKey) : m_keyHandle(NULL) {
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

RegistryKey::~RegistryKey() {
	RegCloseKey(m_keyHandle);
}

LPWSTR RegistryKey::getKeyValue(LPCWSTR field) {
	DWORD length = 0;
	LSTATUS lResult = RegGetValueW(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, NULL, &length); //To get needed length
	if (lResult != ERROR_SUCCESS && lResult != ERROR_MORE_DATA) {
		std::cout << "Couldn't get key length" << std::endl;
		return NULL;
	}	
	LPWSTR keyData = new WCHAR[length];
	lResult = RegGetValueW(m_keyHandle, NULL, field, RRF_RT_REG_SZ, NULL, keyData, &length);
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Failed loading key after length check " << length << std::endl;
		return NULL;
	}
	return keyData;
}

BOOL RegistryKey::setKeyValue(LPCWSTR value, LPCWSTR field) {
	//uses BYTE* because other key field types are allowed. not only strings.
	const BYTE* byteData = reinterpret_cast<const BYTE*>(value);
	const size_t dataSize = (wcslen(value) + 1) * sizeof(wchar_t);
	LSTATUS lResult = RegSetValueExW(m_keyHandle, field, FALSE, REG_SZ, byteData, dataSize); 
	if (lResult != ERROR_SUCCESS) {
		std::cout << "Error encountered in setKeyValue " << value << " " << field << " " << lResult << std::endl;
		return FALSE;
	}
	return TRUE;
}
