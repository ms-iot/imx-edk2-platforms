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

#ifndef _DDC_H_
#define _DDC_H_

EFI_STATUS
Imx6DdcRead (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_INTERFACE_TYPE   DisplayInterface,
  IN  UINT8               SlaveAddress,
  IN  UINT8               RegisterAddress,
  IN  UINT32              ReadSize,
  IN  UINT8               *DataReadPtr
  );

#endif  /* _DDC_H_ */
