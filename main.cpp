#include <processthreadsapi.h> //GetCurrentProcess().
//#include <winuser.h> //MessageBox()
#include <windows.h>


const LPCTSTR MESSAGE = "MANAGEMENT PROGRAM IS UP";
const LPCTSTR TITLE = "Managment";

/*
    int msgboxID = MessageBoxA(
        NULL,
        (LPCWSTR)L"Resource not available\nDo you want to try again?",
        (LPCWSTR)L"Account Details",
        MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
    );

    switch (msgboxID)
    {
    case IDCANCEL:
        // TODO: add code
        break;
    case IDTRYAGAIN:
        // TODO: add code
        break;
    case IDCONTINUE:
        // TODO: add code
        break;
    }
*/

int main() {
	MessageBox(NULL, MESSAGE, TITLE, MB_OK | MB_ICONINFORMATION);
	return 0;
}
