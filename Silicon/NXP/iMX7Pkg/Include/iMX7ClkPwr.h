/** @file
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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

#ifndef _IMX7_CLK_PWR_H_
#define _IMX7_CLK_PWR_H_

//
// Public functions
//
VOID ImxUngateActiveClock (
    );

EFI_STATUS
ImxSetLcdIfClockRate (
    UINT32 ClockRate
    );

EFI_STATUS
ImxSetAudioMclkClockRate (
    UINT32 ClockRate
    );

#endif

