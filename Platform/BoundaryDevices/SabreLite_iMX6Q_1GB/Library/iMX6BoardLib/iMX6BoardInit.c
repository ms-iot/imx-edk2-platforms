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

#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6BoardLib.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6Timer.h>
#include <iMX6UsbPhy.h>


typedef enum {
	IMX_PAD_CFG_EIM_DATA22_USB_OTG_PWR = _IMX_MAKE_PADCFG(
		IMX_SRE_SLOW,
		IMX_DSE_40_OHM,
		IMX_SPEED_MEDIUM,
		IMX_ODE_DISABLE,
		IMX_PKE_ENABLE,
		IMX_PUE_PULL,
		IMX_PUS_100K_OHM_PU,
		IMX_HYS_ENABLED,
		IMX_SION_DISABLED,
		IMX_IOMUXC_EIM_D22_ALT4_USB_OTG_PWR),

	IMX_PAD_CFG_KEY_COL4_USB_OTG_OC = _IMX_MAKE_PADCFG_INPSEL(
		IMX_SRE_SLOW,
		IMX_DSE_40_OHM,
		IMX_SPEED_LOW,
		IMX_ODE_DISABLE,
		IMX_PKE_ENABLE,
		IMX_PUE_PULL,
		IMX_PUS_100K_OHM_PU,
		IMX_HYS_ENABLED,
		IMX_SION_DISABLED,
		IMX_IOMUXC_KEY_COL4_ALT2_USB_OTG_OC,
		IOMUXC_USB_OTG_OC_SELECT_INPUT,
		KEY_COL4_ALT2),

	IMX_PAD_CFG_GPIO01_USB_OTG_ID_HOST = _IMX_MAKE_PADCFG(
		IMX_SRE_FAST,
		IMX_DSE_90_OHM,
		IMX_SPEED_MEDIUM,
		IMX_ODE_DISABLE,
		IMX_PKE_ENABLE,
		IMX_PUE_PULL,
		IMX_PUS_100K_OHM_PD,
		IMX_HYS_ENABLED,
		IMX_SION_DISABLED,
		IMX_IOMUXC_GPIO_1_ALT3_USB_OTG_ID),

	IMX_PAD_CFG_GPIO01_USB_OTG_ID_DEVICE = _IMX_MAKE_PADCFG(
		IMX_SRE_FAST,
		IMX_DSE_90_OHM,
		IMX_SPEED_MEDIUM,
		IMX_ODE_DISABLE,
		IMX_PKE_ENABLE,
		IMX_PUE_PULL,
		IMX_PUS_100K_OHM_PU,
		IMX_HYS_ENABLED,
		IMX_SION_DISABLED,
		IMX_IOMUXC_GPIO_1_ALT3_USB_OTG_ID),

	IMX_PAD_CFG_GPIO17_GPIO7_IO12_USBH1_PWR = _IMX_MAKE_PADCFG(
		IMX_SRE_SLOW,
		IMX_DSE_40_OHM,
		IMX_SPEED_MEDIUM,
		IMX_ODE_DISABLE,
		IMX_PKE_ENABLE,
		IMX_PUE_PULL,
		IMX_PUS_100K_OHM_PU,
		IMX_HYS_ENABLED,
		IMX_SION_DISABLED,
		IMX_IOMUXC_GPIO_17_ALT5_GPIO7_IO12),

} IMX_EHCI_PADCFG;

/**
  Turn off clock gates which are not needed during boot. The PEP is responsible
  to ungate clocks as needed.
**/
static VOID GateUnusedClocks ()
{
  static const IMX_CLK_GATE gatesToTurnOff[] = {
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
    // IMX_GPT_SERIAL_CLK_ENABLE,
    // IMX_GPU2D_CLK_ENABLE,
    // IMX_GPU3D_CLK_ENABLE,
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
    IMX_PWM2_CLK_ENABLE,
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

  ImxClkPwrSetClockGates (
      gatesToTurnOff,
      sizeof (gatesToTurnOff) / sizeof (gatesToTurnOff[0]),
      IMX_CLOCK_GATE_STATE_OFF);
}

/**
  Turn on required clocks, that are not handled by the PEP.
**/
static VOID UngateRequiredClocks ()
{
    static const IMX_CLK_GATE gatesToTurnOn[] = {
        IMX_SDMA_CLK_ENABLE,
        IMX_SPBA_CLK_ENABLE,
        IMX_GPT_SERIAL_CLK_ENABLE,
        IMX_USBOH3_CLK_ENABLE,
    };

    ImxClkPwrSetClockGates(
        gatesToTurnOn,
        sizeof(gatesToTurnOn) / sizeof(gatesToTurnOn[0]),
        IMX_CLOCK_GATE_STATE_ON);
}


VOID EnetInit()
{
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
VOID SetupAudio()
{
}

/**
  Initalize LVDS
**/
VOID LvdsInit()
{
    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

    IMX_CCM_CCGR3_REG ccgr3 = (IMX_CCM_CCGR3_REG) MmioRead32((UINTN) &ccmRegisters->CCGR[3]);
    ccgr3.ldb_di0_clk_enable = 0x3;
    MmioWrite32((UINTN) &ccmRegisters->CCGR[3], ccgr3.AsUint32);
}

/**
  Initialize clock and power for modules on the SOC.
**/
VOID ImxClkPwrInit()
{
  EFI_STATUS status;

  GateUnusedClocks();
  UngateRequiredClocks();

  if (FeaturePcdGet (PcdGpuEnable)) {
    status = ImxClkPwrGpuEnable();
    ASSERT_EFI_ERROR (status);
  }

  if (FeaturePcdGet(PcdLvdsEnable)) {
    status = ImxClkPwrIpuLDBxEnable ();
    ASSERT_EFI_ERROR (status);
  }

  status = ImxClkPwrIpuDIxEnable();
  ASSERT_EFI_ERROR (status);

  ASSERT_EFI_ERROR (ImxClkPwrValidateClocks());

  // TODO - move next code to ArmPlatformInitialize() as soon as this method is moved to this file.
  if (FeaturePcdGet(PcdLvdsEnable)) {
    LvdsInit ();
  }

}

/**
  Initialize SDHC modules on the SOC and perform required pin-muxing
**/
VOID SdhcInit()
{
}

/**
Initialize EHCI modules on the SOC and perform required pin-muxing.
This routine also initializes PHY0(OTG) and PHY1 (USBH1).
**/
VOID EhciInit()
{
	DEBUG((DEBUG_ERROR, "! EhciInit \n"));
	//
	// Pin-mux OTG Over Current
	//
	ImxPadConfig(IMX_PAD_KEY_COL4, IMX_PAD_CFG_KEY_COL4_USB_OTG_OC);

	//
	// Pin-mux OTG power
	//
	ImxPadConfig(IMX_PAD_EIM_D22, IMX_PAD_CFG_EIM_DATA22_USB_OTG_PWR);

	//
	// Pin-mux USB_OTG_ID as device
	//
	ImxPadConfig(IMX_PAD_GPIO_1, IMX_PAD_CFG_GPIO01_USB_OTG_ID_DEVICE);

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
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
VOID I2cInit()
{
}

/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit()
{
}

