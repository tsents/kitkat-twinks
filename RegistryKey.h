#pragma once
#include <windows.h>
#include <ntdef.h>


/*
 * Implements a simplified context object for reading and setting registry keys.
 * the class assumes that the key value is RRF_RT_REG_SZ (string) and that names
 * are given in ASCI format (uses A functions instead of W).
 */ 
class RegistryKey {
public:
	/*
 	 * Opens a handle to the key. saving into m_keyHandle.
 	 * Uses the same paramater scheme as RegGetValueA.
 	 *
 	 * rootKey [IN] Handle to the key to search under. 
 	 * subKey  [IN] Path to subkey to search. use NULL to ignore.
 	 */
	RegistryKey(HKEY rootKey, LPCSTR subKey);
	// Closes the m_keyHandle.
	~RegistryKey();	

	/*
 	 * Gets the value stored in a key, based on specific field.
 	 * The returned string is allocated on the heap. make sure to delete it.
 	 *
 	 * field  [IN]  The filed under the key to get.
 	 * return [OUT] A buffer of cost char. this buffer is allocated on the heap.
 	 */
	LPSTR getKeyValue(LPCSTR field);

	/*
	 * Sets the value of the field given to be <value> overriding any old data.
	 *
	 * value  [IN]  The value to put inside the key.
	 * entry  [IN]  The entry under the key to set.
	 * return [OUT] TRUE or FALSE based if the action succeeded.
	 */
	int setKeyValue(LPCSTR value, LPCSTR field);
private:
	HKEY m_keyHandle;
};
