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

#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <iMX6.h>
#include <iMX6Audio.h>

VOID
InitializeAudmux (
  UINT32 SsiPortNumber,
  UINT32 ExternalPortNumber
  )
{
  volatile AUDMUX_REGISTERS*  pAudmuxReg;
  AUDMUX_PDCR_REG             ExternalPdcr;
  AUDMUX_PTCR_REG             ExternalPtcr;
  AUDMUX_PDCR_REG             SsiPdcr;
  AUDMUX_PTCR_REG             SsiPtcr;

  ASSERT (SsiPortNumber < AUDMUX_PORT_MAX);
  ASSERT (ExternalPortNumber < AUDMUX_PORT_MAX);

  pAudmuxReg = (AUDMUX_REGISTERS *) AUDMUX_BASE;
  ExternalPdcr = (AUDMUX_PDCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->Audmux_Pdcr5);
  ExternalPdcr.MODE = AUDMUX_NORMAL_MODE;
  ExternalPdcr.RXDSEL = SsiPortNumber;
  // According to the spec, ReceiveClockDirectionControl and
  // SynchronousAsynchronousSelect should never be changed at the same time.
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Pdcr5, ExternalPdcr.AsUint32);
  ExternalPdcr.TXRXEN = AUDMUX_TRSE_NOSWITCH;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Pdcr5, ExternalPdcr.AsUint32);

  ExternalPtcr = (AUDMUX_PTCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->Audmux_Ptcr5);
  ExternalPtcr.TFS_DIR = AUDMUX_OUTPUT;
  ExternalPtcr.TFSEL = SsiPortNumber;
  ExternalPtcr.TCLKDIR = AUDMUX_OUTPUT;
  ExternalPtcr.TCSEL = SsiPortNumber;
  ExternalPtcr.RFS_DIR = AUDMUX_OUTPUT;
  ExternalPtcr.RFSEL = SsiPortNumber;
  ExternalPtcr.RCLKDIR = AUDMUX_OUTPUT;
  ExternalPtcr.RCSEL = SsiPortNumber;
  ExternalPtcr.SYN = AUDMUX_SYNCHRONOUS_MODE;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Ptcr5, ExternalPtcr.AsUint32);

  SsiPdcr = (AUDMUX_PDCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->Audmux_Pdcr7);
  SsiPdcr.MODE = AUDMUX_NORMAL_MODE;
  SsiPdcr.RXDSEL = ExternalPortNumber;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Pdcr7, SsiPdcr.AsUint32);
  // According to the spec, ReceiveClockDirectionControl and
  // SynchronousAsynchronousSelect should never be changed at the same time.
  SsiPdcr.TXRXEN = AUDMUX_TRSE_NOSWITCH;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Pdcr7, SsiPdcr.AsUint32);

  SsiPtcr = (AUDMUX_PTCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->Audmux_Ptcr7);
  SsiPtcr.TFS_DIR = AUDMUX_INPUT;
  SsiPtcr.TCLKDIR = AUDMUX_INPUT;
  SsiPtcr.RFS_DIR = AUDMUX_INPUT;
  SsiPtcr.RCLKDIR = AUDMUX_INPUT;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Ptcr7, SsiPtcr.AsUint32);
  // According to the spec, ReceiveClockDirectionControl and
  // SynchronousAsynchronousSelect should never be changed at the same time.
  SsiPtcr.SYN = AUDMUX_SYNCHRONOUS_MODE;
  MmioWrite32 ((UINTN)&pAudmuxReg->Audmux_Ptcr7, SsiPtcr.AsUint32);
}
