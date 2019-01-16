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

#ifndef __IMX6_H__
#define __IMX6_H__

// Platform specific definition
#define EFI_ACPI_OEM_TABLE_ID      SIGNATURE_64('I','M','X','6','E','D','K','2')
#define EFI_ACPI_OEM_REVISION      0x01000101
#define EFI_ACPI_CREATOR_ID        SIGNATURE_32('I','M','X','6')
#define EFI_ACPI_CREATOR_REVISION  0x00000001

#if defined(CPU_IMX6D) || defined(CPU_IMX6Q)
#include "iMX6_DQ.h"
#elif defined(CPU_IMX6DP) || defined(CPU_IMX6QP)
#include "iMX6_DQP.h"
#elif defined(CPU_IMX6S) || defined(CPU_IMX6DL)
#include "iMX6_SDL.h"
#elif defined(CPU_IMX6SX)
#include "iMX6_SX.h"
#elif defined(CPU_IMX6ULL)
#include "iMX6_ULL.h"
#else
#error iMX6 CPU Type Not Defined! (Preprocessor Flag)
#endif

#define SERIAL_DEBUG_PORT_INIT_MSG "\r\nDebug Serial Port Init\r\n"
#define SERIAL_PORT_INIT_MSG "UART"

#endif // __IMX6_H__
