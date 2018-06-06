/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2018 NXP
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

#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/Tpm2Acpi.h>

#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6Audio.h>
#include <iMX6BoardLib.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6Timer.h>
#include <iMX6UsbPhy.h>

STATIC CONST IMX_CLK_GATE GatesToTurnOff[] = {
  IMX_APBHDMA_HCLK_ENABLE,
  IMX_APBHDMA_HCLK_ENABLE,
  IMX_ASRC_CLK_ENABLE,
  IMX_CAN1_CLK_ENABLE,
  IMX_CAN1_SERIAL_CLK_ENABLE,
  IMX_CAN2_CLK_ENABLE,
  IMX_CAN2_SERIAL_CLK_ENABLE,
  IMX_DCIC1_CLK_ENABLE,
  IMX_DCIC2_CLK_ENABLE,
  IMX_DTCP_CLK_ENABLE,
  IMX_ECSPI1_CLK_ENABLE,
  IMX_ECSPI2_CLK_ENABLE,
  IMX_ECSPI3_CLK_ENABLE,
  IMX_ECSPI4_CLK_ENABLE,
  IMX_ECSPI5_CLK_ENABLE,
  IMX_ENET_CLK_ENABLE,
  IMX_ESAI_CLK_ENABLE,
  IMX_I2C1_SERIAL_CLK_ENABLE,
  IMX_I2C2_SERIAL_CLK_ENABLE,
  IMX_I2C3_SERIAL_CLK_ENABLE,
  IMX_IIM_CLK_ENABLE,
  IMX_IPSYNC_IP2APB_TZASC1_IPG_MASTER_CLK_ENABLE,
  IMX_IPSYNC_IP2APB_TZASC2_IPG_MASTER_CLK_ENABLE,
  IMX_IPSYNC_VDOA_IPG_MASTER_CLK_ENABLE,
  IMX_IPU1_DI1_CLK_ENABLE,
  IMX_IPU2_CLK_ENABLE,
  IMX_IPU2_DI0_CLK_ENABLE,
  IMX_IPU2_DI1_CLK_ENABLE,
  IMX_LDB_DI0_CLK_ENABLE,
  IMX_LDB_DI1_CLK_ENABLE,
  IMX_MIPI_CORE_CFG_CLK_ENABLE,
  IMX_MLB_CLK_ENABLE,
  IMX_PL301_MX6QPER1_BCHCLK_ENABLE,
  IMX_PWM1_CLK_ENABLE,
  IMX_PWM3_CLK_ENABLE,
  IMX_PWM4_CLK_ENABLE,
  IMX_RAWNAND_U_BCH_INPUT_APB_CLK_ENABLE,
  IMX_RAWNAND_U_GPMI_BCH_INPUT_BCH_CLK_ENABLE,
  IMX_RAWNAND_U_GPMI_BCH_INPUT_GPMI_IO_CLK_ENABLE,
  IMX_RAWNAND_U_GPMI_INPUT_APB_CLK_ENABLE,
  IMX_SPDIF_CLK_ENABLE,
  IMX_SSI1_CLK_ENABLE,
  IMX_SSI2_CLK_ENABLE,
  IMX_SSI3_CLK_ENABLE,
  IMX_EIM_SLOW_CLK_ENABLE,
  IMX_VDOAXICLK_CLK_ENABLE,
  IMX_VPU_CLK_ENABLE,
};

STATIC CONST IMX_CLK_GATE GatesToTurnOn[] = {
  IMX_SDMA_CLK_ENABLE,
  IMX_SPBA_CLK_ENABLE,
  IMX_GPT_SERIAL_CLK_ENABLE,
  IMX_USBOH3_CLK_ENABLE,
  IMX_PWM2_CLK_ENABLE,
};

/**
  Turn off clock gates which are not needed during boot. The PEP is responsible
  to ungate clocks as needed.
**/
VOID
GateUnusedClocks (
  VOID
  )
{
  ImxClkPwrSetClockGates (
    GatesToTurnOff,
    sizeof (GatesToTurnOff) / sizeof (GatesToTurnOff[0]),
    IMX_CLOCK_GATE_STATE_OFF);
}

/**
  Turn on required clocks, that are not handled by the PEP.
**/
VOID
UngateRequiredClocks (
  VOID
  )
{
  ImxClkPwrSetClockGates (
    GatesToTurnOn,
    sizeof (GatesToTurnOn) / sizeof (GatesToTurnOn[0]),
    IMX_CLOCK_GATE_STATE_ON);
}

VOID
EnetInit (
  VOID
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogReg;
  IMX_CCM_ANALOG_PLL_ENET_REG         CcmAnalogPllReg;
  UINT32                              Counter;

  // Enable ENET PLL, also required for PCIe
  pCcmAnalogReg = (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;
  CcmAnalogPllReg.AsUint32 = MmioRead32 ((UINTN)&pCcmAnalogReg->PLL_ENET);
  CcmAnalogPllReg.POWERDOWN = 0;
  CcmAnalogPllReg.ENABLE = 1;
  MmioWrite32 ((UINTN)&pCcmAnalogReg->PLL_ENET, CcmAnalogPllReg.AsUint32);

  for (Counter = 0; Counter < 100; Counter++) {
    CcmAnalogPllReg.AsUint32 = MmioRead32 ((UINTN)&pCcmAnalogReg->PLL_ENET);
    if (CcmAnalogPllReg.LOCK != 0) {
      break;
    }
    MicroSecondDelay (100);
  }
  if (CcmAnalogPllReg.LOCK == 0) {
    DEBUG ((DEBUG_ERROR, "%a: PLL_ENET is not locked!\n", __FUNCTION__));
    return;
  }

  // Enable PCIe output 125Mhz
  CcmAnalogPllReg.BYPASS = 0;
  CcmAnalogPllReg.ENABLE_125M = 1;
  CcmAnalogPllReg.DIV_SELECT = PLL_ENET_DIV_SELECT_25MHZ;
  MmioWrite32 ((UINTN)&pCcmAnalogReg->PLL_ENET, CcmAnalogPllReg.AsUint32);
  MicroSecondDelay (50000);

  if (CcmAnalogPllReg.LOCK == 0) {
    DEBUG ((DEBUG_ERROR, "%a: PLL_ENET is not locked!\n", __FUNCTION__));
  }
}

/**
  Set SSI3 clock dividers to provide a bit clock capable of
  providing a clock suitable for stereo 44.1 KHz playback at 32 bit
  from PLL3 PFD2 which runs at 508.2 MHz.

  We only need to scale down the PLL3 FPD2 clock by 1:15 to provide a
  reference clock.  The WDM audio class driver will later setup additional
  dividers in the SSI3 block to provide the required bit clock rate.

  This routine also handles all GPIO/PadConfig init required for audio.
**/
VOID
SetupAudio (
  VOID
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmReg;
  IMX_CCM_CCOSR_REG           CcosrReg;
  IMX_CCM_CS1CDR_REG          CsicdrReg;

  pCcmReg = (IMX_CCM_REGISTERS *)IMX_CCM_BASE;
  CsicdrReg = (IMX_CCM_CS1CDR_REG) MmioRead32 ((UINTN)&pCcmReg->CS1CDR);
  CsicdrReg.ssi3_clk_pred = 0; // divide by 1.
  CsicdrReg.ssi3_clk_podf = 14; // divide by 15.
  MmioWrite32 ((UINTN)&pCcmReg->CS1CDR, CsicdrReg.AsUint32);

  // Enable output on CCM_CLKO1, select pll3_sw_clk/2 as the source.
  CcosrReg = (IMX_CCM_CCOSR_REG) MmioRead32 ((UINTN)&pCcmReg->CCOSR);
  CcosrReg.CLKO1_SEL = 0;
  CcosrReg.CLKO1_DIV = 0;
  CcosrReg.CLKO1_EN = 1;

  MmioWrite32 ((UINTN)&pCcmReg->CCOSR, CcosrReg.AsUint32);

    // Set up Audmux where Ssi port = 7 and external port = 4
    InitializeAudmux (AUDMUX_PORT7, AUDMUX_PORT4);
}

/**
  Initialize clock and power for modules on the SoC.
**/
VOID
ImxClkPwrInit (
  VOID
  )
{
  EFI_STATUS  Status;

  GateUnusedClocks();
  UngateRequiredClocks();

  if (FeaturePcdGet (PcdGpuEnable)) {
    Status = ImxClkPwrGpuEnable ();
    ASSERT_EFI_ERROR (Status);
  }

  if (FeaturePcdGet (PcdLvdsEnable)) {
    Status = ImxClkPwrIpuLDBxEnable ();
    ASSERT_EFI_ERROR (Status);
  }

  Status = ImxClkPwrIpuDIxEnable();
  ASSERT_EFI_ERROR (Status);

  ASSERT_EFI_ERROR (ImxClkPwrValidateClocks ());
}

/**
  This routine initializes PHY0(OTG) and PHY1 (USBH1).
**/
VOID
EhciInit (
  VOID
  )
{
  // Initialize PHY0 (OTG)
  ImxUsbPhyInit (IMX_USBPHY0);

  // Initialize PHY1 (USBH1)
  ImxUsbPhyInit (IMX_USBPHY1);
}

/**
    Initialize the TPM2 control area.
**/
VOID
Tpm2AcpiControlAreaInit (
  VOID
  )
{
  EFI_TPM2_ACPI_CONTROL_AREA  *pControlArea;
  EFI_PHYSICAL_ADDRESS        BaseAddress;
  UINT32                      BufferSize;

  BaseAddress = PcdGet64 (PcdTpm2AcpiBufferBase);
  BufferSize = PcdGet32 (PcdTpm2AcpiBufferSize);

  if ((BaseAddress == 0) || (BufferSize == 0)) {
    // TPM not enabled
    return;
  }

  ASSERT (BufferSize >= EFI_PAGE_SIZE * 3);

  pControlArea = (EFI_TPM2_ACPI_CONTROL_AREA *) ((UINTN)BaseAddress);
  ZeroMem (pControlArea, sizeof (EFI_TPM2_ACPI_CONTROL_AREA));
  BufferSize = EFI_PAGE_SIZE;
  pControlArea->Command = (UINT64) ((UINTN) (pControlArea + 1));
  pControlArea->CommandSize = BufferSize;
  pControlArea->Response = pControlArea->Command + BufferSize;
  pControlArea->ResponseSize = BufferSize;
}

/**
  Initalize LVDS
**/
VOID
LvdsInit (
  VOID
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmReg;
  IMX_CCM_CCGR3_REG           Ccgr3Reg;

  pCcmReg = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  Ccgr3Reg = (IMX_CCM_CCGR3_REG) MmioRead32 ((UINTN) &pCcmReg->CCGR[3]);
  Ccgr3Reg.ldb_di0_clk_enable = 0x3;
  MmioWrite32 ((UINTN) &pCcmReg->CCGR[3], Ccgr3Reg.AsUint32);

  // LVDS power enable
  ImxGpioDirection (IMX_GPIO_BANK6, 15, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK6, 15, IMX_GPIO_HIGH);
  MicroSecondDelay (230500);

  // initalize backlight pin
  ImxGpioDirection (IMX_GPIO_BANK4, 30, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK4, 30, IMX_GPIO_HIGH);
}

/**
  Initialize controllers that must setup at the early stage for iMX6 Quad
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    return RETURN_SUCCESS;
  }

  ImxClkPwrInit ();

  // initialize default UEFI debug port early so we can use its debug output
  SerialPortInitialize ();
  SerialPortWrite (
    (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
    (UINTN)sizeof (SERIAL_DEBUG_PORT_INIT_MSG));

  // Initialize timer early on because the following init path will be calling
  // delay functions. PrePi.c calls ArmPlatformInitialize before it calls
  // TimerConstructor to initialize the timer.
  TimerConstructor ();

  EhciInit ();
  EnetInit ();
  SetupAudio ();

  if (FeaturePcdGet (PcdLvdsEnable)) {
    LvdsInit ();
  }

  Tpm2AcpiControlAreaInit ();

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
