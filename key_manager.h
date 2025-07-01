#pragma once
#include <windows.h>
#include <ntdef.h>

BYTE* getKeyValue(HKEY rootKey, LPCSTR subKey, LPCSTR value);

int setKeyValue(HKEY rootKey, LPCSTR subKey, const BYTE* value, LPCSTR valueName);
