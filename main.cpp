#include "key_manager.h"

const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";


int main() {
	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);

	KeyContext *autostartKey = new KeyContext(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");

	//std::cout << autostartKey->getKeyValue("ENGINEER") << std::endl;


	delete autostartKey;
	return 0;
}
