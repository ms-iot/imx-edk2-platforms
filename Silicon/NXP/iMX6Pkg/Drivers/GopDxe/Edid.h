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

#ifndef _EDID_H_
#define _EDID_H_

EFI_STATUS
ReadEdid (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_INTERFACE_TYPE   DisplayInterface,
  IN  UINT8               *EdidDataPtr,
  OUT UINT32              *EdidDataSizePtr
  );

EFI_STATUS
GetEdidPreferredTiming (
  IN  UINT8               *EdidDataPtr,
  IN  UINT32              EdidDataSizePtr,
  OUT IMX_DISPLAY_TIMING  *PreferredTiming
  );

#endif  /* _EDID_H_ */
