/** @file
*
*  Shared memory driver.
*
*  Copyright (c), Microsoft Corporation. All rights reserved.
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

#ifndef __SHMEMP_H
#define __SHMEMP_H

#include <string.h>
#include "ShMem.h"

#ifdef _MSC_VER

#include <Windows.h>
#define MEMORY_READ_BARRIER     MemoryBarrier();
#define MEMORY_WRITE_BARRIER    MemoryBarrier();

#else

//#error Fill these in and comment out this #error
#include <Library/ArmLib.h>

#define MEMORY_READ_BARRIER     ArmDataSyncronizationBarrier();
#define MEMORY_WRITE_BARRIER    ArmDataSyncronizationBarrier();

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#endif

#endif // __SHMEMP_H
