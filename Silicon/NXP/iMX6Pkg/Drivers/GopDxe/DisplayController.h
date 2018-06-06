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

#ifndef _DISPLAY_CONTROLLER_H_
#define _DISPLAY_CONTROLLER_H_

#define IPU_DC_OFFSET 0x00058000

// DC Registers
#define IPU_DC_READ_CH_CONF_OFFSET              IPU_DC_OFFSET + 0x0000
#define IPU_DC_READ_CH_ADDR_OFFSET              IPU_DC_OFFSET + 0x0004
#define IPU_DC_RL0_CH_0_OFFSET                  IPU_DC_OFFSET + 0x0008
#define IPU_DC_RL1_CH_0_OFFSET                  IPU_DC_OFFSET + 0x000C
#define IPU_DC_RL2_CH_0_OFFSET                  IPU_DC_OFFSET + 0x0010
#define IPU_DC_RL3_CH_0_OFFSET                  IPU_DC_OFFSET + 0x0014
#define IPU_DC_RL4_CH_0_OFFSET                  IPU_DC_OFFSET + 0x0018
#define IPU_DC_WR_CH_CONF_1_OFFSET              IPU_DC_OFFSET + 0x001C
#define IPU_DC_WR_CH_ADDR_1_OFFSET              IPU_DC_OFFSET + 0x0020
#define IPU_DC_RL0_CH_1_OFFSET                  IPU_DC_OFFSET + 0x0024
#define IPU_DC_RL1_CH_1_OFFSET                  IPU_DC_OFFSET + 0x0028
#define IPU_DC_RL2_CH_1_OFFSET                  IPU_DC_OFFSET + 0x002C
#define IPU_DC_RL3_CH_1_OFFSET                  IPU_DC_OFFSET + 0x0030
#define IPU_DC_RL4_CH_1_OFFSET                  IPU_DC_OFFSET + 0x0034
#define IPU_DC_WR_CH_CONF_2_OFFSET              IPU_DC_OFFSET + 0x0038
#define IPU_DC_WR_CH_ADDR_2_OFFSET              IPU_DC_OFFSET + 0x003C
#define IPU_DC_RL0_CH_2_OFFSET                  IPU_DC_OFFSET + 0x0040
#define IPU_DC_RL1_CH_2_OFFSET                  IPU_DC_OFFSET + 0x0044
#define IPU_DC_RL2_CH_2_OFFSET                  IPU_DC_OFFSET + 0x0048
#define IPU_DC_RL3_CH_2_OFFSET                  IPU_DC_OFFSET + 0x004C
#define IPU_DC_RL4_CH_2_OFFSET                  IPU_DC_OFFSET + 0x0050
#define IPU_DC_CMD_CH_CONF_3_OFFSET             IPU_DC_OFFSET + 0x0054
#define IPU_DC_CMD_CH_CONF_4_OFFSET             IPU_DC_OFFSET + 0x0058
#define IPU_DC_WR_CH_CONF_5_OFFSET              IPU_DC_OFFSET + 0x005C
#define IPU_DC_WR_CH_ADDR_5_OFFSET              IPU_DC_OFFSET + 0x0060
#define IPU_DC_RL0_CH_5_OFFSET                  IPU_DC_OFFSET + 0x0064
#define IPU_DC_RL1_CH_5_OFFSET                  IPU_DC_OFFSET + 0x0068
#define IPU_DC_RL2_CH_5_OFFSET                  IPU_DC_OFFSET + 0x006C
#define IPU_DC_RL3_CH_5_OFFSET                  IPU_DC_OFFSET + 0x0070
#define IPU_DC_RL4_CH_5_OFFSET                  IPU_DC_OFFSET + 0x0074
#define IPU_DC_WR_CH_CONF_6_OFFSET              IPU_DC_OFFSET + 0x0078
#define IPU_DC_WR_CH_ADDR_6_OFFSET              IPU_DC_OFFSET + 0x007C
#define IPU_DC_RL0_CH_6_OFFSET                  IPU_DC_OFFSET + 0x0080
#define IPU_DC_RL1_CH_6_OFFSET                  IPU_DC_OFFSET + 0x0084
#define IPU_DC_RL2_CH_6_OFFSET                  IPU_DC_OFFSET + 0x0088
#define IPU_DC_RL3_CH_6_OFFSET                  IPU_DC_OFFSET + 0x008C
#define IPU_DC_RL4_CH_6_OFFSET                  IPU_DC_OFFSET + 0x0090
#define IPU_DC_WR_CH_CONF1_8_OFFSET             IPU_DC_OFFSET + 0x0094
#define IPU_DC_WR_CH_CONF2_8_OFFSET             IPU_DC_OFFSET + 0x0098
#define IPU_DC_RL1_CH_8_OFFSET                  IPU_DC_OFFSET + 0x009C
#define IPU_DC_RL2_CH_8_OFFSET                  IPU_DC_OFFSET + 0x00A0
#define IPU_DC_RL3_CH_8_OFFSET                  IPU_DC_OFFSET + 0x00A4
#define IPU_DC_RL4_CH_8_OFFSET                  IPU_DC_OFFSET + 0x00A8
#define IPU_DC_RL5_CH_8_OFFSET                  IPU_DC_OFFSET + 0x00AC
#define IPU_DC_RL6_CH_8_OFFSET                  IPU_DC_OFFSET + 0x00B0
#define IPU_DC_WR_CH_CONF1_9_OFFSET             IPU_DC_OFFSET + 0x00B4
#define IPU_DC_WR_CH_CONF2_9_OFFSET             IPU_DC_OFFSET + 0x00B8
#define IPU_DC_RL1_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00BC
#define IPU_DC_RL2_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00C0
#define IPU_DC_RL3_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00C4
#define IPU_DC_RL4_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00C8
#define IPU_DC_RL5_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00CC
#define IPU_DC_RL6_CH_9_OFFSET                  IPU_DC_OFFSET + 0x00D0
#define IPU_DC_GEN_OFFSET                       IPU_DC_OFFSET + 0x00D4
#define IPU_DC_DISP_CONF1_0_OFFSET              IPU_DC_OFFSET + 0x00D8
#define IPU_DC_DISP_CONF1_1_OFFSET              IPU_DC_OFFSET + 0x00DC
#define IPU_DC_DISP_CONF1_2_OFFSET              IPU_DC_OFFSET + 0x00E0
#define IPU_DC_DISP_CONF1_3_OFFSET              IPU_DC_OFFSET + 0x00E4
#define IPU_DC_DISP_CONF2_0_OFFSET              IPU_DC_OFFSET + 0x00E8
#define IPU_DC_DISP_CONF2_1_OFFSET              IPU_DC_OFFSET + 0x00EC
#define IPU_DC_DISP_CONF2_2_OFFSET              IPU_DC_OFFSET + 0x00F0
#define IPU_DC_DISP_CONF2_3_OFFSET              IPU_DC_OFFSET + 0x00F4
#define IPU_DC_DI0_CONF1_OFFSET                 IPU_DC_OFFSET + 0x00F8
#define IPU_DC_DI0_CONF2_OFFSET                 IPU_DC_OFFSET + 0x00FC
#define IPU_DC_DI1_CONF1_OFFSET                 IPU_DC_OFFSET + 0x0100
#define IPU_DC_DI1_CONF2_OFFSET                 IPU_DC_OFFSET + 0x0104
#define IPU_DC_MAP_CONF_0_OFFSET                IPU_DC_OFFSET + 0x0108
#define IPU_DC_MAP_CONF_1_OFFSET                IPU_DC_OFFSET + 0x010C
#define IPU_DC_MAP_CONF_2_OFFSET                IPU_DC_OFFSET + 0x0110
#define IPU_DC_MAP_CONF_3_OFFSET                IPU_DC_OFFSET + 0x0114
#define IPU_DC_MAP_CONF_4_OFFSET                IPU_DC_OFFSET + 0x0118
#define IPU_DC_MAP_CONF_5_OFFSET                IPU_DC_OFFSET + 0x011C
#define IPU_DC_MAP_CONF_6_OFFSET                IPU_DC_OFFSET + 0x0120
#define IPU_DC_MAP_CONF_7_OFFSET                IPU_DC_OFFSET + 0x0124
#define IPU_DC_MAP_CONF_8_OFFSET                IPU_DC_OFFSET + 0x0128
#define IPU_DC_MAP_CONF_9_OFFSET                IPU_DC_OFFSET + 0x012C
#define IPU_DC_MAP_CONF_10_OFFSET               IPU_DC_OFFSET + 0x0130
#define IPU_DC_MAP_CONF_11_OFFSET               IPU_DC_OFFSET + 0x0134
#define IPU_DC_MAP_CONF_12_OFFSET               IPU_DC_OFFSET + 0x0138
#define IPU_DC_MAP_CONF_13_OFFSET               IPU_DC_OFFSET + 0x013C
#define IPU_DC_MAP_CONF_14_OFFSET               IPU_DC_OFFSET + 0x0140
#define IPU_DC_MAP_CONF_15_OFFSET               IPU_DC_OFFSET + 0x0144
#define IPU_DC_MAP_CONF_16_OFFSET               IPU_DC_OFFSET + 0x0148
#define IPU_DC_MAP_CONF_17_OFFSET               IPU_DC_OFFSET + 0x014C
#define IPU_DC_MAP_CONF_18_OFFSET               IPU_DC_OFFSET + 0x0150
#define IPU_DC_MAP_CONF_19_OFFSET               IPU_DC_OFFSET + 0x0154
#define IPU_DC_MAP_CONF_20_OFFSET               IPU_DC_OFFSET + 0x0158
#define IPU_DC_MAP_CONF_21_OFFSET               IPU_DC_OFFSET + 0x015C
#define IPU_DC_MAP_CONF_22_OFFSET               IPU_DC_OFFSET + 0x0160
#define IPU_DC_MAP_CONF_23_OFFSET               IPU_DC_OFFSET + 0x0164
#define IPU_DC_MAP_CONF_24_OFFSET               IPU_DC_OFFSET + 0x0168
#define IPU_DC_MAP_CONF_25_OFFSET               IPU_DC_OFFSET + 0x016C
#define IPU_DC_MAP_CONF_26_OFFSET               IPU_DC_OFFSET + 0x0170
#define IPU_DC_UGDE0_0_OFFSET                   IPU_DC_OFFSET + 0x0174
#define IPU_DC_UGDE0_1_OFFSET                   IPU_DC_OFFSET + 0x0178
#define IPU_DC_UGDE0_2_OFFSET                   IPU_DC_OFFSET + 0x017C
#define IPU_DC_UGDE0_3_OFFSET                   IPU_DC_OFFSET + 0x0180
#define IPU_DC_UGDE1_0_OFFSET                   IPU_DC_OFFSET + 0x0184
#define IPU_DC_UGDE1_1_OFFSET                   IPU_DC_OFFSET + 0x0188
#define IPU_DC_UGDE1_2_OFFSET                   IPU_DC_OFFSET + 0x018C
#define IPU_DC_UGDE1_3_OFFSET                   IPU_DC_OFFSET + 0x0190
#define IPU_DC_UGDE2_0_OFFSET                   IPU_DC_OFFSET + 0x0194
#define IPU_DC_UGDE2_1_OFFSET                   IPU_DC_OFFSET + 0x0198
#define IPU_DC_UGDE2_2_OFFSET                   IPU_DC_OFFSET + 0x019C
#define IPU_DC_UGDE2_3_OFFSET                   IPU_DC_OFFSET + 0x01A0
#define IPU_DC_UGDE3_0_OFFSET                   IPU_DC_OFFSET + 0x01A4
#define IPU_DC_UGDE3_1_OFFSET                   IPU_DC_OFFSET + 0x01A8
#define IPU_DC_UGDE3_2_OFFSET                   IPU_DC_OFFSET + 0x01AC
#define IPU_DC_UGDE3_3_OFFSET                   IPU_DC_OFFSET + 0x01B0
#define IPU_DC_LLA0_OFFSET                      IPU_DC_OFFSET + 0x01B4
#define IPU_DC_LLA1_OFFSET                      IPU_DC_OFFSET + 0x01B8
#define IPU_DC_R_LLA0_OFFSET                    IPU_DC_OFFSET + 0x01BC
#define IPU_DC_R_LLA1_OFFSET                    IPU_DC_OFFSET + 0x01C0
#define IPU_DC_WR_CH_ADDR_5_ALT_OFFSET          IPU_DC_OFFSET + 0x01C4
#define IPU_DC_STAT_OFFSET                      IPU_DC_OFFSET + 0x01C8

// Microcode template
#define IPU_DC_TEMPLATE_REGS_ADDR_OFFSET        0x00180000

typedef enum {
  DC_CHANNEL_READ = 0,
  DC_CHANNEL_DC_SYNC_ASYNC = 1,
  DC_CHANNEL_DC_ASYNC = 2,
  DC_CHANNEL_DP_MAIN = 5,
  DC_CHANNEL_DP_SECONDARY = 6,
} DC_CHANNEL;

#pragma pack(push, 1)

// IPU_DC_WR_CH_CONF_1 0x1C
// IPU_DC_WR_CH_CONF_5 0x5C
typedef union {
  struct {
    UINT32 W_SIZE : 2;
    UINT32 PROG_DI_ID : 1;
    UINT32 PROG_DISP_ID : 2;
    UINT32 PROG_CHAN_TYP : 3;
    UINT32 CHAN_MASK_DEFAULT : 1;
    UINT32 FILED_MODE : 1;
    UINT32 Reserved1 : 6;
    UINT32 PROG_START_TIME : 11;
    UINT32 Reserved2 : 5;
  };
  UINT32 Reg;
} IPU_DC_WR_CH_CONF_1_REG, IPU_DC_WR_CH_CONF_5_REG;

typedef enum {
  PROG_CHAN_TYP_DISABLED,
  PROG_CHAN_TYP_RESERVED1,
  PROG_CHAN_TYP_NORMAL = 4,
  PROG_CHAN_TYP_NORMAL_ANTI_TEARING,
  PROG_CHAN_TYP_RESERVED2,
} PROG_CHAN_TYP;

// IPU_DC_GEN 0xD4
typedef union {
  struct {
    UINT32 Reserved1 : 1;
    UINT32 Sync_1_6 : 2;
    UINT32 Reserved2 : 1;
    UINT32 MASK_EN : 1;
    UINT32 MASK4CHAN_5 : 1;
    UINT32 SYNC_PRIORITY_5 : 1;
    UINT32 SYNC_PRIORITY_1 : 1;
    UINT32 DC_CH5_TYPE : 1;
    UINT32 Reserved3 : 7;
    UINT32 DC_BKDIV : 8;
    UINT32 DC_BK_EN : 1;
    UINT32 Reserved4 : 7;
  };
  UINT32 Reg;
} IPUx_IPU_DC_GEN_REG;

// IPU_DC_RL0_CH_5 0x0064
typedef union {
  struct {
    UINT32 COD_NF_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved1 : 4;
    UINT32 COD_NF_START_CHAN_5 : 8;
    UINT32 COD_NL_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved2 : 4;
    UINT32 COD_NL_START_CHAN_5 : 24;
  };
  UINT32 Reg;
} IPU_DC_RL0_CH_5_REG;

// IPU_DC_RL1_CH_5 0x006C
typedef union {
  struct {
    UINT32 COD_EOF_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved1 : 4;
    UINT32 COD_EOF_START_CHAN_5 : 8;
    UINT32 COD_NFIELD_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved2 : 4;
    UINT32 COD_NFIELD_START_CHAN_5 : 24;
  };
  UINT32 Reg;
} IPU_DC_RL1_CH_5_REG;

// IPU_DC_RL2_CH_5 0x0068
typedef union {
  struct {
    UINT32 COD_EOL_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved1 : 4;
    UINT32 COD_EOL_START_CHAN_5 : 8;
    UINT32 COD_EOFIELD_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved2 : 4;
    UINT32 COD_EOFIELD_START_CHAN_5 : 24;
  };
  UINT32 Reg;
} IPU_DC_RL2_CH_5_REG;

// IPU_DC_RL3_CH_5 0x0070
typedef union {
  struct {
    UINT32 COD_NEW_ADDR_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved1 : 4;
    UINT32 COD_NEW_ADDR_START_CHAN_5 : 8;
    UINT32 COD_NEW_CHAN_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved2 : 4;
    UINT32 COD_NEW_CHAN_START_CHAN_5 : 24;
  };
  UINT32 Reg;
} IPU_DC_RL3_CH_5_REG;

// IPU_DC_RL4_CH_5 0x0074
typedef union {
  struct {
    UINT32 COD_NEW_DATA_PRIORITY_CHAN_5 : 4;
    UINT32 Reserved1 : 4;
    UINT32 COD_NEW_DATA_START_CHAN_5 : 8;
    UINT32 Reserved2 : 16;
  };
  UINT32 Reg;
} IPU_DC_RL4_CH_5_REG;

// DC_DISP_CONF1 0xD8 - 0xE4
typedef union {
  struct {
    UINT32 DISP_TYP : 2;
    UINT32 ADDR_INCREMENT : 2;
    UINT32 ADDR_BE_L_INC : 2;
    UINT32 MCU_ACC_LB_MASK_3 : 1;
    UINT32 DISP_RD_VALUE_PTR : 1;
    UINT32 Reserved : 24;
  };
  UINT32 Reg;
} IPUx_DC_DISP_CONF1_REG;

// DC_MAP_CONF_MAP_OFFSET 0x0108 - 0x0140
typedef union {
  struct {
    UINT32 MAPPING_PNTR_BYTE0_X : 5;
    UINT32 MAPPING_PNTR_BYTE1_X : 5;
    UINT32 MAPPING_PNTR_BYTE2_X : 5;
    UINT32 Reserved1 : 1;
    UINT32 MAPPING_PNTR_BYTE0_Y : 5;
    UINT32 MAPPING_PNTR_BYTE1_Y : 5;
    UINT32 MAPPING_PNTR_BYTE2_Y : 5;
    UINT32 Reserved2 : 1;
  };
  UINT32 Reg;
} IPUx_DC_MAP_CONF_MAP_REG;

// DC_MAP_CONF_OFFSET_MASK_OFFSET 0x0144 - 0x0170
typedef union {
  struct {
    UINT32 MD_MASK_X : 8;
    UINT32 MD_OFFSET_X : 5;
    UINT32 Reserved1 : 3;
    UINT32 MD_MASK_Y : 8;
    UINT32 MD_OFFSET_Y : 5;
    UINT32 Reserved2 : 3;
  };
  UINT32 Reg;
} DC_MAP_CONF_OFFSET_MASK_REG;

typedef union {
  struct {
    UINT32 SYNC : 4;
    UINT32 GLUELOGIC : 7;
    UINT32 WAVEFORM : 4;
    UINT32 MAPPING : 5;
    UINT32 DATA : 16;
    UINT32 OPCODE : 5;
    UINT32 STOP : 1;
    UINT32 Unused : 22;
  };
  struct     {
    UINT32 LowWord;
    UINT32 HighWord;
  };
} DISPLAY_CONTROLLER_WROD_COMMAND;

#pragma pack(pop)

EFI_STATUS
SetDisplayControllerChannelState (
  IN  VOID *IpuMmioBasePtr,
  IN  PROG_CHAN_TYP ChannelType
  );

EFI_STATUS
ConfigureDisplayControllerChannel (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  DISPLAY_INTERFACE_TYPE      DisplayInterface,
  IN  UINT32                      DisplayIndex,
  IN  IMX_DISPLAY_TIMING          *DisplayTimingPtr
  );

#endif  /* _DISPLAY_CONTROLLER_H_ */
