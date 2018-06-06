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

#ifndef __IMX_USDHC_H__
#define __IMX_USDHC_H__

//
// uSDHCx Registers Layout
//
typedef struct {
  UINT32 DS_ADDR;
  UINT32 BLK_ATT;
  UINT32 CMD_ARG;
  UINT32 CMD_XFR_TYP;
  UINT32 CMD_RSP0;
  UINT32 CMD_RSP1;
  UINT32 CMD_RSP2;
  UINT32 CMD_RSP3;
  UINT32 DATA_BUFF_ACC_PORT;
  UINT32 PRES_STATE;
  UINT32 PROT_CTRL;
  UINT32 SYS_CTRL;
  UINT32 INT_STATUS;
  UINT32 INT_STATUS_EN;
  UINT32 INT_SIGNAL_EN;
  UINT32 AUTOCMD12_ERR_STATUS;
  UINT32 HOST_CTRL_CAP;
  UINT32 WTMK_LVL;
  UINT32 MIX_CTRL;
  UINT32 _pad0;
  UINT32 FORCE_EVENT;
  UINT32 ADMA_ERR_STATUS;
  UINT32 ADMA_SYS_ADDR;
  UINT32 _pad1;
  UINT32 DLL_CTRL;
  UINT32 DLL_STATUS;
  UINT32 CLK_TUNE_CTRL_STATUS;
  UINT32 _pad2[21];
  UINT32 VEND_SPEC;
  UINT32 MMC_BOOT;
  UINT32 VEND_SPEC2;
} USDHC_REGISTERS;

//
// Block Attributes uSDHCx_BLK_ATT fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 BLKSIZE    : 13; // 0:12
    UINT32 _reserved0 : 3;  // 13:15
    UINT32 BLKCNT     : 16; // 16:31
  } Fields;
} USDHC_BLK_ATT_REG;

//
// Command Transfer Type uSDHCx_CMD_XFR_TYP fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 _reserved0   : 16; // 0:15
    UINT32 RSPTYP       : 2; // 16:17
    UINT32 _reserved1   : 1; // 18
    UINT32 CCCEN        : 1; // 19
    UINT32 CICEN        : 1; // 20
    UINT32 DPSEL        : 1; // 21
    UINT32 CMDTYP       : 2; // 22:23
    UINT32 CMDINX       : 6; // 24:29
    UINT32 _reserved2   : 2; // 30:31
  } Fields;
} USDHC_CMD_XFR_TYP_REG;

#define USDHC_CMD_XFR_TYP_RSPTYP_NO_RSP          0
#define USDHC_CMD_XFR_TYP_RSPTYP_RSP_136         1
#define USDHC_CMD_XFR_TYP_RSPTYP_RSP_48          2
#define USDHC_CMD_XFR_TYP_RSPTYP_RSP_48_CHK_BSY  3
#define USDHC_CMD_XFR_TYP_CMDTYP_ABORT           3

//
// System Control uSDHCx_SYS_CTRL fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 _reserved0   : 4; // 0:3
    UINT32 DVS          : 4; // 4:7
    UINT32 SDCLKFS      : 8; // 8:15
    UINT32 DTOCV        : 4; // 16:19
    UINT32 _reserved1   : 3; // 20:22
    UINT32 IPP_RST_N    : 1; // 23
    UINT32 RSTA         : 1; // 24
    UINT32 RSTC         : 1; // 25
    UINT32 RSTD         : 1; // 26
    UINT32 INITA        : 1; // 27
    UINT32 _reserved2   : 4; // 28-31
  } Fields;
} USDHC_SYS_CTRL_REG;

//
// Host Controller Capabilities Register uSDHCx_HOST_CTRL_CAP fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 _reserved0       : 16; // 0:15
    UINT32 MBL              : 3; // 16:18
    UINT32 _reserved1       : 1; // 19
    UINT32 ADMAS            : 1; // 20
    UINT32 HSS              : 1; // 21
    UINT32 DMAS             : 1; // 22
    UINT32 SRS              : 1; // 23
    UINT32 VS33             : 1; // 24
    UINT32 VS30             : 1; // 25
    UINT32 VS18             : 1; // 26
    UINT32 _reserved2       : 5; // 27:31
  } Fields;
} USDHC_HOST_CTRL_CAP_REG;

//
// Watermark Level uSDHCx_WTMK_LVL
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 RD_WML           : 8; // 0:7
    UINT32 RD_BRST_LEN      : 5; // 8:12
    UINT32 _reserved0       : 3; // 13:15
    UINT32 WR_WML           : 8; // 16:23
    UINT32 WR_BRST_LEN      : 5; // 24:28
    UINT32 _reserved1       : 3; // 29:31
  } Fields;
} USDHC_WTMK_LVL_REG;

#define USDHC_WTMK_RD_WML_MAX_VAL   0x10
#define USDHC_WTMK_WR_WML_MAX_VAL   0x80

//
// Mixer Control uSDHCx_MIX_CTRL fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DMAEN          : 1; // 0
    UINT32 BCEN           : 1; // 1
    UINT32 AC12EN         : 1; // 2
    UINT32 DDR_EN         : 1; // 3
    UINT32 DTDSEL         : 1; // 4
    UINT32 MSBSEL         : 1; // 5
    UINT32 NIBBLE_POS     : 1; // 6
    UINT32 AC23EN         : 1; // 7
    UINT32 _reserved0     : 14; // 8:21
    UINT32 EXE_TUNE       : 1; // 22
    UINT32 SMP_CLK_SEL    : 1; // 23
    UINT32 AUTO_TUNE_EN   : 1; //24
    UINT32 FBCLK_SEL      : 1; // 25
    UINT32 _reserved1     : 6; // 26-31
  } Fields;
} USDHC_MIX_CTRL_REG;

//
// Present State uSDHCx_PRES_STATE fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 CIHB         : 1; // 0
    UINT32 CDIHB        : 1; // 1
    UINT32 DLA          : 1; // 2
    UINT32 SDSTB        : 1; // 3
    UINT32 IPGOFF       : 1; // 4
    UINT32 HCKOFF       : 1; // 5
    UINT32 PEROFF       : 1; // 6
    UINT32 SDOFF        : 1; // 7
    UINT32 WTA          : 1; // 8
    UINT32 RTA          : 1; // 9
    UINT32 BWEN         : 1; // 10
    UINT32 BREN         : 1; // 11
    UINT32 PTR          : 1; // 12
    UINT32 _reserved0   : 3; // 13:15
    UINT32 CINST        : 1; // 16
    UINT32 _reserved1   : 1; // 17
    UINT32 CDPL         : 1; // 18
    UINT32 WPSPL        : 1; // 19
    UINT32 _reserved2   : 3; // 20:22
    UINT32 CLSL         : 1; // 23
    UINT32 DLSL         : 8; // 24:31
  } Fields;
} USDHC_PRES_STATE_REG;

//
// Present State uSDHCx_PROT_CTRL fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 LCTL             : 1; // 0
    UINT32 DTW              : 2; // 1:2
    UINT32 D3CD             : 1; // 3
    UINT32 EMODE            : 2; // 4:5
    UINT32 CDTL             : 1; // 6
    UINT32 CDSS             : 1; // 7
    UINT32 DMASEL           : 2; // 8:9
    UINT32 _reserved0       : 6; // 10:15
    UINT32 SABGREQ          : 1; // 16
    UINT32 CREQ             : 1; // 17
    UINT32 RWCTL            : 1; // 18
    UINT32 IABG             : 1; // 19
    UINT32 RD_DONE_NO_8CLK  : 1; // 20
    UINT32 _reserved1       : 3; // 21:23
    UINT32 WECINT           : 1; // 24
    UINT32 WECINS           : 1; // 25
    UINT32 WECRM            : 1; // 26
    UINT32 BURST_LEN_EN     : 3; // 27:29
    UINT32 NON_EXACT_BLK_RD : 1; // 30
    UINT32 _reserved2       : 1; // 31
  } Fields;
} USDHC_PROT_CTRL_REG;

#define USDHC_PROT_CTRL_DTW_1BIT             0x0
#define USDHC_PROT_CTRL_DTW_4BIT             0x1
#define USDHC_PROT_CTRL_DTW_8BIT             0x2
#define USDHC_PROT_CTRL_EMODE_LITTLE_ENDIAN  0x2

//
// Interrupt Status uSDHCx_INT_STATUS fields
//
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 CC           : 1; // 0
    UINT32 TC           : 1; // 1
    UINT32 BGE          : 1; // 2
    UINT32 DINT         : 1; // 3
    UINT32 BWR          : 1; // 4
    UINT32 BRR          : 1; // 5
    UINT32 CINS         : 1; // 6
    UINT32 CRM          : 1; // 7
    UINT32 CINT         : 1; // 8
    UINT32 _reserved0   : 3; // 9:11
    UINT32 RTE          : 1; // 12
    UINT32 _reserved1   : 1; // 13
    UINT32 TP           : 1; // 14
    UINT32 _reserved2   : 1; // 15
    UINT32 CTOE         : 1; // 16
    UINT32 CCE          : 1; // 17
    UINT32 CEBE         : 1; // 18
    UINT32 CIE          : 1; // 19
    UINT32 DTOE         : 1; // 20
    UINT32 DCE          : 1; // 21
    UINT32 DEBE         : 1; // 22
    UINT32 _reserved3   : 1; // 23
    UINT32 AC12E        : 1; // 24
    UINT32 _reserved4   : 1; // 25
    UINT32 TNE          : 1; // 26
    UINT32 _reserved5   : 1; // 27
    UINT32 DMAE         : 1; // 28
    UINT32 _reserved6   : 3; // 29:31
  } Fields;
} USDHC_INT_STATUS_REG;

#define USDHC_INT_STATUS_CMD_ERROR   (BIT16 | BIT17 | BIT18 | BIT19)
#define USDHC_INT_STATUS_DATA_ERROR  (BIT20 | BIT21 | BIT22)
#define USDHC_INT_STATUS_ERROR       (USDHC_INT_STATUS_CMD_ERROR | USDHC_INT_STATUS_DATA_ERROR)

#endif // __IMX_USDHC_H__
