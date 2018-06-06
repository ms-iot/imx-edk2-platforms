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

#ifndef _IMX6_BOARD_LIB_H_
#define _IMX6_BOARD_LIB_H_

/*
  Mandatory functions to implement by the board library.
*/

VOID
ImxClkPwrInit (
  );

/*
  Optional functions to implement by the board library.
  The default implementation of these functions if not overridden is NOOP.
*/

VOID
SdhcInit (
  );

VOID
EhciInit (
  );

VOID
I2cInit (
  );

VOID
SpiInit (
  );

VOID
PcieInit (
  );

VOID
SetupAudio (
  );

#endif // _IMX6_BOARD_LIB_H_
