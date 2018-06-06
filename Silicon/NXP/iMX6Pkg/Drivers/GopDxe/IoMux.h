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

#ifndef _IO_MUX_H_
#define _IO_MUX_H_

#define HDMI_MUX_CTL_OFFSET         2
#define HDMI_MUX_CTL_MASK           0x000C
#define MIPI_MUX_CTL_OFFSET         4
#define MIPI_MUX_CTL_MASK           0x0030
#define LVDS0_MUX_CTL_OFFSET        6
#define LVDS0_MUX_CTL_MASK          0x00C0
#define LVDS1_MUX_CTL_OFFSET        8
#define LVDS1_MUX_CTL_MASK          0x0300

EFI_STATUS
SetupDisplayMux (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  );

#endif  /* _IO_MUX_H_ */
