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

#ifndef _SDHC_DXE_H_
#define _SDHC_DXE_H_

typedef struct {
  UINT32 IoNumber;
  IMX_GPIO_BANK Bank;
} IMX_GPIO_PIN;

#define PCD_GPIO_PIN_IO_NUMBER(X)   ((X) & 0xFF)
#define PCD_GPIO_PIN_BANK(X)        (((X) >> 8) & 0xFF)

// A special value to indicate that GPIO is not the signal source for either
// CardDetect or WriteProtect
typedef enum {
  USDHC_SIGNAL_GPIO_START = 0x000,
  USDHC_SIGNAL_GPIO_END = 0xFF00,
  USDHC_SIGNAL_OVERRIDE_PIN_LOW = 0xFF00,
  USDHC_SIGNAL_OVERRIDE_PIN_HIGH = 0xFF01,
  USDHC_SIGNAL_INTERNAL_PIN = 0xFFFF
} USDHC_SIGNAL_SOURCE;

#define USDHC_IS_GPIO_SIGNAL_SOURCE(X)  \
    (((X) >= USDHC_SIGNAL_GPIO_START) && ((X) < USDHC_SIGNAL_GPIO_END))

typedef struct {
  UINT32 SdhcId;
  EFI_HANDLE SdhcProtocolHandle;
  USDHC_REGISTERS *RegistersBase;
  USDHC_SIGNAL_SOURCE CardDetectSignal;
  USDHC_SIGNAL_SOURCE WriteProtectSignal;
  IMX_GPIO_PIN CardDetectGpioPin;
  IMX_GPIO_PIN WriteProtectGpioPin;
} USDHC_PRIVATE_CONTEXT;

#define LOG_FMT_HELPER(FMT, ...) \
    "SDHC%d:" FMT "%a\n", ((SdhcCtx != NULL) ? SdhcCtx->SdhcId : -1), __VA_ARGS__

#define LOG_INFO(...) \
    DEBUG((DEBUG_INFO | DEBUG_BLKIO, LOG_FMT_HELPER(__VA_ARGS__, "")))

#define LOG_TRACE(...) \
    DEBUG((DEBUG_VERBOSE | DEBUG_BLKIO, LOG_FMT_HELPER(__VA_ARGS__, "")))

#define LOG_ERROR(...) \
    DEBUG((DEBUG_ERROR | DEBUG_BLKIO, LOG_FMT_HELPER(__VA_ARGS__, "")))

#define LOG_ASSERT(TXT) \
    ASSERT(!"Sdhc: " TXT "\n")

// uSDHC read/write FIFO is 128x32-bit
#define USDHC_FIFO_MAX_WORD_COUNT       128

// Max block count allowed in a single transfer
#define USDHC_MAX_BLOCK_COUNT           0xFFFF

// Number of register maximum polls
#define USDHC_POLL_RETRY_COUNT          100000

// Waits between each registry poll
#define USDHC_POLL_WAIT_US              20

// uSDHC input clock. Ideally, should query it from clock manager
#define USDHC_BASE_CLOCK_FREQ_HZ        198000000

#define USDHC_BLOCK_LENGTH_BYTES               512

#endif // _SDHC_DXE_H_
