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

#ifndef _IMX6_CLK_PWR_PRIVATE_H_
#define _IMX6_CLK_PWR_PRIVATE_H_

#include <Base.h>

#define PLL2_FREQUENCY_MULITPLIER 18
#define PLL3_FREQUENCY_MULTIPLIER 18

typedef enum {
  IMX_PLL_PFD0,
  IMX_PLL_PFD1,
  IMX_PLL_PFD2,
  IMX_PLL_PFD3,
} IMX_PLL_PFD;

typedef struct {
  IMX_CLK Clock;
  IMX_CLOCK_INFO Info;
} IMX_CLOCK_CONTEXT;

typedef struct {
  UINT16 RegisterIndex;   // Register index (0-6)
  UINT16 GateNumber;      // Gate number within register (0-15)
} IMX_CCGR_INDEX;

IMX_CCGR_INDEX
ImxpCcgrIndexFromClkGate (
  IN  IMX_CLK_GATE    ClockGate
  );

VOID
ImxCcmConfigureGpuClockTree (
  VOID
  );

VOID
ImxCcmConfigureIPUDIxClockTree (
  VOID
  );

VOID
ImxCcmConfigureIPULDBxClockTree (
  VOID
  );

VOID
ImxSetClockRatePLL5 (
  IN  UINT32                                  ClockRate,
  IN  IMX_CCM_PLL_VIDEO_CTRL_POST_DIV_SELECT  PostDivSelect
  );

EFI_STATUS
ImxpGetClockInfo (
  IN      IMX_CLK               ClockId,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

VOID
ImxpGetOsc24ClkInfo (
  OUT IMX_CLOCK_INFO  *ClockInfo
  );

EFI_STATUS
ImxpGetPll1MainClkInfo  (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPll2MainClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPll2PfdClkInfo (
  IN      IMX_PLL_PFD           PfdIndex,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPll3MainClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPll3PfdClkInfo (
  IN      IMX_PLL_PFD           PfdIndex,
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPll3SwClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetAxiClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPeriphClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPrePeriphClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetPeriphClk2Info (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetArmClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetMmdcCh0ClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetAhbClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetIpgClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetGpu2dAxiClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetGpu3dAxiClkRootInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetGpu2dCoreClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetGpu3dCoreClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

EFI_STATUS
ImxpGetGpu3dShaderClkInfo (
  OUT     IMX_CLOCK_INFO        *ClockInfo
  );

VOID
ImxEnableGpuVpuPowerDomain (
  VOID
  );

VOID
ImxDisableGpuVpuPowerDomain (
  VOID
  );

#endif // _IMX6_CLK_PWR_PRIVATE_H_
