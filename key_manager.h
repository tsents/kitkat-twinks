#pragma once
#include <windows.h>
#include <ntdef.h>

const unsigned int BUFFER_SIZE = 1024;

char* getKeyValue(HKEY rootKey, LPCSTR subKey, LPCSTR value);
