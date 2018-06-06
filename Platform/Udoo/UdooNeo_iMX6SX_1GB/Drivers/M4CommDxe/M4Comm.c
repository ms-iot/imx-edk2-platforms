//// 
//  Microsoft Coporation
//  SiArch
//
//  Portions Copyright (c) Microsoft Corporation
//  Portions Copyright (c) Intel Corporation
//  Portions Copyright (c) Apple
//  Portions Copyright (c) ARM Ltd.
//  Portions Copyright (c) Freescale
////

#include <Uefi.h>

#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/HardwareInterrupt.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePathFromText.h>

#include <IMX6.h>

#include <imx6sxdef.inc>
#include <udooneo.inc>

#include "shmem.h"

extern SHMEM_CHANNEL * const pChan;

#define MU_ASR_STATUS_BIT       IMX6SX_MU_ASR_GIP0
#define MU_ACR_SEND_IRQ_BIT     IMX6SX_MU_ACR_GIR0

SHMEM_CHANNEL * const pChan = (SHMEM_CHANNEL *)SOC_OCRAM_M4COMM_BUFFER_BASE;

static EFI_HARDWARE_INTERRUPT_PROTOCOL *gInterrupt = NULL;

#define ONE_DIR_BYTES ((SOC_OCRAM_M4COMM_BUFFER_LENGTH - sizeof(SHMEM_CHANNEL)) / 2)

static UINT8            sgCheckBuf[128];
static UINT32 volatile  sgEchoInterrupts = 0;

extern UINT8  gM4TestBinary[];
extern UINT32 gM4TestBinarySize;

VOID
EFIAPI
M4InterruptHandler(
    IN  HARDWARE_INTERRUPT_SOURCE   Source,
    IN  EFI_SYSTEM_CONTEXT          SystemContext
    )
{
    UINT32      regVal;
    EFI_TPL     OriginalTPL;

    OriginalTPL = gBS->RaiseTPL(TPL_HIGH_LEVEL);

    // read the status register
    regVal = MmioRead32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ASR);

    // clear interrupt status
    if (regVal & MU_ASR_STATUS_BIT)
    {
        MmioWrite32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ASR, MU_ASR_STATUS_BIT);
        sgEchoInterrupts++;
        ArmDataMemoryBarrier();
    }

    gBS->RestoreTPL (OriginalTPL);
}

static
void
sCopyWithVerify(
    void *      apTarg,
    void const *apSrc,
    UINTN       aByteCount
    )
{
    if (aByteCount == 0)
        return;
    do
    {
        *((UINT8 *)apTarg) = *((UINT8 *)apSrc);
        if (*((UINT8 volatile *)apTarg) != *((UINT8 *)apSrc))
            break;
        apTarg = ((UINT8 *)apTarg) + 1;
        apSrc = ((UINT8 *)apSrc) + 1;
    } while (--aByteCount);
    ASSERT(aByteCount == 0);
}

EFI_STATUS
CheckM4Init(
    VOID
    )
{
    UINT32      regVal;

    // ensure M4 platform is enabled - platform pei should have done this
    // stop if it has not
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    if (!(regVal & IMX6SX_SRC_SCR_M4_ENABLE))
    {
        DebugPrint(0xFFFFFFFF, "M4 is not enabled. M4Comm stopped.\n");
        return EFI_DEVICE_ERROR;
    }

    regVal = MmioRead32(IMX6SX_PHYSADDR_CCM_CCGR3);
    if ((regVal & (IMX6SX_RUN_ONLY << IMX6SX_SHL_CCM_CCGR3_M4)) == 0)
    {
        DebugPrint(0xFFFFFFFF, "M4 module clock is not running. M4Comm stopped\n");
        return EFI_DEVICE_ERROR;
    }

    if (!(regVal & IMX6SX_SRC_SCR_M4C_NON_SCLR_RST))
    {
        DebugPrint(0xFFFFFFFF, "M4 is not in reset. M4Comm stopped.\n");
        return EFI_DEVICE_ERROR;
    }

//    DebugPrint(0xFFFFFFFF, "M4 clock runs at %d Hz\n", IMX6SX_ClockRate_M4());

    return EFI_SUCCESS;
}

VOID
InitInterrupt(
    VOID
    )
{
    EFI_STATUS  Status;

    Status = gBS->LocateProtocol(&gHardwareInterruptProtocolGuid, NULL, (VOID **)&gInterrupt);
    ASSERT_EFI_ERROR(Status);

    // Install interrupt handler
    Status = gInterrupt->RegisterInterruptSource(gInterrupt, IMX6SX_IRQ_MU_TO_A9, M4InterruptHandler);
    ASSERT_EFI_ERROR(Status);
}

EFI_STATUS
InitSharedMemory(
    VOID
    )
{
    ASSERT(SOC_OCRAM_M4COMM_BUFFER_LENGTH >= SHMEM_CalculateBytesNeededForChannel(0, ONE_DIR_BYTES, ONE_DIR_BYTES, 4));

    if (0 == SHMEM_CreateChannel(pChan, 0, ONE_DIR_BYTES, ONE_DIR_BYTES, 4)) {
        return EFI_DEVICE_ERROR;
    }

    pChan->Control[0].ReaderIdent = 0;
    pChan->Control[1].ReaderIdent = 1;

    return EFI_SUCCESS;
}

VOID
InitMessagingUnit(
    VOID
    )
{
    UINT32      regVal;

    // reset the messaging unit
    MmioWrite32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR, IMX6SX_MU_ACR_MUR);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR);
    } while (regVal & IMX6SX_MU_ACR_MUR);

    // clear any pending interrupt status
    MmioWrite32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ASR, MU_ASR_STATUS_BIT);

    // enable interrupt from M
    regVal = MmioRead32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR);
    regVal |= MU_ASR_STATUS_BIT;
    MmioWrite32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR, regVal);
}

VOID
ResetM4(
    VOID
    )
{
    UINT32      regVal;

    // assert non-self-clearing M4C (core) reset
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal |= IMX6SX_SRC_SCR_M4C_NON_SCLR_RST;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);

    MicroSecondDelay(50);

    // reset the M4 *PLATFORM* and wait for the reset to complete
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal |= IMX6SX_SRC_SCR_M4P_RST;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    } while (regVal & IMX6SX_SRC_SCR_M4P_RST);

    DEBUG ((DEBUG_INIT, "Reset M4\n"));
}

EFI_STATUS
LoadM4Instructions(
    UINT8                        *BinaryBuffer,
    UINTN                        BinarySize
    )
{
    UINT32      ixCheck;

    // copy in M image to its TCM.  straight copy without byte per byte verification seems to have
    // problems and data gets lost.  readback per byte copied seems to work.
    ASSERT(BinarySize < SOC_M4TCM_PHYSICAL_LENGTH);
    sCopyWithVerify((void *)SOC_M4TCM_PHYSICAL_BASE, BinaryBuffer, BinarySize);

    // read it back again after the whole copy is finished to check it
    for (ixCheck = 0; ixCheck < BinarySize; ixCheck++)
    {
        if ((*(((UINT8 *)SOC_M4TCM_PHYSICAL_BASE) + ixCheck)) != BinaryBuffer[ixCheck]) {
            return EFI_DEVICE_ERROR;
        }
    }
    if (ixCheck != BinarySize) {
        return EFI_DEVICE_ERROR;
    }

    DEBUG ((DEBUG_INIT, "M4 Load Successful\n"));

    return EFI_SUCCESS;
}

VOID
StartM4Execution(
    VOID
    )
{
    UINT32      regVal;

    // deassert the non-self-clearing M4C (core) reset
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal &= ~IMX6SX_SRC_SCR_M4C_NON_SCLR_RST;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);

    // give M4 code 50ms to come up
    MicroSecondDelay(50 * 1000);
}

EFI_STATUS
TestM4Interrupt(
    UINT8                        *BinaryBuffer,
    UINTN                        BinarySize
    )
{
    UINT32      regVal;

    // send it 128 bytes of data
    regVal = SHMEM_Write(pChan, 0, BinaryBuffer, 128);
    if (regVal != 128)
    {
        DebugPrint(0xFFFFFFFF, "Write to shared memory channel failed\n");
        return EFI_DEVICE_ERROR;
    }
    regVal = MmioRead32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR);
    regVal |= MU_ACR_SEND_IRQ_BIT;
    MmioWrite32(IMX6SX_PHYSADDR_MU_A + IMX6SX_MU_OFFSET_ACR, regVal);

    // wait for the echo interrupt to come in
    while (sgEchoInterrupts == 0);

    // read 128 bytes, should be 0 left in buffer after we do
    regVal = SHMEM_Read(pChan, 0, sgCheckBuf, 128);
    if ((regVal != 0) ||
        (CompareMem(sgCheckBuf, BinaryBuffer, 128)))
    {
        DebugPrint(0xFFFFFFFF, "M4 messaging failure\n");
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
RunM4TestProgram(
    VOID
    )
{
    EFI_STATUS Status;

    ResetM4();

    Status = LoadM4Instructions(gM4TestBinary, gM4TestBinarySize);
    if (EFI_ERROR(Status)) {
        DebugPrint(0xFFFFFFFF, "M4TestProgram: instruction load failure\n");
        return Status;
    }

    sgEchoInterrupts = 0;

    StartM4Execution();

    Status = TestM4Interrupt(gM4TestBinary, gM4TestBinarySize);
    if (EFI_ERROR(Status)) {
        DebugPrint(0xFFFFFFFF, "M4TestProgram: interrupt test failure\n");
        return Status;
    }

    DebugPrint(DEBUG_INIT, "M4TestProgram: M4 is communicating normally (echo).\n");

    return EFI_SUCCESS;
}

// Note: Extracted from Bds.c
EFI_STATUS
FindFileFullDevicePath (
    IN OUT UINTN            *Index,
    IN CHAR8                *AsciiAppPath,
    IN CHAR16               *UnicodeAppPath,
    EFI_DEVICE_PATH         **FullAppDevicePath
    )
{
    EFI_STATUS      Status;
    EFI_HANDLE      *FileSystemHandles;
    UINTN           NumberFileSystemHandles;
    BOOLEAN         FoundPath = FALSE;

    Status = BdsConnectAllDrivers ();
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "FindFileFullDevicePath: FAIL to connect all drivers\n"));
        return EFI_NOT_FOUND;
    }

    // Get all the handles to a recognize file system to see
    // if any of them contains the requested application
    Status = gBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        &NumberFileSystemHandles,
        &FileSystemHandles
        );
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "No recognize filesystem to parse\n"));
        return EFI_NOT_FOUND;
    }

    if (*Index >= NumberFileSystemHandles) {
        DEBUG ((EFI_D_ERROR, "Invalid file system index %d\n", *Index));
        return EFI_NOT_FOUND;
    }

    for (; (*Index) < NumberFileSystemHandles; (*Index)++) {
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *Volume = NULL;
        EFI_FILE_HANDLE                     Root = NULL;
        EFI_FILE_HANDLE                     ThisFile = NULL;
        EFI_HANDLE                          Handle;
        EFI_DEVICE_PATH                     *AppDevicePath;
        EFI_DEVICE_PATH                     *RemainingDevicePath;
        EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL  *EfiDevicePathFromTextProtocol;
        FILEPATH_DEVICE_PATH*               FilePathDevicePath;

        Status = gBS->LocateProtocol (
            &gEfiDevicePathFromTextProtocolGuid,
            NULL,
            (VOID **)&EfiDevicePathFromTextProtocol
            );
        if (EFI_ERROR (Status)) {
            DEBUG ((EFI_D_ERROR, "Error: DevicePathFromTextProtocol required\n"));
            goto NextFleSystemHandle;
        }

        AppDevicePath = EfiDevicePathFromTextProtocol->ConvertTextToDevicePath ((CHAR16*)AsciiAppPath);
        ASSERT (AppDevicePath != NULL);

        *FullAppDevicePath = AppendDevicePath (DevicePathFromHandle (
                FileSystemHandles[*Index]),
                AppDevicePath
                );
        if (*FullAppDevicePath == NULL) {
            DEBUG ((EFI_D_ERROR, "Error:Insufficient memory to allocate device path\n"));
            goto NextFleSystemHandle;
        }

        Status = BdsConnectAndUpdateDevicePath (
            FullAppDevicePath,
            &Handle,
            &RemainingDevicePath
            );
        if (EFI_ERROR (Status)) {
            DEBUG (
                (EFI_D_ERROR, 
                "Error: BdsConnectAndUpdateDevicePath failed (Status '0x%x')\n", 
                Status));
            goto NextFleSystemHandle;
        }

        FilePathDevicePath = (FILEPATH_DEVICE_PATH*)RemainingDevicePath;

        Status = gBS->HandleProtocol (
            Handle, 
            &gEfiSimpleFileSystemProtocolGuid, 
            (VOID **)&Volume
            );
        if (EFI_ERROR (Status)) {
            goto NextFleSystemHandle;
        }

        Status = Volume->OpenVolume (Volume, &Root);
        if (EFI_ERROR (Status)) {
            goto NextFleSystemHandle;
        }

        Status = Root->Open (
            Root, 
            &ThisFile, 
            FilePathDevicePath->PathName, 
            EFI_FILE_MODE_READ, 
            0
            );
        if (EFI_ERROR (Status)) {
            goto NextFleSystemHandle;
        }

        FoundPath = TRUE;

    NextFleSystemHandle:
        if (ThisFile) {
            ThisFile->Close (ThisFile);
        }
        if (Root) {
            Root->Close (Root);
        }
        if (FoundPath) {
            (*Index)++;
            return EFI_SUCCESS;
        }
        if (*FullAppDevicePath) {
            FreePool (*FullAppDevicePath);
            *FullAppDevicePath = NULL;
        }
    }

    return EFI_NOT_FOUND;
}

// Prototype for method in BdsFilePath.c
EFI_STATUS
BdsLoadImage (
  IN     EFI_DEVICE_PATH       *DevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *FileSize
  );

EFI_STATUS
M4StartApplication (
    IN EFI_HANDLE                  ParentImageHandle,
    IN EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
    IN UINTN                       LoadOptionsSize,
    IN VOID*                       LoadOptions
    )
{
    EFI_STATUS                   Status = EFI_SUCCESS;
    EFI_PHYSICAL_ADDRESS         BinaryBuffer;
    UINTN                        BinarySize;
    UINT8                        *binaryBufferPtr = NULL;

    ResetM4();

    // Find the nearest supported file loader
    Status = BdsLoadImage (DevicePath, AllocateAnyPages, &BinaryBuffer, &BinarySize);
    if (EFI_ERROR (Status)) {
        goto Exit;
    }

    DEBUG ((DEBUG_INIT, "M4 BinarySize: %d\n", BinarySize));

    binaryBufferPtr = (UINT8*)(UINTN)BinaryBuffer;

    Status = LoadM4Instructions(binaryBufferPtr, BinarySize);
    if (EFI_ERROR(Status)) {
        goto Exit;
    }

    StartM4Execution();

    DebugPrint(DEBUG_INIT, "Started M4 Program from SD\n");

Exit:
     return Status;
}

EFI_STATUS
M4CommInitialize(
    IN EFI_HANDLE          ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
    )
{
    EFI_STATUS      Status;
    UINTN           Index = 0;
    CHAR16          *UnicodePreBootAppPath = L"m4_program.bin";
    CHAR8           *AsciiPreBootAppPath = (CHAR8*)UnicodePreBootAppPath;
    EFI_DEVICE_PATH *FullPath = NULL;

    Status = CheckM4Init();
    if (EFI_ERROR(Status)) {
        goto Exit;
    }

    InitInterrupt();

    Status = InitSharedMemory();
    if(EFI_ERROR(Status)) {
        goto Exit;
    }

    InitMessagingUnit();

    Status = RunM4TestProgram();
    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "M4 Test Program Failed (Status '0x%x')\n", Status));
        goto Exit;
    }

    Status = FindFileFullDevicePath (
        &Index,
        AsciiPreBootAppPath,
        UnicodePreBootAppPath,
        &FullPath
        );

    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Find M4 File %s (Status '0x%x')\n", AsciiPreBootAppPath, Status));
        goto Exit;
    }

    Status = M4StartApplication (gImageHandle,
                                 FullPath,
                                 0,
                                 NULL);
    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "Fail to launch M4 app %s (Status '0x%x')\n", AsciiPreBootAppPath, Status));
        goto Exit;
    }
    else {
        DEBUG ((DEBUG_INIT, "Launching M4 app %s (Status '0x%x')\n", AsciiPreBootAppPath, Status));
    }

    Status = EFI_SUCCESS;

Exit:
    if (gInterrupt != NULL && EFI_ERROR(Status)) {
        gInterrupt->RegisterInterruptSource(gInterrupt, IMX6SX_IRQ_MU_TO_A9, NULL);
    }

    return Status;
}

