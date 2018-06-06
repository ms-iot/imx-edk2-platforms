/** @file
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
**/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // getopt
#include <Library/DebugLib.h> // ASSERT

#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>

#ifdef CPU_IMX6SX
#include "imx6sxdef.inc"
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#if defined(CPU_IMX6DQ) || defined(CPU_IMX6SDL) || defined (CPU_IMX6DQP)
typedef enum {
  IMX_PAD_CFG_GPIO_0_CCM_CLKO1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_50_OHM,
                              IMX_SPEED_MAXIMUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT0_CCM_CLKO1)
} IMX_CCM_PADCFG;
#elif defined(CPU_IMX6SX)
typedef enum {
  IMX_PAD_CFG_GPIO1_IO11_CCM_CLKO1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_50_OHM,
                              IMX_SPEED_MAXIMUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO1_IO11_ALT3_CCM_CLKO1)
} IMX_CCM_PADCFG;
#elif defined(CPU_IMX6ULL)
#else
#error iMX6ClkPwr - iMX6 CPU type not defined!
#endif

IMX_CLK ImxClkFromString (const char *ClockStr)
{
  wchar_t clockStrWide[64];
  mbstowcs (clockStrWide, ClockStr, ARRAYSIZE(clockStrWide));

  int i;
  for (i = 0; i < IMX_CLK_MAX; ++i) {
    if (wcscmp (clockStrWide, StringFromImxClk ((IMX_CLK) i)) == 0) {
      return (IMX_CLK) i;
    }
  }

  return IMX_CLK_NONE;
}

EFI_STATUS PinOutClock (IMX_CLK Clock)
{
  if (Clock == IMX_CLK_NONE) {
    wprintf (L"Stopping clock on GPIO0\r\n");
    ImxClkPwrClkOut1Disable ();
    return EFI_SUCCESS;
  }

  wprintf (
    L"Pinning out %ls / 8 on GPIO0\r\n",
    StringFromImxClk (Clock));

  EFI_STATUS status = ImxClkPwrClkOut1Enable (Clock, 8);
  if (EFI_ERROR (status)) {
    wprintf (
      L"Failed to pin out %ls (status = 0x%x)\r\n",
      StringFromImxClk (Clock),
      status);

    return status;
  }

#if defined(CPU_IMX6DQ) || defined (CPU_IMX6DQP)
  ImxPadConfig (IMX_PAD_GPIO_0, IMX_PAD_CFG_GPIO_0_CCM_CLKO1);
#elif defined(CPU_IMX6SDL)
  ImxPadConfig(IMX_PAD_GPIO_0, IMX_PAD_CFG_GPIO_0_CCM_CLKO1);
#elif defined(CPU_IMX6SX)
  ImxPadConfig (IMX_PAD_GPIO1_IO11, IMX_PAD_CFG_GPIO1_IO11_CCM_CLKO1);
#elif defined(CPU_IMX6ULL)
#else
#error iMX6 CPU Type not defined
#endif

  return EFI_SUCCESS;
}

EFI_STATUS DumpOne (IMX_CLK Clock)
{
  IMX_CLOCK_INFO clockInfo;
  EFI_STATUS status = ImxClkPwrGetClockInfo (Clock, &clockInfo);
  if (EFI_ERROR (status)) {
    wprintf (
      L"%ls: Failed to get clock info (status = 0x%x)\r\n",
      StringFromImxClk (Clock),
      status);
    return status;
  }

  wprintf (
    L"%ls\r\n"
    L"    Frequency: %d (%d Mhz)\r\n"
    L"    Parent: %ls\n\r"
    L"\r\n",
    StringFromImxClk (Clock),
    clockInfo.Frequency,
    clockInfo.Frequency / 1000000,
    StringFromImxClk (clockInfo.Parent));

  return EFI_SUCCESS;
}

void DumpAll ()
{
  int i;
  for (i = IMX_CLK_NONE + 1; i < IMX_CLK_MAX; ++i) {
    DumpOne ((IMX_CLK) i);
  }
}

static const wchar_t Help[] =
L"ImxClkPwr: Utility to inspect the clock tree on IMX6\r\n"
L"\r\n"
L"Usage:\r\n"
L"  ImxClkPwr.efi [-p CLOCK_NAME] [-d CLOCK_NAME]\r\n"
L"\r\n"
L"Options:\r\n"
L"  -p CLOCK_NAME     Pin out the specified clock on GPIO0. This does not\r\n"
L"                    ungate the clock. If the clock is gated, you will not see\r\n"
L"                    any output.\r\n"
L"  -d CLOCK_NAME     Dumps the frequency and parent of the specified clock.\r\n"
L"\r\n"
L"Examples:\r\n"
L"\r\n"
L"  Dump all clocks:\r\n"
L"    ImxClkPwr.efi\r\n"
L"\r\n"
L"  Dump a single clock:\r\n"
L"    ImxClkPwr.efi -d AXI_CLK_ROOT\r\n"
L"\r\n"
L"  Pin out a clock on GPIO0:\r\n"
L"    ImxClkPwr.efi -p GPU3D_CORE_CLK_ROOT\r\n"
L"\r\n"
L"  Stop pinning out a clock on GPIO0:\r\n"
L"    ImxClkPwr.efi -p\r\n"
L"\r\n";

void Usage ()
{
  wprintf (Help);
  wprintf (L"Clock Names:\r\n");

  int i;
  for (i = IMX_CLK_NONE + 1; i < IMX_CLK_MAX; ++i) {
    wprintf (L"  %ls\r\n", StringFromImxClk ((IMX_CLK) i));
  }
}

int main (IN int argc, IN char **argv)
{
  enum {
    ACTION_DUMP_ALL,
    ACTION_DUMP_ONE,
    ACTION_PINOUT,
  } action = ACTION_DUMP_ALL;

  const char *clockName = NULL;

  int opt;
  while ((opt = getopt (argc, argv, ":hd:p:")) != -1) {
    switch (opt) {
    case 'h':
      Usage ();
      return 0;
    case 'd':
      action = ACTION_DUMP_ONE;
      clockName = optarg;
      break;
    case 'p':
      action = ACTION_PINOUT;
      clockName = optarg;
      break;
    case ':':
    default:
      fwprintf (stderr, L"Invalid usage.\r\n");
      Usage ();
      return 1;
    }
  }

  if (action == ACTION_DUMP_ALL) {
    DumpAll ();
    return 0;
  }

  EFI_STATUS status;
  switch (action) {
  case ACTION_DUMP_ONE:
  {
    if ((clockName == NULL) || (strlen (clockName) == 0)) {
      fwprintf (stderr, L"Missing required parameter 'CLOCK_NAME' for -d option\r\n");
      return 1;
    }

    IMX_CLK clock = ImxClkFromString (clockName);
    if (clock == IMX_CLK_NONE) {
      fwprintf (stderr, L"Invalid clock name: %s\r\n", clockName);
      return 1;
    }

    status = DumpOne (clock);
    break;
  }
  case ACTION_PINOUT:
  {
    IMX_CLK clock;
    if ((clockName == NULL) || (strlen (clockName) == 0)) {
      clock = IMX_CLK_NONE;
    } else {
      clock = ImxClkFromString (clockName);
      if (clock == IMX_CLK_NONE) {
        fwprintf (stderr, L"Invalid clock name: %s\r\n", clockName);
        return 1;
      }
    }

    status = PinOutClock (clock);
    break;
  }
  default:
    ASSERT (FALSE);
    return 1;
  }

  return EFI_ERROR (status) ? 1 : 0;
}
