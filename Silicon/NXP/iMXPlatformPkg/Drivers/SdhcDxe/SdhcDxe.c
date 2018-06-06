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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EmbeddedExternalDevice.h>
#include <Protocol/Sdhc.h>

#include <iMXuSdhc.h>
#include <iMXGpio.h>
#include "SdhcDxe.h"

VOID
DumpState (
  IN USDHC_PRIVATE_CONTEXT *SdhcCtx
  )
{
  DEBUG_CODE_BEGIN ();

  USDHC_REGISTERS         *Reg;
  USDHC_BLK_ATT_REG       BlkAtt;
  UINT32                  CmdArg;
  USDHC_CMD_XFR_TYP_REG   CmdXfrTyp;
  UINT32                  IntSignalEn;
  USDHC_INT_STATUS_REG    IntStatus;
  UINT32                  IntStatusEn;
  USDHC_MIX_CTRL_REG      MixCtrl;
  UINT32                  MmcBoot;
  USDHC_PRES_STATE_REG    PresState;
  USDHC_PROT_CTRL_REG     ProtCtrl;
  USDHC_WTMK_LVL_REG      WtmkLvl;
  UINT32                  VendSpec;
  UINT32                  VendSpec2;

  Reg = SdhcCtx->RegistersBase;
  BlkAtt.AsUint32 = MmioRead32 ((UINTN)&Reg->BLK_ATT);
  CmdArg = MmioRead32 ((UINTN)&Reg->CMD_ARG);
  CmdXfrTyp.AsUint32 = MmioRead32 ((UINTN)&Reg->CMD_XFR_TYP);
  ProtCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->PROT_CTRL);
  WtmkLvl.AsUint32 = MmioRead32 ((UINTN)&Reg->WTMK_LVL);
  MixCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->MIX_CTRL);
  IntStatusEn = MmioRead32 ((UINTN)&Reg->INT_STATUS_EN);
  IntSignalEn = MmioRead32 ((UINTN)&Reg->INT_SIGNAL_EN);
  VendSpec = MmioRead32 ((UINTN)&Reg->VEND_SPEC);
  MmcBoot = MmioRead32 ((UINTN)&Reg->MMC_BOOT);
  VendSpec2 = MmioRead32 ((UINTN)&Reg->VEND_SPEC2);
  IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);

  LOG_INFO (
    " - BLK_ATT\t:0x%08x BLKSIZE:0x%x BLKCNT:0x%08x",
    BlkAtt.AsUint32,
    BlkAtt.Fields.BLKSIZE,
    BlkAtt.Fields.BLKCNT);

  LOG_INFO (" - CMD_ARG\t:0x%08x", CmdArg);

  LOG_INFO (
    " - CMD_XFR_TYP\t:0x%08x RSPTYP:%d CCCEN:%d CICEN:%d DPSEL:%d CMDTYP:%d CMDINX:%d",
    CmdXfrTyp.AsUint32,
    CmdXfrTyp.Fields.RSPTYP,
    CmdXfrTyp.Fields.CCCEN,
    CmdXfrTyp.Fields.CICEN,
    CmdXfrTyp.Fields.DPSEL,
    CmdXfrTyp.Fields.CMDTYP,
    CmdXfrTyp.Fields.CMDINX);

  LOG_INFO (
    " - PROT_CTRL\t:0x%08x DTW:%d D3CD:%d CDSS:%d EMODE:%d DMASEL:%d SABGREQ:%d BURST_LEN_EN:%d",
    ProtCtrl.AsUint32,
    ProtCtrl.Fields.DTW,
    ProtCtrl.Fields.D3CD,
    ProtCtrl.Fields.CDSS,
    ProtCtrl.Fields.EMODE,
    ProtCtrl.Fields.DMASEL,
    ProtCtrl.Fields.SABGREQ,
    ProtCtrl.Fields.BURST_LEN_EN);

  LOG_INFO (
    " - WTMK_LVL\t:0x%08x RD_WML:%d RD_BRST_LEN:%d WR_WML:%d WR_BRST_LEN:%d",
    WtmkLvl.AsUint32,
    WtmkLvl.Fields.RD_WML,
    WtmkLvl.Fields.RD_BRST_LEN,
    WtmkLvl.Fields.WR_WML,
    WtmkLvl.Fields.WR_BRST_LEN);

  LOG_INFO (
    " - MIX_CTRL\t:0x%08x DMAEN:%d BCEN:%d AC12EN:%d DTDSEL:%d MSBSEL:%d AC23EN:%d FBCLK_SEL:%d",
    MixCtrl.AsUint32,
    MixCtrl.Fields.DMAEN,
    MixCtrl.Fields.BCEN,
    MixCtrl.Fields.AC12EN,
    MixCtrl.Fields.DTDSEL,
    MixCtrl.Fields.MSBSEL,
    MixCtrl.Fields.AC23EN,
    MixCtrl.Fields.FBCLK_SEL);

  LOG_INFO (" - INT_STATUS_EN\t:0x%08x", IntStatusEn);
  LOG_INFO (" - INT_SIGNAL_EN\t:0x%08x", IntSignalEn);
  LOG_INFO (" - VEND_SPEC\t:0x%08x", VendSpec);
  LOG_INFO (" - MMC_BOOT\t:0x%08x", MmcBoot);
  LOG_INFO (" - VEND_SPEC2\t:0x%08x", VendSpec2);

  LOG_INFO (
    " - INT_STATUS\t:0x%08x CC:%d TC:%d BWR:%d BRR:%d CTOE:%d CCE:%d CEBE:%d CIE:%d DTOE:%d DCE:%d DEBE:%d",
    IntStatus.AsUint32,
    IntStatus.Fields.CC,
    IntStatus.Fields.TC,
    IntStatus.Fields.BWR,
    IntStatus.Fields.BRR,
    IntStatus.Fields.CTOE,
    IntStatus.Fields.CCE,
    IntStatus.Fields.CEBE,
    IntStatus.Fields.CIE,
    IntStatus.Fields.DTOE,
    IntStatus.Fields.DCE,
    IntStatus.Fields.DEBE);

  LOG_INFO (
    " - PRES_STATE\t:0x%08x CIHB:%d CDIHB:%d DLA:%d WTA:%d RTA:%d BWEN:%d BREN:%d CINST:%d DLSL:0x%x",
    PresState.AsUint32,
    PresState.Fields.CIHB,
    PresState.Fields.CDIHB,
    PresState.Fields.DLA,
    PresState.Fields.WTA,
    PresState.Fields.RTA,
    PresState.Fields.BWEN,
    PresState.Fields.BREN,
    PresState.Fields.CINST,
    PresState.Fields.DLSL);

  DEBUG_CODE_END ();
}

EFI_STATUS
WaitForReadFifo (
  IN USDHC_PRIVATE_CONTEXT *SdhcCtx
  )
{
  USDHC_REGISTERS       *Reg;
  USDHC_INT_STATUS_REG  IntStatus;
  UINT32                Retry;

  Reg = SdhcCtx->RegistersBase;
  IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  Retry = USDHC_POLL_RETRY_COUNT;

  while (!IntStatus.Fields.BRR &&
         !(IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) &&
         Retry) {
    --Retry;
    gBS->Stall (USDHC_POLL_WAIT_US);
    IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  }

  if (IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) {
    LOG_ERROR ("Error detected");
    DumpState (SdhcCtx);
    return EFI_DEVICE_ERROR;
  } else if (IntStatus.Fields.BRR) {
    MmioWrite32 ((UINTN)&Reg->INT_STATUS, IntStatus.AsUint32);
    return EFI_SUCCESS;
  } else {
    ASSERT (!Retry);
    LOG_ERROR ("Time-out waiting on read FIFO");
    DumpState (SdhcCtx);
    return EFI_TIMEOUT;
  }
}

EFI_STATUS
WaitForWriteFifo (
  IN USDHC_PRIVATE_CONTEXT *SdhcCtx
  )
{
  USDHC_REGISTERS       *Reg;
  USDHC_INT_STATUS_REG  IntStatus;
  UINT32                Retry;

  Reg = SdhcCtx->RegistersBase;
  IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS) ;
  Retry = USDHC_POLL_RETRY_COUNT;

  while (!IntStatus.Fields.BWR &&
         !(IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) &&
         Retry) {
    --Retry;
    gBS->Stall (USDHC_POLL_WAIT_US);
    IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  }

  if (IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) {
    LOG_ERROR ("Error detected");
    DumpState (SdhcCtx);
    return EFI_DEVICE_ERROR;
  } else if (IntStatus.Fields.BWR) {
    MmioWrite32 ((UINTN)&Reg->INT_STATUS, IntStatus.AsUint32);
    return EFI_SUCCESS;
  } else {
    ASSERT (!Retry);
    LOG_ERROR ("Time-out waiting on write FIFO");
    DumpState (SdhcCtx);
    return EFI_TIMEOUT;
  }
}

EFI_STATUS
WaitForCmdAndOrDataLine (
  IN USDHC_PRIVATE_CONTEXT *SdhcCtx,
  IN CONST SD_COMMAND *Cmd
  )
{
  USDHC_REGISTERS       *Reg;
  USDHC_INT_STATUS_REG  IntStatus;
  USDHC_PRES_STATE_REG  PresState;
  UINT32                Retry;
  BOOLEAN               WaitForDataLine;

  // Waiting on the DATA lines is the default behavior if no CMD is specified
  if (Cmd == NULL) {
    WaitForDataLine = TRUE;
  } else {
    // Per datasheet, the SDHC can isssue CMD0, CMD12, CMD13 and CMD52
    // when the DATA lines are busy during a data transfer. Other commands
    // should wait on the DATA lines before issuing
    switch (Cmd->Index) {
    case 0:
    case 12:
    case 13:
    case 52:
      WaitForDataLine = FALSE;
      break;
    default:
      WaitForDataLine = TRUE;
    }
  }

  Reg = SdhcCtx->RegistersBase;
  PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
  IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS) ;
  Retry = USDHC_POLL_RETRY_COUNT;

  while (PresState.Fields.CIHB &&
         (!WaitForDataLine || PresState.Fields.CDIHB) &&
         !(IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) &&
         Retry) {
    gBS->Stall (USDHC_POLL_WAIT_US);
    --Retry;
    PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
    IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  }

  if (IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) {
    LOG_ERROR ("Error detected");
    DumpState (SdhcCtx);
    return EFI_DEVICE_ERROR;
  } else if (!(PresState.Fields.CIHB &&
             (!WaitForDataLine || PresState.Fields.CDIHB))) {
    return EFI_SUCCESS;
  } else {
    ASSERT (!Retry);
    LOG_ERROR ("Time-out waiting on CMD and/or DATA lines");
    DumpState (SdhcCtx);
    return EFI_TIMEOUT;
  }
}

EFI_STATUS
WaitForCmdResponse (
  IN USDHC_PRIVATE_CONTEXT *SdhcCtx
  )
{
  USDHC_REGISTERS       *Reg;
  USDHC_INT_STATUS_REG  IntStatus;
  UINT32                Retry;

  Reg = SdhcCtx->RegistersBase;
  IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS) ;
  Retry = USDHC_POLL_RETRY_COUNT;

  // Wait for command to finish execution either with success or failure
  while (!IntStatus.Fields.CC &&
         !(IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) &&
         Retry) {
    gBS->Stall (USDHC_POLL_WAIT_US);
    --Retry;
    IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
  }

  if (IntStatus.AsUint32 & USDHC_INT_STATUS_ERROR) {
    LOG_ERROR ("Error detected");
    DumpState (SdhcCtx);
    return EFI_DEVICE_ERROR;
  } else if (IntStatus.Fields.CC) {
    MmioWrite32 ((UINTN)&Reg->INT_STATUS, IntStatus.AsUint32);
    return EFI_SUCCESS;
  } else {
    ASSERT (!Retry);
    LOG_ERROR ("Time-out waiting on command completion");
    DumpState (SdhcCtx);
    return EFI_TIMEOUT;
  }
}

EFI_STATUS
SdhcSetBusWidth (
  IN EFI_SDHC_PROTOCOL *This,
  IN SD_BUS_WIDTH BusWidth
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  USDHC_PROT_CTRL_REG     ProtCtrl;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  Reg = SdhcCtx->RegistersBase;
  ProtCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->PROT_CTRL);

  LOG_TRACE ("SdhcSetBusWidth(%d)", BusWidth);

  switch (BusWidth) {
  case SdBusWidth1Bit:
    ProtCtrl.Fields.DTW = USDHC_PROT_CTRL_DTW_1BIT;
    break;
  case SdBusWidth4Bit:
    ProtCtrl.Fields.DTW = USDHC_PROT_CTRL_DTW_4BIT;
    break;
  case SdBusWidth8Bit:
    ProtCtrl.Fields.DTW = USDHC_PROT_CTRL_DTW_8BIT;
    break;
  default:
    LOG_ASSERT ("Invalid bus width");
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 ((UINTN)&Reg->PROT_CTRL, ProtCtrl.AsUint32);

  return EFI_SUCCESS;
}

EFI_STATUS
SdhcSetClock (
  IN EFI_SDHC_PROTOCOL *This,
  IN UINT32 TargetFreqHz
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  UINT32                  BestDivisor;
  UINT32                  BestPrescaler;
  UINT32                  Divisor;
  UINT32                  FreqDistance;
  UINT32                  MinFreqDistance;
  USDHC_MIX_CTRL_REG      MixCtrl;
  UINT32                  Prescaler;
  USDHC_PRES_STATE_REG    PresState;
  UINT32                  Retry;
  UINT32                  SdClk;
  USDHC_SYS_CTRL_REG      SysCtrl;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  LOG_TRACE ("SdhcSetClock(%dHz)", TargetFreqHz);

  Reg = SdhcCtx->RegistersBase;
  SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);

  // SdClk = (Base Clock) / (prescaler x divisor)
  MixCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->MIX_CTRL);

  // Bruteforce to find the best prescaler and divisor that result
  // in SdClk less than or equal to the requested frequency
  //
  // Allowed |Base clock divided By
  // SDCLKFS |DDR_EN=0   |DDR_EN=1
  // 80h      256         512
  // 40h      128         256
  // 20h      64          128
  // 10h      32          64
  // 08h      16          32
  // 04h      8           16
  // 02h      4           8
  // 01h      2           4
  // 00h      1           2
  CONST UINT32 PRESCALER_MIN = (MixCtrl.Fields.DDR_EN ? 2 : 1);
  CONST UINT32 PRESCALER_MAX = (MixCtrl.Fields.DDR_EN ? 512 : 256);;
  CONST UINT32 DIVISOR_MIN = 1;
  CONST UINT32 DIVISOR_MAX = 16;
  MinFreqDistance = MAX_UINT32;
  BestPrescaler = 0;
  BestDivisor = 0;

  //
  // Bruteforce to find the best prescaler and divisor that result
  // in SdClk less than or equal to the requested frequency
  //
  for (Prescaler = PRESCALER_MAX; Prescaler >= PRESCALER_MIN; Prescaler /= 2) {
    for (Divisor = DIVISOR_MIN; Divisor <= DIVISOR_MAX; ++Divisor) {
      SdClk = USDHC_BASE_CLOCK_FREQ_HZ / (Prescaler * Divisor);

      //
      // We are not willing to choose clocks higher than the target one
      // to avoid exceeding device limits
      //
      if (SdClk > TargetFreqHz) {
        continue;
      } else if (SdClk == TargetFreqHz) {
        BestPrescaler = Prescaler;
        BestDivisor = Divisor;
        break;
      } else {
        FreqDistance = TargetFreqHz - SdClk;
        if (FreqDistance < MinFreqDistance) {
          MinFreqDistance = FreqDistance;
          BestPrescaler = Prescaler;
          BestDivisor = Divisor;
        }
      }
    }
  }

  // Wait for clock to become stable before any modifications
  PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
  Retry = USDHC_POLL_RETRY_COUNT;

  while (!PresState.Fields.SDSTB &&
         Retry) {
    gBS->Stall (USDHC_POLL_WAIT_US);
    --Retry;
    PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
  }

  if (!PresState.Fields.SDSTB) {
    ASSERT (!Retry);
    LOG_ERROR ("Time-out waiting on SD clock to stabilize");
    DumpState (SdhcCtx);
    return EFI_TIMEOUT;
  }

  SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
  SysCtrl.Fields.SDCLKFS = BestPrescaler / (MixCtrl.Fields.DDR_EN ? 4 : 2);
  SysCtrl.Fields.DVS = BestDivisor - 1;

  MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);

  SdClk = USDHC_BASE_CLOCK_FREQ_HZ / (BestPrescaler * BestDivisor);

  LOG_TRACE (
    "Current SdClk:%dHz SDCLKFS:0x%x DVS:0x%x",
    SdClk,
    (UINT32)SysCtrl.Fields.SDCLKFS,
    (UINT32)SysCtrl.Fields.DVS);

  return EFI_SUCCESS;
}

BOOLEAN
SdhcIsCardPresent (
  IN EFI_SDHC_PROTOCOL *This
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  IMX_GPIO_VALUE          CardDetectLevel;
  BOOLEAN                 IsCardPresent;
  USDHC_PRES_STATE_REG    PresState;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  if (SdhcCtx->CardDetectSignal == USDHC_SIGNAL_INTERNAL_PIN) {
    Reg = SdhcCtx->RegistersBase;
    PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
    IsCardPresent = (PresState.Fields.CINST == 1);
  } else {
    if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->CardDetectSignal)) {
      //Read the state of  CD_B pin for the card socket
      CardDetectLevel = ImxGpioRead (
                         SdhcCtx->CardDetectGpioPin.Bank,
                         SdhcCtx->CardDetectGpioPin.IoNumber
                         );
    } else if (SdhcCtx->CardDetectSignal == USDHC_SIGNAL_OVERRIDE_PIN_LOW) {
      CardDetectLevel = IMX_GPIO_LOW;
    } else if (SdhcCtx->CardDetectSignal == USDHC_SIGNAL_OVERRIDE_PIN_HIGH) {
      CardDetectLevel = IMX_GPIO_HIGH;
    } else {
      ASSERT (!"Invalid CardDetect signal source");
      CardDetectLevel = IMX_GPIO_LOW;
    }

    // When no card is present,  CD_B is pulled-high, and the SDCard when
    // inserted will pull CD_B low
    // CD_B=0 means card present, while CD_B=1 means card not present
    IsCardPresent = (CardDetectLevel == IMX_GPIO_LOW);
  }

  LOG_TRACE ("SdhcIsCardPresent(): %d", IsCardPresent);

  return IsCardPresent;
}

BOOLEAN
SdhcIsReadOnly (
  IN EFI_SDHC_PROTOCOL *This
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  BOOLEAN                 IsReadOnly;
  USDHC_PRES_STATE_REG    PresState;
  IMX_GPIO_VALUE          WriteProtectLevel;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;

  if (SdhcCtx->WriteProtectSignal == USDHC_SIGNAL_INTERNAL_PIN) {
    Reg = SdhcCtx->RegistersBase;
    PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
    IsReadOnly = (PresState.Fields.WPSPL == 0);
  } else {
    if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->WriteProtectSignal)) {
      //Read the state of  WP pin for the card socket
      WriteProtectLevel = ImxGpioRead (
                           SdhcCtx->WriteProtectGpioPin.Bank,
                           SdhcCtx->WriteProtectGpioPin.IoNumber
                           );
    } else if (SdhcCtx->WriteProtectSignal == USDHC_SIGNAL_OVERRIDE_PIN_LOW) {
      WriteProtectLevel = IMX_GPIO_LOW;
    } else if (SdhcCtx->WriteProtectSignal == USDHC_SIGNAL_OVERRIDE_PIN_HIGH) {
      WriteProtectLevel = IMX_GPIO_HIGH;
    } else {
      ASSERT (!"Invalid WriteProtect signal source");
      WriteProtectLevel = IMX_GPIO_LOW;
    }

    // When no card is present,  WP is pulled-high, and the SDCard when
    // inserted will pull WP low if WP switch is configured to write enable
    // the SDCard, otherwise it WP will stay pulled-high
    // WP=0 means write enabled, while WP=1 means write protected
    IsReadOnly = (WriteProtectLevel != IMX_GPIO_LOW);
  }

  LOG_TRACE ("SdhcIsReadOnly(): %d", IsReadOnly);
  return IsReadOnly;
}

EFI_STATUS
SdhcSendCommand (
  IN EFI_SDHC_PROTOCOL *This,
  IN CONST SD_COMMAND *Cmd,
  IN UINT32 Argument,
  IN OPTIONAL CONST SD_COMMAND_XFR_INFO *XfrInfo
  )
{
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  USDHC_REGISTERS         *Reg;
  USDHC_BLK_ATT_REG       BlkAtt;
  USDHC_CMD_XFR_TYP_REG   CmdXfrTyp;
  USDHC_MIX_CTRL_REG      MixCtrl;
  EFI_STATUS              Status;
  USDHC_WTMK_LVL_REG      WtmkLvl;
  UINT32                  WtmkThreshold;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  Reg = SdhcCtx->RegistersBase;

  LOG_TRACE (
    "SdhcSendCommand(%cCMD%d, %08x)",
    ((Cmd->Class == SdCommandClassApp) ? 'A' : ' '),
    (UINT32)Cmd->Index,
    Argument);

  Status = WaitForCmdAndOrDataLine (SdhcCtx, Cmd);
  if (Status != EFI_SUCCESS) {
    LOG_ERROR ("SdhcWaitForCmdAndDataLine failed");
    return Status;
  }

  // Clear Interrupt status
  MmioWrite32 ((UINTN)&Reg->INT_STATUS, (UINT32)~0);

  // Setup data transfer command
  if (XfrInfo) {
    if (XfrInfo->BlockCount > USDHC_MAX_BLOCK_COUNT) {
      LOG_ERROR (
        "Provided %d block count while SDHC max block count is %d",
        XfrInfo->BlockCount,
        USDHC_MAX_BLOCK_COUNT);
      return EFI_INVALID_PARAMETER;
    }

    // Set block size and count
    BlkAtt.AsUint32 = 0;
    BlkAtt.Fields.BLKSIZE = XfrInfo->BlockSize;
    ASSERT (XfrInfo->BlockCount > 0);
    BlkAtt.Fields.BLKCNT = XfrInfo->BlockCount;
    MmioWrite32 ((UINTN)&Reg->BLK_ATT, BlkAtt.AsUint32);

    // Set transfer parameters
    MixCtrl.AsUint32 = 0;
    if (Cmd->TransferDirection == SdTransferDirectionRead) {
      MixCtrl.Fields.DTDSEL = 1;
    }

    if (XfrInfo->BlockCount > 1) {
      ASSERT ((Cmd->TransferType == SdTransferTypeMultiBlock) ||
              (Cmd->TransferType == SdTransferTypeMultiBlockNoStop));
      MixCtrl.Fields.MSBSEL = 1;
      MixCtrl.Fields.BCEN = 1;
    }

    MmioWrite32 ((UINTN)&Reg->MIX_CTRL, MixCtrl.AsUint32);

    WtmkLvl.AsUint32 = 0;

    WtmkThreshold = USDHC_BLOCK_LENGTH_BYTES / 4;
    if (Cmd->TransferDirection == SdTransferDirectionRead) {
      if (WtmkThreshold > USDHC_WTMK_RD_WML_MAX_VAL) {
        WtmkThreshold = USDHC_WTMK_RD_WML_MAX_VAL;
      }
      WtmkLvl.Fields.RD_WML = WtmkThreshold;
    } else {
      if (WtmkThreshold > USDHC_WTMK_WR_WML_MAX_VAL) {
        WtmkThreshold = USDHC_WTMK_WR_WML_MAX_VAL;
      }
      WtmkLvl.Fields.WR_WML = WtmkThreshold;
    }

    MmioWrite32 ((UINTN)&Reg->WTMK_LVL, WtmkLvl.AsUint32);
  }

  // Set CMD parameters
  CmdXfrTyp.AsUint32 = 0;
  CmdXfrTyp.Fields.CMDINX = Cmd->Index;

  switch (Cmd->ResponseType) {
  case SdResponseTypeNone:
    break;

  case SdResponseTypeR1:
  case SdResponseTypeR5:
  case SdResponseTypeR6:
    CmdXfrTyp.Fields.RSPTYP = USDHC_CMD_XFR_TYP_RSPTYP_RSP_48;
    CmdXfrTyp.Fields.CICEN = 1;
    CmdXfrTyp.Fields.CCCEN = 1;
    break;

  case SdResponseTypeR1B:
  case SdResponseTypeR5B:
    CmdXfrTyp.Fields.RSPTYP = USDHC_CMD_XFR_TYP_RSPTYP_RSP_48_CHK_BSY;
    CmdXfrTyp.Fields.CICEN = 1;
    CmdXfrTyp.Fields.CCCEN = 1;
    break;

  case SdResponseTypeR2:
    CmdXfrTyp.Fields.RSPTYP = USDHC_CMD_XFR_TYP_RSPTYP_RSP_136;
    CmdXfrTyp.Fields.CCCEN = 1;
    break;

  case SdResponseTypeR3:
  case SdResponseTypeR4:
    CmdXfrTyp.Fields.RSPTYP = USDHC_CMD_XFR_TYP_RSPTYP_RSP_48;
    break;

  default:
    LOG_ASSERT ("SdhcSendCommand(): Invalid response type");
    return EFI_INVALID_PARAMETER;
  }

  if (Cmd->Type == SdCommandTypeAbort) {
    CmdXfrTyp.Fields.CMDTYP = USDHC_CMD_XFR_TYP_CMDTYP_ABORT;
  }

  if (XfrInfo) {
    CmdXfrTyp.Fields.DPSEL = 1;
  }

  // Send command and wait for response
  MmioWrite32 ((UINTN)&Reg->CMD_ARG, Argument);
  MmioWrite32 ((UINTN)&Reg->CMD_XFR_TYP, CmdXfrTyp.AsUint32);

  Status = WaitForCmdResponse (SdhcCtx);
  if (EFI_ERROR (Status)) {
    LOG_ERROR ("WaitForCmdResponse() failed. %r", Status);
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SdhcReceiveResponse (
  IN EFI_SDHC_PROTOCOL *This,
  IN CONST SD_COMMAND *Cmd,
  OUT UINT32 *Buffer
  )
{

  USDHC_REGISTERS *Reg;
  USDHC_PRIVATE_CONTEXT *SdhcCtx;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;

  if (Buffer == NULL) {
    LOG_ERROR ("Input Buffer is NULL");
    return EFI_INVALID_PARAMETER;
  }

  Reg = SdhcCtx->RegistersBase;

  switch (Cmd->ResponseType) {
  case SdResponseTypeNone:
    break;
  case SdResponseTypeR1:
  case SdResponseTypeR1B:
  case SdResponseTypeR3:
  case SdResponseTypeR4:
  case SdResponseTypeR5:
  case SdResponseTypeR5B:
  case SdResponseTypeR6:
    Buffer[0] = MmioRead32 ((UINTN)&Reg->CMD_RSP0);
    LOG_TRACE (
      "SdhcReceiveResponse(Type: %x), Buffer[0]: %08x",
      Cmd->ResponseType,
      Buffer[0]);
    break;
  case SdResponseTypeR2:
    Buffer[0] = MmioRead32 ((UINTN)&Reg->CMD_RSP0);
    Buffer[1] = MmioRead32 ((UINTN)&Reg->CMD_RSP1);
    Buffer[2] = MmioRead32 ((UINTN)&Reg->CMD_RSP2);
    Buffer[3] = MmioRead32 ((UINTN)&Reg->CMD_RSP3);

    LOG_TRACE (
      "SdhcReceiveResponse(Type: %x), Buffer[0-3]: %08x, %08x, %08x, %08x",
      Cmd->ResponseType,
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3]);
    break;
  default:
    LOG_ASSERT ("SdhcReceiveResponse(): Invalid response type");
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SdhcReadBlockData (
  IN EFI_SDHC_PROTOCOL *This,
  IN UINTN LengthInBytes,
  OUT UINT32 *Buffer
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  UINT32                  FifoWords;
  UINTN                   NumWords;
  EFI_STATUS              Status;
  UINTN                   WordIdx;
  USDHC_WTMK_LVL_REG      WtmkLvl;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;

  LOG_TRACE (
    "SdhcReadBlockData(LengthInBytes: 0x%x, Buffer: 0x%x)",
    LengthInBytes,
    Buffer);

  ASSERT (Buffer != NULL);
  ASSERT (LengthInBytes % sizeof (UINT32) == 0);

  WordIdx = 0;
  NumWords = LengthInBytes / sizeof (UINT32);
  Reg = SdhcCtx->RegistersBase;
  WtmkLvl.AsUint32 = MmioRead32 ((UINTN)&Reg->WTMK_LVL);

  ASSERT (WtmkLvl.Fields.RD_WML > 0);

  while (WordIdx < NumWords) {
    Status = WaitForReadFifo (SdhcCtx);
    if (EFI_ERROR (Status)) {
      LOG_ERROR (
        "WaitForReadFifo() failed at Word%d. %r",
        (UINT32)WordIdx,
        Status);
      return Status;
    }

    FifoWords = WtmkLvl.Fields.RD_WML;
    while (WordIdx < NumWords && FifoWords--) {
      Buffer[WordIdx] = MmioRead32 ((UINTN)&Reg->DATA_BUFF_ACC_PORT);
      ++WordIdx;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SdhcWriteBlockData (
  IN EFI_SDHC_PROTOCOL *This,
  IN UINTN LengthInBytes,
  IN CONST UINT32 *Buffer
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  USDHC_INT_STATUS_REG    IntStatus;
  UINTN                   NumBlocks;
  USDHC_PRES_STATE_REG    PresState;
  UINTN                   RemainingWords;
  INT32                   Retry;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;

  LOG_TRACE (
    "SdhcWriteBlockData(LengthInBytes: 0x%x, Buffer: 0x%x)",
    LengthInBytes,
    Buffer);

  ASSERT (Buffer != NULL);
  ASSERT (LengthInBytes % USDHC_BLOCK_LENGTH_BYTES == 0);

  CONST UINTN BlockWordCount = USDHC_BLOCK_LENGTH_BYTES / sizeof (UINT32);
  NumBlocks = LengthInBytes / USDHC_BLOCK_LENGTH_BYTES;
  Reg = SdhcCtx->RegistersBase;
  Retry = 100000;

  while (NumBlocks > 0) {
    RemainingWords = BlockWordCount;
    IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
    PresState.AsUint32 = MmioRead32 ((UINTN)&Reg->PRES_STATE);
    while (!PresState.Fields.BWEN && --Retry);
    if (Retry <= 0) {
      LOG_ERROR ("Timeout waiting for FIFO PRES_STATE.BWEN flag");
      return EFI_TIMEOUT;
    }

    while (RemainingWords > 0 && !IntStatus.Fields.TC) {
      MicroSecondDelay (100);
      IntStatus.AsUint32 = MmioRead32 ((UINTN)&Reg->INT_STATUS);
      MmioWrite32 ((UINTN)&Reg->DATA_BUFF_ACC_PORT, *Buffer);
      Buffer++;
      RemainingWords--;
    }
    NumBlocks--;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SdhcSoftwareReset (
  IN EFI_SDHC_PROTOCOL *This,
  IN SDHC_RESET_TYPE ResetType
  )
{
  USDHC_REGISTERS         *Reg;
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  USDHC_PROT_CTRL_REG     ProtCtrl;
  UINT32                  Retry;
  EFI_STATUS              Status;
  USDHC_SYS_CTRL_REG      SysCtrl;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  Reg = SdhcCtx->RegistersBase;

  if (ResetType == SdhcResetTypeAll) {
    LOG_TRACE ("SdhcSoftwareReset(ALL)");
    // Software reset for ALL
    SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    SysCtrl.Fields.RSTA = 1;
    MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);
    Retry = USDHC_POLL_RETRY_COUNT;

    // Wait for reset to complete
    while (SysCtrl.Fields.RSTA && Retry) {
      --Retry;
      gBS->Stall (USDHC_POLL_WAIT_US);
      SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    }

    if (SysCtrl.Fields.RSTA) {
      ASSERT (!Retry);
      LOG_ERROR ("Time-out waiting on RSTA for self-clear");
      return EFI_TIMEOUT;
    }

    // Disconnect interrupt signals between SDHC and GIC
    MmioWrite32 ((UINTN)&Reg->INT_SIGNAL_EN, 0);

    // Clear and Enable all interrupts
    MmioWrite32 ((UINTN)&Reg->INT_STATUS, (UINT32)~0);
    MmioWrite32 ((UINTN)&Reg->INT_STATUS_EN, (UINT32)~0);

    LOG_TRACE ("Waiting for CMD and DATA lines");

    // Wait for CMD and DATA lines to become available
    Status = WaitForCmdAndOrDataLine (SdhcCtx, NULL);
    if (Status != EFI_SUCCESS) {
      LOG_ERROR ("SdhcWaitForCmdAndDataLine() failed. %r", Status);
      return Status;
    }

    // Send 80 clock ticks to power-up the card
    SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    SysCtrl.Fields.INITA = 1;
    MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);
    Retry = USDHC_POLL_RETRY_COUNT;

    // Wait for the 80 clock ticks to complete
    while (SysCtrl.Fields.INITA && Retry) {
      --Retry;
      gBS->Stall (USDHC_POLL_WAIT_US);
      SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    }

    if (SysCtrl.Fields.INITA) {
      ASSERT (!Retry);
      LOG_ERROR ("Time-out waiting on INITA for self-clear");
      return EFI_TIMEOUT;
    }

    LOG_TRACE ("Card power-up complete");

    // Set max data-timout counter value
    SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    SysCtrl.Fields.DTOCV = 0xF;
    MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);

    // Reset Mixer Control
    MmioWrite32 ((UINTN)&Reg->MIX_CTRL, 0);

    ProtCtrl.AsUint32 = 0;
    ProtCtrl.Fields.EMODE = USDHC_PROT_CTRL_EMODE_LITTLE_ENDIAN;
    ProtCtrl.Fields.LCTL = 1;
    MmioWrite32 ((UINTN)&Reg->PROT_CTRL, ProtCtrl.AsUint32);

    LOG_TRACE ("Reset ALL complete");

  } else if (ResetType == SdhcResetTypeCmd) {
    LOG_TRACE ("SdhcSoftwareReset(CMD)");
    //
    // Software reset for CMD
    //
    SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    SysCtrl.Fields.RSTC = 1;
    MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);
    Retry = USDHC_POLL_RETRY_COUNT;

    //
    // Wait for reset to complete
    //
    while (SysCtrl.Fields.RSTC && Retry) {
      --Retry;
      gBS->Stall (USDHC_POLL_WAIT_US);
      SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    }

    if (SysCtrl.Fields.RSTC) {
      ASSERT (!Retry);
      LOG_ERROR ("Time-out waiting on RSTC for self-clear");
      return EFI_TIMEOUT;
    }

    MmioWrite32 ((UINTN)&Reg->INT_STATUS, (UINT32)~0);

    LOG_TRACE ("Reset CMD complete");

  } else if (ResetType == SdhcResetTypeData) {
    LOG_TRACE ("SdhcSoftwareReset(DAT)");
    // Software reset for DAT
    SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    SysCtrl.Fields.RSTD = 1;
    MmioWrite32 ((UINTN)&Reg->SYS_CTRL, SysCtrl.AsUint32);
    Retry = USDHC_POLL_RETRY_COUNT;

    // Wait for reset to complete
    while (SysCtrl.Fields.RSTD && Retry) {
      --Retry;
      gBS->Stall (USDHC_POLL_WAIT_US);
      SysCtrl.AsUint32 = MmioRead32 ((UINTN)&Reg->SYS_CTRL);
    }

    if (SysCtrl.Fields.RSTD) {
      ASSERT (!Retry);
      LOG_ERROR ("Time-out waiting on RSTD for self-clear");
      return EFI_TIMEOUT;
    }

    MmioWrite32 ((UINTN)&Reg->INT_STATUS, (UINT32)~0);

    LOG_TRACE ("Reset DAT complete");

  } else {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

VOID
SdhcCleanup (
  IN EFI_SDHC_PROTOCOL *This
  )
{
  if (This->PrivateContext != NULL) {
    FreePool (This->PrivateContext);
    This->PrivateContext = NULL;
  }

  FreePool (This);
}

VOID
SdhcGetCapabilities (
  IN EFI_SDHC_PROTOCOL *This,
  OUT SDHC_CAPABILITIES *Capabilities
  )
{
  USDHC_REGISTERS           *Reg;
  USDHC_PRIVATE_CONTEXT     *SdhcCtx;
  USDHC_HOST_CTRL_CAP_REG   Caps;

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)This->PrivateContext;
  Reg = SdhcCtx->RegistersBase;
  Caps.AsUint32 = MmioRead32 ((UINTN)&Reg->HOST_CTRL_CAP);
  Capabilities->MaximumBlockSize = (UINT32) (512 << Caps.Fields.MBL);
  Capabilities->MaximumBlockCount = 0xFFFF;
}

static EFI_SDHC_PROTOCOL mSdhcProtocolTemplate = {
  SDHC_PROTOCOL_INTERFACE_REVISION,   // Revision
  0,                                  // DeviceId
  NULL,                               // PrivateContext
  SdhcGetCapabilities,
  SdhcSoftwareReset,
  SdhcSetClock,
  SdhcSetBusWidth,
  SdhcIsCardPresent,
  SdhcIsReadOnly,
  SdhcSendCommand,
  SdhcReceiveResponse,
  SdhcReadBlockData,
  SdhcWriteBlockData,
  SdhcCleanup
};

EFI_STATUS
SdhcDeviceRegister (
  IN EFI_HANDLE ImageHandle,
  IN UINT32 SdhcId,
  IN VOID *RegistersBase,
  IN USDHC_SIGNAL_SOURCE CardDetectSignal,
  IN USDHC_SIGNAL_SOURCE WriteProtectSignal
  )
{
  USDHC_PRIVATE_CONTEXT   *SdhcCtx;
  EFI_SDHC_PROTOCOL       *SdhcProtocol;
  EFI_STATUS              Status;

  SdhcProtocol = NULL;
  SdhcCtx = NULL;

  if (ImageHandle == NULL ||
      RegistersBase == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  // Allocate per-device SDHC protocol and private context storage
  SdhcProtocol = AllocateCopyPool (sizeof (EFI_SDHC_PROTOCOL),
                                   &mSdhcProtocolTemplate);
  if (SdhcProtocol == NULL) {
    Status =  EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  SdhcProtocol->SdhcId = SdhcId;
  SdhcProtocol->PrivateContext = AllocateZeroPool (sizeof (
                                   USDHC_PRIVATE_CONTEXT));
  if (SdhcProtocol->PrivateContext == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  SdhcCtx = (USDHC_PRIVATE_CONTEXT *)SdhcProtocol->PrivateContext;
  SdhcCtx->SdhcId = SdhcId;
  SdhcCtx->RegistersBase = (USDHC_REGISTERS *)RegistersBase;
  SdhcCtx->CardDetectSignal = CardDetectSignal;
  if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->CardDetectSignal)) {
    SdhcCtx->CardDetectGpioPin.IoNumber = PCD_GPIO_PIN_IO_NUMBER ((
                                            UINT16)CardDetectSignal);
    SdhcCtx->CardDetectGpioPin.Bank = PCD_GPIO_PIN_BANK (CardDetectSignal);
  }

  SdhcCtx->WriteProtectSignal = WriteProtectSignal;
  if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->WriteProtectSignal)) {
    SdhcCtx->WriteProtectGpioPin.IoNumber = PCD_GPIO_PIN_IO_NUMBER ((
        UINT16)WriteProtectSignal);
    SdhcCtx->WriteProtectGpioPin.Bank = PCD_GPIO_PIN_BANK (WriteProtectSignal);
  }

  LOG_INFO (
    "Initializing uSDHC%d @%p GPIO CD?:%d WP?:%d",
    SdhcId,
    RegistersBase,
    (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->CardDetectSignal) ? 1 : 0),
    (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->WriteProtectSignal) ? 1 : 0));

  if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->CardDetectSignal)) {
    LOG_INFO (
      "Using GPIO%d_IO%d for CardDetect",
      SdhcCtx->CardDetectGpioPin.Bank,
      SdhcCtx->CardDetectGpioPin.IoNumber);
  }

  if (USDHC_IS_GPIO_SIGNAL_SOURCE (SdhcCtx->WriteProtectSignal)) {
    LOG_INFO (
      "Using GPIO%d_IO%d for WriteProtect",
      SdhcCtx->WriteProtectGpioPin.Bank,
      SdhcCtx->WriteProtectGpioPin.IoNumber);
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
             &SdhcCtx->SdhcProtocolHandle,
             &gEfiSdhcProtocolGuid,
             SdhcProtocol,
             NULL);
  if (EFI_ERROR (Status)) {
    LOG_ERROR ("InstallMultipleProtocolInterfaces failed. %r", Status);
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status)) {
    LOG_ERROR ("Failed to register and initialize uSDHC%d", SdhcId);

    if (SdhcProtocol != NULL && SdhcProtocol->PrivateContext != NULL) {
      FreePool (SdhcProtocol->PrivateContext);
      SdhcProtocol->PrivateContext = NULL;
    }

    if (SdhcProtocol != NULL) {
      FreePool (SdhcProtocol);
      SdhcProtocol = NULL;
    }
  }

  return Status;
}

EFI_STATUS
SdhcInitialize (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  UINT32      SdhcRegisteredCount;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  SdhcRegisteredCount = 0;

  // Register uSDHC1 through uSDHC4 if their base address is non-zero

  // uSDHC1
  if (FixedPcdGet32 (PcdSdhc1Enable)) {
    Status = SdhcDeviceRegister (
               ImageHandle,
               1,
               (VOID *)FixedPcdGet32 (PcdSdhc1Base),
               FixedPcdGet32 (PcdSdhc1CardDetectSignal),
               FixedPcdGet32 (PcdSdhc1WriteProtectSignal));
    if (!EFI_ERROR (Status)) {
      ++SdhcRegisteredCount;
    }
  }

  // uSDHC2
  if (FixedPcdGet32 (PcdSdhc2Enable)) {
    Status = SdhcDeviceRegister (
               ImageHandle,
               2,
               (VOID *)FixedPcdGet32 (PcdSdhc2Base),
               FixedPcdGet32 (PcdSdhc2CardDetectSignal),
               FixedPcdGet32 (PcdSdhc2WriteProtectSignal));
    if (!EFI_ERROR (Status)) {
      ++SdhcRegisteredCount;
    }
  }

  // uSDHC3
  if (FixedPcdGet32 (PcdSdhc3Enable)) {
    Status = SdhcDeviceRegister (
               ImageHandle,
               3,
               (VOID *)FixedPcdGet32 (PcdSdhc3Base),
               FixedPcdGet32 (PcdSdhc3CardDetectSignal),
               FixedPcdGet32 (PcdSdhc3WriteProtectSignal));
    if (!EFI_ERROR (Status)) {
      ++SdhcRegisteredCount;
    }
  }

  // uSDHC4
  if (FixedPcdGet32 (PcdSdhc4Enable)) {
    Status = SdhcDeviceRegister (
               ImageHandle,
               4,
               (VOID *)FixedPcdGet32 (PcdSdhc4Base),
               FixedPcdGet32 (PcdSdhc4CardDetectSignal),
               FixedPcdGet32 (PcdSdhc4WriteProtectSignal));
    if (!EFI_ERROR (Status)) {
      ++SdhcRegisteredCount;
    }
  }

  // Succeed driver loading if at least one enabled uSDHC got registered successfully
  if ((Status != EFI_SUCCESS) && (SdhcRegisteredCount > 0)) {
    Status = EFI_SUCCESS;
  }

  return Status;
}
