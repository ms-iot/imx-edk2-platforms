/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
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

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include "iMX6SxClkPwr_private.h"
#include <imx6sxdef.inc>

static IMX_CLOCK_TREE_CACHE mImxpClockPwrCache;   // Cached clock value

/**
  Get the CCGR register index and gate number for a clock gate.
**/
IMX_CCGR_INDEX ImxpCcgrIndexFromClkGate (IMX_CLK_GATE ClockGate)
{
  static const IMX_CCGR_INDEX ImxpCcgrIndexMap[] = {
    {0, 0},  // IMX_AIPS_TZ1_CLK_ENABLE
    {0, 1},  // IMX_AIPS_TZ2_CLK_ENABLE
    {0, 2},  // IMX_APBHDMA_HCLK_ENABLE
    {0, 3},  // IMX_ASRC_CLK_ENABLE
    {0, 4},  // IMX_CAAM_SECURE_MEM_CLK_ENABLE
    {0, 5},  // IMX_CAAM_WRAPPER_ACLK_ENABLE
    {0, 6},  // IMX_CAAM_WRAPPER_IPG_ENABLE
    {0, 7},  // IMX_CAN1_CLK_ENABLE
    {0, 8},  // IMX_CAN1_SERIAL_CLK_ENABLE
    {0, 9},  // IMX_CAN2_CLK_ENABLE
    {0, 10}, // IMX_CAN2_SERIAL_CLK_ENABLE
    {0, 11}, // IMX_ARM_DBG_CLK_ENABLE
    {0, 12}, // IMX_DCIC1_CLK_ENABLE
    {0, 13}, // IMX_DCIC2_CLK_ENABLE
    {0, 15}, // IMX_AIPS_TZ3_CLK_ENABLE
    {1, 0},  // IMX_ECSPI1_CLK_ENABLE
    {1, 1},  // IMX_ECSPI2_CLK_ENABLE
    {1, 2},  // IMX_ECSPI3_CLK_ENABLE
    {1, 3},  // IMX_ECSPI4_CLK_ENABLE
    {1, 4},  // IMX_ECSPI5_CLK_ENABLE
    {1, 6},  // IMX_EPIT1_CLK_ENABLE
    {1, 7},  // IMX_EPIT2_CLK_ENABLE
    {1, 8},  // IMX_ESAI_CLK_ENABLE
    {1, 9},  // IMX_WAKEUP_CLK_ENABLE
    {1, 10}, // IMX_GPT_CLK_ENABLE
    {1, 11}, // IMX_GPT_SERIAL_CLK_ENABLE
    {1, 13}, // IMX_GPU_CLK_ENABLE
    {1, 14}, // IMX_OCRAM_S_CLK_ENABLE
    {1, 15}, // IMX_CANFD_CLK_ENABLE
    {2, 1},  // IMX_CSI_CLK_ENABLE
    {2, 3},  // IMX_I2C1_SERIAL_CLK_ENABLE
    {2, 4},  // IMX_I2C2_SERIAL_CLK_ENABLE
    {2, 5},  // IMX_I2C3_SERIAL_CLK_ENABLE
    {2, 6},  // IMX_IIM_CLK_ENABLE
    {2, 7},  // IMX_IOMUX_IPT_CLK_IO_ENABLE
    {2, 8},  // IMX_IPMUX1_CLK_ENABLE
    {2, 9},  // IMX_IPMUX2_CLK_ENABLE
    {2, 10}, // IMX_IPMUX3_CLK_ENABLE
    {2, 11}, // IMX_IPSYNC_IP2APB_TZASC1_IPG_MASTER_CLK_ENABLE
    {2, 14}, // IMX_LCD_CLK_ENABLE
    {2, 15}, // IMX_PXP_CLK_ENABLE
    {3, 1},  // IMX_M4_CLK_ENABLE
    {3, 2},  // IMX_ENET_CLK_ENABLE
    {3, 3},  // IMX_DISP_AXI_CLK_ENABLE
    {3, 4},  // IMX_LCDIF2_PIX_CLK_ENABLE
    {3, 5},  // IMX_LCDIF1_PIX_CLK_ENABLE
    {3, 6},  // IMX_LDB_DI0_CLK_ENABLE
    {3, 7},  // IMX_QSPI1_CLK_ENABLE
    {3, 9},  // IMX_MLB_CLK_ENABLE
    {3, 10}, // IMX_MMDC_CORE_ACLK_FAST_CORE_P0_ENABLE
    {3, 12}, // IMX_MMDC_CORE_IPG_CLK_P0_ENABLE
    {3, 13}, // IMX_MMDC_CORE_IPG_CLK_P1_ENABLE
    {3, 14}, // IMX_OCRAM_CLK_ENABLE
    {4, 0},  // IMX_PCIE_ROOT_ENABLE
    {4, 5},  // IMX_QSPI2_CLK_ENABLE
    {4, 6},  // IMX_PL301_MX6QPER1_BCHCLK_ENABLE
    {4, 7},  // IMX_PL301_MX6QPER2_MAINCLK_ENABLE
    {4, 8},  // IMX_PWM1_CLK_ENABLE
    {4, 9},  // IMX_PWM2_CLK_ENABLE
    {4, 10}, // IMX_PWM3_CLK_ENABLE
    {4, 11}, // IMX_PWM4_CLK_ENABLE
    {4, 12}, // IMX_RAWNAND_U_BCH_INPUT_APB_CLK_ENABLE
    {4, 13}, // IMX_RAWNAND_U_GPMI_BCH_INPUT_BCH_CLK_ENABLE
    {4, 14}, // IMX_RAWNAND_U_GPMI_BCH_INPUT_GPMI_IO_CLK_ENABLE
    {4, 15}, // IMX_RAWNAND_U_GPMI_INPUT_APB_CLK_ENABLE
    {5, 0},  // IMX_ROM_CLK_ENABLE
    {5, 3},  // IMX_SDMA_CLK_ENABLE
    {5, 6},  // IMX_SPBA_CLK_ENABLE
    {5, 7},  // IMX_SPDIF_AND_AUDIO_CLK_ENABLE
    {5, 9},  // IMX_SSI1_CLK_ENABLE
    {5, 10}, // IMX_SSI2_CLK_ENABLE
    {5, 11}, // IMX_SSI3_CLK_ENABLE
    {5, 12}, // IMX_UART_CLK_ENABLE
    {5, 13}, // IMX_UART_SERIAL_CLK_ENABLE
    {5, 14}, // IMX_SAI1_CLK_ENABLE
    {5, 15}, // IMX_SAI2_CLK_ENABLE
    {6, 0},  // IMX_USBOH3_CLK_ENABLE
    {6, 1},  // IMX_USDHC1_CLK_ENABLE
    {6, 2},  // IMX_USDHC2_CLK_ENABLE
    {6, 3},  // IMX_USDHC3_CLK_ENABLE
    {6, 4},  // IMX_USDHC4_CLK_ENABLE
    {6, 5},  // IMX_EIM_SLOW_CLK_ENABLE
    {6, 8},  // IMX_PWM8_CLK_ENABLE
    {6, 10}, // IMX_VADC_CLK_ENABLE
    {6, 11}, // IMX_GIS_CLK_ENABLE
    {6, 12}, // IMX_I2C4_SERIAL_CLK_ENABLE
    {6, 13}, // IMX_PWM5_CLK_ENABLE
    {6, 14}, // IMX_PWM6_CLK_ENABLE
    {6, 15}, // IMX_PWM7_CLK_ENABLE
  }; // Gate 0: SX RM.p850
     // Gate 1: SX RM.p852
     // Gate 2: SX RM.p853
     // Gate 3: SX RM.p855
     // Gate 4: SX RM.p856
     // Gate 5: SX RM.p857
     // Gate 6: SX RM.p859

  return ImxpCcgrIndexMap[ClockGate];
}

CONST CHAR16 *StringFromImxClk (IMX_CLK Value)
{
  switch (Value) {
  case IMX_CLK_NONE: return L"(none)";
  case IMX_OSC_CLK: return L"OSC_CLK";
  case IMX_PLL1_MAIN_CLK: return L"PLL1_MAIN_CLK";
  case IMX_PLL2_MAIN_CLK: return L"PLL2_MAIN_CLK";
  case IMX_PLL2_PFD0: return L"PLL2_PFD0";
  case IMX_PLL2_PFD1: return L"PLL2_PFD1";
  case IMX_PLL2_PFD2: return L"PLL2_PFD2";
  case IMX_PLL2_PFD3: return L"PLL2_PFD3";
  case IMX_PLL3_MAIN_CLK: return L"PLL3_MAIN_CLK";
  case IMX_PLL3_PFD0: return L"PLL3_PFD0";
  case IMX_PLL3_PFD1: return L"PLL3_PFD1";
  case IMX_PLL3_PFD2: return L"PLL3_PFD2";
  case IMX_PLL3_PFD3: return L"PLL3_PFD3";
  case IMX_PLL4_MAIN_CLK: return L"PLL4_MAIN_CLK";
  case IMX_PLL5_MAIN_CLK: return L"PLL5_MAIN_CLK";
  case IMX_CLK1: return L"CLK1";
  case IMX_CLK2: return L"CLK2";

  case IMX_PLL1_SW_CLK: return L"PLL1_SW_CLK";
  case IMX_STEP_CLK: return L"STEP_CLK";
  case IMX_PLL3_SW_CLK: return L"PLL3_SW_CLK";

  case IMX_PERIPH_CLK2: return L"PERIPH_CLK2";
  case IMX_PERIPH_CLK: return L"_PERIPH_CLK";
  case IMX_PRE_PERIPH_CLK: return L"PRE_PERIPH_CLK";

  case IMX_ARM_CLK_ROOT: return L"ARM_CLK_ROOT";
  case IMX_MMDC_CLK_ROOT: return L"MMDC_CLK_ROOT";
  case IMX_FABRIC_CLK_ROOT: return L"FABRIC_CLK_ROOT";
  case IMX_OCRAM_CLK_ROOT: return L"OCRAM_CLK_ROOT";
  case IMX_PCIE_CLK_ROOT: return L"PCIE_CLK_ROOT";
  case IMX_AHB_CLK_ROOT: return L"AHB_CLK_ROOT";
  case IMX_PERCLK_CLK_ROOT: return L"PERCLK_CLK_ROOT";
  case IMX_IPG_CLK_ROOT: return L"IPG_CLK_ROOT";
  case IMX_USDHC1_CLK_ROOT: return L"USDHC1_CLK_ROOT";
  case IMX_USDHC2_CLK_ROOT: return L"USDHC2_CLK_ROOT";
  case IMX_USDHC3_CLK_ROOT: return L"USDHC3_CLK_ROOT";
  case IMX_USDHC4_CLK_ROOT: return L"USDHC4_CLK_ROOT";
  case IMX_ACLK_EIM_SLOW_CLK_ROOT: return L"ACLK_EIM_SLOW_CLK_ROOT";
  case IMX_GPU_AXI_CLK_ROOT: return L"GPU_AXI_CLK_ROOT";
  case IMX_GPU_CORE_CLK_ROOT: return L"GPU_CORE_CLK_ROOT";
  case IMX_VID_CLK_ROOT: return L"VID_CLK_ROOT";
  case IMX_ESAI_CLK_ROOT: return L"ESAI_CLK_ROOT";
  case IMX_AUDIO_CLK_ROOT: return L"AUDIO_CLK_ROOT";
  case IMX_SPDIF0_CLK_ROOT: return L"SPDIF0_CLK_ROOT";
  case IMX_SSI1_CLK_ROOT: return L"SSI1_CLK_ROOT";
  case IMX_SSI2_CLK_ROOT: return L"SSI2_CLK_ROOT";
  case IMX_SSI3_CLK_ROOT: return L"SSI3_CLK_ROOT";
  case IMX_LCDIF2_PIX_CLK_ROOT: return L"LCDIF2_PIX_CLK_ROOT";
  case IMX_LCDIF1_PIX_CLK_ROOT: return L"LCDIF1_PIX_CLK_ROOT";
  case IMX_LVDS_CLK_ROOT: return L"LVDS_CLK_ROOT";
  case IMX_M4_CLK_ROOT: return L"M4_CLK_ROOT";
  case IMX_ENET_CLK_ROOT: return L"ENET_CLK_ROOT";
  case IMX_QSPI1_CLK_ROOT: return L"QSPI1_CLK_ROOT";
  case IMX_QSPI2_CLK_ROOT: return L"QSPI2_CLK_ROOT";
  case IMX_DISPLAY_CLK_ROOT: return L"DISPLAY_CLK_ROOT";
  case IMX_CSI_CLK_ROOT: return L"CSI_CLK_ROOT";
  case IMX_CAN_CLK_ROOT: return L"CAN_CLK_ROOT";
  case IMX_ECSPI_CLK_ROOT: return L"ECSPI_CLK_ROOT";
  case IMX_UART_CLK_ROOT: return L"UART_CLK_ROOT";
  default:
    ASSERT (FALSE);
    return L"[invalid IMX_CLK value]";
  }
}

IMX_CLK ImxpClkFromBypassClkSource (IMX_PLL_BYPASS_CLK_SRC BypassClockSource)
{
  switch (BypassClockSource) {
  case IMX_PLL_BYPASS_CLK_SRC_REF_CLK_24M:
    return IMX_OSC_CLK;
  case IMX_PLL_BYPASS_CLK_SRC_CLK1:
    return IMX_CLK1;
  case IMX_PLL_BYPASS_CLK_SRC_GPANAIO:
  case IMX_PLL_BYPASS_CLK_SRC_CHRG_DET_B:
  default:
    ASSERT (FALSE);
    return IMX_CLK_NONE;
  }
}


EFI_STATUS
ImxpGetPll2PfdClkInfo (
  IN OUT IMX_CLOCK_TREE_CACHE *Cache,
  IMX_PLL_PFD PfdIndex,
  OUT IMX_CLOCK_INFO *ClockInfo
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS *ccmAnalogRegisters =
      (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;

  IMX_CCM_PFD_528_REG pfd528Reg; pfd528Reg.AsUint32 =
      MmioRead32 ((UINTN) &ccmAnalogRegisters->PFD_528);

  UINT32 pfdFrac;
  switch (PfdIndex) {
  case IMX_PLL_PFD0:
    pfdFrac = pfd528Reg.PFD0_FRAC;
    break;
  case IMX_PLL_PFD1:
    pfdFrac = pfd528Reg.PFD1_FRAC;
    break;
  case IMX_PLL_PFD2:
    pfdFrac = pfd528Reg.PFD2_FRAC;
    break;
  case IMX_PLL_PFD3:
    pfdFrac = pfd528Reg.PFD3_FRAC;
    break;
  default:
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  IMX_CLOCK_INFO parentInfo;
  EFI_STATUS status = ImxpGetClockInfo (Cache, IMX_PLL2_MAIN_CLK, &parentInfo);
  if (EFI_ERROR (status)) {
    return status;
  }

  // The resulting frequency shall be 528*18/PFDn_FRAC
  // where PFD0_FRAC is in the range 12-35.
  ASSERT ((pfdFrac >= 12) && (pfdFrac <= 35));
  ClockInfo->Frequency = (UINT32) ((UINT64) parentInfo.Frequency * 18 / pfdFrac);
  ClockInfo->Parent = IMX_PLL2_MAIN_CLK;

  return EFI_SUCCESS;
} // ADD MACRO FOR SX, SX RM.p795


EFI_STATUS
ImxpGetPerclkClkRootInfo (
  IN OUT IMX_CLOCK_TREE_CACHE *Cache,
  OUT IMX_CLOCK_INFO *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

  IMX_CCM_CSCMR1_REG    cscmr1;  cscmr1.AsUint32 = 
    MmioRead32 ((UINTN) &ccmRegisters->CSCMR1);

  IMX_CLK parent;
  switch(cscmr1.perclk_clk_sel) {
   case IMX_CCM_PERCLK_CLK_SEL_IPG_CLK_ROOT:
    parent = IMX_IPG_CLK_ROOT;
    break;
   case IMX_CCM_PERCLK_CLK_SEL_OSC_CLK:
    parent = IMX_OSC_CLK;
    break;
  }

  IMX_CLOCK_INFO parentInfo;
  EFI_STATUS status = ImxpGetClockInfo (Cache, parent, &parentInfo);
  if (EFI_ERROR(status)) {
    return status;
  }

  ClockInfo->Frequency = parentInfo.Frequency / (1 + cscmr1.perclk_podf);
  ClockInfo->Parent = parent;

  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPeriphClk2Info (
  IN OUT IMX_CLOCK_TREE_CACHE *Cache,
  OUT IMX_CLOCK_INFO *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

  IMX_CCM_CBCMR_REG cbcmrReg; cbcmrReg.AsUint32 =
      MmioRead32 ((UINTN) &ccmRegisters->CBCMR);

  IMX_CLK parent;
  switch (cbcmrReg.periph_clk2_sel) {
  case IMX_CCM_PERIPH_CLK2_SEL_PLL3_SW_CLK:
    parent = IMX_PLL3_SW_CLK;
    break;
  case IMX_CCM_PERIPH_CLK2_SEL_OSC_CLK:
    parent = IMX_OSC_CLK;
    break;
  case IMX_CCM_PERIPH_CLK2_SEL_PLL2_BYPASS_CLK:
    parent = IMX_PLL2_MAIN_CLK;
    break;
  default:
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  IMX_CCM_CBCDR_REG cbcdrReg; cbcdrReg.AsUint32 =
      MmioRead32 ((UINTN) &ccmRegisters->CBCDR);

  IMX_CLOCK_INFO parentInfo;
  EFI_STATUS status = ImxpGetClockInfo (Cache, parent, &parentInfo);
  if (EFI_ERROR (status)) {
    return status;
  }

  ClockInfo->Frequency = parentInfo.Frequency / (1 + cbcdrReg.periph_clk2_podf);
  ClockInfo->Parent = parent;

  return EFI_SUCCESS;
} // ADD MACROS, SX RM.797

EFI_STATUS
ImxpGetPeriphClkInfo (
  IN OUT IMX_CLOCK_TREE_CACHE *Cache,
  OUT IMX_CLOCK_INFO *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

  IMX_CCM_CBCDR_REG cbcdrReg; cbcdrReg.AsUint32 =
      MmioRead32 ((UINTN) &ccmRegisters->CBCDR);

  IMX_CLK parent;

  // NOTE: periph_clk_sel is OR'd with PLL_bypass_en2 (from jtag) to
  //       produce the input value to the MUX. We assume PLL_bypass_en2 is 0.
  if (cbcdrReg.periph_clk_sel == 0) {
    parent = IMX_PRE_PERIPH_CLK;
  } else {
    ASSERT (cbcdrReg.periph_clk_sel == 1);
    parent = IMX_PERIPH_CLK2;
  }

  IMX_CLOCK_INFO parentInfo;
  EFI_STATUS status = ImxpGetClockInfo (Cache, parent, &parentInfo);
  if (EFI_ERROR (status)) {
    return status;
  }

  ClockInfo->Frequency = parentInfo.Frequency; // SX RM.p797
  ClockInfo->Parent = parent;

  return EFI_SUCCESS;
} // NO CHANGE, SX RM.797

EFI_STATUS
ImxpGetClockInfo (
  IN OUT IMX_CLOCK_TREE_CACHE *Cache,
  IN IMX_CLK ClockId,
  OUT IMX_CLOCK_INFO *ClockInfo
  )
{
  ASSERT (ClockId < ARRAYSIZE(Cache->Table));

  // First try to satisfy from cache
  {
    UINTN cacheValidBits = Cache->Valid[ClockId / _BITS_PER_UINTN];
    if (cacheValidBits & (1 << (ClockId % _BITS_PER_UINTN))) {
      *ClockInfo = Cache->Table[ClockId];
      return EFI_SUCCESS;
    }
  }

  EFI_STATUS status;
  switch (ClockId) {
  case IMX_OSC_CLK:
    ImxpGetOsc24ClkInfo (ClockInfo);
    status = EFI_SUCCESS;
    break;
  case IMX_PLL1_MAIN_CLK:
    status = ImxpGetPll1MainClkInfo (Cache, ClockInfo);
    break;
  case IMX_PLL2_MAIN_CLK:
    status = ImxpGetPll2MainClkInfo (Cache, ClockInfo);
    break;
  case IMX_PLL2_PFD0:
    status = ImxpGetPll2PfdClkInfo (Cache, IMX_PLL_PFD0, ClockInfo);
    break;
  case IMX_PLL2_PFD1:
    status = ImxpGetPll2PfdClkInfo (Cache, IMX_PLL_PFD1, ClockInfo);
    break;
  case IMX_PLL2_PFD2:
    status = ImxpGetPll2PfdClkInfo (Cache, IMX_PLL_PFD2, ClockInfo);
    break;
  case IMX_PLL3_MAIN_CLK:
    status = ImxpGetPll3MainClkInfo (Cache, ClockInfo);
    break;
  case IMX_PLL3_PFD0:
    status = ImxpGetPll3PfdClkInfo (Cache, IMX_PLL_PFD0, ClockInfo);
    break;
  case IMX_PLL3_PFD1:
    status = ImxpGetPll3PfdClkInfo (Cache, IMX_PLL_PFD1, ClockInfo);
    break;
  case IMX_PLL3_PFD2:
    status = ImxpGetPll3PfdClkInfo (Cache, IMX_PLL_PFD2, ClockInfo);
    break;
  case IMX_PLL3_PFD3:
    status = ImxpGetPll3PfdClkInfo (Cache, IMX_PLL_PFD3, ClockInfo);
    break;
  case IMX_PLL3_SW_CLK:
    status = ImxpGetPll3SwClkInfo (Cache, ClockInfo);
    break;
  case IMX_PERIPH_CLK2:
    status = ImxpGetPeriphClk2Info (Cache, ClockInfo);
    break;
  case IMX_PERIPH_CLK:
    status = ImxpGetPeriphClkInfo (Cache, ClockInfo);
    break;
  case IMX_PRE_PERIPH_CLK:
    status = ImxpGetPrePeriphClkInfo (Cache, ClockInfo);
    break;
  case IMX_ARM_CLK_ROOT:
    status = ImxpGetArmClkRootInfo (Cache, ClockInfo);
    break;
  case IMX_AHB_CLK_ROOT:
    status = ImxpGetAhbClkRootInfo (Cache, ClockInfo);
    break;
  case IMX_IPG_CLK_ROOT:
    status = ImxpGetIpgClkRootInfo (Cache, ClockInfo);
    break;
  case IMX_PERCLK_CLK_ROOT:
    status = ImxpGetPerclkClkRootInfo (Cache, ClockInfo);
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  if (EFI_ERROR (status)) {
    return status;
  }

  // Update the cache
  Cache->Table[ClockId] = *ClockInfo;
  Cache->Valid[ClockId / _BITS_PER_UINTN] |= (1 << (ClockId % _BITS_PER_UINTN));

  return EFI_SUCCESS;
}

//
// Public functions
//

EFI_STATUS ImxClkPwrClkOut1Enable (IMX_CLK Clock, UINT32 Divider)
{
  return EFI_UNSUPPORTED;
}

VOID ImxClkPwrClkOut1Disable ()
{
}

EFI_STATUS ImxClkPwrValidateClocks ()
{
  return EFI_UNSUPPORTED;
}

VOID ImxClkPwrLcdClockDisable (UINT32 LcdIfBaseAddr)
{
    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
    IMX_CCM_CCGR2_REG ccgr2;
    IMX_CCM_CCGR3_REG ccgr3;

    // gate the LCD pixel and AXI clocks CCGR3.CG5
    ccgr3.AsUint32 = MmioRead32((UINTN) &ccmRegisters->CCGR[3]);
    if (LcdIfBaseAddr == IMX6SX_PHYSADDR_LCDIF1) {
        ccgr3.lcdif1_pix_clk_enable = IMX6SX_CCM_CLOCK_OFF;
    } else {
        ccgr3.lcdif2_pix_clk_enable = IMX6SX_CCM_CLOCK_OFF;
    }
    ccgr3.disp_axi_clk_enable = IMX6SX_CCM_CLOCK_OFF;
    MmioWrite32((UINTN) &ccmRegisters->CCGR[3], ccgr3.AsUint32);

    ccgr2.AsUint32 = MmioRead32((UINTN) &ccmRegisters->CCGR[2]);
    ccgr2.lcd_clk_enable = IMX6SX_CCM_CLOCK_OFF;
    MmioWrite32((UINTN) &ccmRegisters->CCGR[2], ccgr2.AsUint32);
}

VOID ImxClkPwrLcdClockEnable (UINT32 LcdIfBaseAddr)
{
    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
    UINT32 value32;
    IMX_CCM_CCGR2_REG ccgr2;
    IMX_CCM_CCGR3_REG ccgr3;

    // Ungate the LCD pixel clock
    ccgr3.AsUint32 = MmioRead32((UINTN) &ccmRegisters->CCGR[3]);
    value32 = MmioRead32((UINTN) &ccmRegisters->CSCDR2);
    if (LcdIfBaseAddr == IMX6SX_PHYSADDR_LCDIF1) {
        value32 &= ~(0x07 << 15);
        ccgr3.lcdif1_pix_clk_enable = IMX6SX_RUN_ONLY;
    } else {
        value32 &= ~(0x07 << 0);
        ccgr3.lcdif2_pix_clk_enable = IMX6SX_RUN_ONLY;
    }
    MmioWrite32((UINTN) &ccmRegisters->CSCDR2, value32);
    ccgr3.disp_axi_clk_enable = IMX6SX_RUN_ONLY;
    MmioWrite32((UINTN) &ccmRegisters->CCGR[3], ccgr3.AsUint32);

    ccgr2.AsUint32 = MmioRead32((UINTN) &ccmRegisters->CCGR[2]);
    ccgr2.lcd_clk_enable = IMX6SX_RUN_ONLY;
    MmioWrite32((UINTN) &ccmRegisters->CCGR[2], ccgr2.AsUint32);
}

VOID ImxSetClockRatePLL5 (
  UINT32 TargetClockRate
  )
{
    IMX_CCM_PLL_VIDEO_CTRL_REG videoControl;
    volatile IMX_CCM_ANALOG_REGISTERS *analogRegisters = (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;

    //
    // set the divider for the source clock to the video PLL to divide by 1 
    //
    MmioWrite32((UINTN) &analogRegisters->MISC0_CLR, 0x80000000);

    //
    // fire up the video PLL to the correct frequency
    // before division
    //
    videoControl.AsUint32 = 0;
    videoControl.POST_DIV_SELECT = 0x03;
    videoControl.BYPASS = 0x01;
    videoControl.POWERDOWN = 0x01;
    videoControl.DIV_SELECT = 0x7f;
    MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO_CLR, videoControl.AsUint32);
   
 /*
   MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO_CLR,
        IMX6SX_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK |
        IMX6SX_ANALOG_PLL_VIDEO_BYPASS |
        IMX6SX_ANALOG_PLL_VIDEO_POWERDOWN |
        IMX6SX_ANALOG_PLL_VIDEO_DIV_SELECT_MASK);
*/

    //
    // PLL output frequency = (Reference Freq) * (DIV_SELECT + NUM / DENOM)
    //
    // Use clock rate as denominator for simple fractional calculation.
    // This way we just need to figure out the target clock rate ratio
    // to the 24MHz reference.
    //
    {
        IMX_CCM_PLL_VIDEO_CTRL_REG pllVideoCtrlReg; 
        UINT32 denom = IMX_REF_CLK_24M_FREQ;
        UINT32 divSelect = TargetClockRate / IMX_REF_CLK_24M_FREQ;
        UINT32 numerator = TargetClockRate % IMX_REF_CLK_24M_FREQ;
        
        pllVideoCtrlReg.AsUint32 = MmioRead32((UINTN) &analogRegisters->PLL_VIDEO);

        ASSERT (numerator < denom);
        ASSERT ((divSelect >= 27) && (divSelect <= 54));

        pllVideoCtrlReg.DIV_SELECT = divSelect;
        pllVideoCtrlReg.POST_DIV_SELECT = IMX_POST_DIV_SELECT_DIVIDE_1;

        DEBUG ((
            DEBUG_INFO,
            "PLL 5 divSelect (%d) numerator (%d) denom %d\n",
            divSelect,
            numerator,
            denom
            ));

        MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO, pllVideoCtrlReg.AsUint32);
        MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO_NUM, numerator);
        MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO_DENOM, denom);
    }

    // wait for PLL to lock
    do {
        videoControl.AsUint32 = MmioRead32((UINTN) &analogRegisters->PLL_VIDEO);
    } while (!(videoControl.LOCK));

    // enable the PLL output
    videoControl.AsUint32 = 0;
    videoControl.ENABLE = 1;
    MmioWrite32((UINTN) &analogRegisters->PLL_VIDEO_SET, videoControl.AsUint32);
}

EFI_STATUS 
ImxSetLcdIfClockRate (
    UINT32 LcdIfBaseAddr,
    UINT32 ClockRate
    )
{
    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
    BOOLEAN foundConfig = FALSE;
    UINT32 targetFreq;
    UINT32 preDivSelectCount;
    UINT32 postDivSelectCount;
    UINT32 preDividerLcdif[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    UINT32 postDividerLcdif[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    UINT32 value32;

    value32 = MmioRead32((UINTN) &ccmRegisters->CSCDR2);
    if (LcdIfBaseAddr == IMX6SX_PHYSADDR_LCDIF1) {
        if ((value32 & (0x07 << 9)) != 0) {
            return EFI_SUCCESS;
        }
    } else {
        if ((value32 & (0x07 << 0)) != 0) {
            return EFI_SUCCESS;
        }
    }

    for (preDivSelectCount = 0;
        preDivSelectCount < ARRAYSIZE (preDividerLcdif);
        ++preDivSelectCount) {

        for (postDivSelectCount = 0;
            postDivSelectCount < ARRAYSIZE (postDividerLcdif);
            ++postDivSelectCount) {

            targetFreq =
                ClockRate *
                preDividerLcdif[preDivSelectCount] *
                postDividerLcdif[postDivSelectCount] *
                1;

            //
            // The valid range for PLL loop divider is 27-54 so we
            // need to target freq need to fit within the valid range.
            //
            if ((targetFreq >= PLL5_MIN_FREQ) &&
                (targetFreq <= PLL5_MAX_FREQ)) {
                foundConfig = TRUE;
                break;
            }
        }

        if (foundConfig == TRUE) {
            break;
        }
    }

    if (foundConfig == FALSE) {
        DEBUG((DEBUG_ERROR, "No valid configuration found for clock rate %d\n", ClockRate));
        ASSERT(FALSE);
        return EFI_INVALID_PARAMETER;
    }

    DEBUG ((
        DEBUG_INFO,
        "PLL 5 setting (%d) Target Freq (%d) PreDiv %d PostDiv %d\n",
        ClockRate,
        targetFreq,
        preDividerLcdif[preDivSelectCount],
        postDividerLcdif[postDivSelectCount]
        ));

    ImxSetClockRatePLL5 (targetFreq);

    // turn off LCD clocks
    ImxClkPwrLcdClockDisable(LcdIfBaseAddr);

    //
    // select the video PLL in the LCDIF clock selector
    // and set the CDIF1_PRED value
    //
    value32 = MmioRead32((UINTN) &ccmRegisters->CSCDR2);
    if (LcdIfBaseAddr == IMX6SX_PHYSADDR_LCDIF1) {
        // Clear LCDIF1_CLK_SEL, LCDIF1_PRED and LCDIF1_PRE_CLK_SEL
        value32 &= ~0x0003FE00;
        // Set the predivider value and derive clock from PLL5
        value32 |= ((preDividerLcdif[preDivSelectCount] - 1) << 12) | (2 << 15);
    } else {
        // Clear LCDIF2_CLK_SEL, LCDIF1_PRED and LCDIF1_PRE_CLK_SEL
        value32 &= ~0x000001FF;
        // Set the predivider value and derive clock from PLL5
        value32 |= ((preDividerLcdif[preDivSelectCount] - 1) << 3) | (2 << 6);
    }
    MmioWrite32((UINTN) &ccmRegisters->CSCDR2, value32);


    if (LcdIfBaseAddr == IMX6SX_PHYSADDR_LCDIF1) {
        // set the post divider in CBCMR
        value32 = MmioRead32((UINTN) &ccmRegisters->CBCMR);
        // Clear LCDIF1_PODF
        value32 &= ~0x03800000;
        value32 |= ((postDividerLcdif[postDivSelectCount] - 1) << 23);
        MmioWrite32((UINTN) &ccmRegisters->CBCMR, value32);
    } else {
        // set the post divider in CSCMR1
        value32 = MmioRead32((UINTN) &ccmRegisters->CSCMR1);
        // Clear LCDIF2_PODF
        value32 &= ~0x00700000;
        value32 |= ((postDividerLcdif[postDivSelectCount] - 1) << 20);
        MmioWrite32((UINTN) &ccmRegisters->CSCMR1, value32);
    }

    // turn on LCD clocks
    ImxClkPwrLcdClockEnable(LcdIfBaseAddr);

    return EFI_SUCCESS;
}

/**
  Reset/invalidate the clock tree cache.

  The clock tree cache must be invalidated whenever the clock tree is modified,
  e.g. when changing PLL configuration, clock mux, or divider.

**/
VOID
ImxpClkPwrCacheReset (
  VOID
  )
{
  SetMem (&mImxpClockPwrCache.Valid, sizeof (mImxpClockPwrCache.Valid), 0);
}

/**
  Configure clock gating for the specified clock signal.

  @param[in]  ClockGate   Specific clock signal to configure.
  @param[in]  State       State to set the clock signal to.
**/
VOID
ImxClkPwrSetClockGate (
  IN  IMX_CLK_GATE          ClockGate,
  IN  IMX_CLOCK_GATE_STATE  State
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  UINTN                       EndBit;
  IMX_CCGR_INDEX              Index;
  UINTN                       StartBit;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

  // Extract register index
  Index = ImxpCcgrIndexFromClkGate (ClockGate);
  StartBit = Index.GateNumber * 2;
  EndBit = StartBit + 1;

  MmioBitFieldWrite32 (
    (UINTN) &pCcmRegisters->CCGR[Index.RegisterIndex],
    StartBit,
    EndBit,
    State);
}

/**
  Determine if gating TZASC1_IPG_MASTER_CLK should be skipped.
 **/
BOOLEAN
ImxClkPwrShouldSkipTZASC1 (
  VOID
  )
{
#if defined(CPU_IMX6D) || defined(CPU_IMX6Q)
  IMX_IOMUXC_GPR_REGISTERS  *IoMuxMmioBasePtr;
  UINTN                     IomuxGPR9;
#endif
  BOOLEAN                   Skip;

  Skip = FALSE;
#if defined(CPU_IMX6D) || defined(CPU_IMX6Q)
  IoMuxMmioBasePtr = (IMX_IOMUXC_GPR_REGISTERS *)IOMUXC_GPR_BASE_ADDRESS;

  IomuxGPR9 = MmioRead32 ((UINTN) &IoMuxMmioBasePtr->GPR9);
  if (IomuxGPR9 & IMX_IOMUXC_TZASC1_BYP) {
    // TZASC-1 is active.
    Skip = TRUE;
  }
#endif

  return Skip;
}

/**
  Determine if a clock gate should be skipped

  @param[in]  ClockGate   Specific clock signal to configure.
 **/
BOOLEAN
ImxClkPwrShouldSkipGate (
  IN  IMX_CLK_GATE  ClockGate
  )
{
  switch (ClockGate) {
  case IMX_IPSYNC_IP2APB_TZASC1_IPG_MASTER_CLK_ENABLE:
    return ImxClkPwrShouldSkipTZASC1 ();

  default:
    return FALSE;
  }
}

/**
  Set multiple clock signals to a given state.

  @param[in]  ClockGateList   Pointer to list of possible clock signals.
  @param[in]  ClockGateCount  Number of clock signals to gate.
  @param[in]  State           State to set the clock signal to.
**/
VOID
ImxClkPwrSetClockGates (
  CONST IMX_CLK_GATE *ClockGateList,
  UINTN ClockGateCount,
  IMX_CLOCK_GATE_STATE State
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  UINTN                       i;
  IMX_CCGR_INDEX              Index;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

  // Read all CCGR registers to local copy
  UINT32 ccgrRegisters[ARRAYSIZE (pCcmRegisters->CCGR)];
  for (i = 0; i < ARRAYSIZE (ccgrRegisters); ++i) {
    ccgrRegisters[i] = MmioRead32 ((UINTN) &pCcmRegisters->CCGR[i]);
  }

  // Compute new CCGR register values
  for (i = 0; i < ClockGateCount; ++i) {
    if (ImxClkPwrShouldSkipGate (ClockGateList[i])) {
      continue;
    }

    Index = ImxpCcgrIndexFromClkGate (ClockGateList[i]);
    ccgrRegisters[Index.RegisterIndex] =
      (ccgrRegisters[Index.RegisterIndex] & ~(0x3 << (2 * Index.GateNumber))) |
      (State << (2 * Index.GateNumber));
  }

  // Write back to registers
  for (i = 0; i < ARRAYSIZE (ccgrRegisters); ++i) {
    MmioWrite32 ((UINTN) &pCcmRegisters->CCGR[i], ccgrRegisters[i]);
  }
}

/**
  Get the current clock gating setting for the specified clock gate.

  @param[in]  ClockGate     Specific clock signal to fetch clock gate info from.
**/
IMX_CLOCK_GATE_STATE
ImxClkPwrGetClockGate (
  IN  IMX_CLK_GATE  ClockGate
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  UINTN                       EndBit;
  IMX_CCGR_INDEX              Index;
  UINTN                       StartBit;
  UINT32                      Value;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  Index = ImxpCcgrIndexFromClkGate (ClockGate);
  StartBit = Index.GateNumber * 2;
  EndBit = StartBit + 1;

  Value = MmioBitFieldRead32 (
            (UINTN) &pCcmRegisters->CCGR[Index.RegisterIndex],
            StartBit,
            EndBit);

  if ((Value != IMX_CCM_CCGR_OFF) &&
      (Value != IMX_CCM_CCGR_ON_RUN) &&
      (Value != IMX_CCM_CCGR_ON)) {
    ASSERT (FALSE);
  }

  return (IMX_CLOCK_GATE_STATE) Value;
}

EFI_STATUS
ImxpGetPll3MainClkInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegisters;
  IMX_CLK                             Parent;
  IMX_CLOCK_INFO                      ParentInfo;
  IMX_CCM_ANALOG_PLL_USB1_REG         PllUsb1Reg;
  EFI_STATUS                          Status;

  pCcmAnalogRegisters = (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;
  PllUsb1Reg.AsUint32 = MmioRead32 ((UINTN)&pCcmAnalogRegisters->PLL_USB1);
  Parent = ImxpClkFromBypassClkSource (PllUsb1Reg.BYPASS_CLK_SRC);
  Status = ImxpGetClockInfo (Cache, Parent, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PllUsb1Reg.DIV_SELECT == 0) {
    ClockInfo->Frequency = ParentInfo.Frequency * 20;
  } else {
    ClockInfo->Frequency = ParentInfo.Frequency * 22;
  }

  ClockInfo->Parent = Parent;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPll3PfdClkInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  IN      IMX_PLL_PFD           PfdIndex,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegisters;
  IMX_CLOCK_INFO                      ParentInfo;
  IMX_CCM_PFD_480_REG                 Pfd480Reg;
  UINT32                              PfdFrac;
  EFI_STATUS                          Status;

  pCcmAnalogRegisters = (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;
  Pfd480Reg.AsUint32 = MmioRead32 ((UINTN) &pCcmAnalogRegisters->PFD_480);
  switch (PfdIndex) {
  case IMX_PLL_PFD0:
    PfdFrac = Pfd480Reg.PFD0_FRAC;
    break;
  case IMX_PLL_PFD1:
    PfdFrac = Pfd480Reg.PFD1_FRAC;
    break;
  case IMX_PLL_PFD2:
    PfdFrac = Pfd480Reg.PFD2_FRAC;
    break;
  case IMX_PLL_PFD3:
    PfdFrac = Pfd480Reg.PFD3_FRAC;
    break;
  default:
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = ImxpGetClockInfo (Cache, IMX_PLL3_MAIN_CLK, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // The resulting frequency shall be 480*18/PFDn_FRAC
  // where PFD0_FRAC is in the range 12-35.
  ASSERT ((PfdFrac >= 12) && (PfdFrac <= 35));
  ClockInfo->Frequency = (UINT32) ((UINT64) ParentInfo.Frequency * 18 / PfdFrac);
  ClockInfo->Parent = IMX_PLL3_MAIN_CLK;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPll3SwClkInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  IMX_CCM_CCSR_REG            CcsrReg;
  IMX_CLK                     Parent;
  IMX_CLOCK_INFO              ParentInfo;
  EFI_STATUS                  Status;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  CcsrReg.AsUint32 = MmioRead32 ((UINTN) &pCcmRegisters->CCSR);
  if (CcsrReg.pll3_sw_clk_sel == IMX_CCM_PLL3_SW_CLK_SEL_PLL3_MAIN_CLK) {
    Parent = IMX_PLL3_MAIN_CLK;
  } else {
    ASSERT (CcsrReg.pll3_sw_clk_sel == IMX_CCM_PLL3_SW_CLK_SEL_PLL3_BYPASS_CLK);
    ASSERT (!"Not implemented");
    return EFI_UNSUPPORTED;
  }

  Status = ImxpGetClockInfo (Cache, Parent, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ClockInfo->Frequency = ParentInfo.Frequency;
  ClockInfo->Parent = Parent;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPll1MainClkInfo  (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegisters;
  IMX_CLK                             Parent;
  IMX_CLOCK_INFO                      ParentInfo;
  IMX_CCM_ANALOG_PLL_ARM_REG          PllArmReg;
  EFI_STATUS                          Status;

  pCcmAnalogRegisters = (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;
  PllArmReg.AsUint32 = MmioRead32 ((UINTN) &pCcmAnalogRegisters->PLL_ARM);
  Parent = ImxpClkFromBypassClkSource (PllArmReg.BYPASS_CLK_SRC);
  Status = ImxpGetClockInfo (Cache, Parent, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PllArmReg.BYPASS != 0) {
    ClockInfo->Frequency = ParentInfo.Frequency;
    ClockInfo->Parent = Parent;
    return EFI_SUCCESS;
  }

  ClockInfo->Frequency = (UINT32) ((UINT64) ParentInfo.Frequency * PllArmReg.DIV_SELECT / 2);
  ClockInfo->Parent = Parent;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPll2MainClkInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegisters;
  IMX_CLK                             Parent;
  IMX_CLOCK_INFO                      ParentInfo;
  IMX_CCM_ANALOG_PLL_SYS_REG          PllSysReg;
  EFI_STATUS                          Status;

  pCcmAnalogRegisters = (IMX_CCM_ANALOG_REGISTERS *) IMX_CCM_ANALOG_BASE;
  PllSysReg.AsUint32 = MmioRead32 ((UINTN) &pCcmAnalogRegisters->PLL_SYS);
  // Determine the reference clock source
  Parent = ImxpClkFromBypassClkSource (PllSysReg.BYPASS_CLK_SRC);
  Status = ImxpGetClockInfo (Cache, Parent, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PllSysReg.BYPASS != 0) {
    ClockInfo->Frequency = ParentInfo.Frequency;
    ClockInfo->Parent = Parent;
    return EFI_SUCCESS;
  }

  if (PllSysReg.DIV_SELECT == 0) {
    ClockInfo->Frequency = ParentInfo.Frequency * 20;
  } else {
    ASSERT (PllSysReg.DIV_SELECT == 1);
    ClockInfo->Frequency = ParentInfo.Frequency * 22;
  }

  ClockInfo->Parent = Parent;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetArmClkRootInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  IMX_CCM_CACRR_REG           CacrrReg;
  IMX_CLOCK_INFO              Pll1Info;
  EFI_STATUS                  Status;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  Status = ImxpGetClockInfo (Cache, IMX_PLL1_MAIN_CLK, &Pll1Info);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CacrrReg.AsUint32 = MmioRead32 ((UINTN) &pCcmRegisters->CACRR);
  ClockInfo->Frequency = Pll1Info.Frequency / (1 + CacrrReg.arm_podf);
  ClockInfo->Parent = IMX_PLL1_MAIN_CLK;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetPrePeriphClkInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  IMX_CCM_CBCMR_REG           CbcmrReg;
  IMX_CLK                     Parent;
  IMX_CLOCK_INFO              ParentInfo;
  EFI_STATUS                  Status;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  CbcmrReg.AsUint32 = MmioRead32 ((UINTN) &pCcmRegisters->CBCMR);
  switch (CbcmrReg.pre_periph_clk_sel) {
  case IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2:
    Parent = IMX_PLL2_MAIN_CLK;
    break;
  case IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD2:
    Parent = IMX_PLL2_PFD2;
    break;
  case IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD0:
    Parent = IMX_PLL2_PFD0;
    break;
  case IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD2_DIV2:
    Parent = IMX_PLL2_PFD2;
    break;
  default:
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = ImxpGetClockInfo (Cache, Parent, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CbcmrReg.pre_periph_clk_sel == IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD2_DIV2) {
    ClockInfo->Frequency = ParentInfo.Frequency / 2;
  } else {
    ClockInfo->Frequency = ParentInfo.Frequency;
  }

  ClockInfo->Parent = Parent;
  return EFI_SUCCESS;
}

VOID
ImxpGetOsc24ClkInfo (
  OUT IMX_CLOCK_INFO  *ClockInfo
  )
{
  ClockInfo->Frequency = IMX_REF_CLK_24M_FREQ;
  ClockInfo->Parent = IMX_CLK_NONE;
}

EFI_STATUS
ImxpGetAhbClkRootInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  IMX_CCM_CBCDR_REG           CbcdrReg;
  IMX_CLOCK_INFO              ParentInfo;
  EFI_STATUS                  Status;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  Status = ImxpGetClockInfo (Cache, IMX_PERIPH_CLK, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CbcdrReg.AsUint32 = MmioRead32 ((UINTN) &pCcmRegisters->CBCDR);
  ClockInfo->Frequency = ParentInfo.Frequency / (1 + CbcdrReg.ahb_podf);
  ClockInfo->Parent = IMX_PERIPH_CLK;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxpGetIpgClkRootInfo (
  IN OUT  IMX_CLOCK_TREE_CACHE  *Cache,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  )
{
  volatile IMX_CCM_REGISTERS  *pCcmRegisters;
  IMX_CCM_CBCDR_REG           CbcdrReg;
  IMX_CLOCK_INFO              ParentInfo;
  EFI_STATUS                  Status;

  pCcmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  Status = ImxpGetClockInfo (Cache, IMX_AHB_CLK_ROOT, &ParentInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CbcdrReg.AsUint32 = MmioRead32 ((UINTN) &pCcmRegisters->CBCDR);
  ClockInfo->Frequency = ParentInfo.Frequency / (1 + CbcdrReg.ipg_podf);
  ClockInfo->Parent = IMX_AHB_CLK_ROOT;
  return EFI_SUCCESS;
}

EFI_STATUS
ImxClkPwrGetClockInfo (
  IN  IMX_CLK         ClockId,
  OUT IMX_CLOCK_INFO  *ClockInfo
  )
{
  return ImxpGetClockInfo (&mImxpClockPwrCache, ClockId, ClockInfo);
}
