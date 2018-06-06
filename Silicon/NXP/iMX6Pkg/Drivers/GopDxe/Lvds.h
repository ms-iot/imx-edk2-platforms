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

#ifndef _LVDS_H_
#define _LVDS_H_

// LDB Register Base Address
#define LDB_BASE 0x020E0008

// LDB Control Register offset
#define LDB_CTRL    0

#pragma pack(push, 1)

// LDB_CTRL_REG
typedef union {
  struct {
    UINT32 ch0_mode : 2;
    UINT32 ch1_mode : 2;
    UINT32 split_mode_en : 1;
    UINT32 data_width_ch0 : 1;
    UINT32 bit_mapping_ch0 : 1;
    UINT32 data_width_ch1 : 1;
    UINT32 bit_mapping_ch1 : 1;
    UINT32 di0_vs_polarity : 1;
    UINT32 di1_vs_polarity : 1;
    UINT32 Reserved11_15 : 5;
    UINT32 lvds_clk_shift : 3;
    UINT32 Reserved19 : 1;
    UINT32 counter_reset_val : 2;
    UINT32 Reserved22_31 : 10;
  };
  UINT32 Reg;
}  LDB_CTRL_REG;

#pragma pack(pop)

EFI_STATUS
InitLvds (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  );

EFI_STATUS
SetLvdsPower (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  BOOLEAN                     PowerState
  );

EFI_STATUS
SetLvdsDisplay (
  IN  DISPLAY_INTERFACE_CONTEXT   *LvdsDisplayContextPtr,
  IN  IMX_DISPLAY_TIMING          *Timings
  );

#endif  /* _LVDS_H_ */
