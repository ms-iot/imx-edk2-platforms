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

#include "iMX6BoardInit.h"

STATIC CONST IMX_CLK_GATE GatesToTurnOff[] = {
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

STATIC CONST IMX_CLK_GATE GatesToTurnOn[] = {
  IMX_SDMA_CLK_ENABLE,
  IMX_SPBA_CLK_ENABLE,
  IMX_GPT_SERIAL_CLK_ENABLE,
  IMX_USBOH3_CLK_ENABLE,
  IMX_PWM1_CLK_ENABLE
};

STATIC IMX_PAD GpioPins[] = {
  IMX_PAD_ENET_RX_ER,
  IMX_PAD_EIM_A22,
  IMX_PAD_EIM_A21,
  IMX_PAD_EIM_A20,
  IMX_PAD_EIM_A19,
  IMX_PAD_EIM_A18,
  IMX_PAD_EIM_A17,
  IMX_PAD_EIM_A16,
  IMX_PAD_EIM_EB0,
  IMX_PAD_EIM_EB1,
  IMX_PAD_EIM_DA0,
  IMX_PAD_EIM_DA1,
  IMX_PAD_EIM_DA2,
  IMX_PAD_EIM_DA3,
  IMX_PAD_EIM_DA4,
  IMX_PAD_EIM_DA5,
  IMX_PAD_EIM_DA6,
  IMX_PAD_EIM_DA7,
  IMX_PAD_EIM_DA8,
  IMX_PAD_EIM_DA9,
  IMX_PAD_EIM_DA10,
  IMX_PAD_EIM_DA11,
  IMX_PAD_EIM_DA12,
  IMX_PAD_EIM_DA13,
  IMX_PAD_EIM_DA14,
  IMX_PAD_EIM_DA15,
  IMX_PAD_EIM_D26,
  IMX_PAD_EIM_D27,
  IMX_PAD_EIM_D30,
  IMX_PAD_EIM_D31,
  IMX_PAD_EIM_A24,
  IMX_PAD_EIM_A23,
  IMX_PAD_GPIO_16,
};

CONST IMX_CLK_GATE UartClockGatesToTurnOn[] = {
  IMX_PLL3_MAIN_CLK, // IMX_UART_CLK_ENABLE,
  IMX_UART_CLK_ROOT  // UART_SERIAL_CLK_ENABLE,
};

/**
  Turn off clock gates which are not needed during boot. The PEP is responsible
  to ungate clocks as needed.
**/
STATIC
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
STATIC
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

STATIC
VOID
EnetInit (
  VOID
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogReg;
  IMX_CCM_ANALOG_PLL_ENET_REG         CcmAnalogPllReg;
  UINT32                              Counter;

  // Apply ENET pin-muxing configurations (ALT 1)
  ImxPadConfig (IMX_PAD_ENET_MDIO, IMX_PAD_ENET_MDIO_ENET_MDIO);
  ImxPadConfig (IMX_PAD_ENET_MDC, IMX_PAD_ENET_MDC_ENET_MDC);
  ImxPadConfig (IMX_PAD_KEY_ROW4, IMX_PAD_CFG_KEY_ROW4_GPIO4_IO15);
  ImxPadConfig (IMX_PAD_DI0_PIN2, IMX_PAD_DI0_PIN2_GPIO4_IO18);
  ImxPadConfig (IMX_PAD_GPIO_16, IMX_PAD_GPIO_16_ENET_REF_CLK);
  ImxPadConfig (IMX_PAD_ENET_REF_CLK, IMX_PAD_ENET_REF_CLK_ENET_REF_CLK);
  ImxPadConfig (IMX_PAD_RGMII_TD0, IMX_PAD_RGMII_TD0_RGMII_TD0);
  ImxPadConfig (IMX_PAD_RGMII_TD1, IMX_PAD_RGMII_TD1_RGMII_TD1);
  ImxPadConfig (IMX_PAD_RGMII_TD2, IMX_PAD_RGMII_TD2_RGMII_TD2);
  ImxPadConfig (IMX_PAD_RGMII_TD3, IMX_PAD_RGMII_TD3_RGMII_TD3);
  ImxPadConfig (IMX_PAD_RGMII_TXC, IMX_PAD_RGMII_TXC_RGMII_TXC);
  ImxPadConfig (IMX_PAD_RGMII_TX_CTL, IMX_PAD_RGMII_TX_CTL_RGMII_TX_CTL);

  ImxPadConfig (IMX_PAD_RGMII_RD0, IMX_PAD_RGMII_RD0_ENET_RGMII_RD0);
  ImxPadConfig (IMX_PAD_RGMII_RD1, IMX_PAD_RGMII_RD1_ENET_RGMII_RD1);
  ImxPadConfig (IMX_PAD_RGMII_RD2, IMX_PAD_RGMII_RD2_ENET_RGMII_RD2);
  ImxPadConfig (IMX_PAD_RGMII_RD3, IMX_PAD_RGMII_RD3_ENET_RGMII_RD3);
  ImxPadConfig (IMX_PAD_RGMII_RXC, IMX_PAD_RGMII_RXC_ENET_RGMII_RXC);
  ImxPadConfig (IMX_PAD_RGMII_RX_CTL, IMX_PAD_RGMII_RX_CTL_RGMII_RX_CTL);

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
STATIC
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
  // Set GPIO05 alt config to ALT3 (CCM_CLKO1).
  // This clock drives the master clock for the audio codec.
  ImxPadConfig (IMX_PAD_GPIO_5, IMX_PAD_CFG_GPIO_5_CCM_CLKO1);

  // Set alt config for KEY_COL0, KEY_ROW0, KEY_COL1 to ALT2 (AUD5_TXC/TXD/TXFS)
  // DSP_DAT19 is set to ALT3 for AUD5_RXD.
  ImxPadConfig (IMX_PAD_KEY_COL0, IMX_PAD_CFG_KEY_COL0_AUD5_TXC);
  ImxPadConfig (IMX_PAD_KEY_ROW0, IMX_PAD_CFG_KEY_ROW0_AUD5_TXD);
  ImxPadConfig (IMX_PAD_KEY_COL1, IMX_PAD_CFG_KEY_COL1_AUD5_TXFS);
  ImxPadConfig (IMX_PAD_DISP0_DAT19, IMX_PAD_CFG_DISP0_DAT19_AUD5_RXD);

  // Set up Audmux where Ssi port = 7 and external port = 5
  InitializeAudmux (AUDMUX_PORT7, AUDMUX_PORT5);
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
  Initialize SDHC modules on the SOC and perform required pin-muxing
**/
VOID
SdhcInit (
  VOID
  )
{
  // uSDHC2: SDCard Socket
  ImxPadConfig (IMX_PAD_SD2_CLK, IMX_PAD_CFG_SD2_CLK_SD2_CLK);
  ImxPadConfig (IMX_PAD_SD2_CMD, IMX_PAD_CFG_SD2_CMD_SD2_CMD);
  ImxPadConfig (IMX_PAD_SD2_DAT0, IMX_PAD_CFG_SD2_DAT0_SD2_DATA0);
  ImxPadConfig (IMX_PAD_SD2_DAT1, IMX_PAD_CFG_SD2_DAT1_SD2_DATA1);
  ImxPadConfig (IMX_PAD_SD2_DAT2, IMX_PAD_CFG_SD2_DAT2_SD2_DATA2);
  ImxPadConfig (IMX_PAD_SD2_DAT3, IMX_PAD_CFG_SD2_DAT3_SD2_DATA3);
  ImxPadConfig (IMX_PAD_GPIO_4, IMX_PAD_CFG_GPIO_4_SD2_CD_B);
  ImxPadConfig (IMX_PAD_KEY_ROW1, IMX_PAD_CFG_KEY_ROW1_SD2_VSELECT);

#if DEBUG
  DEBUG ((DEBUG_INIT, "uSDHC2 PAD Config:\n"));
  ImxPadDumpConfig ("CLK", IMX_PAD_SD2_CLK);
  ImxPadDumpConfig ("CMD", IMX_PAD_SD2_CMD);
  ImxPadDumpConfig ("DAT0", IMX_PAD_SD2_DAT0);
  ImxPadDumpConfig ("DAT1", IMX_PAD_SD2_DAT1);
  ImxPadDumpConfig ("DAT2", IMX_PAD_SD2_DAT2);
  ImxPadDumpConfig ("DAT3", IMX_PAD_SD2_DAT3);
  ImxPadDumpConfig ("VSELECT", IMX_PAD_KEY_ROW1);
  ImxPadDumpConfig ("CD", IMX_PAD_GPIO_4);
#endif // DEBUG

  // Power-on SDCard through the GPIO signal connected to
  // Q1 MOSFET Gate acting as a switch to SDCard VDD supply voltage
  ImxPadConfig (IMX_PAD_DISP0_DAT9, IMX_PAD_CFG_DISP0_DAT9_GPIO4_IO30);
  ImxGpioDirection (IMX_GPIO_BANK4, 30, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK4, 30, IMX_GPIO_LOW);

  // Pre-configure the GPIO ALT5 of all SD2_* pads to be input pins.
  // SD2_CLK: GPIO1_IO10
  ImxGpioDirection (IMX_GPIO_BANK1, 10, IMX_GPIO_DIR_INPUT);

  // SD2_CMD: GPIO1_IO11
  ImxGpioDirection (IMX_GPIO_BANK1, 11, IMX_GPIO_DIR_INPUT);

  // SD2_DATA0: GPIO1_IO15
  ImxGpioDirection (IMX_GPIO_BANK1, 15, IMX_GPIO_DIR_INPUT);

  // SD2_DATA1: GPIO1_IO14
  ImxGpioDirection (IMX_GPIO_BANK1, 14, IMX_GPIO_DIR_INPUT);

  // SD2_DATA2: GPIO1_IO13
  ImxGpioDirection (IMX_GPIO_BANK1, 13, IMX_GPIO_DIR_INPUT);

  // SD2_DATA3: GPIO1_IO12
  ImxGpioDirection (IMX_GPIO_BANK1, 12, IMX_GPIO_DIR_INPUT);

  // uSDHC3: eMMC Socket
  ImxPadConfig (IMX_PAD_SD3_CLK, IMX_PAD_CFG_SD3_CLK_SD3_CLK);
  ImxPadConfig (IMX_PAD_SD3_CMD, IMX_PAD_CFG_SD3_CMD_SD3_CMD);
  ImxPadConfig (IMX_PAD_SD3_RST, IMX_PAD_CFG_SD3_RST_SD3_RST);
  ImxPadConfig (IMX_PAD_SD3_DAT0, IMX_PAD_CFG_SD3_DAT0_SD3_DATA0);
  ImxPadConfig (IMX_PAD_SD3_DAT1, IMX_PAD_CFG_SD3_DAT1_SD3_DATA1);
  ImxPadConfig (IMX_PAD_SD3_DAT2, IMX_PAD_CFG_SD3_DAT2_SD3_DATA2);
  ImxPadConfig (IMX_PAD_SD3_DAT3, IMX_PAD_CFG_SD3_DAT3_SD3_DATA3);
  ImxPadConfig (IMX_PAD_SD3_DAT4, IMX_PAD_CFG_SD3_DAT4_SD3_DATA4);
  ImxPadConfig (IMX_PAD_SD3_DAT5, IMX_PAD_CFG_SD3_DAT5_SD3_DATA5);
  ImxPadConfig (IMX_PAD_SD3_DAT6, IMX_PAD_CFG_SD3_DAT6_SD3_DATA6);
  ImxPadConfig (IMX_PAD_SD3_DAT7, IMX_PAD_CFG_SD3_DAT7_SD3_DATA7);
}

/**
  Initialize EHCI modules on the SOC and perform required pin-muxing.
  This routine also initializes PHY0(OTG) and PHY1 (USBH1).
**/
VOID
EhciInit (
  VOID
  )
{
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIoMuxGprReg;
  IMX_IOMUXC_GPR1_REG                 IoMuxGpr1Reg;

  // Pin-mux OTG Over Current
  ImxPadConfig (IMX_PAD_KEY_COL4, IMX_PAD_CFG_KEY_COL4_USB_OTG_OC);

  // Pin-mux and enable OTG power
  ImxPadConfig (IMX_PAD_EIM_D22, IMX_PAD_CFG_EIM_DATA22_GPIO3_IO22_USB_OTG_PWR);
  ImxGpioDirection (IMX_GPIO_BANK3, 22, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK3, 22, IMX_GPIO_HIGH);

  // Pin-mux and configure USB_OTG_ID as HOST
  ImxPadConfig (IMX_PAD_GPIO_1, IMX_PAD_CFG_GPIO01_USB_OTG_ID_HOST);

  // Configure USB_OTG_ID pin
  pIoMuxGprReg = (IMX_IOMUXC_GPR_REGISTERS *) IOMUXC_GPR_BASE_ADDRESS;
  IoMuxGpr1Reg = (IMX_IOMUXC_GPR1_REG) MmioRead32 ((UINTN)&pIoMuxGprReg->GPR1);
  IoMuxGpr1Reg.USB_OTG_ID_SEL = IMX_IOMUXC_GPR1_USB_OTG_ID_SEL_GPIO_1;
  MmioWrite32 ((UINTN)&pIoMuxGprReg->GPR1, IoMuxGpr1Reg.AsUint32);

  // Pin-mux and enable USBH1 power
  ImxPadConfig (IMX_PAD_GPIO_0, IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR);
  ImxGpioDirection (IMX_GPIO_BANK1, 0, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK1, 0, IMX_GPIO_HIGH);

  // Initialize PHY0 (OTG)
  ImxUsbPhyInit (IMX_USBPHY0);

  // Initialize PHY1 (USBH1)
  ImxUsbPhyInit (IMX_USBPHY1);
}

/**
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
VOID
I2cInit (
  VOID
  )
{
  // Enable 1.8V and 3.3V power rails for sensors connected to I2C1.
  // The SENSOR_PWR_EN on EIM_EB3 line powers the pullups on I2c1.
  ImxPadConfig (IMX_PAD_EIM_EB3, IMX_PAD_CFG_EIM_EB3_GPIO2_IO31);
  ImxGpioDirection (IMX_GPIO_BANK2, 31, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK2, 31, IMX_GPIO_HIGH); // set GPIO2_IO31 to 1
  DEBUG ((DEBUG_INFO, "%a: Mux IMX_PAD_EIM_EB3 to GPIO Alt5. Set GPIO2_IO31\n",
          __FUNCTION__));

  // Configure I2C1. EIM_D21 is I2C1_SCL and EIM_D28 is I2C1_SDA. Alt6, Alt1
  ImxPadConfig (IMX_PAD_EIM_D21, IMX_PAD_CFG_EIM_D21_I2C1_SCL);
  ImxPadConfig (IMX_PAD_EIM_D28, IMX_PAD_CFG_EIM_D28_I2C1_SDA);
  DEBUG ((DEBUG_INFO, "%a: I2C1 pin muxed via EIM D28,21\n", __FUNCTION__));

  // I2C2 is used by GOP/Windows display driver. Do not configure I2C2 SCL via KEY_COL3, SDA via KEY_ROW3

  // Configure I2C3 SCL via EIM_D17, SDA via EIM_D18. Inputs Alt6
  ImxPadConfig (IMX_PAD_EIM_D17, IMX_PAD_CFG_EIM_D17_I2C3_SCL);
  ImxPadConfig (IMX_PAD_EIM_D18, IMX_PAD_CFG_EIM_D18_I2C3_SDA);
  DEBUG ((DEBUG_INFO, "%a: I2C3 pin muxed EIM_D17, 18\n", __FUNCTION__));
}

/**
  Initialize SPI modules on the SOC and perform required pin-muxing
**/
VOID
SpiInit (
  VOID
  )
{
  // On Hummingboard only ESPI2 is exposed.

  ImxPadConfig (IMX_PAD_EIM_RW, IMX_PAD_CFG_SPI2_SS0);
  ImxPadConfig (IMX_PAD_EIM_LBA, IMX_PAD_CFG_SPI2_SS1);
}

/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID
PcieInit (
  VOID
  )
{
  // PCIe GPIO Reset
  ImxPadConfig (IMX_PAD_SD4_DAT3, IMX_PAD_CFG_SD4_DATA3_GPIO2_IO11);
  ImxGpioDirection (IMX_GPIO_BANK2, 11, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK2, 11, IMX_GPIO_LOW);
}

/**
  Initialize exposed GPIO pins on carrier board. By default some
  pins are set to CMOS inputs while others are set to Schmitt
  triggers.  Normalize them all to Schmitt trigger inputs by setting
  the hysteresis bit (16) in the pad ctl register.
**/
VOID
GpioInit (
  VOID
  )
{
  UINT32  i;
  IMX_PAD CurPad;

  for (i = 0; i < sizeof (GpioPins) / sizeof (IMX_PAD); ++i) {
    CurPad = GpioPins[i];
    MmioOr32 (IMX_IOMUXC_BASE + IMX_IOMUX_PAD_CTL_OFFSET (CurPad), 1 << 16);
  }
}

/**
  Initalize the PWM controllers
  PWM1 is exposed through Mikrobus header
  Pinmux pad DISP0_DAT8 to (ALT2) PWM1_OUT
**/
VOID
PwmInit (
  VOID
  )
{
  ImxPadConfig (IMX_PAD_DISP0_DAT8, IMX_PAD_CFG_DISP0_DAT8_PWM1_OUT);
}

/**
    Initalize the UART controllers
**/
VOID
UartInit (
  VOID
  )
{
  IMX_IOMUXC_SEL_INP_CTL Uart2RxSelInpReg;

  // UARTs share same Clock:
  // PLL3 (480 MHz) -> pll3_sw_clk -> CG -> /6 -> uart_clk_root = 80 MHz
  ImxClkPwrSetClockGates (
    UartClockGatesToTurnOn,
    sizeof (UartClockGatesToTurnOn) / sizeof (UartClockGatesToTurnOn[0]),
    IMX_CLOCK_GATE_STATE_ON);

  // Configure pin mux for UART 1,2,3
  ImxPadConfig (IMX_PAD_CSI0_DAT11, IMX_PAD_UART1_RX_DATA);
  ImxPadConfig (IMX_PAD_CSI0_DAT10, IMX_PAD_UART1_TX_DATA);
  ImxPadConfig (IMX_PAD_SD4_DAT4, IMX_PAD_UART2_RX_DATA);
  ImxPadConfig (IMX_PAD_SD4_DAT7, IMX_PAD_UART2_TX_DATA);
  // Note: UART2 requires an extra step to manually initialize SELECT_INPUT.
  // This is because the Imx Pad Macro cannot handle SELECT_INPUT values of
  // 4 or higher due to int overflow.
  Uart2RxSelInpReg = (IMX_IOMUXC_SEL_INP_CTL) MmioRead32 (
                                (UINTN)IOMUXC_UART2_UART_RX_DATA_SELECT_INPUT);
  Uart2RxSelInpReg.Fields.DAISY = SD4_DATA4_ALT2;
  MmioWrite32 ((UINTN)IOMUXC_UART2_UART_RX_DATA_SELECT_INPUT, Uart2RxSelInpReg.AsUint32);
  ImxPadConfig (IMX_PAD_EIM_D25, IMX_PAD_UART3_RX_DATA);
  ImxPadConfig (IMX_PAD_EIM_D24, IMX_PAD_UART3_TX_DATA);

  SerialPortInitialize ();
  SerialPortWrite (
    (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
    (UINTN)sizeof (SERIAL_DEBUG_PORT_INIT_MSG));
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

  // initalize backlight pin
  ImxPadConfig (IMX_PAD_SD4_DAT1, IMX_PAD_CFG_SD4_DATA1_GPIO2_IO09);
  ImxGpioDirection (IMX_GPIO_BANK2, 9, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK2, 9, IMX_GPIO_HIGH);
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

  SdhcInit ();
  EhciInit ();
  EnetInit ();
  I2cInit ();
  SpiInit ();
  PcieInit ();
  SetupAudio ();
  GpioInit ();
  PwmInit ();

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
