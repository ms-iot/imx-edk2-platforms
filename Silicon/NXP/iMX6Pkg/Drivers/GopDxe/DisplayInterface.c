/** @file
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

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMXDisplay.h>

#include "Ipu.h"
#include "Display.h"
#include "DisplayInterface.h"

#ifdef DEBUG
VOID
DumpBasicDisplayInterfaceReg (
  IN  VOID          *IpuMmioBasePtr,
  IN  IPU_DIx_REGS  *IpuDiRegsPtr
  )
{
  UINT32 index, setNumber, regVal;
  UINT32 printTotalGen = 8; // Limit printing (max 12)

  DEBUG ((DEBUG_VERBOSE, "%a: ------- DI Register Dump -------\n", __FUNCTION__));
  // Print out generator value for D0
  DEBUG ((DEBUG_VERBOSE, "%a: ## Wave Gen\n", __FUNCTION__));
  for (index = 0; index < printTotalGen; ++index) {
    regVal = READ_WAVE_GEN (IpuDiRegsPtr, index);
    DEBUG ((DEBUG_VERBOSE, "%a: DI0_DW_GEN_%d 0x%08x\n",
      __FUNCTION__, index, regVal));
  }
  // Print out generator value for D0
  DEBUG ((DEBUG_VERBOSE, "%a: ## Wave Set\n", __FUNCTION__));
  for (index = 0; index < printTotalGen; ++index) {
    for (setNumber = 0; setNumber < 4; ++setNumber) {
      regVal = READ_WAVE_SET (IpuDiRegsPtr, index, setNumber);
      DEBUG ((DEBUG_VERBOSE, "%a: DI0_DW_SET%d_%d 0x%08x\n",
        __FUNCTION__, setNumber, index, regVal));
    }
  }

  regVal = IpuRead32 (IpuMmioBasePtr, IPU_IPU_PM_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_IPU_PM_OFFSET %x\n",
    __FUNCTION__, regVal));

  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_BS_CLKGEN0_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_BS_CLKGEN0_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_BS_CLKGEN1_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_BS_CLKGEN1_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SCR_CONF_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SCR_CONF_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN0_1_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN0_1_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN1_1_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN1_1_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN0_2_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN0_2_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN1_2_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN1_2_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN0_3_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN0_3_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN1_3_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN1_3_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN0_4_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN0_4_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN1_4_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN1_4_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN0_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN0_5_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SW_GEN1_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SW_GEN1_5_OFFSET %x\n",
    __FUNCTION__, regVal));

  for (index = 0; index < 5; ++index) {
    regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_STP_REP_OFFSET +  (index * 0x4));
    DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_STP_%d_REP_OFFSET %x\n",
      __FUNCTION__, index + 1, regVal));
  }

  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_SYNC_AS_GEN_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_SYNC_AS_GEN_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_GENERAL_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_GENERAL_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_DIx_POL_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DIx_POL_OFFSET %x\n",
    __FUNCTION__, regVal));
  regVal = DiRead32 (IpuDiRegsPtr, IPU_IPU_DISP_GEN_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_IPU_DISP_GEN_OFFSET %x\n",
    __FUNCTION__, regVal));
  DEBUG ((DEBUG_VERBOSE, "%a: ------------------------------\n\n", __FUNCTION__));
}
#endif /* DEBUG */

VOID
ConfigureSyncWave (
  IN  VOID          *IpuMmioBasePtr,
  IN  IPU_DIx_REGS  *IpuDiRegsPtr,
  IN  UINT32        CounterIndex,
  IN  UINT32        RunValue,
  IN  UINT32        RunResolution,
  IN  UINT32        OffsetValue,
  IN  UINT32        OffsetResolution,
  IN  UINT32        CounterPolarityGenEn,
  IN  UINT32        CounterAutoReload,
  IN  UINT32        CounterClearSelect,
  IN  UINT32        CounterDown,
  IN  UINT32        CounterPolarityTriggerSelect,
  IN  UINT32        CounterPolarityClearSelect,
  IN  UINT32        CounterUp,
  IN  UINT32        StepRepeat
  )
{
  IPUx_DIx_SW_GEN0_x_REG  DiSwGen0Reg;
  IPUx_DIx_SW_GEN1_x_REG  DiSwGen1Reg;
  UINT32                  StepIndex;
  IPUx_DIx_STP_REP_REG    StepRepeatReg;

  ZeroMem ((VOID *)&DiSwGen0Reg, sizeof (DiSwGen0Reg));
  DiSwGen0Reg.dix_offset_resolution = OffsetResolution;
  DiSwGen0Reg.dix_offset_value = OffsetValue;
  DiSwGen0Reg.dix_run_resolution = RunResolution;
  DiSwGen0Reg.dix_run_value_m1 = RunValue;
  DiWrite32 (
    IpuDiRegsPtr,
    IPU_DIx_SW_GEN0_1_OFFSET + ((CounterIndex - 1) * 0x04),
    DiSwGen0Reg.Reg
  );

  ZeroMem ((VOID *)&DiSwGen1Reg, sizeof (DiSwGen1Reg));
  DiSwGen1Reg.dix_cnt_up = CounterUp;
  DiSwGen1Reg.dix_cnt_polarity_clr_sel = CounterPolarityClearSelect;
  DiSwGen1Reg.dix_cnt_polarity_trigger_sel = CounterPolarityTriggerSelect;
  DiSwGen1Reg.dix_cnt_down = CounterDown;
  DiSwGen1Reg.dix_cnt_clr_sel = CounterClearSelect;
  DiSwGen1Reg.dix_cnt_auto_reload = CounterAutoReload;
  DiSwGen1Reg.dix_cnt_polarity_gen_en = CounterPolarityGenEn;
  DiWrite32 (
    IpuDiRegsPtr,
    IPU_DIx_SW_GEN1_1_OFFSET + ((CounterIndex - 1) * 0x04),
    DiSwGen1Reg.Reg
  );

  StepIndex = (CounterIndex - 1) / 2;
  StepRepeatReg.Reg = IpuRead32 (
                        IpuMmioBasePtr,
                        IPU_DI0_STP_REP_OFFSET + (StepIndex * 0x4)
                      );

  if (CounterIndex % 2) {
    StepRepeatReg.dix_step_repeat_2i_minus_1 = StepRepeat;
  } else {
    StepRepeatReg.dix_step_repeat_2i = StepRepeat;
  }
  IpuWrite32 (
    IpuMmioBasePtr,
    IPU_DI0_STP_REP_OFFSET + (StepIndex * 0x4),
    StepRepeatReg.Reg
  );
}

EFI_STATUS
ConfigureDisplayInterface (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  UINT32                      DisplayIndex,
  IN  IMX_DISPLAY_TIMING          *DisplayTimingPtr
  )
{
  IPU_DIx_REGS              *pIpuDiRegs;
  VOID                      *pIpuMmioBase;
  UINT32                    BaseDiv;
  UINT32                    DispGenReg;
  UINT64                    DisplayInterfaceFrequency;
  IPUx_DIx_DW_GEN_REG       DixDwGenReg;
  IPUx_DIx_DW_SET_REG       DixDwSetReg;
  IPUx_DIx_GENERAL_REG      DixGeneralReg;
  IPUx_DIx_POL_REG          DixPolReg;
  IPUx_DIx_SYNC_AS_GEN_REG  DixSyncAsGenReg;
  UINT32                    HorizontalLength;
  EFI_STATUS                Status;
  UINT32                    VerticalLength;

  DisplayInterfaceFrequency = DisplayTimingPtr->PixelClock;
  HorizontalLength = DisplayTimingPtr->HActive + DisplayTimingPtr->HBlank;
  VerticalLength = DisplayTimingPtr->VActive + DisplayTimingPtr->VBlank;
  pIpuMmioBase = DisplayInterfaceContextPtr->IpuMmioBasePtr;
  pIpuDiRegs = DisplayInterfaceContextPtr->IpuDiRegsPtr;
  Status = EFI_SUCCESS;

  Status = ImxSetPll5ReferenceRate (DisplayTimingPtr->PixelClock);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to setup PLL5=%r\n", __FUNCTION__, Status));
    goto Exit;
  }

  // Setup base timer (fundamental timer). The base timer should already
  // setup to match the pixel clock frequency.
  // Shift 4 as the bottom 4 bits are fractional
  BaseDiv = (UINT32) ((DisplayInterfaceFrequency << 4) / DisplayTimingPtr->PixelClock);
  DiWrite32 (pIpuDiRegs, IPU_DIx_BS_CLKGEN0_OFFSET, BaseDiv);

  // Up is always set to 0. Down is half of the pixel clock period where
  // the first bit is fraction
  BaseDiv >>= 4;

  DiWrite32 (pIpuDiRegs, IPU_DIx_BS_CLKGEN1_OFFSET, BaseDiv << 16);
  // Calculate divisor, again this would usually be 1.
  BaseDiv = (UINT32) (DisplayInterfaceFrequency / DisplayTimingPtr->PixelClock);

  // Set up wave, there 12 wave quartet, for now default to the first.
  // Each wave quartet has 4 set register
  // Set 0 is just a blank signal where up and down is set to 0
  ZeroMem ((VOID *)&DixDwSetReg, sizeof (DixDwSetReg));
  DixDwSetReg.dix_data_cnt_upx_i = 0;
  DixDwSetReg.dix_data_cnt_downx_i = 0;
  WRITE_WAVE_SET (
    pIpuDiRegs,
    DwGen0,
    DwSet0,
    DixDwSetReg.Reg
  );

  // Set 3 is setup to match pixel clock
  ZeroMem ((VOID *)&DixDwSetReg, sizeof (DixDwSetReg));
  DixDwSetReg.dix_data_cnt_upx_i = 0;
  DixDwSetReg.dix_data_cnt_downx_i = BaseDiv * 2;
  WRITE_WAVE_SET (
    pIpuDiRegs,
    DwGen0,
    DwSet3,
    DixDwSetReg.Reg
  );

  // All pins blank signal except pin 15
  ZeroMem ((VOID *)&DixDwGenReg, sizeof (DixDwGenReg));
  DixDwGenReg.dix_pt_0_i = DwSet0;
  DixDwGenReg.dix_pt_1_i = DwSet0;
  DixDwGenReg.dix_pt_2_i = DwSet0;
  DixDwGenReg.dix_pt_3_i = DwSet0;
  DixDwGenReg.dix_pt_4_i = DwSet3;
  DixDwGenReg.dix_pt_5_i = DwSet0;
  DixDwGenReg.dix_pt_6_i = DwSet0;
  DixDwGenReg.dix_cst_i = DwSet0;
  // Reuse the base divisor to determine extra IPU cycles.
  DixDwGenReg.dix_componnent_size_i = BaseDiv - 1;
  DixDwGenReg.dix_access_size_i = BaseDiv - 1;
  WRITE_WAVE_GEN (pIpuDiRegs, DwGen0, DixDwGenReg.Reg);

  // Spec mention this as number of display rows but display only works
  // properly if this is setup as vertical total
  DiWrite32 (pIpuDiRegs, IPU_DIx_SCR_CONF_OFFSET, VerticalLength - 1);

  // Internal HSYNC
  ConfigureSyncWave (
    pIpuMmioBase,
    pIpuDiRegs,
    DI_COUNTER_1_INTERNAL_HSYNC, // CounterIndex
    HorizontalLength - 1,   // Runvalue
    DI_COUNTER_0_DISPLAY_CLOCK + 1, // RunResolution
    0,            // OffsetValue
    0,            // OffsetResolution
    0,            // CounterPolarityGenEn
    1,            // CounterAutoReload
    DI_COUNTER_DISABLED, // CounterClearSelect
    0,            // CountDown
    0,            // CounterPolarityTriggerSelect
    0,            // CounterPolarityClearSelect
    0,            // CounterUp
    0             // StepRepeat
  );

  // Output HSYNC
  ConfigureSyncWave (
    pIpuMmioBase,
    pIpuDiRegs,
    DI_COUNTER_2_OUTPUT_HSYNC, // CounterIndex
    HorizontalLength - 1,   // Runvalue
    DI_COUNTER_0_DISPLAY_CLOCK + 1, // RunResolution
    0,            // OffsetValue
    DI_COUNTER_0_DISPLAY_CLOCK + 1, // OffsetResolution - Display clock
    1,            // CounterPolarityGenEn
    1,            // CounterAutoReload
    DI_COUNTER_DISABLED, // CounterClearSelect
    DisplayTimingPtr->HSync * 2,    // CountDown
    1,            // CounterPolarityTriggerSelect
    0,            // CounterPolarityClearSelect
    0,            // CounterUp
    0             // StepRepeat
  );

  // Output VSYNC
  ConfigureSyncWave (
    pIpuMmioBase,
    pIpuDiRegs,
    DI_COUNTER_3_OUTPUT_VSYNC, // CounterIndex
    VerticalLength - 1,   // Runvalue
    DI_COUNTER_1_INTERNAL_HSYNC + 1, // RunResolution - Counter 1
    0,            // OffsetValue
    0,            // OffsetResolution
    1,            // CounterPolarityGenEn
    1,            // CounterAutoReload
    DI_COUNTER_DISABLED, // CounterClearSelect
    DisplayTimingPtr->VSync * 2,    // CountDown
    2,            // CounterPolarityTriggerSelect
    0,            // CounterPolarityClearSelect
    0,            // CounterUp
    0             // StepRepeat
  );

  // Active lines
  ConfigureSyncWave (
    pIpuMmioBase,
    pIpuDiRegs,
    DI_COUNTER_4_ACTIVE_LINE,  // CounterIndex
    0,            // Runvalue
    DI_COUNTER_2_OUTPUT_HSYNC + 1, // RunResolution - Counter 2
    DisplayTimingPtr->VSync + DisplayTimingPtr->VSyncOffset, // Offset
    DI_COUNTER_2_OUTPUT_HSYNC + 1,// OffsetResolution - Counter 2
    0,            // CounterPolarityGenEn
    0,            // CounterAutoReload
    DI_COUNTER_3_OUTPUT_VSYNC + 1, // CounterClearSelect - Counter 3
    0,            // CountDown
    0,            // CounterPolarityTriggerSelect
    0,            // CounterPolarityClearSelect
    0,            // CounterUp
    DisplayTimingPtr->VActive // StepRepeat repeat for total VActive
  );

  // Active clock
  ConfigureSyncWave (
    pIpuMmioBase,
    pIpuDiRegs,
    DI_COUNTER_5_ACTIVE_CLOCK, // CounterIndex
    0,            // Runvalue
    DI_COUNTER_0_DISPLAY_CLOCK + 1, // RunResolution - Display clock
    DisplayTimingPtr->HSync + DisplayTimingPtr->HSyncOffset, // Offset
    DI_COUNTER_0_DISPLAY_CLOCK + 1, // OffsetResolution - Display clock
    0,            // CounterPolarityGenEn
    0,            // CounterAutoReload
    DI_COUNTER_4_ACTIVE_LINE + 1, // CounterClearSelect - Counter 4
    0,            // CountDown
    0,            // CounterPolarityTriggerSelect
    0,            // CounterPolarityClearSelect
    0,            // CounterUp
    DisplayTimingPtr->HActive // StepRepeat
  );

  ZeroMem ((VOID *)&DixSyncAsGenReg, sizeof (DixSyncAsGenReg));
  // VSYNC is setup as counter 3 above, 0 index based
  DixSyncAsGenReg.dix_vsync_sel = 3 - 1;
  // Number of row DI prepares next frame data.
  DixSyncAsGenReg.dix_sync_start = 2;
  DiWrite32 (pIpuDiRegs, IPU_DIx_SYNC_AS_GEN_OFFSET, DixSyncAsGenReg.Reg);

  // Setup general register
  ZeroMem ((VOID *)&DixGeneralReg, sizeof (DixGeneralReg));
  // Counter 1 as display line
  DixGeneralReg.dix_disp_y_sel = DI_COUNTER_1_INTERNAL_HSYNC - 1;
  // Stop at the next edge of the display clock
  DixGeneralReg.DIx_CLOCK_STOP_MODE = 0;
  // The display's clock is stopped after the next VSYNC
  DixGeneralReg.DIx_DISP_CLOCK_INIT = 0;
  // IPP_PIN_2 is coming from counter #2
  DixGeneralReg.dix_mask_sel = 0;
  // External clock - for not the video PLL
  DixGeneralReg.dix_vsync_ext = 1;
  // External clock - for not the video PLL
  DixGeneralReg.dix_clk_ext = 1;
  // 4 cycle watch dog based on BSP
  DixGeneralReg.DIx_WATCHDOG_MODE = 0;
  // default sync to counter 0
  DixGeneralReg.dix_sync_count_sel = DI_COUNTER_1_INTERNAL_HSYNC - 1;
  // In the event of error drive the last component
  DixGeneralReg.dix_err_treatment = 0;
  // An internal VSYNC signal asserted 2 lines before the DI's VSYNC
  DixGeneralReg.dix_erm_vsync_sel = 0;

  switch (DisplayInterfaceContextPtr->displayInterface) {
  case HdmiDisplay:
    // Zero for HDMI display
    DixGeneralReg.dix_polarity_disp_clk = 0;
    DixGeneralReg.dix_polarity_cs1 = 0;
    DixGeneralReg.dix_polarity_cs0 = 0;
    DixGeneralReg.dix_polarity_i_1 = 0;
    break;
  default:
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "%a: Unsupported display interface %d\n",
      __FUNCTION__, DisplayInterfaceContextPtr->displayInterface));
    break;
  }
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  DiWrite32 (pIpuDiRegs, IPU_DIx_GENERAL_OFFSET, DixGeneralReg.Reg);

  ZeroMem ((VOID *)&DixPolReg, sizeof (DixPolReg));
  // CS0
  DixPolReg.DIx_CS0_DATA_POLARITY = 1;
  DixPolReg.dix_cs0_polarity = 0x7F;
  // CS1
  DixPolReg.DIx_CS1_DATA_POLARITY = 1;
  DixPolReg.dix_cs1_polarity = 0x7F;
  // DRDY
  DixPolReg.DIx_DRDY_DATA_POLARITY = 0;
  DixPolReg.dix_drdy_polarity = 0x7F;
  // Wait
  DixPolReg.DIx_WAIT_POLARITY = 0;
  // CS0 byte enable polarity
  DixPolReg.DIx_CS0_BYTE_EN_POLARITY = 0;
  // CS1 byte enable polarity
  DixPolReg.DIx_CS1_BYTE_EN_POLARITY = 0;
  DiWrite32 (pIpuDiRegs, IPU_DIx_POL_OFFSET, DixPolReg.Reg);

  DispGenReg = IpuRead32 (pIpuMmioBase, IPU_IPU_DISP_GEN_OFFSET);
  DispGenReg &= ~(0x0F << 18);
  DispGenReg |= (2 << 18);
  IpuWrite32 (pIpuMmioBase, IPU_IPU_DISP_GEN_OFFSET, DispGenReg);

#ifdef DEBUG
  DumpBasicDisplayInterfaceReg (pIpuMmioBase, pIpuDiRegs);
#endif /* DEBUG */

  Status = EFI_SUCCESS;

Exit:
  return Status;
}
