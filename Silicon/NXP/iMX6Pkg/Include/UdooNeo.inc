/*
*  Portions Copyright (c) Intel Corporation
*  Portions Copyright (c) Apple
*  Portions Copyright (c) ARM Ltd.
*  Portions Copyright (c) Freescale
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
*/
#ifndef __UDOONEO_H__
#define __UDOONEO_H__

//// 
//  Microsoft Coporation
//  SiArch
//

#define SOC_OCRAM_M4COMM_BUFFER_BASE        0x00907000
#define SOC_OCRAM_M4COMM_BUFFER_LENGTH      0x00001000

// M4 TCM
#define SOC_M4TCM_PHYSICAL_BASE             IMX6SX_PHYSADDR_CM4_TCM
#define SOC_M4TCM_PHYSICAL_LENGTH           IMX6SX_PHYSSIZE_CM4_TCM
#define SOC_M4TCM_ATTRIBUTES                DDR_ATTRIBUTES_UNCACHED

// PMIC IC on I2C1 bus
#define UDOONEO_PMIC_I2C_UNIT                     1
#define UDOONEO_PF3000_I2C1_ADDR            0x08

// HDMI on I2C2 Bus
#define UDOONEO_HDMI_I2C_UNIT                     3  
#define UDOONEO_HDMI_I2C2_ADDR              0x70
#define UDOONEO_CEC_I2C2_ADDR               0x34

// Gyroscope on I2C4 Bus
#define UDOONEO_GYRO_I2C4_ADDR              0x20
#define UDOONEO_ACCEL_I2C4_ADDR             0x1E

#endif // #define __UDOONEO_H__
