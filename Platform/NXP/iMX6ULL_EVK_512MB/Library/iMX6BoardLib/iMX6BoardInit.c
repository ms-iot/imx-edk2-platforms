/** @file
* iMX6 Sabre board initialization
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6Audio.h>
#include <iMX6BoardLib.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6UsbPhy.h>

// Prebaked pad configurations that include mux and drive settings where
// each enum named as IMX_<MODULE-NAME>_PADCFG contains configurations
// for pads used by that module

#define ENET_PAD_CTRL(alt) _IMX_MAKE_PADCFG(\
                              IMX_SRE_FAST,\
                              IMX_DSE_50_OHM,\
                              IMX_SPEED_MAXIMUM,\
                              IMX_ODE_DISABLE,\
                              IMX_PKE_DISABLE,\
                              IMX_PUE_PULL,\
                              IMX_PUS_100K_OHM_PU,\
                              IMX_HYS_DISABLED,\
                              IMX_SION_DISABLED,\
                              alt)

typedef enum {
    IMX_PAD_ENET1_MDIO_CTL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_50_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_ENABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO1_IO06_ALT0_ENET1_MDIO,
                              IOMUXC_USB_ENET1_MDIO_SELECT_INPUT,
                              GPIO1_IO06_ALT0),

    IMX_PAD_ENET1_REF_CLK_CTL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_DISABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_ENET1_TX_CLK_ALT4_ENET1_REF_CLK1,
                              IOMUXC_USB_ENET1_CLK_SELECT_INPUT,
                              ENET1_TX_CLK_ALT4),

    IMX_PAD_ENET1_MDC_CTL   = ENET_PAD_CTRL(IMX_IOMUXC_GPIO1_IO07_ALT0_ENET1_MDC),

    IMX_PAD_ENET1_TXD0_CTL  = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_TX_DATA0_ALT0_ENET1_TDATA00),

    IMX_PAD_ENET1_TXD1_CTL  = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_TX_DATA1_ALT0_ENET1_TDATA01),

    IMX_PAD_ENET1_TX_EN_CTL = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_TX_EN_ALT0_ENET1_TX_EN),

    IMX_PAD_ENET1_RX_ER_CTL = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_RX_ER_ALT0_ENET1_RX_ER),

    IMX_PAD_ENET1_RX_EN_CTL = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_RX_EN_ALT0_ENET1_RX_EN),

    IMX_PAD_ENET1_RXD0_CTL  = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_RX_DATA0_ALT0_ENET1_RDATA00),

    IMX_PAD_ENET1_RXD1_CTL  = ENET_PAD_CTRL(IMX_IOMUXC_ENET1_RX_DATA1_ALT0_ENET1_RDATA01), 

} IMX_ENET_PADCFG;

typedef enum {
    IMX_PAD_CFG_USB_OTG_PWR = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT2_USB_OTG),

    IMX_PAD_CFG_USB_OTG1_OC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT2_USB_OTG,
                              IOMUXC_USB_OTG1_OC_SELECT_INPUT,
                              0),
    IMX_PAD_CFG_USB_OTG2_OC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT2_USB_OTG,
                              IOMUXC_USB_OTG2_OC_SELECT_INPUT,
                              0),

    IMX_PAD_CFG_USB_OTG1_ID = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_90_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT2_USB_OTG,
                              IOMUXC_USB_OTG1_ID_SELECT_INPUT,
                              0),

} IMX_EHCI_PADCFG;

typedef enum {
  IMX_PAD_CFG_UART5_TX_I2C2_SCL = _IMX_MAKE_PADCFG_INPSEL (
                                  IMX_SRE_FAST,
                                  IMX_DSE_40_OHM,
                                  IMX_SPEED_MEDIUM,
                                  IMX_ODE_ENABLE,
                                  IMX_PKE_ENABLE,
                                  IMX_PUE_PULL,
                                  IMX_PUS_100K_OHM_PU,
                                  IMX_HYS_ENABLED,
                                  IMX_SION_ENABLED,
                                  IMX_IOMUXC_UART5_TX_DATA_ALT2_I2C2_SCL,
                                  IOMUXC_I2C2_SCL_SELECT_INPUT,
                                  UART5_TX_DATA_ALT2),

  IMX_PAD_CFG_UART5_RX_I2C2_SDA = _IMX_MAKE_PADCFG_INPSEL (
                                  IMX_SRE_FAST,
                                  IMX_DSE_40_OHM,
                                  IMX_SPEED_MEDIUM,
                                  IMX_ODE_ENABLE,
                                  IMX_PKE_ENABLE,
                                  IMX_PUE_PULL,
                                  IMX_PUS_100K_OHM_PU,
                                  IMX_HYS_ENABLED,
                                  IMX_SION_ENABLED,
                                  IMX_IOMUXC_UART5_RX_DATA_ALT2_I2C2_SDA,
                                  IOMUXC_I2C2_SDA_SELECT_INPUT,
                                  UART5_RX_DATA_ALT2),
} IMX_I2C_PADCFG;

VOID EnetInit ()
{
  // Apply ENET pin-muxing configurations
  ImxPadConfig(IMX_PAD_ENET1_MDIO      , IMX_PAD_ENET1_MDIO_CTL       );
  ImxPadConfig(IMX_PAD_ENET1_MDC       , IMX_PAD_ENET1_MDC_CTL        );
  ImxPadConfig(IMX_PAD_ENET1_TXD0      , IMX_PAD_ENET1_TXD0_CTL       );
  ImxPadConfig(IMX_PAD_ENET1_TXD1      , IMX_PAD_ENET1_TXD1_CTL       );
  ImxPadConfig(IMX_PAD_ENET1_TX_EN     , IMX_PAD_ENET1_TX_EN_CTL      );
  ImxPadConfig(IMX_PAD_ENET1_RX_ER     , IMX_PAD_ENET1_RX_ER_CTL      );
  ImxPadConfig(IMX_PAD_ENET1_RX_EN     , IMX_PAD_ENET1_RX_EN_CTL      );
  ImxPadConfig(IMX_PAD_ENET1_RXD0      , IMX_PAD_ENET1_RXD0_CTL       );
  ImxPadConfig(IMX_PAD_ENET1_RXD1      , IMX_PAD_ENET1_RXD1_CTL       );

  // Enable ENET PLL
  {
    UINT32 counter = 0;
    IMX_CCM_ANALOG_PLL_ENET_REG ccmAnalogPllReg;
    volatile IMX_CCM_ANALOG_REGISTERS *ccmAnalogRegistersPtr =
      (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;

    ccmAnalogPllReg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);

    ccmAnalogPllReg.POWERDOWN = 0;
    ccmAnalogPllReg.ENET1_125M_EN = 1;
    ccmAnalogPllReg.ENET1_DIV_SELECT = PLL_ENET_DIV_SELECT_50MHZ;

    MmioWrite32(
        (UINTN)&ccmAnalogRegistersPtr->PLL_ENET,
        ccmAnalogPllReg.AsUint32);

    do {
      ccmAnalogPllReg.AsUint32 =
        MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);

      MicroSecondDelay(100);
      ++counter;
    } while ((ccmAnalogPllReg.LOCK == 0) && (counter < 100));

    if (ccmAnalogPllReg.LOCK == 0) {
      DEBUG ((DEBUG_ERROR, "PLL_ENET is not locked!\n"));
    }
  }
}

VOID SetupAudio ()
{
}

/**
  Initialize clock and power for modules on the SOC.
**/
VOID ImxClkPwrInit ()
{
}

/**
  Initialize SDHC modules on the SOC and perform required pin-muxing
**/
VOID SdhcInit ()
{
}

/**
  This routine initializes PHY0(OTG).
**/
VOID EhciInit ()
{
  DEBUG ((EFI_D_INIT, "++EhciInit() S\r\n"));
  // Pin-mux and configure USB_OTG_ID as HOST!
  ImxPadConfig (IMX_PAD_GPIO_0, IMX_PAD_CFG_USB_OTG1_ID);
  DEBUG ((EFI_D_INIT, "S EhciInit( )  Pin-muxed and configured USB_OTG_ID as HOST\r\n"));

  // Initialize PHY0 (OTG)
  ImxUsbPhyInit (IMX_USBPHY0);

  // Initialize PHY1 (USBH1)
  ImxUsbPhyInit (IMX_USBPHY1);
}

/**
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
VOID I2cInit ()
{
  // Configure I2C2
  ImxPadConfig (IMX_PAD_UART5_TX_DATA, IMX_PAD_CFG_UART5_TX_I2C2_SCL); // sion 1
  ImxPadConfig (IMX_PAD_UART5_RX_DATA, IMX_PAD_CFG_UART5_RX_I2C2_SDA); // sion 1
}

/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
}
