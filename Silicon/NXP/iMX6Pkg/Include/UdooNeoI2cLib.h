/** @file
*
*  Header defining the iMX6 SoloX interface for i2c
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

#ifndef __UDOO_NEO_I2C_LIB_H__
#define __UDOO_NEO_I2C_LIB_H__

typedef struct _IMX6SX_I2C_TARGET
{
    UINT32  SlaveAddr;
    UINT32  SlaveRegSizeBytes;
    UINT32  SlaveSpeed;
    UINT32  XferTimeoutUs;
} IMX6SX_I2C_TARGET;

typedef struct _INT_I2C_UNIT
{
    IMX6SX_I2C_TARGET   Target;
    UINT32              Regs;
} INT_I2C_UNIT;

BOOLEAN
IMX6SX_I2C_SetTarget(
    UINT32                      UnitNum,
    IMX6SX_I2C_TARGET const *   Target
    );

BOOLEAN
IMX6SX_I2C_Read(
    UINT32  UnitNum,
    UINT32  SlaveReg,
    UINT32  ReadByteCount,
    UINT8 * ReadBuffer
    );

BOOLEAN
IMX6SX_I2C_Write(
    UINT32          UnitNum,
    UINT32          SlaveReg,
    UINT32          WriteByteCount,
    UINT8 const *   WriteBuffer
    );

typedef BOOLEAN (*PFN_InitI2C)(UINT32 aRegId);

BOOLEAN
ReadI2CByte(
    UINTN Unit,
    UINT32 Register,
    UINT8 *Value,
    PFN_InitI2C Callback
    );

BOOLEAN
WriteI2CByte(
    UINTN Unit,
    UINT32 Register,
    UINT8 Value8,
    PFN_InitI2C Callback
    );

#endif // __UDOO_NEO_I2C_LIB_H__