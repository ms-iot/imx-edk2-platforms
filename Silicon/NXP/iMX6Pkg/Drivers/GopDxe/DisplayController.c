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
#include "DisplayController.h"
#include "DisplayInterface.h"

#ifdef DEBUG
VOID
DumpBasicDisplayControllerReg (
  IN  VOID  *IpuMmioBasePtr
  )
{
  UINT32 Counter;
  UINT32 Index;
  UINT32 RegVal;

  DEBUG ((DEBUG_VERBOSE, "%a: ------- DC Register Dump -------\n", __FUNCTION__));

  DEBUG ((DEBUG_VERBOSE, "%a: ## Configuration\n\n", __FUNCTION__));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_WR_CH_CONF_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_WR_CH_CONF_5_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_WR_CH_CONF_1_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_WR_CH_CONF_1_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_DISP_CONF1_0_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_DISP_CONF1_0_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_DISP_CONF1_1_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_DISP_CONF1_1_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_DISP_CONF1_2_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_DISP_CONF1_2_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_DISP_CONF1_3_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_DISP_CONF1_3_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_DISP_CONF2_0_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_DISP_CONF2_0_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_GEN_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_GEN_OFFSET %x\n",
    __FUNCTION__, RegVal));

  DEBUG ((DEBUG_VERBOSE, "%a: ## Bus MAPPING\n\n", __FUNCTION__));
  {
    for (Counter = 0, Index = 0; Index < 26; Counter += 4, ++Index) {
      RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_MAP_CONF_0_OFFSET + Counter);
      DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_MAP_CONF_%d %x\n",
        __FUNCTION__, Index, RegVal));
    }
  }

  DEBUG ((DEBUG_VERBOSE, "%a: ## Channel MicroCode setup\n\n", __FUNCTION__));
  // Only print out channel 5 as we only support single mode for now
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_RL0_CH_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_RL0_CH_5_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_RL1_CH_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_RL1_CH_5_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_RL2_CH_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_RL2_CH_5_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_RL3_CH_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_RL3_CH_5_OFFSET %x\n",
    __FUNCTION__, RegVal));
  RegVal = IpuRead32 (IpuMmioBasePtr, IPU_DC_RL4_CH_5_OFFSET);
  DEBUG ((DEBUG_VERBOSE, "%a: IPU_DC_RL4_CH_5_OFFSET %x\n",
    __FUNCTION__, RegVal));

  DEBUG ((DEBUG_VERBOSE, "%a: ## MicroCode\n\n", __FUNCTION__));
  // There are 256 template, only print out the first 10
  for (Counter = 0, Index = 0; Index < 10; Counter += 8, ++Index) {
    DEBUG ((DEBUG_VERBOSE, "%a: (%d)\n", __FUNCTION__, Index));
    RegVal = IpuRead32 (
                IpuMmioBasePtr,
                IPU_DC_TEMPLATE_REGS_ADDR_OFFSET + Counter);
    DEBUG ((DEBUG_VERBOSE, "%a: - %8x\n", __FUNCTION__, RegVal));
    RegVal = IpuRead32 (
                IpuMmioBasePtr,
                IPU_DC_TEMPLATE_REGS_ADDR_OFFSET + Counter + 4);
    DEBUG ((DEBUG_VERBOSE, "%a:  %8x -\n", __FUNCTION__, RegVal));
  }

  DEBUG ((DEBUG_VERBOSE, "%a: ---------------------------\n\n", __FUNCTION__));
}
#endif /* DEBUG */

EFI_STATUS
WriteWrodCommand (
  IN  VOID    *IpuMmioBasePtr,
  IN  UINT32  MicroCodeAddr,
  IN  UINT32  Data,
  IN  UINT32  Mapping,
  IN  UINT32  WaveForm,
  IN  UINT32  GlueLogic,
  IN  UINT32  Sync
  )
{
  UINT32                            MicroCodeAddrOffset;
  DISPLAY_CONTROLLER_WROD_COMMAND   WrodCommand;


  MicroCodeAddrOffset = IPU_DC_TEMPLATE_REGS_ADDR_OFFSET + (MicroCodeAddr * 8);
  ZeroMem ((VOID *)&WrodCommand, sizeof (WrodCommand));
  WrodCommand.STOP = 1;
  WrodCommand.OPCODE = 0x18;
  WrodCommand.DATA = Data;
  WrodCommand.MAPPING = Mapping;
  WrodCommand.WAVEFORM = WaveForm + 1;
  WrodCommand.GLUELOGIC = GlueLogic;
  WrodCommand.SYNC = Sync;
  IpuWrite32 (IpuMmioBasePtr, MicroCodeAddrOffset, WrodCommand.LowWord);
  IpuWrite32 (IpuMmioBasePtr, MicroCodeAddrOffset + 4, WrodCommand.HighWord);
  return EFI_SUCCESS;
}

EFI_STATUS
SetDisplayControllerChannelState (
  IN  VOID *IpuMmioBasePtr,
  IN  PROG_CHAN_TYP ChannelType
  )
{
  IPU_DC_WR_CH_CONF_5_REG   WrChConfigReg;

  WrChConfigReg.Reg = IpuRead32 (IpuMmioBasePtr, IPU_DC_WR_CH_CONF_5_OFFSET);
  WrChConfigReg.PROG_CHAN_TYP = ChannelType;
  IpuWrite32 (IpuMmioBasePtr, IPU_DC_WR_CH_CONF_5_OFFSET, WrChConfigReg.Reg);
  return EFI_SUCCESS;
}

EFI_STATUS
ConfigureDisplayControllerChannel (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  DISPLAY_INTERFACE_TYPE      DisplayInterface,
  IN  UINT32                      DisplayIndex,
  IN  IMX_DISPLAY_TIMING          *DisplayTimingPtr
  )
{
  VOID                          *pIpuMmioBase;
  DC_MAP_CONF_OFFSET_MASK_REG   DcConfOffsetMaskReg;
  IPUx_DC_MAP_CONF_MAP_REG      DcMapConf0Reg;
  IPU_DC_RL0_CH_5_REG           DcRl0Ch5Reg;
  IPU_DC_RL2_CH_5_REG           DcRl2Ch5Reg;
  IPU_DC_RL4_CH_5_REG           DcRl4Ch5Reg;
  IPUx_DC_DISP_CONF1_REG        DisplayConfig1Reg;
  IPUx_IPU_DC_GEN_REG           DisplayControllerGenReg;
  UINT32                        Mask0;
  UINT32                        Mask1;
  UINT32                        Mask2;
  UINT32                        Offset0;
  UINT32                        Offset1;
  UINT32                        Offset2;
  EFI_STATUS                    Status;
  IPU_DC_WR_CH_CONF_5_REG       WrChConfigReg;

  pIpuMmioBase = DisplayInterfaceContextPtr->IpuMmioBasePtr;
  Status = EFI_SUCCESS;

  ZeroMem ((VOID *)&WrChConfigReg, sizeof (WrChConfigReg));
  WrChConfigReg.PROG_START_TIME = 0;
  WrChConfigReg.FILED_MODE = 0;
  WrChConfigReg.CHAN_MASK_DEFAULT = 0; // only used highest priority
  WrChConfigReg.PROG_CHAN_TYP = 0; // Begin as disable
  WrChConfigReg.PROG_DISP_ID = DisplayInterface;
  WrChConfigReg.PROG_DI_ID = DisplayIndex % 2;
  WrChConfigReg.W_SIZE = 0x02; // 24 Bits
  WrChConfigReg.Reserved1 = 0;
  WrChConfigReg.Reserved2 = 0;
  // Channel 5 is used main primary flow
  IpuWrite32 (pIpuMmioBase, IPU_DC_WR_CH_CONF_5_OFFSET, WrChConfigReg.Reg);
  // Start address of memory always 0
  IpuWrite32 (pIpuMmioBase, IPU_DC_WR_CH_ADDR_5_OFFSET, 0);

  ZeroMem ((VOID *)&WrChConfigReg, sizeof (WrChConfigReg));
  WrChConfigReg.FILED_MODE = 0;
  WrChConfigReg.CHAN_MASK_DEFAULT = 0; // only used highest priority
  WrChConfigReg.PROG_CHAN_TYP = 4; // Enable
  WrChConfigReg.PROG_DISP_ID = DisplayInterface;
  WrChConfigReg.PROG_DI_ID = DisplayIndex % 2;
  WrChConfigReg.W_SIZE = 0x02; // 1 Bits
  WrChConfigReg.Reserved1 = 0;
  WrChConfigReg.Reserved2 = 0;

  // Channel 1 is used as sync/async flow
  IpuWrite32 (pIpuMmioBase, IPU_DC_WR_CH_CONF_1_OFFSET, WrChConfigReg.Reg);
  IpuWrite32 (pIpuMmioBase, IPU_DC_WR_CH_ADDR_1_OFFSET, 0);

  DisplayConfig1Reg.DISP_TYP = 0x02; // What is byte_enabled
  DisplayConfig1Reg.ADDR_INCREMENT = 0; // Increase by 1 byte
  DisplayConfig1Reg.ADDR_BE_L_INC = 0;
  DisplayConfig1Reg.MCU_ACC_LB_MASK_3 = 0;
  DisplayConfig1Reg.DISP_RD_VALUE_PTR = 0;
  IpuWrite32 (pIpuMmioBase, IPU_DC_DISP_CONF1_0_OFFSET, DisplayConfig1Reg.Reg);

  // Set stride
  IpuWrite32 (pIpuMmioBase, IPU_DC_DISP_CONF2_0_OFFSET, DisplayTimingPtr->VActive);

  // Setup general register. Channel 5 is the main channel.
  DisplayControllerGenReg.Sync_1_6 = 2; // Sync flow
  DisplayControllerGenReg.MASK_EN = 0; // Disable masking
  DisplayControllerGenReg.MASK4CHAN_5 = 0; // Ignore as mask is disabled
  DisplayControllerGenReg.SYNC_PRIORITY_5 = 1; // Higher sync priority for channel 5
  DisplayControllerGenReg.SYNC_PRIORITY_1 = 0; // Lower sync priority
  DisplayControllerGenReg.DC_CH5_TYPE = 0; // Normal mode, sync flow through channel 5
  DisplayControllerGenReg.DC_BK_EN = 0; // No cursor support
  DisplayControllerGenReg.DC_BKDIV = 0; // No cursor support
  IpuWrite32 (pIpuMmioBase, IPU_DC_GEN_OFFSET, DisplayControllerGenReg.Reg);

  // Do not use any user event
  IpuWrite32 (pIpuMmioBase, IPU_DC_UGDE0_0_OFFSET, 0);
  IpuWrite32 (pIpuMmioBase, IPU_DC_UGDE1_0_OFFSET, 0);
  IpuWrite32 (pIpuMmioBase, IPU_DC_UGDE2_0_OFFSET, 0);
  IpuWrite32 (pIpuMmioBase, IPU_DC_UGDE3_0_OFFSET, 0);

  DcMapConf0Reg.MAPPING_PNTR_BYTE0_X = 0;
  DcMapConf0Reg.MAPPING_PNTR_BYTE1_X = 1;
  DcMapConf0Reg.MAPPING_PNTR_BYTE2_X = 2;
  DcMapConf0Reg.MAPPING_PNTR_BYTE0_Y = 3; // Unused
  DcMapConf0Reg.MAPPING_PNTR_BYTE1_Y = 4; // Unused
  DcMapConf0Reg.MAPPING_PNTR_BYTE2_Y = 5; // Unused
  IpuWrite32 (pIpuMmioBase, IPU_DC_MAP_CONF_0_OFFSET, DcMapConf0Reg.Reg);

  switch (DisplayInterface) {
  // PixelFormat RGB24
  case HdmiDisplay:
    Mask0 = 0xFF;
    Mask1 = 0xFF;
    Mask2 = 0xFF;
    Offset0 = 7;
    Offset1 = 15;
    Offset2 = 23;
    break;
  // PixelFormat RGB666
  case Lvds0Display:
  case Lvds1Display:
    Mask0 = 0xFC;
    Mask1 = 0xFC;
    Mask2 = 0xFC;
    Offset0 = 5;
    Offset1 = 11;
    Offset2 = 17;
    break;
  default:
    ASSERT (FALSE);
    Status = EFI_UNSUPPORTED;
    break;
  }
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  DcConfOffsetMaskReg.MD_MASK_X = Mask0;
  DcConfOffsetMaskReg.MD_OFFSET_X = Offset0; // Blue
  DcConfOffsetMaskReg.MD_MASK_Y = Mask1;
  DcConfOffsetMaskReg.MD_OFFSET_Y = Offset1; // Green
  IpuWrite32 (pIpuMmioBase, IPU_DC_MAP_CONF_15_OFFSET, DcConfOffsetMaskReg.Reg);

  DcConfOffsetMaskReg.MD_MASK_X = Mask2;
  DcConfOffsetMaskReg.MD_OFFSET_X = Offset2; // Red
  DcConfOffsetMaskReg.MD_MASK_Y = 0x00;
  DcConfOffsetMaskReg.MD_OFFSET_Y = 0; // Unused
  IpuWrite32 (pIpuMmioBase, IPU_DC_MAP_CONF_16_OFFSET, DcConfOffsetMaskReg.Reg);

  // Setup microcode
  // New line event point to the first microcode (0)
  ZeroMem ((VOID *)&DcRl0Ch5Reg, sizeof (DcRl0Ch5Reg));
  DcRl0Ch5Reg.COD_NL_START_CHAN_5 = 0;
  DcRl0Ch5Reg.COD_NL_PRIORITY_CHAN_5 = 3;
  IpuWrite32 (pIpuMmioBase, IPU_DC_RL0_CH_5_OFFSET, DcRl0Ch5Reg.Reg);

  // End of line event point to the second microcode (1)
  ZeroMem ((VOID *)&DcRl2Ch5Reg, sizeof (DcRl2Ch5Reg));
  DcRl2Ch5Reg.COD_EOL_START_CHAN_5 = 1;
  DcRl2Ch5Reg.COD_EOL_PRIORITY_CHAN_5 = 2;
  IpuWrite32 (pIpuMmioBase, IPU_DC_RL2_CH_5_OFFSET, DcRl2Ch5Reg.Reg);

  // New data event point to the first microcode (2)
  ZeroMem ((VOID *)&DcRl4Ch5Reg, sizeof (DcRl4Ch5Reg));
  DcRl4Ch5Reg.COD_NEW_DATA_START_CHAN_5 = 2;
  DcRl4Ch5Reg.COD_NEW_DATA_PRIORITY_CHAN_5 = 1;
  IpuWrite32 (pIpuMmioBase, IPU_DC_RL4_CH_5_OFFSET, DcRl4Ch5Reg.Reg);

  // MicroCodeAddr
  // - 0 set for new line event
  // Data
  // - Unsused
  // Map to mapping parameter 0
  // - In order to point to MAPPING_PNTR_BYTE2_0, MAPPING_PNTR_BYTE1_0,
  //   MAPPING_PNTR_BYTE0_0 the user should write 1 to the MAPPING field
  // WaveForm
  // - Points to DI0_DW_GEN_0 or DI1_DW_GEN_0 (Define which waveform
  //   register is used, default to first IPUx_DI0_DW_SET0_1)
  // GlueLogic
  // - Once the signal is asserted then it remains asserted (high or low
  //   according to the polarity)
  // Sync
  // - Sync with Counter 5
  WriteWrodCommand (
    pIpuMmioBase,
    0,
    0,
    1,
    DwGen0,
    8,
    DI_COUNTER_5_ACTIVE_CLOCK
  );

  // MicroCodeAddr
  // - 1 set for end of line event
  // Data
  // - Unsused
  // Map to mapping parameter 0
  // - In order to point to MAPPING_PNTR_BYTE2_0, MAPPING_PNTR_BYTE1_0,
  //   MAPPING_PNTR_BYTE0_0 the user should write 1 to the MAPPING field
  // WaveForm
  // - Points to DI0_DW_GEN_0 or DI1_DW_GEN_0 (Define which waveform
  //   register is used, default to first IPUx_DI0_DW_SET0_1)
  // GlueLogic
  // - Once the signal is negated then it remains negated (high or low
  //   according to the polarity)
  // Sync
  // - Sync with Counter 5
  WriteWrodCommand (
    pIpuMmioBase,
    1,
    0,
    1,
    DwGen0,
    4,
    DI_COUNTER_5_ACTIVE_CLOCK
  );

  // MicroCodeAddr
  // - 2 set for new data event
  // Data
  // - Unsused
  // Map to mapping parameter 0
  // - In order to point to MAPPING_PNTR_BYTE2_0, MAPPING_PNTR_BYTE1_0,
  //   MAPPING_PNTR_BYTE0_0 the user should write 1 to the MAPPING field
  // WaveForm
  // - Points to DI0_DW_GEN_0 or DI1_DW_GEN_0 (Define which waveform
  //   register is used, default to first IPUx_DI0_DW_SET0_1)
  // GlueLogic
  // - CS mode No impact on the waveform
  // Sync
  // - Sync with channel 5
  WriteWrodCommand (
    pIpuMmioBase,
    2,
    0,
    1,
    DwGen0,
    8,
    DI_COUNTER_5_ACTIVE_CLOCK
  );

  // Turn on channel without anti tearing
  SetDisplayControllerChannelState (
    DisplayInterfaceContextPtr->IpuMmioBasePtr,
    PROG_CHAN_TYP_NORMAL
  );

#ifdef DEBUG
  DumpBasicDisplayControllerReg (pIpuMmioBase);
#endif /* DEBUG */

  Status = EFI_SUCCESS;

Exit:
  return Status;
}
