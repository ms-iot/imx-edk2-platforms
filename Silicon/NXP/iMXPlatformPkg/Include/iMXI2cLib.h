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

#ifndef _IMX_I2C_H_
#define _IMX_I2C_H_

#define IMX_I2C_I2SR_RXAK        0x0001
#define IMX_I2C_I2SR_IIF         0x0002
#define IMX_I2C_I2SR_SRW         0x0004
#define IMX_I2C_I2SR_IAL         0x0010
#define IMX_I2C_I2SR_IBB         0x0020
#define IMX_I2C_I2SR_IAAS        0x0040
#define IMX_I2C_I2SR_ICF         0x0080

#define IMX_I2C_I2SR_IIF_NO_INTERRUPT_PENDING   0x0
#define IMX_I2C_I2SR_IIF_INTERRUPT_PENDING      0x1
#define IMX_I2C_I2SR_IAL_NO_ARBITRATION_LOST    0x0
#define IMX_I2C_I2SR_IAL_ARBITRATION_LOST       0x1

#define IMX_I2C_I2CR_RSTA_REPEAT_START_DISABLE  0x0
#define IMX_I2C_I2CR_RSTA_REPEAT_START_ENABLE   0x1
#define IMX_I2C_I2CR_TXAK_SEND_TRANSMIT_ACK     0x0
#define IMX_I2C_I2CR_TXAK_NO_TRANSMIT_ACK       0x1
#define IMX_I2C_I2CR_MTX_RECEIVE_MODE           0x0
#define IMX_I2C_I2CR_MTX_TRANSMIT_MODE          0x1
#define IMX_I2C_I2CR_MSTA_SLAVE_MODE            0x0
#define IMX_I2C_I2CR_MSTA_MASTER_MODE           0x1
#define IMX_I2C_I2CR_IEN_INTERRUPT_DISABLED     0x0
#define IMX_I2C_I2CR_IEN_INTERRUPT_ENABLED      0x1

#define IMX_I2C_TX 0
#define IMX_I2C_RX 1

typedef union {
  UINT16 Raw;
  struct {
    UINT16 Reserved0 : 1;
    UINT16 ADR : 7;
    UINT16 Reserved1 : 8;
  };
} IMX_I2C_IADR_REGISTER;

typedef union {
  UINT16 Raw;
  struct {
    UINT16 IC : 6;
    UINT16 Reserved0 : 10;
  };
} IMX_I2C_IFDR_REGISTER;

typedef union {
  UINT16 Raw;
  struct {
    UINT16 Reserved0 : 2;
    UINT16 RSTA : 1;
    UINT16 TXAK : 1;
    UINT16 MTX : 1;
    UINT16 MSTA : 1;
    UINT16 IIEN : 1;
    UINT16 IEN : 1;
    UINT16 Reserved1 : 8;
  };
} IMX_I2C_I2CR_REGISTER;

typedef union {
  UINT8 Raw;
  struct {
    UINT8 Direction : 1;
    UINT8 DeviceAddress : 7;
  };
} IMX_I2C_DEVICE_ADDRESS_PACKET;

typedef union {
  UINT16 Raw;
  struct {
    UINT16 RXAK : 1;
    UINT16 IIF : 1;
    UINT16 SRW : 1;
    UINT16 Reserved0 : 1;
    UINT16 IAL : 1;
    UINT16 IBB : 1;
    UINT16 IAAS : 1;
    UINT16 ICF : 1;
    UINT16 Reserved1 : 8;
  };
} IMX_I2C_I2SR_REGISTER;

typedef union {
  UINT16 Raw;
  struct {
    UINT16 DATA : 8;
    UINT16 Reserved0 : 8;
  };
} IMX_I2C_I2DR_REGISTER;

typedef struct {
  IMX_I2C_IADR_REGISTER IADR;
  UINT16 Reserved0;
  IMX_I2C_IFDR_REGISTER IFDR;
  UINT16 Reserved1;
  IMX_I2C_I2CR_REGISTER I2CR;
  UINT16 Reserved2;
  IMX_I2C_I2DR_REGISTER I2SR;
  UINT16 Reserved3;
  IMX_I2C_I2DR_REGISTER I2DR;
  UINT16 Reserved4;
} IMX_I2C_REGISTERS;

typedef struct {
  UINT32 ControllerAddress;
  UINT32 ControllerSlaveAddress;
  UINT32 ReferenceFrequency;
  UINT32 TargetFrequency;
  UINT32 SlaveAddress;
  UINT32 TimeoutInUs;
} IMX_I2C_CONTEXT;

typedef struct {
  UINT32 Divider;
  UINT32 I2cClockRate;
} IMX_I2C_DIVIDER;

/**
  Perform I2C read operation.

  The iMXI2cRead perform I2C read operation by programming the I2C controller.
  The caller is responsible to provide I2C controller configuration.

  @param[in]    I2cContext        Pointer to structure containing the targeted
                                  I2C controller to be used for I2C operation.
  @param[in]    RegisterAddress   Targeted device register address to start read.
  @param[out]   ReadBufferPtr     Caller supplied buffer that would be written
                                  into with data from the read operation.
  @param[in]    ReadBufferSize    Size of caller supplied buffer.

  @retval   RETURN_SUCCESS        I2C Read operation succeeded.
  @retval   RETURN_DEVICE_ERROR   The I2C device is not functioning correctly.

**/
RETURN_STATUS
iMXI2cRead (
  IN IMX_I2C_CONTEXT  *I2cContext,
  IN UINT8            RegisterAddress,
  OUT UINT8           *ReadBufferPtr,
  IN UINT32           ReadBufferSize
  );

/**
  Perform I2C write operation.

  The iMXI2cWrite perform I2C write operation by programming the I2C
  controller. The caller is responsible to provide I2C controller
  configuration.

  @param[in]    IMX_I2C_CONFIG    Pointer to structure containing the targeted
                                  I2C controller to be used for I2C operation.
  @param[in]    RegisterAddress   Targeted device register address to start write.
  @param[out]   WriteBufferPtr    Caller supplied buffer that contained data that
                                  would be read from for I2C write operation.
  @param[in]    WriteBufferSize   Size of caller supplied buffer.

  @retval   RETURN_SUCCESS        I2C Write operation succeeded.
  @retval   RETURN_DEVICE_ERROR   The I2C device is not functioning correctly.

**/
RETURN_STATUS
iMXI2cWrite (
  IN IMX_I2C_CONTEXT  *I2cContext,
  IN UINT8            RegisterAddress,
  IN UINT8            *WriteBufferPtr,
  IN UINT32           WriteBufferSize
  );

#endif
