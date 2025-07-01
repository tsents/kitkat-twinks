#pragma once
#include <windows.h>
#include <ntdef.h>


/*
 * Implements a simplified context object for reading and setting registry keys.
 * the class assumes that the key value is RRF_RT_REG_SZ (string) and that names
 * are given in ASCI format (uses A functions instead of W).
 */ 
class KeyContext {
public:
	/*
 	 * Opens a handle to the key. saving into m_keyHandle.
 	 */
	KeyContext(HKEY rootKey, LPCSTR subKey);
	// Closes the m_keyHandle.
	~KeyContext();	

	/*
 	 * Gets the value stored in a key, based on specific field.
 	 */
	LPSTR getKeyValue(LPCSTR field);

	/*
	 * Sets the value of the field given to be <value> overriding any old data.
	 *
	 * return [OUT] TRUE or FALSE based if the action succeeded 
	 */
	int setKeyValue(const BYTE* value, LPCSTR field);
private:
	HKEY m_keyHandle;
};
