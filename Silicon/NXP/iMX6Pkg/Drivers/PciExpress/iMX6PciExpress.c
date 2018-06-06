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

#include <IndustryStandard/Pci.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include "iMX6PciExpress.h"

PCI_RESOURCE PcieResource[] = {
  // Memory resource
  {
    PCIE_MEMORY_SPACE_BASE,
    PCIE_MEMORY_SPACE_SIZE,
    PCIE_MEMORY_SPACE_BASE
  },
};

// Pcie read and write function
EFI_STATUS
PciePciWrite (
  IN EFI_PCI_IO_PROTOCOL_WIDTH Width,
  IN UINTN Address,
  IN UINTN Count,
  IN VOID *Buffer
  );

EFI_STATUS
PciePciRead (
  IN EFI_PCI_IO_PROTOCOL_WIDTH Width,
  IN UINTN Address,
  IN UINTN Count,
  IN VOID *Buffer
  );

// Internal Address Translation Unit configuration table. Map the Pcie device
// configuration baesd on configuration. Pci IO space is not supported on
// Windows. Memory space segment is just mapped back to the same address.
//
// The following table is used to setup basic translation setting on various
// ATU (Address Translation Unit). The ATU is responsible to retranslate
// address for inbound and outbound message.
//
// Address match mode address translation is based on the following formula :
//     Address = Address - Base Address + Target Address
//
// There really isnt a need to retranslate the address for iMX6 however proceed
// the program the ATU to for configuration and memory message.
IATU_SETTINGS iMX6iAtuSettings[] = {
  // Configuration message
  {
    OUTBOUND,
    0,
    CFG0_TYPE,
    PCIE_DEVICE_CONFIG_BASE_REG,
    0,
    PCIE_DEVICE_CONFIG_BASE_REG + PCIE_DEVICE_CONFIG_SIZE - 1,
    PCIE_DEVICE_CONFIG_BASE_REG,
    0,
    REGION_ENABLE,
  },

  // Memory message
  {
    OUTBOUND,
    2,
    MEMORY_TYPE,
    PCIE_MEMORY_SPACE_BASE,
    0,
    PCIE_MEMORY_SPACE_BASE + PCIE_MEMORY_SPACE_SIZE - 1,
    PCIE_MEMORY_SPACE_BASE,
    0,
    REGION_ENABLE,
  },
};

VOID
PcieSetupiAtu (
  IN  IATU_SETTINGS   *SettingsPtr
  )
{
  volatile CSP_PCIE_PL_REGS *pPortLogicRegs;

  ASSERT (SettingsPtr->RegionIndex < MAX_iATU_REGION);
  pPortLogicRegs = (CSP_PCIE_PL_REGS *)PCIE_CTRL_PORT_LOGIG_BASE_REG;

  // Program specific ATU region
  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATUVR,
    (SettingsPtr->RegionDirection << 31 | SettingsPtr->RegionIndex));

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURC2,
    REGION_DISABLE);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURLBA,
    SettingsPtr->LowerBaseAddr);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURUBA,
    SettingsPtr->UpperBaseAddr);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURLA,
    SettingsPtr->LimitAddr);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURLTA,
    SettingsPtr->LowerTargetAddr);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURUTA,
    SettingsPtr->UpperTargetAddr);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURC1,
    SettingsPtr->Type);

  MmioWrite32 (
    (UINTN)&pPortLogicRegs->PCIE_PL_iATURC2,
    SettingsPtr->State);
}

VOID
PcieSetupiAtuSettings (
  VOID
  )
{
  UINT32 i;

  // Initialize internal Address Translation Unit based on settings specify
  // in iMX6iAtuSettings table.
  for (i = 0; i < ARRAYSIZE (iMX6iAtuSettings); ++i) {
    PcieSetupiAtu (&iMX6iAtuSettings[i]);
  }

  return;
}

EFI_STATUS
PcieSetPhyState (
  IN  BOOLEAN   State
  )
{
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIoMuxcGprRegisters;
  IMX_IOMUXC_GPR1_REG                 Gpr1Reg;

  pIoMuxcGprRegisters = (IMX_IOMUXC_GPR_REGISTERS *)IOMUXC_GPR_BASE_ADDRESS;
  Gpr1Reg.AsUint32 = MmioRead32 ((UINTN)&pIoMuxcGprRegisters->GPR1);
  if (State == TRUE) {
#if defined(CPU_IMX6DQP)
    Gpr1Reg.PCIE_SW_RST = 0;
#endif
    Gpr1Reg.REF_SSP_EN = 1;     // Enable Pcie PHY
    Gpr1Reg.TEST_POWERDOWN = 0; // Power down is not requested
  } else {
    Gpr1Reg.REF_SSP_EN = 0;     // Disable Pcie PHY
    Gpr1Reg.TEST_POWERDOWN = 1; // Power down is requested
  }
  MmioWrite32 ((UINTN)&pIoMuxcGprRegisters->GPR1, Gpr1Reg.AsUint32);
  return EFI_SUCCESS;
}

EFI_STATUS
PcieSetupInitSetting (
  VOID
  )
{
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIoMuxcGprRegisters;
  EFI_STATUS                          Status;
  IMX_IOMUXC_GPR1_REG                 Gpr1Reg;
  IMX_IOMUXC_GPR12_REG                Gpr12Reg;
  IMX_IOMUXC_GPR8_REG                 Gpr8Reg;

  pIoMuxcGprRegisters = (IMX_IOMUXC_GPR_REGISTERS *)IOMUXC_GPR_BASE_ADDRESS;

  // If Pcie PHY is already enabled we are in an unexpected state, just exit
  // and assume a bootloader has already setup Pcie and assigned resources.
  Gpr1Reg.AsUint32 = MmioRead32 ((UINTN)&pIoMuxcGprRegisters->GPR1);
  if (Gpr1Reg.REF_SSP_EN == 1) {
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Disable the PHY first, without this Pci link randomly does not come up
  Status = PcieSetPhyState (FALSE);
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to disable Pcie PHY\n");
    goto Exit;
  }

  // First configure Pcie and Pcie PHY default setting
  Gpr12Reg.AsUint32 = MmioRead32 ((UINTN)&pIoMuxcGprRegisters->GPR12);
  Gpr12Reg.APP_LTSSM_ENABLE = 0;          // Set application not ready
  Gpr12Reg.DIA_STATUS_BUS_SELECT = 0xB;   // Debug functionality
  Gpr12Reg.DEVICE_TYPE = 0x4;             // Set to RC mode
  Gpr12Reg.LOS_LEVEL = 0x9;               // Set to 0x9 per reference manual
  MmioWrite32 ((UINTN)&pIoMuxcGprRegisters->GPR12, Gpr12Reg.AsUint32);

  // Gen1 | Gen2 3p5 | Gen2 6 | Swing full 127 | Swing low 127
  Gpr8Reg.PCS_TX_DEEMPH_GEN1 = 0;
  Gpr8Reg.PCS_TX_DEEMPH_GEN2_3P5DB = 0;
  Gpr8Reg.PCS_TX_DEEMPH_GEN2_6DB = 20;
  Gpr8Reg.PCS_TX_SWING_FULL = 127;
  Gpr8Reg.PCS_TX_SWING_LOW = 127;
  MmioWrite32 ((UINTN)&pIoMuxcGprRegisters->GPR8, Gpr8Reg.AsUint32);

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
PcieSetClockGate (
  IN  IMX_CLOCK_GATE_STATE  State
  )
{
  ImxClkPwrSetClockGate (IMX_PCIE_ROOT_ENABLE, State);
  return EFI_SUCCESS;
}

EFI_STATUS
PcieVerifyClocks (
  VOID
  )
{
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegisters;
  IMX_CCM_ANALOG_PLL_ENET_REG         CcmAnalogPllReg;

  pCcmAnalogRegisters = (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;
  CcmAnalogPllReg.AsUint32 = MmioRead32 ((UINTN)&pCcmAnalogRegisters->PLL_ENET);
  if ((CcmAnalogPllReg.POWERDOWN == 0) &&
      (CcmAnalogPllReg.BYPASS == 0) &&
      (CcmAnalogPllReg.ENABLE_125M == 1) &&
      (CcmAnalogPllReg.LOCK == 1)) {
    return EFI_SUCCESS;
  }

  return EFI_DEVICE_ERROR;
}

VOID
PcieEnablePerstLine (
  VOID
  )
{
  // Enable board specific PERST line if one is defined
  if (FixedPcdGet32 (PcdPcieResetGpio)) {
    ImxGpioWrite (
      FixedPcdGet32 (PcdPcieResetGpioBankNumber),
      FixedPcdGet32 (PcdPcieResetGpioIoNumber),
      IMX_GPIO_HIGH);
    gBS->Stall (20000);
  }
}

EFI_STATUS
PcieSetupPciBridge (
  VOID
  )
{
  UINT8 classCode[0];

  // Setup the bridge class
  classCode[0] = PCI_IF_BRIDGE_P2P;
  classCode[1] = PCI_CLASS_BRIDGE_P2P;
  classCode[2] = PCI_CLASS_BRIDGE;

  return PciePciWrite (
           EfiPciIoWidthUint8,
           PCIE_HOST_CONFIG_BASE_REG + PCI_CLASSCODE_OFFSET,
           3,
           classCode);
}

EFI_STATUS
PcieSetLinkStatus (
  IN  BOOLEAN   State
  )
{
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIoMuxcGprRegisters;
  IMX_IOMUXC_GPR12_REG                Gpr12Reg;

  pIoMuxcGprRegisters = (IMX_IOMUXC_GPR_REGISTERS *)IOMUXC_GPR_BASE_ADDRESS;
  Gpr12Reg.AsUint32 = MmioRead32 ((UINTN)&pIoMuxcGprRegisters->GPR12);
  if (State == TRUE) {
    Gpr12Reg.APP_LTSSM_ENABLE = 1; // Enable link
  } else {
    Gpr12Reg.APP_LTSSM_ENABLE = 0; // Disable link
  }
  MmioWrite32 ((UINTN)&pIoMuxcGprRegisters->GPR12, Gpr12Reg.AsUint32);

  return EFI_SUCCESS;
}

BOOLEAN
PcieIsLinkUp (
  VOID
  )
{
  volatile CSP_PCIE_PL_REGS   *pPortLogicRegs;
  UINT32                      Debug1Reg;

  pPortLogicRegs = (CSP_PCIE_PL_REGS *)PCIE_CTRL_PORT_LOGIG_BASE_REG;
  Debug1Reg = MmioRead32 ((UINTN)&pPortLogicRegs->PCIE_PL_DEBUG1);
  return (Debug1Reg & PCIE_PL_DEBUG1_PHY_LINK_UP) ? TRUE : FALSE;
}

EFI_STATUS
PcieWaitForLink (
  VOID
  )
{
  UINT32    Counter;
  BOOLEAN   LinkStatus;

  Counter = 200;
  LinkStatus = PcieIsLinkUp ();

  // To optimize boot time, consider lowering timeout value
  while (LinkStatus == FALSE && Counter > 0) {
    --Counter;
    gBS->Stall (1000);
    LinkStatus = PcieIsLinkUp ();
  }

  return (LinkStatus) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}

EFI_STATUS
PcieGetAlignAddress (
  IN  UINTN   Address,
  IN  UINTN   AlignmentSize,
  OUT UINTN   *AlignAddress
  )
{
  EFI_STATUS Status;

  *AlignAddress = 0;
  if ((AlignmentSize & (AlignmentSize - 1)) != 0) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  // Even though we do not add a (AlignmentSize + 1) to the incoming address
  // we would still align to the upper boundary as bit [19:00] is assumed to
  // be 0x000FFFFF per Pcie spec.
  *AlignAddress = (Address) & ~(AlignmentSize - 1);
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
PcieGetPciConfigAddress (
  IN  UINTN   BusNumber,
  IN  UINTN   DevNumber,
  IN  UINTN   FuncNumber,
  IN  UINTN   Register,
  OUT UINTN   *Address
  )
{
  UINT64      Offset;
  EFI_STATUS  Status;

  // For now only support bus 0 and bus 1 with one device in each bus
  if (BusNumber == 0 && DevNumber == 0) {
    Offset = EFI_PCI_ADDRESS (BusNumber, DevNumber, FuncNumber, Register);
    *Address = PCIE_HOST_CONFIG_BASE_REG + Offset;
    Status = EFI_SUCCESS;
  } else if (BusNumber == 1 && DevNumber == 0) {
    Offset = EFI_PCI_ADDRESS (BusNumber, DevNumber, FuncNumber, Register);
    Offset -= EFI_PCI_ADDRESS (1, 0, FuncNumber, 0);
    *Address = PCIE_DEVICE_CONFIG_BASE_REG + Offset;
    Status = EFI_SUCCESS;
  } else {
    *Address = 0;
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

EFI_STATUS
PciePciRead (
  IN  EFI_PCI_IO_PROTOCOL_WIDTH   Width,
  IN  UINTN                       Address,
  IN  UINTN                       Count,
  OUT VOID                        *Buffer
  )
{
  UINT8       *pDest;
  EFI_STATUS  Status;
  UINTN       Stride;

  pDest = (UINT8 *)Buffer;
  Width = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH) (Width & 0x03);
  Stride = (UINTN)1 << Width;

  switch (Width) {
  case EfiPciWidthUint8:
    for (; Count > 0; --Count, pDest += Stride, Address += Stride) {
      *pDest = MmioRead8 (Address);
    }
    Status = EFI_SUCCESS;
    break;
  case EfiPciWidthUint16:
    for (; Count > 0; --Count, pDest += Stride, Address += Stride) {
      *((UINT16 *)pDest) = MmioRead16 (Address);
    }
    Status = EFI_SUCCESS;
    break;
  case EfiPciWidthUint32:
    for (; Count > 0; --Count, pDest += Stride, Address += Stride) {
      *((UINT32 *)pDest) = MmioRead32 (Address);
    }
    Status = EFI_SUCCESS;
    break;
  default:
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

Exit:
  return Status;
}

EFI_STATUS
PciePciWrite (
  IN  EFI_PCI_IO_PROTOCOL_WIDTH   Width,
  IN  UINTN                       Address,
  IN  UINTN                       Count,
  IN  VOID                        *Buffer
  )
{
  UINT8       *pSrc;
  EFI_STATUS  Status;
  UINTN       Stride;

  pSrc = (UINT8 *)Buffer;
  Width = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH) (Width & 0x03);
  Stride = (UINTN)1 << Width;

  switch (Width) {
  case EfiPciWidthUint8:
    for (; Count > 0; --Count, pSrc += Stride, Address += Stride) {
      MmioWrite8 (Address, *pSrc);
    }
    Status = EFI_SUCCESS;
    break;
  case EfiPciWidthUint16:
    for (; Count > 0; --Count, pSrc += Stride, Address += Stride) {
      MmioWrite16 (Address, *((UINT16 *)pSrc));
    }
    Status = EFI_SUCCESS;
    break;
  case EfiPciWidthUint32:
    for (; Count > 0; --Count, pSrc += Stride, Address += Stride) {
      MmioWrite32 (Address, *((UINT32 *)pSrc));
    }
    Status = EFI_SUCCESS;
    break;
  default:
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

Exit:
  return Status;
}

EFI_STATUS
PcieDevicePresent (
  OUT PCI_TYPE00  *PciDevice,
  IN  UINTN       Bus,
  IN  UINTN       Device,
  IN  UINTN       Func
  )
{
  UINTN       Address;
  EFI_STATUS  Status;

  // Create Pci address map in terms of Bus, Device, and Func
  Status = PcieGetPciConfigAddress (Bus, Device, Func, 0, &Address);
  if (EFI_ERROR (Status)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  // Read the Vendor ID register
  Status = PciePciRead (
             EfiPciWidthUint32,
             Address,
             1,
             PciDevice);
  if (!EFI_ERROR (Status) && (PciDevice->Hdr).VendorId != 0xffff) {
    // Read the entire config header for the device
    Status = PciePciRead (
               EfiPciWidthUint32,
               Address,
               sizeof (PCI_TYPE00) / sizeof (UINT32),
               PciDevice);
    if (EFI_ERROR (Status)) {
      PCIE_ERROR ("Failed to read Pci config space\n");
    }
  } else {
    PCIE_INFO ("No Pcie device found\n");
    Status = EFI_NOT_FOUND;
  }

Exit:
  return Status;
}

EFI_STATUS
PcieGetMemoryBarResource (
  IN  UINTN     BarSize,
  IN  UINTN     *BarAddress,
  IN  BOOLEAN   IsBridgeDevice
  )
{
  EFI_STATUS  Status;

  if (BarSize > PcieResource->Size) {
    PCIE_ERROR ("Insufficient Pcie memory for 0x%08x (Current size 0x%08x)\n",
                BarSize,
                PcieResource->Size);
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  *BarAddress = PcieResource->Curr;

  if (IsBridgeDevice == FALSE) {
    PcieResource->Curr += BarSize;
    PcieResource->Size -= BarSize;

    PCIE_INFO ("Allocating memory resource 0x%08x size 0x%08x\n",
               *BarAddress,
               BarSize);
  }

  PCIE_INFO ("Current memory resource 0x%08x Size 0x%08x\n",
             PcieResource->Curr,
             PcieResource->Size);

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
PcieParseAssignBar (
  IN  UINTN     BaseAddress,
  IN  UINTN     MaxBarIndex,
  IN  BOOLEAN   IsBridgeDevice
  )
{
  UINT32        AllOne32;
  UINT32        AllZero;
  UINTN         BarIndex;
  UINTN         BarOffset;
  UINTN         BarSize;
  UINTN         Originalvalue;
  UINTN         ResourceAddress;
  UINTN         ResponseValue;
  EFI_STATUS    Status;

  AllZero = 0;
  AllOne32 = MAX_UINT32;
  for (BarOffset = 0x10, BarIndex = 0;
       BarOffset <= 0x24 && BarIndex < MaxBarIndex;
       BarOffset += sizeof (UINT32), ++BarIndex) {

    Status = PciePciRead (
               EfiPciWidthUint32,
               BaseAddress + BarOffset,
               1,
               &Originalvalue);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint32,
               BaseAddress + BarOffset,
               1,
               &AllOne32);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciRead (
               EfiPciWidthUint32,
               BaseAddress + BarOffset,
               1,
               &ResponseValue);
    ASSERT_EFI_ERROR (Status);

    // No support for IO memory
    // Refer : Pci Local Bus Specification (6.2.5.1)
    if ((ResponseValue & 0x01) == 0x01) {
      Status = PciePciWrite (
                 EfiPciIoWidthUint32,
                 BaseAddress + BarOffset,
                 1,
                 &Originalvalue);
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    // No support for prefetch memory
    if (((ResponseValue & 0x01) == 0x00) &&
        ((ResponseValue & 0x08) == 0x08)) {
      Status = PciePciWrite (
                 EfiPciIoWidthUint32,
                 BaseAddress + BarOffset,
                 1,
                 &Originalvalue);
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    BarSize = (~(ResponseValue & 0xFFFFFFF0)) + 1;

    Status = PcieGetMemoryBarResource (
               BarSize,
               &ResourceAddress,
               IsBridgeDevice);
    if (EFI_ERROR (Status)) {
      PCIE_ERROR ("Failed to acquire BAR resource\n");
      goto Exit;
    }

    Status = PciePciWrite (
               EfiPciIoWidthUint32,
               BaseAddress + BarOffset,
               1,
               &ResourceAddress);
    ASSERT_EFI_ERROR (Status);

    // The subsequent BAR is the upper 32 bit address
    if (((ResponseValue & 0x04) == 0x04) &&
        (BarIndex + 1) < MaxBarIndex) {
      BarOffset += sizeof (UINT32);
      ++BarIndex;

      Status = PciePciWrite (
                 EfiPciIoWidthUint32,
                 BaseAddress + BarOffset,
                 1,
                 &AllZero);
      ASSERT_EFI_ERROR (Status);

      continue;
    }
  }

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
PcieConfigureDevice (
  IN  PCI_TYPE00  PciDevice,
  IN  UINTN       BusNumber,
  IN  UINTN       DevNumber,
  IN  UINTN       FuncNumber
  )
{
  UINT32      AllZero;
  UINTN       BaseAddress;
  UINT8       FixedCacheLineSize;
  UINT16      PciCommand;
  EFI_STATUS  Status;

  AllZero = 0;

  PCIE_INFO (
    "Configuring B:%02d D:%02d F:%02d\n",
    BusNumber,
    DevNumber,
    FuncNumber);

  Status = PcieGetPciConfigAddress (
             BusNumber,
             DevNumber,
             FuncNumber,
             0,
             &BaseAddress);
  ASSERT_EFI_ERROR (Status);

  // Use a fixed cacheline size
  FixedCacheLineSize = 0x10;

  Status = PciePciWrite (
             EfiPciIoWidthUint8,
             BaseAddress + PCI_CACHELINE_SIZE_OFFSET,
             1,
             &FixedCacheLineSize);
  ASSERT_EFI_ERROR (Status);

  if (IS_PCI_BRIDGE (&PciDevice)) {
    PCIE_INFO ("Pci Bridge\n");
    // Pcie initialization sequence, referenced from
    // InitializePpb in MdeModulePkg/Bus/Pci/PciBusDxe
    // No support for IO and prefetch memory
    Status = PciePciWrite (
               EfiPciIoWidthUint8,
               BaseAddress + 0x1C,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint8,
               BaseAddress + 0x1D,
               1,
               &AllZero);

    Status = PciePciWrite (
               EfiPciIoWidthUint16,
               BaseAddress + 0x24,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint16,
               BaseAddress + 0x26,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint32,
               BaseAddress + 0x28,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint32,
               BaseAddress + 0x2C,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint16,
               BaseAddress + 0x30,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    Status = PciePciWrite (
               EfiPciIoWidthUint16,
               BaseAddress + 0x32,
               1,
               &AllZero);
    ASSERT_EFI_ERROR (Status);

    // Type 1 bridge only has 2 BAR register
    Status = PcieParseAssignBar (
               BaseAddress,
               2,
               TRUE);
    if (EFI_ERROR (Status)) {
      PCIE_ERROR ("Failed to assign resource to Pci bridge\n");
      goto Exit;
    }
  } else {
    // Device specific configuration should be implemented here
    PCIE_INFO ("Pci device\n");

    Status = PcieParseAssignBar (
               BaseAddress,
               PCI_MAX_BAR,
               FALSE);
    if (EFI_ERROR (Status)) {
      PCIE_ERROR ("Failed to assign resource to Pci device\n");
      goto Exit;
    }
  }

  Status = PciePciRead (
             EfiPciIoWidthUint16,
             BaseAddress + PCI_COMMAND_OFFSET,
             1,
             &PciCommand);
  ASSERT_EFI_ERROR (Status);

  PciCommand |=
    (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);

  Status = PciePciWrite (
             EfiPciIoWidthUint16,
             BaseAddress + PCI_COMMAND_OFFSET,
             1,
             &PciCommand);
  ASSERT_EFI_ERROR (Status);

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
PcieSimpleScanBusAndAssignResource (
  IN  UINTN   BusNumber
  )
{
  UINTN         BridgeMemory;
  UINTN         BridgeMemoryBase;
  UINTN         BridgeMemoryLimit;
  UINTN         BusBaseRegisterAddress;
  UINT16        BusRegister;
  UINTN         DevNumber;
  UINTN         FunctionNumber;
  PCI_TYPE00    PciDevice;
  UINTN         ResourceAddress;
  EFI_STATUS    Status;
  UINT8         SubBus;

  for (DevNumber = 0; DevNumber <= PCI_MAX_DEVICE; ++DevNumber) {
    for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; ++FunctionNumber) {
      PCIE_INFO ("Scanning device B: %02d D: %02d F: %02d\n",
                 BusNumber,
                 DevNumber,
                 FunctionNumber);

      Status = PcieDevicePresent (
                 &PciDevice,
                 BusNumber,
                 DevNumber,
                 FunctionNumber);
      if (Status == EFI_NOT_FOUND) {
        PCIE_INFO ("No Pci device found\n");
        Status = EFI_SUCCESS;
        goto Exit;
      } else if (EFI_ERROR (Status)) {
        PCIE_ERROR ("Error detecting Pci device\n");
        goto Exit;
      }

      Status = PcieConfigureDevice (
                 PciDevice,
                 BusNumber,
                 DevNumber,
                 FunctionNumber);
      if (EFI_ERROR (Status)) {
        PCIE_ERROR (
          "Failed to configure device B:%02d D:%02d F:%02d\n",
          BusNumber,
          DevNumber,
          FunctionNumber);
        continue;
      }

      if (IS_PCI_BRIDGE (&PciDevice)) {
        BusRegister = (UINT16) (((BusNumber + 1) << 8) | (UINT16)BusNumber);
        Status = PcieGetPciConfigAddress (
                   BusNumber,
                   DevNumber,
                   FunctionNumber,
                   0,
                   &BusBaseRegisterAddress);

        ASSERT_EFI_ERROR (Status);

        Status = PciePciWrite (
                   EfiPciWidthUint16,
                   BusBaseRegisterAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
                   1,
                   &BusRegister);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to update bridge bus number %d\n", BusNumber);
          continue;
        }

        // Temporarily set maximum subordinate bus number, although for now
        // only support 2 buses.
        SubBus = 0XFF;
        Status = PciePciWrite (
                   EfiPciWidthUint8,
                   BusBaseRegisterAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET,
                   1,
                   &SubBus);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to update bridge bus number %d\n", BusNumber);
          continue;
        }

        // Setup the memory base.
        Status = PcieGetMemoryBarResource (
                   0,
                   &BridgeMemoryBase,
                   TRUE);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to acquire BAR resource\n");
          goto Exit;
        }

        BridgeMemory = (BridgeMemoryBase >> 16) & 0xFFF0;

        Status = PciePciWrite (
                   EfiPciIoWidthUint32,
                   BusBaseRegisterAddress + 0x20,
                   1,
                   &BridgeMemory);
        ASSERT_EFI_ERROR (Status);

        Status = PcieSimpleScanBusAndAssignResource (
                   BusNumber + 1);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to scan new bus %d\n", BusNumber + 1);
          continue;
        }

        // Setup the memory limit.
        Status = PcieGetMemoryBarResource (
                   0,
                   &ResourceAddress,
                   TRUE);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to acquire BAR resource\n");
          goto Exit;
        }

        ASSERT (BridgeMemoryBase != ResourceAddress);

        // Per spec align address has to be 1MB boundary
        PcieGetAlignAddress (
          ResourceAddress,
          0x00100000,
          &BridgeMemoryLimit);
        ASSERT_EFI_ERROR (Status);

        BridgeMemory |= BridgeMemoryLimit;

        Status = PciePciWrite (
                   EfiPciIoWidthUint32,
                   BusBaseRegisterAddress + 0x20,
                   1,
                   &BridgeMemory);
        ASSERT_EFI_ERROR (Status);

        SubBus = (BusNumber + 1);
        Status = PciePciWrite (
                   EfiPciWidthUint8,
                   BusBaseRegisterAddress +
                   PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET,
                   1,
                   &SubBus);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR (
            "Failed to update subordinate bus number %d\n",
            BusNumber);
          continue;
        }

        // Claim any memory that is used for padding
        Status = PcieGetMemoryBarResource (
                   (BridgeMemoryLimit + 0x00100000) - ResourceAddress,
                   &ResourceAddress,
                   FALSE);
        if (EFI_ERROR (Status)) {
          PCIE_ERROR ("Failed to realign resource\n");
          goto Exit;
        }
      }

      // Skip sub functions, this is not a multi function device
      if (FunctionNumber == 0 && !IS_PCI_MULTI_FUNC (&PciDevice)) {
        FunctionNumber = PCI_MAX_FUNC;
      }
    }
  }

Exit:
  return Status;
}

EFI_STATUS
PcieInitialize (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTablePtr
  )
{
  EFI_STATUS  Status;

  Status = PcieSetupInitSetting ();
  if (EFI_ERROR (Status)) {
    // EFI_DEVICE_ERROR indicates that a bootloader has already setup the
    // Pcie controller. In this case just return success immediately
    if (Status == EFI_DEVICE_ERROR) {
      Status = EFI_SUCCESS;
      PCIE_WARNING ("Pcie already initialized\n");
      goto Exit;
    }

    PCIE_ERROR ("Failed to enable Pcie gates\n");
    goto Exit;
  }

  Status = PcieSetClockGate (IMX_CLOCK_GATE_STATE_ON);
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to enable Pcie gates\n");
    goto Exit;
  }

  Status = PcieVerifyClocks ();
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to verify Pcie clocks, not configured!\n");
    goto Exit;
  }

  Status = PcieSetPhyState (TRUE);
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to enable Pcie PHY\n");
    goto Exit;
  }

  // Very important to wait for Pcie PHY to settle here or the controller
  // behaviour becomes unpredictable.
  gBS->Stall (50000);

  PcieEnablePerstLine ();

  Status = PcieSetupPciBridge ();
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to setup Pci bridge\n");
    goto Exit;
  }

  Status = PcieSetLinkStatus (TRUE);
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to enable Pcie link\n");
    goto Exit;
  }

  Status = PcieWaitForLink ();
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Pci link never came up\n");
    goto Exit;
  }

  PcieSetupiAtuSettings ();

  // Start scanning from bus 0 onward
  Status = PcieSimpleScanBusAndAssignResource (0);
  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("PcieSimpleScanBusAndAssignResource failed %r\n", Status);
    goto Exit;
  }

#ifdef DEBUG
  volatile UINT32   *pPrintAddr;
  UINT32            PrintIndex;

  pPrintAddr = (UINT32 *)PCIE_HOST_CONFIG_BASE_REG;

  PCIE_INFO ("===============================\n");
  PCIE_INFO ("Host Device Configuration space\n");
  PCIE_INFO ("===============================\n");
  for (PrintIndex = 0; PrintIndex < 16; ++PrintIndex) {
    PCIE_INFO ("PCI [%02x] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
                PrintIndex * 16, \
                pPrintAddr[0], \
                pPrintAddr[1], \
                pPrintAddr[2], \
                pPrintAddr[3]);

    pPrintAddr += 4;
  }

  PCIE_INFO ("===============================\n");
  PCIE_INFO ("Device Configuration space 0x%08x\n", pPrintAddr);
  PCIE_INFO ("===============================\n");
  for (PrintIndex = 0; PrintIndex < 16; ++PrintIndex) {
    PCIE_INFO ("PCI [%02x] 0x%08x 0x%08x 0x%08x 0x%08x\n", \
                PrintIndex * 16, \
                pPrintAddr[0], \
                pPrintAddr[1], \
                pPrintAddr[2], \
                pPrintAddr[3]);

    pPrintAddr += 4;
  }
  PCIE_INFO ("===============================\n");
#endif

Exit:

  if (EFI_ERROR (Status)) {
    PCIE_ERROR ("Failed to initialize Pcie, disabling controller\n");
    PcieSetLinkStatus (FALSE);
    PcieSetPhyState (FALSE);
    PcieSetClockGate (IMX_CLOCK_GATE_STATE_OFF);
  }

  // For debug printout the state of the PLL/PHY
#ifdef DEBUG
  volatile IMX_CCM_ANALOG_REGISTERS   *pCcmAnalogRegs;
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIoMuxcRegs;

  pCcmAnalogRegs = (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;
  pIoMuxcRegs = (IMX_IOMUXC_GPR_REGISTERS *)IMX_IOMUXC_BASE;

  PCIE_INFO ( "IMX_CCM_PLL_ENET 0x%08X\n",
              MmioRead32 ((UINTN) &pCcmAnalogRegs->PLL_ENET));
  PCIE_INFO ( "IOMUXC_GPR1 0x%08X\n", MmioRead32 ((UINTN) &pIoMuxcRegs->GPR1));
#endif
  return Status;
}
