/*
    @0vercl0k - Force4 factory - 0vercl0k.tuxfamily.org :)
*/

#include <windows.h>
#include <winioctl.h>

#define IOCTL_F4 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DEVICE_NAME "\\\\.\\2"

typedef enum
{
    ChangeLabel,
    IncreaseAlcoholPercentage,
    DecreaseAlcoholPercentage
} OPERATION;

typedef struct
{
    OPERATION op;
    BYTE alcohol_percentage;
    BYTE label[256];
} FORCE4,
  *PFORCE4;

void displayForce4(PFORCE4 pBeer)
{
    printf("\nForce4 information ------------\n");
    printf("Alcohol Percentage: %.3u%%\n", pBeer->alcohol_percentage);
    printf("Label: '%s'\n\n", pBeer->label);
}

int main()
{
    HANDLE hDevice = NULL;
    DWORD byte = 0;
    FORCE4 force4 = {0};

    strcpy(force4.label, "Force 4");

    printf("Force4 factory, welcome !\nCheck here the properties of your beer: \n");
    displayForce4(&force4);

    printf("Okay this beer need more alcohol, let's ask our force4 driver to do that..\n");
    force4.op = IncreaseAlcoholPercentage;


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

    printf("Sending beer..\n");

    /* On envoit la sauce ! */
    DeviceIoControl(
        hDevice,
        IOCTL_F4,
        &force4,
        sizeof(force4),
        &force4,
        sizeof(force4),
        &byte,
        NULL
    );

    CloseHandle(hDevice);

    printf("All right, check this out:\n");
    displayForce4(&force4);
    return EXIT_SUCCESS;
}
