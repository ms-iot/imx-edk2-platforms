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
#include "GopDxe.h"
#include "Hdmi.h"
#include "Edid.h"

PLL_MPLL_CONFIG PllMpllGenericConfigSetting[] = {
  {  13500000, 2,  8, {{3, 0, 3, 3, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  13500000, 2, 10, {{1, 4, 3, 3, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  13500000, 2, 12, {{2, 4, 3, 3, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{0, 0, 0,}}, },
  {  13500000, 2, 16, {{3, 1, 3, 2, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 0, 0,}}, },
  {  13500000, 4,  8, {{3, 1, 3, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 0, 0,}}, },
  {  13500000, 4, 10, {{1, 5, 3, 2, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 0, 0,}}, },
  {  13500000, 4, 12, {{2, 5, 3, 2, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 0, 0,}}, },
  {  13500000, 4, 16, {{3, 2, 3, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 0, 0,}}, },
  {  18000000, 3,  8, {{2, 1, 3, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 0, 0,}}, },
  {  18000000, 3, 16, {{2, 2, 3, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 0, 0,}}, },
  {  24175000, 1,  8, {{0, 0, 3, 3, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  24175000, 1, 10, {{1, 0, 3, 3, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  24175000, 1, 12, {{2, 0, 3, 3, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{0, 0, 0,}}, },
  {  24175000, 1, 16, {{3, 0, 2, 2, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  27000000, 1,  8, {{0, 0, 3, 3, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  27000000, 1, 10, {{1, 0, 3, 3, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  27000000, 1, 12, {{2, 0, 3, 3, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{0, 0, 0,}}, },
  {  27000000, 1, 16, {{3, 0, 2, 2, 0, 0, 3, 0,}}, {{4, 3, 5, 4, 0,}}, {{1, 0, 0,}}, },
  {  27000000, 2,  8, {{3, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  27000000, 2, 10, {{1, 1, 3, 2, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 0, 0,}}, },
  {  27000000, 2, 12, {{2, 1, 3, 2, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 0, 0,}}, },
  {  27000000, 2, 16, {{3, 1, 2, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 1, 0,}}, },
  {  27000000, 4,  8, {{3, 1, 2, 1, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 1, 0,}}, },
  {  27000000, 4, 10, {{1, 2, 3, 1, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 0, 0,}}, },
  {  27000000, 4, 12, {{2, 2, 3, 1, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 0, 0,}}, },
  {  27000000, 4, 16, {{3, 2, 2, 0, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 1, 0,}}, },
  {  32000000, 1,  8, {{0, 0, 3, 3, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  36000000, 1,  8, {{0, 0, 3, 3, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{0, 0, 0,}}, },
  {  36000000, 1, 16, {{3, 0, 2, 2, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  50350000, 1,  8, {{0, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  50350000, 1, 10, {{1, 0, 2, 2, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  50350000, 1, 12, {{2, 0, 2, 2, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  50350000, 1, 16, {{3, 0, 1, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  50350000, 2,  8, {{3, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  50350000, 2, 10, {{1, 1, 2, 1, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 1, 0,}}, },
  {  50350000, 2, 12, {{2, 1, 2, 1, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 1, 0,}}, },
  {  50350000, 2, 16, {{3, 1, 1, 0, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 2, 0,}}, },
  {  54000000, 1,  8, {{0, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  54000000, 1, 10, {{1, 0, 2, 2, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  54000000, 1, 12, {{2, 0, 2, 2, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  54000000, 1, 16, {{3, 0, 1, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  54000000, 2,  8, {{3, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  54000000, 2, 10, {{1, 1, 2, 1, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 1, 0,}}, },
  {  54000000, 2, 12, {{2, 1, 2, 1, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 1, 0,}}, },
  {  54000000, 2, 16, {{3, 1, 1, 0, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 2, 0,}}, },
  {  58400000, 1,  8, {{0, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{1, 1, 0,}}, },
  {  58400000, 1, 10, {{1, 0, 2, 2, 0, 0, 1, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  58400000, 1, 12, {{2, 0, 2, 2, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  58400000, 1, 16, {{3, 0, 1, 1, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  72000000, 1,  8, {{0, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  72000000, 1, 10, {{1, 0, 2, 2, 0, 0, 1, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  72000000, 1, 12, {{2, 0, 1, 1, 0, 0, 2, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  72000000, 1, 16, {{3, 0, 1, 1, 0, 0, 3, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  {  74250000, 1,  8, {{0, 0, 2, 2, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{1, 1, 0,}}, },
  {  74250000, 1, 10, {{1, 0, 1, 1, 0, 0, 1, 0,}}, {{4, 3, 5, 5, 0,}}, {{2, 2, 0,}}, },
  {  74250000, 1, 12, {{2, 0, 1, 1, 0, 0, 2, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  {  74250000, 1, 16, {{3, 0, 1, 1, 0, 0, 3, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 108000000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  { 108000000, 1, 10, {{1, 0, 1, 1, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  { 108000000, 1, 12, {{2, 0, 1, 1, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 108000000, 1, 16, {{3, 0, 0, 0, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 3, 0,}}, },
  { 118800000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  { 118800000, 1, 10, {{1, 0, 1, 1, 0, 0, 1, 0,}}, {{4, 3, 4, 4, 0,}}, {{2, 2, 0,}}, },
  { 118800000, 1, 12, {{2, 0, 1, 1, 0, 0, 2, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 118800000, 1, 16, {{3, 0, 0, 0, 0, 0, 3, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 3, 0,}}, },
  { 144000000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 144000000, 1, 10, {{1, 0, 0, 0, 0, 0, 1, 0,}}, {{4, 3, 5, 5, 0,}}, {{3, 3, 0,}}, },
  { 144000000, 1, 12, {{2, 0, 0, 0, 0, 0, 2, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 3, 0,}}, },
  { 144000000, 1, 16, {{3, 0, 0, 0, 0, 0, 3, 0,}}, {{4, 3, 3, 3, 0,}}, {{3, 3, 0,}}, },
  { 148500000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 148500000, 1, 10, {{1, 0, 0, 0, 0, 0, 1, 0,}}, {{4, 3, 5, 5, 0,}}, {{3, 3, 0,}}, },
  { 148500000, 1, 12, {{2, 0, 0, 0, 0, 0, 2, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 3, 0,}}, },
  { 148500000, 1, 16, {{3, 0, 0, 0, 0, 0, 3, 0,}}, {{4, 3, 3, 3, 0,}}, {{3, 3, 0,}}, },
  { 216000000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
  { 216000000, 1, 10, {{1, 0, 0, 0, 0, 0, 1, 0,}}, {{4, 3, 5, 5, 0,}}, {{3, 3, 0,}}, },
  { 216000000, 1, 12, {{2, 0, 0, 0, 0, 0, 2, 0,}}, {{4, 3, 4, 4, 0,}}, {{3, 3, 0,}}, },
  // Fallback
  {  65000000, 1,  8, {{0, 0, 1, 1, 0, 0, 0, 0,}}, {{4, 3, 3, 3, 0,}}, {{2, 2, 0,}}, },
};

BOOLEAN
HdmiPhyPollI2cDone (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  UINT32                      Timeout
  )
{
  HDMI_IH_I2CMPHY_STAT0_REG   I2cmphyStat0Reg;
  BOOLEAN                     WaitResult;

  WaitResult = FALSE;
  for (; Timeout > 0; Timeout--) {
    I2cmphyStat0Reg.Reg = MmioRead8 (
                            (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                            HDMI_IH_I2CMPHY_STAT0
                          );
    if (I2cmphyStat0Reg.i2cmphydone) {
      WaitResult = TRUE;
      break;
    }
    gBS->Stall (1);
  }

#if DEBUG
  if ((Timeout == 0) || (I2cmphyStat0Reg.i2cmphyerror == 1)) {
    DEBUG ((DEBUG_ERROR, "%a: HDMI I2C failed value %x time out %d\n",
      __FUNCTION__, I2cmphyStat0Reg.Reg, Timeout));
  }
#endif

  return WaitResult;
}

BOOLEAN
HdmiPhyI2cRead (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  UINT8                       Addr,
  OUT UINT16                      *DataPtr
  )
{
  UINT16                            Data0;
  UINT16                            Data1;
  HDMI_PHY_I2CM_OPERATION_ADDR_REG  I2cmOperationReg;
  HDMI_IH_I2CMPHY_STAT0_REG         I2cmphyStat0Reg;
  BOOLEAN                           ReadStatus;

  I2cmphyStat0Reg.i2cmphyerror = 1;
  I2cmphyStat0Reg.i2cmphydone = 1;
  I2cmphyStat0Reg.reserved = 0;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_IH_I2CMPHY_STAT0,
    I2cmphyStat0Reg.Reg
  );
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_ADDRESS_ADDR,
    Addr
  );

  I2cmOperationReg.Reg = MmioRead8 (
                           (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                           HDMI_PHY_I2CM_OPERATION_ADDR
                         );
  I2cmOperationReg.read = 1;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_OPERATION_ADDR,
    I2cmOperationReg.Reg
  );

  ReadStatus = HdmiPhyPollI2cDone (HdmiDisplayContextPtr, 1000);
  if (!ReadStatus) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to read I2c HDMI Phy\n", __FUNCTION__));
    goto Exit;
  }

  Data0 = MmioRead8 (
            (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
            HDMI_PHY_I2CM_DATAI_0_ADDR
          );
  Data1 = MmioRead8 (
            (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
            HDMI_PHY_I2CM_DATAI_1_ADDR
          );
  *DataPtr = Data0 | (Data1 <<  8);

Exit:
  return ReadStatus;
}

BOOLEAN
HdmiPhyI2cWrite (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  UINT8                       Addr,
  IN  UINT16                      Data
  )
{
  UINT8                             Data0;
  UINT8                             Data1;
  HDMI_PHY_I2CM_OPERATION_ADDR_REG  I2cmOperationReg;
  HDMI_IH_I2CMPHY_STAT0_REG         I2cmphyStat0Reg;

  Data0 = (Data & 0x00FF);
  Data1 = (Data >>  8);
  I2cmphyStat0Reg.i2cmphyerror = 1;
  I2cmphyStat0Reg.i2cmphydone = 1;
  I2cmphyStat0Reg.reserved = 1;
  I2cmOperationReg.read = 0;
  I2cmOperationReg.reserved0 = 0;
  I2cmOperationReg.write = 0;
  I2cmOperationReg.reserved1 = 0;

  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_IH_I2CMPHY_STAT0,
    I2cmphyStat0Reg.Reg
  );
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_ADDRESS_ADDR,
    Addr
  );
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_DATAO_0_ADDR,
    Data0
  );
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_DATAO_1_ADDR,
    Data1
  );

  I2cmOperationReg.read = 0;
  I2cmOperationReg.write = 1;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_I2CM_OPERATION_ADDR,
    I2cmOperationReg.Reg
  );
  return HdmiPhyPollI2cDone (HdmiDisplayContextPtr, 1000);
}

BOOLEAN
GetGenericConfigSetting (
  IN  IMX_DISPLAY_TIMING    *DisplayTimingPtr,
  OUT PLL_MPLL_CONFIG       **ConfigGenericSettingPPtr
  )
{
  UINT32    ColorDepth;
  BOOLEAN   FoundConfig;
  UINT32    SettingIndex;

  FoundConfig = FALSE;
  ColorDepth = GetColorDepth (DisplayTimingPtr->PixelFormat);

  for (SettingIndex = 0;
       SettingIndex < ARRAYSIZE (PllMpllGenericConfigSetting);
       ++SettingIndex)
  {
    if ((DisplayTimingPtr->PixelClock ==
         PllMpllGenericConfigSetting[SettingIndex].PixelClock) &&
        (DisplayTimingPtr->PixelRepetition ==
         PllMpllGenericConfigSetting[SettingIndex].PixelRepetition) &&
        (ColorDepth == PllMpllGenericConfigSetting[SettingIndex].ColorDepth))
    {
      FoundConfig = TRUE;
      *ConfigGenericSettingPPtr = &PllMpllGenericConfigSetting[SettingIndex];
      break;
    }
  }

  // Use the fallback value the last index if no configuration is found
  if (FoundConfig == FALSE) {
    *ConfigGenericSettingPPtr =
      &PllMpllGenericConfigSetting[ARRAYSIZE (PllMpllGenericConfigSetting)];
    FoundConfig = TRUE;
  }

  return FoundConfig;
}

EFI_STATUS
InitHdmi (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  )
{
  DISPLAY_INTERFACE_CONTEXT   *pHdmiDisplayContext;
  EFI_STATUS                  Status;

  pHdmiDisplayContext = &DisplayContextPtr->DiContext[HdmiDisplay];
  Status = EFI_SUCCESS;
  ZeroMem (pHdmiDisplayContext, sizeof (*pHdmiDisplayContext));

  pHdmiDisplayContext->MmioBasePtr = (VOID *)HDMI_BASE;
  if (pHdmiDisplayContext->MmioBasePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to map HDMI register\n", __FUNCTION__));
    goto Exit;
  }

  // Setup HDMI DDC muxing
  MmioWrite32 ((UINT32)IOMUXC_SW_MUX_CTL_PAD_KEY_COL3, 0x00000012);
  MmioWrite32 ((UINT32)IOMUXC_SW_MUX_CTL_PAD_KEY_ROW3, 0x00000012);
  MmioWrite32 ((UINT32)IOMUXC_HDMI_II2C_CLKIN_SELECT_INPUT, 0x00000001);
  MmioWrite32 ((UINT32)IOMUXC_HDMI_II2C_DATAIN_SELECT_INPUT, 0x00000001);
  SetHdmiPower (pHdmiDisplayContext, TRUE);

  // Mask all HDMI PHY interrupt
  MmioWrite8 (
    (UINT32)pHdmiDisplayContext->MmioBasePtr + HDMI_PHY_MASK0,
    0xFF
  );

  Status = ReadEdid (
             DisplayContextPtr,
             HdmiDisplay,
             pHdmiDisplayContext->EdidData,
             &pHdmiDisplayContext->EdidDataSize
           );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "%a: Fail to read HDMI EDID data\n", __FUNCTION__));
    Status = EFI_SUCCESS;
  }

  Status = GetPreferredTiming (
             pHdmiDisplayContext->EdidData,
             pHdmiDisplayContext->EdidDataSize,
             &pHdmiDisplayContext->PreferredTiming
           );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to retrieve HDMI preferred timing\n",
      __FUNCTION__));
    goto Exit;
  }

  if ((pHdmiDisplayContext->PreferredTiming.HActive == 1920) &&
      (pHdmiDisplayContext->PreferredTiming.VActive == 1080))
  {
    pHdmiDisplayContext->PreferredTiming.HBlank -= 6;
  }

Exit:
  return Status;
}

EFI_STATUS
SetHdmiPower (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  BOOLEAN                     PowerState
  )
{
  HDMI_PHY_CONF0_REG  CurrentHdmiPhyConf0Reg;

  CurrentHdmiPhyConf0Reg.Reg = MmioRead8 (
                                 (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                                 HDMI_PHY_CONF0
                               );
  if (PowerState) {
    // Setup PHY
    CurrentHdmiPhyConf0Reg.PDZ = 1;
    CurrentHdmiPhyConf0Reg.ENTMDS = 1;
    CurrentHdmiPhyConf0Reg.gen2_pddq = 1;
    CurrentHdmiPhyConf0Reg.gen2_txpwron = 1;
    CurrentHdmiPhyConf0Reg.seldataenpol = 1;
    CurrentHdmiPhyConf0Reg.seldipif = 0;
  } else {
    // Just power down PHY for shutdown
    CurrentHdmiPhyConf0Reg.PDZ = 0;
  }

  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_CONF0,
    CurrentHdmiPhyConf0Reg.Reg
  );
  gBS->Stall (3);

  return EFI_SUCCESS;
}

EFI_STATUS
SetHdmiPhy (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  IMX_DISPLAY_TIMING          *Timings
  )
{
  PLL_MPLL_CONFIG           *pPllMpllConfig;
  HDMI_PHY_CONF0_REG        CurrentHdmiPhyConf0Reg;
  HDMI_FC_AUDSCONF_REG      FcAudsconfReg;
  HDMI_MC_HEACPHY_RST_REG   HeacphyRstReg;
  HDMI_MC_CLKDIS_REG        McClkdisReg;
  HDMI_MC_PHYRSTZ_REG       PhyRstzReg;
  HDMI_PHY_STAT0_REG        PhyStat0Reg;
  BOOLEAN                   PhyStatus;
  UINT32                    RetryCount;
  EFI_STATUS                Status;

  // Disable Audio
  FcAudsconfReg.Reg = MmioRead8 (
                        (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                        HDMI_FC_AUDSCONF
                      );
  FcAudsconfReg.aud_packet_layout = 0;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_FC_AUDSCONF,
    FcAudsconfReg.Reg
  );

  // Minimum PCLK period / frequency (pixel repetition) : 74 ns / 13.5 MHz
  // Minimum PCLK period / frequency (no pixel repetition) : 39.7 ns / 24.175 MHz
  if (Timings->PixelClock < 13500000) {
    DEBUG ((DEBUG_ERROR, "%a: Unsupported pixel clock %d\n",
      __FUNCTION__, Timings->PixelClock));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (GetGenericConfigSetting (Timings, &pPllMpllConfig) == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: No compatible generic config found\n",
      __FUNCTION__));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  // Color Space Converter : Not used in UEFI
  McClkdisReg.Reg = MmioRead8 (
                      (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                      HDMI_MC_CLKDIS
                    );
  // Disable CEC, color converter, audio & pixel repitition
  McClkdisReg.cecclk_disable = 1;
  McClkdisReg.cscclk_disable = 1;
  McClkdisReg.audclk_disable = 1;
  McClkdisReg.prepclk_disable = 1;
  McClkdisReg.hdcpclk_disable = 1;
  McClkdisReg.tmdsclk_disable = 0;
  McClkdisReg.pixelclk_disable = 0;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_MC_CLKDIS,
    McClkdisReg.Reg
  );

  // Power down the PHY
  // To set the HDMI_PHY in Power-down mode, set the TX_PWRON signal to 1'b0
  // and the PDDQ signal to 1'b1. To power up the HDMI 3D Tx PHY and place it
  // in Active mode, set TX_PWRON to 1'b1 and PDDQ to 1'b0. Any configuration
  // programmed on the HDMI_PHY must be done in Power-down mode.
  CurrentHdmiPhyConf0Reg.Reg = MmioRead8 (
                                 (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                                 HDMI_PHY_CONF0
                               );
  CurrentHdmiPhyConf0Reg.gen2_txpwron = 0;
  CurrentHdmiPhyConf0Reg.gen2_pddq = 1;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_CONF0,
    CurrentHdmiPhyConf0Reg.Reg
  );

  // Let's reset the PHY to a well defined state based on spec.
  // The PHY_RESET signal is used to place the digital section of the IP in
  // a well - defined state
  PhyRstzReg.Reg = MmioRead8 (
                     (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                     HDMI_MC_PHYRSTZ
                   );
  PhyRstzReg.phyrstz = 1;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_MC_PHYRSTZ,
    PhyRstzReg.Reg
  );
  PhyRstzReg.phyrstz = 0;
  gBS->Stall (10);
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_MC_PHYRSTZ,
    PhyRstzReg.Reg
  );

  HeacphyRstReg.Reg = MmioRead8 (
                        (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                        HDMI_MC_HEACPHY_RST
                      );
  HeacphyRstReg.heacphyrst = 1;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_MC_HEACPHY_RST,
    HeacphyRstReg.Reg
  );

  // Program clock
  // PLL / MPLL Operation
  // The PLL / MPLL can be configured in Coherent mode or NonCoherent mode (default).
  //  In Coherent mode, the TMDS clock is the MPLL feedback clock, which is
  //  coherent with the MPLL's high-speed output clock, because both clocks are
  //  shaped by the MPLL response.
  //  In NonCoherent mode, the TMDS clock is the MPLL reference clock, which is
  //  not coherent with the MPLL's high-speed output clock.
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_CPCE_CTRL,
                pPllMpllConfig->HdmiPhyCpceCtrl.Reg
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_CPCE_CTRL %x\n",
      __FUNCTION__, pPllMpllConfig->HdmiPhyCpceCtrl.Reg));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_CURRCTRL,
                pPllMpllConfig->HdmiPhyCurrctrl.Reg
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_CURRCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  };

  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_GMPCTRL,
                pPllMpllConfig->HdmiPhyGmpctrl.Reg
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_GMPCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Maintaining the order of phy register writes
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_PLLPHBYCTRL,
                0x0000
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_PLLPHBYCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Coherent mode
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_MSM_CTRL,
                0x0006
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_MSM_CTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Resistance value 133.33 ohm
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_TXTERM,
                0x0005
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_TXTERM\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Enable clock symbol
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_CKSYMTXCTRL,
                0x8009
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_CKSYMTXCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_VLEVCTRL,
                0x0210
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_VLEVCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Enable override
  PhyStatus = HdmiPhyI2cWrite (
                HdmiDisplayContextPtr,
                HDMI_PHY_CKCALCTRL,
                0x8000
              );
  if (PhyStatus == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to write to HDMI_PHY_CKCALCTRL\n",
      __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  CurrentHdmiPhyConf0Reg.gen2_txpwron = 1;
  CurrentHdmiPhyConf0Reg.gen2_pddq = 0;
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_PHY_CONF0,
    CurrentHdmiPhyConf0Reg.Reg
  );

  Status = EFI_DEVICE_ERROR;
  for (RetryCount = 5; RetryCount > 0; RetryCount--) {
    PhyStat0Reg.Reg = MmioRead8 (
                        (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                        HDMI_PHY_STAT0
                      );
    if (!PhyStat0Reg.TX_PHY_LOCK) {
      Status = EFI_SUCCESS;
      break;
    }
    gBS->Stall (1000);
  }

  if (RetryCount == 0) {
    DEBUG ((DEBUG_ERROR, "%a: TX PHY remain unlock\n", __FUNCTION__));
  }

Exit:
  return Status;
}

EFI_STATUS
SetHdmiDisplay (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  IMX_DISPLAY_TIMING          *Timings
  )
{
  EFI_STATUS  Status;

  Status = SetHdmiPhy (HdmiDisplayContextPtr, Timings);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: SetHdmiPhy failed\n", __FUNCTION__));
    goto Exit;
  }

Exit:
  return Status;
}

VOID
SetDdcSpeed (
  IN  DISPLAY_INTERFACE_CONTEXT *HdmiDisplayContextPtr,
  IN  DDC_MODE                  Mode
  )
{
  MmioWrite8 ((UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_I2CM_DIV, Mode);
}

EFI_STATUS
HdmiDdcRead (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  UINT8                       SlaveAddress,
  IN  UINT8                       RegisterAddress,
  IN  UINT32                      ReadSize,
  IN  DDC_MODE                    DDCMode,
  IN  UINT8                       *DataReadPtr
  )
{
  UINT8       *pCurrentDataRead;
  UINT32      AddrCount;
  UINT8       I2cmIntStatus;
  UINT32      I2cRetryCount;
  EFI_STATUS  Status;

  pCurrentDataRead = DataReadPtr;
  Status = EFI_SUCCESS;

  // Setup EDID transaction and loop through all byte request
  SetDdcSpeed (HdmiDisplayContextPtr, DDCMode);
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_IH_I2CM_STAT0,
    I2C_MASTER_ERROR | I2C_MASTER_DONE
  );
  MmioWrite8 (
    (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_I2CM_SLAVE,
    SlaveAddress
  );

  for (AddrCount = 0; AddrCount < ReadSize; ++AddrCount) {

    MmioWrite8 (
      (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_I2CM_ADDRESS,
      (UINT8) ( RegisterAddress + AddrCount)
    );
    MmioWrite8 (
      (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_I2CM_SEGADDR,
      0x00
    );
    MmioWrite8 (
      (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_I2CM_OPERATION,
      DDC_READ_OPERATION
    );

    // Poll for completion
    I2cmIntStatus = MmioRead8 (
                      (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                      HDMI_IH_I2CM_STAT0
                    );
    for (I2cRetryCount = 10; I2cRetryCount > 0; I2cRetryCount--) {
      I2cmIntStatus = MmioRead8 (
                        (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                        HDMI_IH_I2CM_STAT0
                      );
      if (I2cmIntStatus != 0) {
        break;
      }
      gBS->Stall (2000);
    }

    if (I2cRetryCount == 0) {
      Status = EFI_DEVICE_ERROR;
      DEBUG ((DEBUG_ERROR, "%a: Timeout waiting for interrupt 0x%02x\n",
        __FUNCTION__, I2cmIntStatus));
      goto Exit;
    }

    if ((I2cmIntStatus & I2C_MASTER_DONE) &&
        !(I2cmIntStatus & I2C_MASTER_ERROR))
    {
      *pCurrentDataRead = MmioRead8 (
                            (UINT32)HdmiDisplayContextPtr->MmioBasePtr +
                            HDMI_I2CM_DATAI
                          );
      pCurrentDataRead++;
      MmioWrite8 (
        (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_IH_I2CM_STAT0,
        I2C_MASTER_ERROR | I2C_MASTER_DONE
      );
    } else {
      Status = EFI_DEVICE_ERROR;
      DEBUG ((DEBUG_ERROR, "%a: Failed to read with DDC 0x%02x\n",
        __FUNCTION__, I2cmIntStatus));
      goto Exit;
    }

    MmioWrite8 (
      (UINT32)HdmiDisplayContextPtr->MmioBasePtr + HDMI_IH_I2CM_STAT0,
      I2C_MASTER_ERROR | I2C_MASTER_DONE
    );
  }

Exit:
  return Status;
}
