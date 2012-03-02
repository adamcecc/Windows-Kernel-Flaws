/*
    @0vercl0k - level1 : overflow my kernel stackz !
*/
#include <Ntifs.h>

#define ERROR(_f_, _status_) DbgPrint("\r\n[!] Error at %s() : 0x%x\r\n", _f_, _status_)
#define IOCTL_HI CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DbgPrint(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0x1337, __VA_ARGS__)

typedef unsigned int DWORD;
typedef unsigned char* PBYTE;
typedef unsigned char BYTE;

//
DRIVER_UNLOAD Unload;
DRIVER_INITIALIZE DriverEntry;
DRIVER_DISPATCH handleIOCTLs;
DRIVER_DISPATCH handleIRP;
//

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj , PUNICODE_STRING pRegistryPath)
{
    DWORD i = 0;
    NTSTATUS status;
    UNICODE_STRING deviceName = {0}, symlinkName = {0};
    PDEVICE_OBJECT pDevice = NULL;

    pDriverObj->DriverUnload = Unload;
    DbgPrint("[ Loading.. ]\r\n");

    RtlInitUnicodeString(&deviceName, L"\\Device\\1");
    RtlInitUnicodeString(&symlinkName, L"\\DosDevices\\1");

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
    PIO_STACK_LOCATION pIoStackLocation = NULL;
    ULONG ioControlCode = 0, inputBufferLength = 0, inputBuffer = 0;
    BYTE buffer[256] = {0};

    pIoStackLocation = IoGetCurrentIrpStackLocation(pIrp);
    ioControlCode = pIoStackLocation->Parameters.DeviceIoControl.IoControlCode;
    inputBufferLength = pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
    inputBuffer = (ULONG)pIrp->AssociatedIrp.SystemBuffer;

    switch(ioControlCode)
    {
        case IOCTL_HI:
        {
            DbgPrint("[ Let's copying this buffer...]");

            /* ! w00tz ! */
            strcpy(buffer, inputBuffer);
            break;
        }
    }

    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return pIrp->IoStatus.Status;
}

VOID Unload(PDRIVER_OBJECT pDrivObj)
{
    DbgPrint("[ Unloading.. ]\n");
    return;
}
