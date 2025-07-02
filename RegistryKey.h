#pragma once
#include <memory>
#include <windows.h>

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
     * Define copy and move constructors. these are define as delete to avoid multiple
     * open handles to a specific key, for setting and writing. if the user wishes 
     * to have multiple handles he can do so explicitly.
     */
    RegistryKey& operator=(const RegistryKey&) = delete;
    RegistryKey(const RegistryKey&) = delete;

    /*
     * Gets the value stored in a key, based on specific field.
     * The returned string is allocated on the heap and held by unique_ptr.
     *
     * field  [IN]  The field under the key to get.
     * return [OUT] A buffer of cost char. the unique_ptr handles its free when out of use.
     */
    std::unique_ptr<WCHAR[]> getKeyValue(LPCWSTR field);

    /*
     * Sets the value of the field given to be <value> overriding any old data.
     *
     * value  [IN]  The value to put inside the key.
     * entry  [IN]  The entry under the key to set.
     * return [OUT] true or false based if the action succeeded.
     */
    bool setKeyValue(LPCWSTR value, LPCWSTR field);

private:
    HKEY m_keyHandle;
};
