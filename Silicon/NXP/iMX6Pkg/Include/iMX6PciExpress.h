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

#pragma once

#ifndef ARRAYSIZE
#define ARRAYSIZE(__array__) (sizeof((__array__))/sizeof((__array__[0])))
#endif

// Print macro
#define PCIE_INIT(PRINT_OUT, ...) \
    DEBUG((DEBUG_INIT, "iMX6PCIe:" PRINT_OUT, ##__VA_ARGS__))
#define PCIE_INFO(PRINT_OUT, ...) \
    DEBUG((DEBUG_INFO, "iMX6PCIe:" PRINT_OUT, ##__VA_ARGS__))
#define PCIE_WARNING(PRINT_OUT, ...) \
    DEBUG((DEBUG_WARN, "iMX6PCIe:" PRINT_OUT, ##__VA_ARGS__))
#define PCIE_ERROR(PRINT_OUT, ...) \
    DEBUG((DEBUG_ERROR, "iMX6PCIe:" PRINT_OUT, ##__VA_ARGS__))

// PCIe related base address
#define PCIE_HOST_CONFIG_BASE_REG       FixedPcdGet32(PcdPcieHostConfigBase)
#define PCIE_CTRL_PORT_LOGIG_BASE_REG   0x01FFC700
#define PCIE_DEVICE_CONFIG_BASE_REG     FixedPcdGet32(PcdPcieDeviceConfigBase)
#define PCIE_DEVICE_CONFIG_SIZE         FixedPcdGet32(PcdPcieDeviceConfigSize)
#define PCIE_IO_SPACE_BASE              FixedPcdGet32(PcdPcieIOBase)
#define PCIE_IO_SPACE_SIZE              FixedPcdGet32(PcdPcieIOSize)
#define PCIE_MEMORY_SPACE_BASE          FixedPcdGet32(PcdPciMemoryBase)
#define PCIE_MEMORY_SPACE_SIZE          FixedPcdGet32(PcdPciMemorySize)
#define PCIE_PREFETCH_MEMORY_SPACE_BASE FixedPcdGet32(PcdPciPrefetchMemoryBase)
#define PCIE_PREFETCH_MEMORY_SPACE_SIZE FixedPcdGet32(PcdPciPrefetchMemorySize)

#pragma pack(push, 1)

typedef struct {
  UINT64 Base;
  UINT64 Size;
  UINT64 Curr;
} PCI_RESOURCE;

// Debug register related bits
#define PCIE_PL_DEBUG1_PHY_LINK_UP          (1 << 4)
#define PCIE_PL_DEBUG1_LINK_IN_TRAINING     (1 << 29)

// Address Translation Unit related definition
#define MAX_iATU_REGION          4

typedef enum _REGION_DIRECTION {
  OUTBOUND,
  INBOUND,
} REGION_DIRECTION;

typedef enum _TLP_TYPE {
  MEMORY_TYPE,
  MEMORY_LOCK_TYPE,
  IO_TYPE,
  CFG0_TYPE = 4,
  CFG1_TYPE = 5,
} TLP_TYPE;

typedef enum _REGION_STATE {
  REGION_DISABLE = 0,
  REGION_ENABLE = 0x80000000,
} REGION_STATE;

typedef struct {
  UINT32 PCIE_PL_ALTRTR;          // ACK latency timer and replay timer
  UINT32 PCIE_PL_VSDR;            // Vendor specific DLLP
  UINT32 PCIE_PL_PFLR;            // Port force link
  UINT32 PCIE_PL_AFLACR;          // ACK frequency and L0-L1 ASPM control
  UINT32 PCIE_PL_PLCR;            // Port link control
  UINT32 PCIE_PL_LSR;             // Lane skew
  UINT32 PCIE_PL_SNR;             // Symbol number
  UINT32 PCIE_PL_STRFM1;          // Symbol timer and filter mask 1
  UINT32 PCIE_PL_STRFM2;          // Filter mask 2
  UINT32 PCIE_PL_AMODNPSR;        // AMBA Multiple Outbound Decomposed NP Sub-Requests
                                  //   Control
  UINT32 PCIE_PL_DEBUG0;          // Debug 0
  UINT32 PCIE_PL_DEBUG1;          // Debug 1
  UINT32 PCIE_PL_TPFCSR;          // Transmit Posted FC Credit Status
  UINT32 PCIE_PL_TNFCSR;          // Transmit Non-Posted FC Credit Status
  UINT32 PCIE_PL_TCFCSR;          // Transmit Completion FC Credit Status
  UINT32 PCIE_PL_QSR;             // Queue status
  UINT32 PCIE_PL_VCTAR1;          // Transmit Completion FC Status 1
  UINT32 PCIE_PL_VCTAR2;          // Transmit Completion FC Status 1
  UINT32 PCIE_PL_VC0PRQC;         // VC0 Posted Receive Queue Control
  UINT32 PCIE_PL_VC0NRQC;         // VC0 Non-Posted Receive Queue Control
  UINT32 PCIE_PL_VC0CRQC;         // VC0 Completion Receive Queue Control
  UINT32 PCIE_PL_VCnPRQC;         // VCn Posted Receive Queue Control
  UINT32 PCIE_PL_VCnNRQC;         // VCn Non-Posted Receive Queue Control
  UINT32 PCIE_PL_VCnCRQC;         // VCn Completion Receive Queue Control
  UINT32 PCIE_PL_RESERVED_0[18];
  UINT32 PCIE_PL_VC0PBD;          // VC0 Posted Buffer Depth
  UINT32 PCIE_PL_VC0NPBD;         // VC0 Non-Posted Buffer Depth
  UINT32 PCIE_PL_VC0CBD;          // VC0 Completion Buffer Depth
  UINT32 PCIE_PL_VC1PBD;          // VCn Posted Buffer Depth
  UINT32 PCIE_PL_VC1NPBD;         // VCn Non-Posted Buffer Depth
  UINT32 PCIE_PL_VC1CBD;          // VCn Completion Buffer Depth
  UINT32 PCIE_PL_RESERVED_1[19];
  UINT32 PCIE_PL_G2CR;            // Gen2 Control
  UINT32 PCIE_PL_PHY_STATUS;      // PHY status
  UINT32 PCIE_PL_PHY_CTRL;        // PHY control
  UINT32 PCIE_PL_MRCCR0;          // Master Response Composer Control 0
  UINT32 PCIE_PL_MRCCR1;          // Master Response Composer Control 1
  UINT32 PCIE_PL_MSICA;           // MSI Controller Address
  UINT32 PCIE_PL_MSICUA;          // MSI Controller Upper Address
  UINT32 PCIE_PL_MSICIn_ENB;      // MSI Controller Interrupt n Enable
  UINT32 PCIE_PL_MSICIn_MASK;     // MSI Controller Interrupt n Mask
  UINT32 PCIE_PL_MSICIn_STATUS;   // MSI Controller Interrupt n Status
  UINT32 PCIE_PL_RESERVED_2[21];
  UINT32 PCIE_PL_MSICGPIO;        // MSI Controller General Purpose IO
  UINT32 PCIE_PL_RESERVED_3[29];
  UINT32 PCIE_PL_iATUVR;          // iATU Viewport
  UINT32 PCIE_PL_iATURC1;         // iATU Control 1
  UINT32 PCIE_PL_iATURC2;         // iATU Control 2
  UINT32 PCIE_PL_iATURLBA;        // iATU Region Lower Base Address
  UINT32 PCIE_PL_iATURUBA;        // iATU Region Upper Base Address
  UINT32 PCIE_PL_iATURLA;         // iATU Region Limit Address
  UINT32 PCIE_PL_iATURLTA;        // iATU Region Lower Target Address
  UINT32 PCIE_PL_iATURUTA;        // iATU Region Upper Target Address
} CSP_PCIE_PL_REGS, *PCSP_PCIE_PL_REGS;

typedef struct _IATU_SETTINGS {
  UINT32 RegionDirection;
  UINT32 RegionIndex;
  TLP_TYPE Type;
  UINT32 LowerBaseAddr;
  UINT32 UpperBaseAddr;
  UINT32 LimitAddr;
  UINT32 LowerTargetAddr;
  UINT32 UpperTargetAddr;
  UINT32 State;
} IATU_SETTINGS, *PIATU_SETTINGS;

#pragma pack(pop)
