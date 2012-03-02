/*
    @0vercl0k - Level 2 : Dereference my force4 !
*/
#include <Ntifs.h>
#include <string.h>

#define ERROR(_f_, _status_) DbgPrint("\r\n[!] Error at %s() : 0x%x\r\n", _f_, _status_)
#define IOCTL_F4 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DbgPrint(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0x1337, __VA_ARGS__)

typedef unsigned int DWORD;
typedef unsigned char* PBYTE;
typedef unsigned char BYTE;

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

typedef VOID (*OPERATION_F)(PFORCE4);

//
DRIVER_UNLOAD Unload;
DRIVER_INITIALIZE DriverEntry;
DRIVER_DISPATCH handleIOCTLs;
DRIVER_DISPATCH handleIRP;
VOID ChangeLab(PFORCE4 pBeer);
VOID IncreaseAlcoholPerc(PFORCE4 pBeer);
VOID DecreaseAlcoholPerc(PFORCE4 pBeer);
//

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj , PUNICODE_STRING pRegistryPath)
{
    DWORD i = 0;
    NTSTATUS status;
    UNICODE_STRING deviceName = {0}, symlinkName = {0};
    PDEVICE_OBJECT pDevice = NULL;

    pDriverObj->DriverUnload = Unload;
    DbgPrint("[ Loading.. ]\r\n");

    RtlInitUnicodeString(&deviceName, L"\\Device\\2");
    RtlInitUnicodeString(&symlinkName, L"\\DosDevices\\2");

    DbgPrint("[ Creating the device...]\n");
    IoCreateDevice(
        pDriverObj,
        0,
        &deviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &pDevice
    );

    DbgPrint("[ Linking...]\n");
    IoCreateSymbolicLink(&symlinkName, &deviceName);

    for(; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        pDriverObj->MajorFunction[i] = handleIRP;

    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = handleIOCTLs;
    return STATUS_SUCCESS;
}

NTSTATUS handleIRP(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
    return STATUS_SUCCESS;
}

NTSTATUS handleIOCTLs(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
    #define EL8 0x1337 /* That's el8. */

    PIO_STACK_LOCATION pIoStackLocation = EL8;
    ULONG ioControlCode = EL8, inputBufferLength = EL8, inputBuffer = EL8;
    PFORCE4 pForce4 = EL8;
    OPERATION_F op = EL8;


    pIoStackLocation = IoGetCurrentIrpStackLocation(pIrp);
    ioControlCode = pIoStackLocation->Parameters.DeviceIoControl.IoControlCode;
    inputBufferLength = pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
    inputBuffer = (ULONG)pIrp->AssociatedIrp.SystemBuffer;

    switch(ioControlCode)
    {
        case IOCTL_F4:
        {
            if(inputBufferLength != sizeof(FORCE4))
                DbgPrint("[ It seems someone tried to give another type of beer, I ONLY HANDLE F4 BUDDY. ]\n");
            else
            {
                DbgPrint("[ Hmm I just received an force4, time to analyse this shit ..]\n");
                pForce4 = inputBuffer;

                switch(pForce4->op)
                {
                    case ChangeLabel:
                        op = ChangeLab;
                    break;

                    case IncreaseAlcoholPercentage:
                        op = IncreaseAlcoholPerc;
                    break;

                    case DecreaseAlcoholPercentage:
                        op = DecreaseAlcoholPerc;
                    break;
                }

                /* op will modify your beer */
                op(pForce4);
                pIrp->IoStatus.Information = sizeof(FORCE4);
            }

            break;
        }
    }

    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return pIrp->IoStatus.Status;
}

VOID IncreaseAlcoholPerc(PFORCE4 pBeer)
{
    if(pBeer->alcohol_percentage <= 245)
        pBeer->alcohol_percentage += 10; /* that's a true beer now, don't you think ? */
}

VOID DecreaseAlcoholPerc(PFORCE4 pBeer)
{
    if(pBeer->alcohol_percentage >= 10)
        pBeer->alcohol_percentage -= 10; /* Don't blame me, that's for chix all right ? */
}

VOID ChangeLab(PFORCE4 pBeer)
{
    memset(pBeer->label, 0, 256);

    /* I recommend you my force4 label */
    strcpy(pBeer->label, "-[ 0vercl0kCorporation");
}

VOID Unload(PDRIVER_OBJECT pDrivObj)
{
    DbgPrint("[ Unloading.. ]\n");
    return;
}
