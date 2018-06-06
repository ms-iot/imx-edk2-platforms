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

#include "Display.h"
#include "CPMem.h"
#include "GopDxe.h"
#include "Ipu.h"

CPMEM_PARAM *CpMemParamBasePtr = (CPMEM_PARAM *)(IPU1_BASE + CSP_IPUV3_CPMEM_REGS_OFFSET);

STATIC CONST UINT8 ChannelMap[] = {
  5, 0,
  11, 2,
  12, 4,
  14, 6,
  15, 8,
  20, 10,
  21, 12,
  22, 14,
  23, 16,
  27, 18,
  28, 20,
  45, 0,
  46, 2,
  47, 4,
  48, 6,
  49, 8,
  50, 10,
};

#ifdef DEBUG
VOID
DumpCpmemParamPack (
  IN  CPMEM_PARAM   *ParamPtr
  )
{
  DEBUG ((DEBUG_VERBOSE, "%a: --WORD0--\n", __FUNCTION__));
  DEBUG ((DEBUG_VERBOSE, "%a: XVVirtualCoordinate 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.XVVirtualCoordinate));
  DEBUG ((DEBUG_VERBOSE, "%a: YVVirtualCoordinate 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.YVVirtualCoordinate));
  DEBUG ((DEBUG_VERBOSE, "%a: XBinnerBlockCoordinate 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.XBinnerBlockCoordinate));
  DEBUG ((DEBUG_VERBOSE, "%a: YBinnerBlockCoordinate 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.YBinnerBlockCoordinate));
  DEBUG ((DEBUG_VERBOSE, "%a: NewSubBlock 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.NewSubBlock));
  DEBUG ((DEBUG_VERBOSE, "%a: CurrentField 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.CurrentField));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollXCounter 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollXCounter));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollYCounter 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollYCounter));
  DEBUG ((DEBUG_VERBOSE, "%a: NumberOfScroll 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.NumberOfScroll));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollDeltaX 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollDeltaX));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollMax 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollMax));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollingConfiguration 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollingConfiguration));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollingEnable 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollingEnable));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollDeltaY 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollDeltaY));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollHorizontalDirection 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollHorizontalDirection));
  DEBUG ((DEBUG_VERBOSE, "%a: ScrollVerticalDirection 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScrollVerticalDirection));
  DEBUG ((DEBUG_VERBOSE, "%a: BitsPerPixel 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.BitsPerPixel));
  DEBUG ((DEBUG_VERBOSE, "%a: DecodeAddressSelect 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.DecodeAddressSelect));
  DEBUG ((DEBUG_VERBOSE, "%a: AccessDimension 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.AccessDimension));
  DEBUG ((DEBUG_VERBOSE, "%a: ScanOrder 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ScanOrder));
  DEBUG ((DEBUG_VERBOSE, "%a: BandMode 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.BandMode));
  DEBUG ((DEBUG_VERBOSE, "%a: BlockMode 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.BlockMode));
  DEBUG ((DEBUG_VERBOSE, "%a: Rotation 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.Rotation));
  DEBUG ((DEBUG_VERBOSE, "%a: HorizontalFlip 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.HorizontalFlip));
  DEBUG ((DEBUG_VERBOSE, "%a: VerticalFlip 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.VerticalFlip));
  DEBUG ((DEBUG_VERBOSE, "%a: ThresholdEnable 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ThresholdEnable));
  DEBUG ((DEBUG_VERBOSE, "%a: ConditionalAccessPolarity 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ConditionalAccessPolarity));
  DEBUG ((DEBUG_VERBOSE, "%a: ConditionalAccessEnable 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.ConditionalAccessEnable));
  DEBUG ((DEBUG_VERBOSE, "%a: FrameWidth 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.FrameWidth));
  DEBUG ((DEBUG_VERBOSE, "%a: FrameHeight 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.FrameHeight));
  DEBUG ((DEBUG_VERBOSE, "%a: EndOfLineInterrupt 0x%08x\n", __FUNCTION__,
    ParamPtr->Word0Pack.EndOfLineInterrupt));

  DEBUG ((DEBUG_VERBOSE, "%a: --WORD1--\n", __FUNCTION__));
  DEBUG ((DEBUG_VERBOSE, "%a: ExtMemBuffer0Address 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.ExtMemBuffer0Address));
  DEBUG ((DEBUG_VERBOSE, "%a: ExtMemBuffer1Address 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.ExtMemBuffer1Address));
  DEBUG ((DEBUG_VERBOSE, "%a: InterlaceOffset 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.InterlaceOffset));
  DEBUG ((DEBUG_VERBOSE, "%a: NumberOfPixelsInWholeBurstAccess 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.NumberOfPixelsInWholeBurstAccess));
  DEBUG ((DEBUG_VERBOSE, "%a: PixelFormatSelect 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.PixelFormatSelect));
  DEBUG ((DEBUG_VERBOSE, "%a: AlphaUsed 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.AlphaUsed));
  DEBUG ((DEBUG_VERBOSE, "%a: AlphaChannelMapping 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.AlphaChannelMapping));
  DEBUG ((DEBUG_VERBOSE, "%a: AxiId 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.AxiId));
  DEBUG ((DEBUG_VERBOSE, "%a: Threshold 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Threshold));
  DEBUG ((DEBUG_VERBOSE, "%a: StrideLine 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.StrideLine));
  DEBUG ((DEBUG_VERBOSE, "%a: Width0 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Width0));
  DEBUG ((DEBUG_VERBOSE, "%a: Width1 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Width1));
  DEBUG ((DEBUG_VERBOSE, "%a: Width2 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Width2));
  DEBUG ((DEBUG_VERBOSE, "%a: Width3 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Width3));
  DEBUG ((DEBUG_VERBOSE, "%a: Offset0 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Offset0));
  DEBUG ((DEBUG_VERBOSE, "%a: Offset1 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Offset1));
  DEBUG ((DEBUG_VERBOSE, "%a: Offset2 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Offset2));
  DEBUG ((DEBUG_VERBOSE, "%a: Offset3 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.Offset3));
  DEBUG ((DEBUG_VERBOSE, "%a: SelectSXSYSet 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.SelectSXSYSet));
  DEBUG ((DEBUG_VERBOSE, "%a: ConditionalReadEnable 0x%08x\n", __FUNCTION__,
    ParamPtr->Word1Pack.ConditionalReadEnable));
}

VOID
DumpBasicCpmemReg (
  IN  CPMEM_PARAM   *pCpmemChannel
  )
{
  DEBUG ((DEBUG_VERBOSE, "%a: ---------- CPMEM Register Dump ----------\n",
    __FUNCTION__));
  DEBUG ((DEBUG_VERBOSE, "%a: CPMEM\n", __FUNCTION__));
  DEBUG ((DEBUG_VERBOSE, "%a: IDMAC_CHANNEL_DP_PRIMARY_FLOW_MAIN_PLANE\n",
    __FUNCTION__));
  DumpCpmemParamPack (pCpmemChannel);
  DEBUG ((DEBUG_VERBOSE, "%a: ------------------------------------\n",
    __FUNCTION__));
}
#endif /* DEBUG */

// Enable IDMAC lock setting, which optimises memory accesses and reduces
// power consumption
VOID
SetIdmacLockEn (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  UINT32                      Channel,
  IN  UINT32                      Setting
  )
{
  UINT32 i;
  UINTN   LockReg;
  UINT32 Shift;
  UINT32 Value;

  Shift = -1;

  for (i = 0; i < ARRAYSIZE (ChannelMap); i += 2) {
    if (ChannelMap[i] == Channel) {
      Shift = ChannelMap[i + 1];
      break;
    }
  }

  if (Shift == -1) {
    DEBUG ((DEBUG_WARN, "%a: Channel %d does not have lock bits\n",
      __FUNCTION__, Channel));
    return;
  }

  if (Channel < 29) {
    LockReg = (UINTN)DisplayInterfaceContextPtr->IpuMmioBasePtr + IPU_IDMAC_LOCK_EN_1;
  } else {
    LockReg = (UINTN)DisplayInterfaceContextPtr->IpuMmioBasePtr + IPU_IDMAC_LOCK_EN_2;
  }

  Value = MmioRead32 ((UINT32)LockReg);
  Value &= ~(0x3 << Shift);
  Value |= (Setting & 0x3) << Shift;
  MmioWrite32 ((UINT32)LockReg, Value);
}

EFI_STATUS
ConfigureCpmemFrameBuffer (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  UINT32                      Channel,
  IN  SURFACE_INFO                *FrameBufferPtr
  )
{
  CPMEM_PARAM *pCpmemChannel;
  EFI_STATUS Status;
  CPMEM_WORD0_PACKED_REG CpmemWord0PackedReg;
  CPMEM_WORD1_PACKED_REG CpmemWord1PackedReg;
  CPMEM_DEC_SEL DecodeAddressSelect;
  UINT32 PixelBurst;
  CPMEM_PFS_PACKED PixelFormatSelector;
  UINT32 Width0;
  UINT32 Width1;
  UINT32 Width2;
  UINT32 Width3;
  UINT32 Offset0;
  UINT32 Offset1;
  UINT32 Offset2;
  UINT32 Offset3;
  UINT32 BytesPerPixel;

  pCpmemChannel = DisplayInterfaceContextPtr->CpMemParamBasePtr;
  Status = EFI_SUCCESS;
  PixelBurst = 1;
  DecodeAddressSelect = CPMEM_DEC_SEL_0_15; // Only applicable for 4 bpp
  ZeroMem (&CpmemWord0PackedReg, sizeof (CpmemWord0PackedReg));
  ZeroMem (&CpmemWord1PackedReg, sizeof (CpmemWord1PackedReg));

  switch (FrameBufferPtr->PixelFormat) {
  case PIXEL_FORMAT_BGRA32:
    PixelBurst = 15; // 16 Pixel. Valid range is 1 - 64 pixel
    PixelFormatSelector = CPMEM_PFS_RGB;
    Offset0 = 8;
    Offset1 = 16;
    Offset2 = 24;
    Offset3 = 0;
    BytesPerPixel = 4;
    break;
  default:
    ASSERT (FALSE);
    Status = EFI_UNSUPPORTED;
    break;
  }
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  switch (FrameBufferPtr->Bpp) {
  case 8:
    Width0 = 0;
    Width1 = 0;
    Width2 = 0;
    Width3 = 0;
    break;
  case 32:
    if (PixelFormatSelector == CPMEM_PFS_RGB) {
      Width0 = 7;
      Width1 = 7;
      Width2 = 7;
      Width3 = 7;
    } else {
      Width0 = 0;
      Width1 = 0;
      Width2 = 0;
      Width3 = 0;
    }
    break;
  default:
    ASSERT (FALSE);
    Status = EFI_UNSUPPORTED;
    break;
  }
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  // Setting up CPMEM word 0
  CpmemWord0PackedReg.XVVirtualCoordinate = 0;
  CpmemWord0PackedReg.YVVirtualCoordinate = 0;

  // Subblock is unused although expect to have some Value after write
  CpmemWord0PackedReg.XBinnerBlockCoordinate = 0;
  CpmemWord0PackedReg.YBinnerBlockCoordinate = 0;
  CpmemWord0PackedReg.NewSubBlock = 0;

  // Verify "current field" definition
  CpmemWord0PackedReg.CurrentField = 0;

  // Disable scrolling
  CpmemWord0PackedReg.ScrollXCounter = 0;
  CpmemWord0PackedReg.ScrollYCounter = 0;
  CpmemWord0PackedReg.NumberOfScroll = 0;
  CpmemWord0PackedReg.ScrollDeltaX = 0;
  CpmemWord0PackedReg.ScrollMax = 0;
  CpmemWord0PackedReg.ScrollingConfiguration = 0;
  CpmemWord0PackedReg.ScrollingEnable = 0;
  CpmemWord0PackedReg.ScrollDeltaY = 0;
  CpmemWord0PackedReg.ScrollHorizontalDirection = 0;
  CpmemWord0PackedReg.ScrollVerticalDirection = 0;

  // Bits per pixel
  CpmemWord0PackedReg.BitsPerPixel = FrameBufferPtr->Bpp;

  // Decode Address Select
  CpmemWord0PackedReg.DecodeAddressSelect = DecodeAddressSelect;

  // Scan order is progressive no support for interlace mode
  CpmemWord0PackedReg.ScanOrder = CPMEM_SO_PROGRESSIVE;

  // Band mode is sub frame double buffering
  CpmemWord0PackedReg.BandMode = CPMEM_BNDM_DISABLE;

  // Block mode used for post filtering and rotation
  CpmemWord0PackedReg.BlockMode = CPMEM_BM_DISABLE;

  // No support for rotation and flipping for now
  CpmemWord0PackedReg.Rotation = CPMEM_ROT_NO_ROTATION;
  CpmemWord0PackedReg.HorizontalFlip = CPMEM_HF_NO_HFLIP;
  CpmemWord0PackedReg.VerticalFlip = CPMEM_HF_NO_VFLIP;
  CpmemWord0PackedReg.ThresholdEnable = CPMEM_THE_DISABLE;

  // Disable conditional access
  CpmemWord0PackedReg.ConditionalAccessEnable = CPMEM_CAP_SKIP_LOW;
  CpmemWord0PackedReg.ConditionalAccessPolarity = CPMEM_CAE_DISABLE;

  // Frame width and height, minus one as 0 == 1 pixel
  CpmemWord0PackedReg.FrameWidth = FrameBufferPtr->Width - 1;
  CpmemWord0PackedReg.FrameHeight = FrameBufferPtr->Height - 1;

  // No interrupt required at the end of line
  CpmemWord0PackedReg.EndOfLineInterrupt = CPMEM_EOLI_DISABLE;

  // Setting up CPMEM word 1
  // Buffer 0, use the [31:3] bit address
  CpmemWord1PackedReg.ExtMemBuffer0Address = FrameBufferPtr->PhyAddr >> 3;

  // Buffer 1, use the same buffer for now
  CpmemWord1PackedReg.ExtMemBuffer1Address = FrameBufferPtr->PhyAddr >> 3;

  // Currently do not support interlace mode
  CpmemWord1PackedReg.InterlaceOffset = 0;

  // Pixel format and burst
  CpmemWord1PackedReg.NumberOfPixelsInWholeBurstAccess = PixelBurst;
  CpmemWord1PackedReg.PixelFormatSelect = PixelFormatSelector;

  // Alpha config
  CpmemWord1PackedReg.AlphaUsed = CPMEM_ALU_SAME_BUFFER;

  // AXI ID 0 should be okay, we don't have anything else contending for it
  CpmemWord1PackedReg.AxiId = CPMEM_ID_ID_0;

  CpmemWord1PackedReg.Threshold = CPMEM_THE_DISABLE;

  // Stride, width and offset
  CpmemWord1PackedReg.StrideLine = (FrameBufferPtr->Pitch * BytesPerPixel) - 1;
  CpmemWord1PackedReg.Width0 = Width0;
  CpmemWord1PackedReg.Width1 = Width1;
  CpmemWord1PackedReg.Width2 = Width2;
  CpmemWord1PackedReg.Width3 = Width3;
  CpmemWord1PackedReg.Offset0 = Offset0;
  CpmemWord1PackedReg.Offset1 = Offset1;
  CpmemWord1PackedReg.Offset2 = Offset2;
  CpmemWord1PackedReg.Offset3 = Offset3;

  // SX SY is ignored since scrolling is disabled
  CpmemWord1PackedReg.SelectSXSYSet = 0;

  // Conditional read is always enabled so fully transperant pixel are
  // not read.
  CpmemWord1PackedReg.ConditionalReadEnable = CPMEM_CRE_ENABLE;

  // Finally write into cpmem IDMAC channel
  pCpmemChannel = (pCpmemChannel + Channel);

  CopyMem (
    &pCpmemChannel->Word0Pack,
    &CpmemWord0PackedReg,
    sizeof (pCpmemChannel->Word0Pack)
  );

  CopyMem (
    &pCpmemChannel->Word1Pack,
    &CpmemWord1PackedReg,
    sizeof (pCpmemChannel->Word1Pack)
  );

  // IDMAC will generate 8 AXI bursts upon assertion of the DMA request
  // This significantly reduces memory activity and power consumption
  SetIdmacLockEn (DisplayInterfaceContextPtr, Channel, 0x3);

#ifdef DEBUG
  DumpBasicCpmemReg (pCpmemChannel);
#endif /* DEBUG */

Exit:
  return Status;
}
