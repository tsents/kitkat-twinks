#pragma once
#include <windows.h>
#include <ntdef.h>

class KeyContext {
public:
	KeyContext(HKEY rootKey, LPCSTR subKey);
	~KeyContext();	

	BYTE* getKeyValue(LPCSTR value);

	int setKeyValue(const BYTE* value, LPCSTR valueName);
private:
	HKEY m_keyHandle;
};
