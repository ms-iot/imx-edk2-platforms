/** @file
*  platform initialization for iMX6 DualLite SOM
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/IoLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6IoMux.h>
#include <iMX6UsbPhy.h>

#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/Tpm2Acpi.h>

//
// Prebaked pad configurations that include mux and drive settings where
// each enum named as IMX_<MODULE-NAME>_PADCFG contains configurations
// for pads used by that module
//

typedef enum {
    IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT5_GPIO1_IO00),
} IMX_EHCI_PADCFG;

typedef enum {
  IMX_USDHC_CD_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED),

  IMX_PAD_CFG_GPIO_4_SD2_CD_B = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CD_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_4_ALT6_SD2_CD_B),
} IMX_USDHC2_PADCFG;

/**
  Initialize SDHC2 module on the SOC and perform required pin-muxing for iMX6 DualLite and Solo
**/
VOID SdhcInit ()
{
  //
  // Set SD2 Card Detect Mux. U-boot does not seem to initialize this
  // PAD correctly.
  //
  // TODO: See if this can be fixed in U-Boot so we don't have to call this function
  //
  ImxPadConfig(IMX_PAD_GPIO_4, IMX_PAD_CFG_GPIO_4_SD2_CD_B);
}

/**
  Initialize EHCI modules on the SOC and perform required pin-muxing.
  This routine also initializes PHY0(OTG) and PHY1 (USBH1).
**/
VOID EhciInit ()
{
    //
    // Pin-mux and enable USBH1 power
    //
    ImxPadConfig(IMX_PAD_GPIO_0, IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR);
    ImxGpioDirection(IMX_GPIO_BANK1, 0, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK1, 0, IMX_GPIO_HIGH);

    //
    // Initialize PHY0 (OTG)
    //
    ImxUsbPhyInit(IMX_USBPHY0);

    //
    // Initialize PHY1 (USBH1)
    //
    ImxUsbPhyInit(IMX_USBPHY1);
}

/**
    Initialize the TPM2 control area.

Note:
    This is temporary, and is normally done in TPM2 OPTEE device lib,
    which will takle place as soon as the TreeDxe driver is enabled.
**/
VOID
Tpm2AcpiControlAreaInit ()
{
    EFI_PHYSICAL_ADDRESS BaseAddress;
    UINT32 BufferSize;
    EFI_TPM2_ACPI_CONTROL_AREA *ControlArea;
   
    BaseAddress = PcdGet64 (PcdTpm2AcpiBufferBase);
    BufferSize = PcdGet32 (PcdTpm2AcpiBufferSize);

    if ((BaseAddress == 0) || (BufferSize == 0)) {
        //
        // TPM not enabled
        //
        return;
    }

    ASSERT (BufferSize >= EFI_PAGE_SIZE * 3);
    
    ControlArea = (EFI_TPM2_ACPI_CONTROL_AREA *)((UINTN)BaseAddress);
    ZeroMem(ControlArea, sizeof(EFI_TPM2_ACPI_CONTROL_AREA));
    BufferSize = EFI_PAGE_SIZE;
    ControlArea->Command = (UINT64)((UINTN)(ControlArea + 1));
    ControlArea->CommandSize = BufferSize;
    ControlArea->Response = ControlArea->Command + BufferSize;
    ControlArea->ResponseSize = BufferSize;
}

/**
  Initialize iMX6 DualLite SOM controllers that must setup at the early stage
**/
RETURN_STATUS
ArmPlatformInitialize(IN UINTN MpId)
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    DEBUG ((DEBUG_INFO, "%a: Not primary core. MpId=%Xh\n", __FUNCTION__, MpId));
    return RETURN_SUCCESS;
  }

  // initialize default UEFI debug port early so we can use its debug output

  SerialPortInitialize ();
  SerialPortWrite (
   (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
   (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));

  SdhcInit ();
  EhciInit ();

  return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}
