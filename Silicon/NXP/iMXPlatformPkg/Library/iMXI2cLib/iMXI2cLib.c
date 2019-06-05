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

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include <iMXI2cLib.h>

// Table of I2C Frequency Divider values (Table 35-3 in iMX6DQRM)
// Used to identify the proper I2C Clock Rate value in the I2C_IFDR register.
// First value is the Divider value and second value is the corresponding
// I2C Clock Rate.
static IMX_I2C_DIVIDER mDividerValue[] = {
  {22, 0x20},
  {24, 0x21},
  {26, 0x22},
  {28, 0x23},
  {30, 0x00},
  {32, 0x24},
  {36, 0x25},
  {40, 0x26},
  {42, 0x03},
  {44, 0x27},
  {48, 0x28},
  {52, 0x05},
  {56, 0x29},
  {60, 0x06},
  {64, 0x2A},
  {72, 0x2B},
  {80, 0x2C},
  {88, 0x09},
  {96, 0x2D},
  {104, 0x0A},
  {112, 0x2E},
  {128, 0x2F},
  {144, 0x0C},
  {160, 0x30},
  {192, 0x31},
  {224, 0x32},
  {240, 0x0F},
  {256, 0x33},
  {288, 0x10},
  {320, 0x34},
  {384, 0x35},
  {448, 0x36},
  {480, 0x13},
  {512, 0x37},
  {576, 0x14},
  {640, 0x38},
  {768, 0x39},
  {896, 0x3A},
  {960, 0x17},
  {1024, 0x3B},
  {1152, 0x18},
  {1280, 0x3C},
  {1536, 0x3D},
  {1792, 0x3E},
  {1920, 0x1B},
  {2048, 0x3F},
  {2304, 0x1C},
  {2560, 0x1D},
  {3072, 0x1E},
  {3840, 0x1F},
};

BOOLEAN
iMXI2cWaitStatusSet (
  IN  IMX_I2C_CONTEXT   *I2cContext,
  IN  UINT16            StatusBits
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  UINT32                  Counter;
  IMX_I2C_I2SR_REGISTER   Data;

  Counter = I2cContext->TimeoutInUs;
  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;
  while (Counter) {
    Data = (IMX_I2C_I2SR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2SR);
    if ((Data.Raw & StatusBits) == StatusBits) {
      return TRUE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  return FALSE;
}

BOOLEAN
iMXI2cWaitStatusClear (
  IN  IMX_I2C_CONTEXT   *I2cContext,
  IN  UINT16            StatusBits
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  UINT32                  Counter;
  IMX_I2C_I2SR_REGISTER   Data;

  Counter = I2cContext->TimeoutInUs;
  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;
  while (Counter) {
    Data = (IMX_I2C_I2SR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2SR);
    if ((Data.Raw & StatusBits) == 0) {
      return TRUE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  return FALSE;
}

BOOLEAN
iMXI2cSendByte (
  IN  IMX_I2C_CONTEXT   *I2cContext,
  IN  UINT8             Data
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  UINT32                  Counter;
  UINT16                  SendData;
  IMX_I2C_I2SR_REGISTER   StatusData;

  SendData = Data;
  Counter = I2cContext->TimeoutInUs;
  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;

  // Clear Interrupt status bits
  MmioWrite16 ((UINTN)&BaseAddress->I2SR, 0);
  // Transfer byte
  MmioWrite16 ((UINTN)&BaseAddress->I2DR, SendData);

  while (Counter) {
    StatusData = (IMX_I2C_I2SR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2SR);
    if (StatusData.IIF == IMX_I2C_I2SR_IIF_INTERRUPT_PENDING) {
      return TRUE;
    } else if (StatusData.IAL == IMX_I2C_I2SR_IAL_ARBITRATION_LOST) {
      DEBUG ((DEBUG_ERROR, "%a: fail 0x%04x\n", __FUNCTION__, StatusData.Raw));
      return FALSE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  DEBUG ((DEBUG_ERROR, "%a: Fail timeout\n", __FUNCTION__));
  return FALSE;
}

RETURN_STATUS
iMXI2cSetupController (
  IN  IMX_I2C_CONTEXT   *I2cContext
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  IMX_I2C_IADR_REGISTER   AddressData;
  IMX_I2C_I2CR_REGISTER   ControlData;
  UINT32                  Divider;
  UINT32                  DividerCount;
  IMX_I2C_IFDR_REGISTER   DividerData;

  BaseAddress = (IMX_I2C_REGISTERS *)I2cContext->ControllerAddress;

  // Disable controller
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, 0);
  // Clear any pending interrupt status
  MmioWrite16 ((UINTN)&BaseAddress->I2SR, 0);

  // Setup Divider if reference frequency is provided.
  // If no reference frequency is provided, fall through and use value setup
  // by first boot loader
  if (I2cContext->ReferenceFrequency != 0) {
    DividerData.Raw = 0;
    Divider = I2cContext->ReferenceFrequency / I2cContext->TargetFrequency;
    for (DividerCount = 0; DividerCount < ARRAY_SIZE(mDividerValue); ++DividerCount) {
      if (mDividerValue[DividerCount].Divider >= Divider) {
        DEBUG ((DEBUG_INFO,
                "%a: Divider %d I2cClockRate 0x%02x\n",
                __FUNCTION__,
                mDividerValue[DividerCount].Divider,
                mDividerValue[DividerCount].I2cClockRate));
        DividerData.IC = mDividerValue[DividerCount].I2cClockRate;
        break;
      }
    }
    MmioWrite16 ((UINTN)&BaseAddress->IFDR, DividerData.Raw);
  }

  // Setup slave address
  AddressData.Raw = 0;
  AddressData.ADR = I2cContext->ControllerSlaveAddress;
  MmioWrite16 ((UINTN)&BaseAddress->IADR, AddressData.Raw);

  // Enable controller
  ControlData = (IMX_I2C_I2CR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2CR);
  ControlData.IEN = IMX_I2C_I2CR_IEN_INTERRUPT_ENABLED;
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, ControlData.Raw);
  MicroSecondDelay (100);

  // Clear pending interrupt status bits
  MmioWrite16 ((UINTN)&BaseAddress->I2SR, 0);

  // Wait for bus to be idle
  if (iMXI2cWaitStatusClear (I2cContext, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Controller remains busy\n", __FUNCTION__));
    return RETURN_DEVICE_ERROR;
  }

  // Select master mode and transmit mode.
  // Note: STOP must have been called prior to this (i.e. MTX = 0)
  ControlData.MTX = IMX_I2C_I2CR_MTX_TRANSMIT_MODE;
  ControlData.MSTA = IMX_I2C_I2CR_MSTA_MASTER_MODE;
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, ControlData.Raw);

  // Now wait for bus to be busy
  if (iMXI2cWaitStatusSet (I2cContext, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Controller remains idle\n", __FUNCTION__));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

RETURN_STATUS
iMXI2cSendDeviceAddress (
  IN  IMX_I2C_CONTEXT   *I2cContext,
  IN  UINT8             DeviceAddress,
  IN  UINT8             Direction
  )
{
  IMX_I2C_REGISTERS               *BaseAddress;
  IMX_I2C_DEVICE_ADDRESS_PACKET   Data;
  BOOLEAN                         Result;

  if ((DeviceAddress < 0) || (DeviceAddress > 0x7F)) {
    return RETURN_DEVICE_ERROR;
  }

  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;

  // Send slave address packet
  Data.DeviceAddress = DeviceAddress;
  Data.Direction = Direction;
  Result = iMXI2cSendByte (I2cContext, Data.Raw);
  if (Result == FALSE) {
    DEBUG ((DEBUG_ERROR,
            "%a: Slave address transfer fail 0x%04x\n",
            __FUNCTION__,
            MmioRead16 ((UINTN)&BaseAddress->I2SR)));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

RETURN_STATUS
iMXI2cGenerateStart (
  IN  IMX_I2C_CONTEXT   *I2cContext,
  IN  UINT8             Direction
  )
{
  RETURN_STATUS     Status;

  // START sequence occurs when Device Address is sent
  Status = iMXI2cSendDeviceAddress (I2cContext, I2cContext->SlaveAddress, Direction);
  return Status;
}

RETURN_STATUS
iMXI2cConfigureRepeatStart (
  IN  IMX_I2C_CONTEXT   *I2cContext
  )
{
  IMX_I2C_REGISTERS      *BaseAddress;
  IMX_I2C_I2CR_REGISTER  Data;

  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;

  // Enable Repeated Start for the controller
  Data = (IMX_I2C_I2CR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2CR);
  Data.RSTA = IMX_I2C_I2CR_RSTA_REPEAT_START_ENABLE;
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, Data.Raw);

  // Spec mandates a delay of at least 2 module clock cycles (78ns) after
  // setting RSTA
  MicroSecondDelay (1);

  return RETURN_SUCCESS;
}

RETURN_STATUS
iMXI2cGenerateStop (
  IN  IMX_I2C_CONTEXT   *I2cContext
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  IMX_I2C_I2CR_REGISTER   Data;

  // Turn off master mode, disable repeat start, and set to RX mode
  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;
  Data = (IMX_I2C_I2CR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2CR);
  Data.MSTA = IMX_I2C_I2CR_MSTA_SLAVE_MODE;
  Data.MTX = IMX_I2C_I2CR_MTX_RECEIVE_MODE;
  Data.RSTA = IMX_I2C_I2CR_RSTA_REPEAT_START_DISABLE;
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, Data.Raw);

  // Wait for bus to go idle
  if (iMXI2cWaitStatusClear (I2cContext, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "%a: Controller remains busy\n", __FUNCTION__));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

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
IN UINT32             ReadBufferSize
  )
{
  IMX_I2C_REGISTERS       *BaseAddress;
  IMX_I2C_I2CR_REGISTER   Data;
  BOOLEAN                 Result;
  RETURN_STATUS           Status;

  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;

  // Initialize controller
  Status = iMXI2cSetupController (I2cContext);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to setup controller %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Generate Start signal and send slave device address
  Status = iMXI2cGenerateStart (I2cContext, IMX_I2C_TX);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: iMXI2cGenerateStart failed %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  // Send target register address
  Result = iMXI2cSendByte (I2cContext, RegisterAddress);
  if (Result == FALSE) {
    DEBUG ((DEBUG_ERROR,
            "%a: Slave register address transfer fail 0x%04x\n",
            __FUNCTION__,
            MmioRead16 ((UINTN)&BaseAddress->I2SR)));
    Status = RETURN_DEVICE_ERROR;
    goto Exit;
  }

  // Configure Repeated Start in order to indicate read
  Status = iMXI2cConfigureRepeatStart(I2cContext);

  // Generate Start condition for the read
  Status = iMXI2cGenerateStart (I2cContext, IMX_I2C_RX);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: iMXI2cGenerateStart failed %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  // Change controller to Master Receive Mode
  Data = (IMX_I2C_I2CR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2CR);
  Data.MTX = IMX_I2C_I2CR_MTX_RECEIVE_MODE;
  MmioWrite16 ((UINTN)&BaseAddress->I2CR, Data.Raw);

  // Clear controller status bits
  MmioWrite16 ((UINTN)&BaseAddress->I2SR, 0);
  // Spec indicates to perform a dummy read to kick off Data Receive flow
  MmioRead16 ((UINTN)&BaseAddress->I2DR);

  do {
    // Wait for read transfer to complete
    if (iMXI2cWaitStatusSet (I2cContext, IMX_I2C_I2SR_IIF) == FALSE) {
      DEBUG ((DEBUG_ERROR, "%a: waiting for read fail\n", __FUNCTION__));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    if (iMXI2cWaitStatusSet (I2cContext, IMX_I2C_I2SR_ICF) == FALSE) {
      DEBUG ((DEBUG_ERROR, "%a: waiting for read fail\n", __FUNCTION__));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    // Before the last byte is read, a Stop signal must be generated
    if (ReadBufferSize == 1) {
      Status = iMXI2cGenerateStop (I2cContext);
      if (RETURN_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: iMXI2cGenerateStop fail %r\n", __FUNCTION__, Status));
        goto Exit;
      }
    }

    // For second to last byte to read, inform controller to not send transmit ack.
    // This will inform the slave to stop sending more data after the next byte.
    if (ReadBufferSize == 2) {
      Data = (IMX_I2C_I2CR_REGISTER)MmioRead16 ((UINTN)&BaseAddress->I2CR);
      Data.TXAK = IMX_I2C_I2CR_TXAK_NO_TRANSMIT_ACK;
      MmioWrite16 ((UINTN)&BaseAddress->I2CR, Data.Raw);
    }

    // Clear controller status bits
    MmioWrite16 ((UINTN)&BaseAddress->I2SR, 0);

    *ReadBufferPtr = MmioRead8 ((UINTN)&BaseAddress->I2DR);
    ++ReadBufferPtr;
    --ReadBufferSize;
  } while (ReadBufferSize > 0);

  // Generate Stop signal and idle controller
  Status = iMXI2cGenerateStop (I2cContext);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Final iMXI2cGenerateStop fail %r\n", __FUNCTION__, Status));
  }

Exit:
  return Status;
}

/**
  Perform I2C write operation.

  The iMXI2cWrite perform I2C write operation by programming the I2C
  controller. The caller is responsible to provide I2C controller
  configuration.

  @param[in]    I2cContext        Pointer to structure containing the targeted
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
  )
{
  IMX_I2C_REGISTERS     *BaseAddress;
  BOOLEAN               Result;
  RETURN_STATUS         Status;

  BaseAddress = (IMX_I2C_REGISTERS*)I2cContext->ControllerAddress;

  // Initialize controller
  Status = iMXI2cSetupController (I2cContext);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to setup controller %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Generate Start signal and send slave device address
  Status = iMXI2cGenerateStart (I2cContext, IMX_I2C_TX);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: iMXI2cGenerateStart fail %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  // Send target register address to indicate where to start write
  Result = iMXI2cSendByte (I2cContext, RegisterAddress);
  if (Result == FALSE) {
    DEBUG ((DEBUG_ERROR,
            "%a: Slave register address transfer fail 0x%04x\n",
            __FUNCTION__,
            MmioRead16 ((UINTN)&BaseAddress->I2SR)));
    Status = RETURN_DEVICE_ERROR;
    goto Exit;
  }

  // Write data
  while (WriteBufferSize > 0) {
    Result = iMXI2cSendByte (I2cContext, *WriteBufferPtr);
    if (Result == FALSE) {
      DEBUG ((DEBUG_ERROR,
              "%a: Slave address transfer fail 0x%04x\n",
              __FUNCTION__,
              MmioRead16 ((UINTN)&BaseAddress->I2SR)));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    ++WriteBufferPtr;
    --WriteBufferSize;
  }

  // Generate Stop signal and idle controller
  Status = iMXI2cGenerateStop (I2cContext);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: iMXI2cGenerateStop fail %r\n", __FUNCTION__, Status));
  }

Exit:
  return Status;
}
