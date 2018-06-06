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

#ifndef _HDMI_H_
#define _HDMI_H_

// HDMI Register Base Address
#define HDMI_BASE 0x00120000

// Interrupt Register Offset
#define HDMI_IH_FC_STAT0                0x0100
#define HDMI_IH_FC_STAT1                0x0101
#define HDMI_IH_FC_STAT2                0x0102
#define HDMI_IH_AS_STAT0                0x0103
#define HDMI_IH_PHY_STAT0               0x0104
#define HDMI_IH_I2CM_STAT0              0x0105
#define HDMI_IH_CEC_STAT0               0x0106
#define HDMI_IH_VP_STAT0                0x0107
#define HDMI_IH_I2CMPHY_STAT0           0x0108
#define HDMI_IH_AHBDMAAUD_STAT0         0x0180
#define HDMI_IH_MUTE_FC_STAT1           0x0181
#define HDMI_IH_MUTE_FC_STAT2           0x0182
#define HDMI_IH_MUTE_AS_STAT0           0x0183
#define HDMI_IH_MUTE_PHY_STAT0          0x0184
#define HDMI_IH_MUTE_I2CM_STAT0         0x0185
#define HDMI_IH_MUTE_CEC_STAT0          0x0186
#define HDMI_IH_MUTE_VP_STAT0           0x0187
#define HDMI_IH_MUTE_I2CMPHY_STAT0      0x0188
#define HDMI_IH_MUTE_AHBDMAAUD_STAT0    0x0189
#define HDMI_IH_MUTE                    0x01FF
#define HDMI_FC_INVIDCONF               0x1000
#define HDMI_FC_INHACTV0                0x1001
#define HDMI_FC_INHACTV1                0x1002
#define HDMI_FC_INHBLANK0               0x1003
#define HDMI_FC_INHBLANK1               0x1004
#define HDMI_FC_INVACTV0                0x1005
#define HDMI_FC_INVACTV1                0x1006
#define HDMI_FC_INVBLANK                0x1007
#define HDMI_FC_HSYNCINDELAY0           0x1008
#define HDMI_FC_HSYNCINDELAY1           0x1009
#define HDMI_FC_HSYNCINWIDTH0           0x100A
#define HDMI_FC_HSYNCINWIDTH1           0x100B
#define HDMI_FC_VSYNCINDELAY            0x100C
#define HDMI_FC_VSYNCINWIDTH            0x100D
#define HDMI_FC_INFREQ0                 0x100E
#define HDMI_FC_INFREQ1                 0x100F
#define HDMI_FC_INFREQ2                 0x1010
#define HDMI_FC_CTRLDUR                 0x1011
#define HDMI_FC_EXCTRLDUR               0x1012
#define HDMI_FC_EXCTRLSPAC              0x1013
#define HDMI_FC_CH0PREAM                0x1014
#define HDMI_FC_CH1PREAM                0x1015
#define HDMI_FC_CH2PREAM                0x1016
#define HDMI_FC_AVICONF3                0x1017
#define HDMI_FC_GCP                     0x1018
#define HDMI_FC_AVICONF0                0x1019
#define HDMI_FC_AVICONF1                0x101A
#define HDMI_FC_AVICONF2                0x101B
#define HDMI_FC_AVIVID                  0x101C
#define HDMI_FC_AVIETB0                 0x101D
#define HDMI_FC_AVIETB1                 0x101E
#define HDMI_FC_AVISBB0                 0x101F
#define HDMI_FC_AVISBB1                 0x1020
#define HDMI_FC_AVIELB0                 0x1021
#define HDMI_FC_AVIELB1                 0x1022
#define HDMI_FC_AVISRB0                 0x1023
#define HDMI_FC_AVISRB1                 0x1024
#define HDMI_FC_AUDICONF0               0x1025
#define HDMI_FC_AUDICONF1               0x1026
#define HDMI_FC_AUDICONF2               0x1027
#define HDMI_FC_AUDICONF3               0x1028
#define HDMI_FC_VSDIEEEID0              0x1029
#define HDMI_FC_VSDSIZE                 0x102A
#define HDMI_FC_VSDIEEEID1              0x1030
#define HDMI_FC_VSDIEEEID2              0x1031
#define HDMI_FC_VSDPAYLOAD0             0x1032
#define HDMI_FC_VSDPAYLOAD1             0x1033
#define HDMI_FC_VSDPAYLOAD2             0x1034
#define HDMI_FC_VSDPAYLOAD3             0x1035
#define HDMI_FC_VSDPAYLOAD4             0x1036
#define HDMI_FC_VSDPAYLOAD5             0x1037
#define HDMI_FC_VSDPAYLOAD6             0x1038
#define HDMI_FC_VSDPAYLOAD7             0x1039
#define HDMI_FC_VSDPAYLOAD8             0x103A
#define HDMI_FC_VSDPAYLOAD9             0x103B
#define HDMI_FC_VSDPAYLOAD10            0x103C
#define HDMI_FC_VSDPAYLOAD11            0x103D
#define HDMI_FC_VSDPAYLOAD12            0x103E
#define HDMI_FC_VSDPAYLOAD13            0x103F
#define HDMI_FC_VSDPAYLOAD14            0x1040
#define HDMI_FC_VSDPAYLOAD15            0x1041
#define HDMI_FC_VSDPAYLOAD16            0x1042
#define HDMI_FC_VSDPAYLOAD17            0x1043
#define HDMI_FC_VSDPAYLOAD18            0x1044
#define HDMI_FC_VSDPAYLOAD19            0x1045
#define HDMI_FC_VSDPAYLOAD20            0x1046
#define HDMI_FC_VSDPAYLOAD21            0x1047
#define HDMI_FC_VSDPAYLOAD22            0x1048
#define HDMI_FC_VSDPAYLOAD23            0x1049
#define HDMI_FC_SPDVENDORNAME0          0x104A
#define HDMI_FC_SPDVENDORNAME1          0x104B
#define HDMI_FC_SPDVENDORNAME2          0x104C
#define HDMI_FC_SPDVENDORNAME3          0x104D
#define HDMI_FC_SPDVENDORNAME4          0x104E
#define HDMI_FC_SPDVENDORNAME5          0x104F
#define HDMI_FC_SPDVENDORNAME6          0x1050
#define HDMI_FC_SPDVENDORNAME7          0x1051
#define HDMI_FC_SDPPRODUCTNAME0         0x1052
#define HDMI_FC_SDPPRODUCTNAME1         0x1053
#define HDMI_FC_SDPPRODUCTNAME2         0x1054
#define HDMI_FC_SDPPRODUCTNAME3         0x1055
#define HDMI_FC_SDPPRODUCTNAME4         0x1056
#define HDMI_FC_SDPPRODUCTNAME5         0x1057
#define HDMI_FC_SDPPRODUCTNAME6         0x1058
#define HDMI_FC_SDPPRODUCTNAME7         0x1059
#define HDMI_FC_SDPPRODUCTNAME8         0x105A
#define HDMI_FC_SDPPRODUCTNAME9         0x105B
#define HDMI_FC_SDPPRODUCTNAME10        0x105C
#define HDMI_FC_SDPPRODUCTNAME11        0x105D
#define HDMI_FC_SDPPRODUCTNAME12        0x105E
#define HDMI_FC_SDPPRODUCTNAME13        0x105F
#define HDMI_FC_SDPPRODUCTNAME14        0x1060
#define HDMI_FC_SPDPRODUCTNAME15        0x1061
#define HDMI_FC_SPDDEVICEINF            0x1062
#define HDMI_FC_AUDSCONF                0x1063
#define HDMI_FC_AUDSSTAT                0x1064
#define HDMI_FC_AUDSV                   0x1065
#define HDMI_FC_AUDSU                   0x1066
#define HDMI_FC_AUDSCHNLS0              0x1067
#define HDMI_FC_AUDSCHNLS1              0x1068
#define HDMI_FC_AUDSCHNLS2              0x1069
#define HDMI_FC_AUDSCHNLS3              0x106A
#define HDMI_FC_AUDSCHNLS4              0x106B
#define HDMI_FC_AUDSCHNLS5              0x106C
#define HDMI_FC_AUDSCHNLS6              0x106D
#define HDMI_FC_AUDSCHNLS7              0x106E
#define HDMI_FC_AUDSCHNLS8              0x106F
#define HDMI_FC_DATACH0FILL             0x1070
#define HDMI_FC_DATACH1FILL             0x1071
#define HDMI_FC_DATACH2FILL             0x1072
#define HDMI_FC_CTRLQHIGH               0x1073
#define HDMI_FC_CTRLQLOW                0x1074
#define HDMI_FC_ACP0                    0x1075
#define HDMI_FC_ACP28                   0x1076
#define HDMI_FC_ACP27                   0x1077
#define HDMI_FC_ACP26                   0x1078
#define HDMI_FC_ACP25                   0x1079
#define HDMI_FC_ACP24                   0x107A
#define HDMI_FC_ACP23                   0x107B
#define HDMI_FC_ACP22                   0x107C
#define HDMI_FC_ACP21                   0x107D
#define HDMI_FC_ACP20                   0x107E
#define HDMI_FC_ACP19                   0x107F
#define HDMI_FC_ACP18                   0x1080
#define HDMI_FC_ACP17                   0x1081
#define HDMI_FC_ACP16                   0x1082
#define HDMI_FC_ACP15                   0x1083
#define HDMI_FC_ACP14                   0x1084
#define HDMI_FC_ACP13                   0x1085
#define HDMI_FC_ACP12                   0x1086
#define HDMI_FC_ACP11                   0x1087
#define HDMI_FC_ACP10                   0x1088
#define HDMI_FC_ACP9                    0x1089
#define HDMI_FC_ACP8                    0x108A
#define HDMI_FC_ACP7                    0x108B
#define HDMI_FC_ACP6                    0x108C
#define HDMI_FC_ACP5                    0x108D
#define HDMI_FC_ACP4                    0x108E
#define HDMI_FC_ACP3                    0x108F
#define HDMI_FC_ACP2                    0x1090
#define HDMI_FC_ACP1                    0x1091
#define HDMI_FC_ISCR1_0                 0x1092
#define HDMI_FC_ISCR1_16                0x1093
#define HDMI_FC_ISCR1_15                0x1094
#define HDMI_FC_ISCR1_14                0x1095
#define HDMI_FC_ISCR1_13                0x1096
#define HDMI_FC_ISCR1_12                0x1097
#define HDMI_FC_ISCR1_11                0x1098
#define HDMI_FC_ISCR1_10                0x1099
#define HDMI_FC_ISCR1_9                 0x109A
#define HDMI_FC_ISCR1_8                 0x109B
#define HDMI_FC_ISCR1_7                 0x109C
#define HDMI_FC_ISCR1_6                 0x109D
#define HDMI_FC_ISCR1_5                 0x109E
#define HDMI_FC_ISCR1_4                 0x109F
#define HDMI_FC_ISCR1_3                 0x10A0
#define HDMI_FC_ISCR1_2                 0x10A1
#define HDMI_FC_ISCR1_1                 0x10A2
#define HDMI_FC_ISCR2_15                0x10A3
#define HDMI_FC_ISCR2_14                0x10A4
#define HDMI_FC_ISCR2_13                0x10A5
#define HDMI_FC_ISCR2_12                0x10A6
#define HDMI_FC_ISCR2_11                0x10A7
#define HDMI_FC_ISCR2_10                0x10A8
#define HDMI_FC_ISCR2_9                 0x10A9
#define HDMI_FC_ISCR2_8                 0x10AA
#define HDMI_FC_ISCR2_7                 0x10AB
#define HDMI_FC_ISCR2_6                 0x10AC
#define HDMI_FC_ISCR2_5                 0x10AD
#define HDMI_FC_ISCR2_4                 0x10AE
#define HDMI_FC_ISCR2_3                 0x10AF
#define HDMI_FC_ISCR2_2                 0x10B0
#define HDMI_FC_ISCR2_1                 0x10B1
#define HDMI_FC_ISCR2_0                 0x10B2
#define HDMI_FC_DATAUTO0                0x10B3
#define HDMI_FC_DATAUTO1                0x10B4
#define HDMI_FC_DATAUTO2                0x10B5
#define HDMI_FC_DATMAN                  0x10B6
#define HDMI_FC_DATAUTO3                0x10B7
#define HDMI_FC_RDRB0                   0x10B8
#define HDMI_FC_RDRB1                   0x10B9
#define HDMI_FC_RDRB2                   0x10BA
#define HDMI_FC_RDRB3                   0x10BB
#define HDMI_FC_RDRB4                   0x10BC
#define HDMI_FC_RDRB5                   0x10BD
#define HDMI_FC_RDRB6                   0x10BE
#define HDMI_FC_RDRB7                   0x10BF
#define HDMI_FC_STAT0                   0x10D0
#define HDMI_FC_INT0                    0x10D1
#define HDMI_FC_MASK0                   0x10D2
#define HDMI_FC_POL0                    0x10D3
#define HDMI_FC_STAT1                   0x10D4
#define HDMI_FC_INT1                    0x10D5
#define HDMI_FC_MASK1                   0x10D6
#define HDMI_FC_POL1                    0x10D7
#define HDMI_FC_STAT2                   0x10D8
#define HDMI_FC_INT2                    0x10D9
#define HDMI_FC_MASK2                   0x10DA
#define HDMI_FC_POL2                    0x10DB
#define HDMI_FC_PRCONF                  0x10E0

// HDMI PHY Register Offset
#define HDMI_PHY_CONF0                      0x3000
#define HDMI_PHY_TST0                       0x3001
#define HDMI_PHY_TST1                       0x3002
#define HDMI_PHY_TST2                       0x3003
#define HDMI_PHY_STAT0                      0x3004
#define HDMI_PHY_INT0                       0x3005
#define HDMI_PHY_MASK0                      0x3006
#define HDMI_PHY_POL0                       0x3007
#define HDMI_PHY_I2CM_SLAVE_ADDR            0x3020
#define HDMI_PHY_I2CM_ADDRESS_ADDR          0x3021
#define HDMI_PHY_I2CM_DATAO_1_ADDR          0x3022
#define HDMI_PHY_I2CM_DATAO_0_ADDR          0x3023
#define HDMI_PHY_I2CM_DATAI_1_ADDR          0x3024
#define HDMI_PHY_I2CM_DATAI_0_ADDR          0x3025
#define HDMI_PHY_I2CM_OPERATION_ADDR        0x3026
#define HDMI_PHY_I2CM_INT_ADDR              0x3027
#define HDMI_PHY_I2CM_CTLINT_ADDR           0x3028
#define HDMI_PHY_I2CM_DIV_ADDR              0x3029
#define HDMI_PHY_I2CM_SOFTRSTZ_ADDR         0x302a
#define HDMI_PHY_I2CM_SS_SCL_HCNT_1_ADDR    0x302b
#define HDMI_PHY_I2CM_SS_SCL_HCNT_0_ADDR    0x302c
#define HDMI_PHY_I2CM_SS_SCL_LCNT_1_ADDR    0x302d
#define HDMI_PHY_I2CM_SS_SCL_LCNT_0_ADDR    0x302e
#define HDMI_PHY_I2CM_FS_SCL_HCNT_1_ADDR    0x302f
#define HDMI_PHY_I2CM_FS_SCL_HCNT_0_ADDR    0x3030
#define HDMI_PHY_I2CM_FS_SCL_LCNT_1_ADDR    0x3031
#define HDMI_PHY_I2CM_FS_SCL_LCNT_0_ADDR    0x3032

// Main Controller Registers
#define HDMI_MC_CLKDIS          0x4001
#define HDMI_MC_SWRSTZ          0x4002
#define HDMI_MC_OPCTRL          0x4003
#define HDMI_MC_FLOWCTRL        0x4004
#define HDMI_MC_PHYRSTZ         0x4005
#define HDMI_MC_LOCKONCLOCK     0x4006
#define HDMI_MC_HEACPHY_RST     0x4007

// HDMI_PHY absolute address
#define HDMI_PHY_PWRCTRL        0x00
#define HDMI_PHY_SERDIVCTRL     0x01
#define HDMI_PHY_SERCKCTRL      0x02
#define HDMI_PHY_SERCKKILLCTRL  0x03
#define HDMI_PHY_TXRESCTRL      0x04
#define HDMI_PHY_CKCALCTRL      0x05
#define HDMI_PHY_CPCE_CTRL      0x06
#define HDMI_PHY_TXCLKMEASCTRL  0x07
#define HDMI_PHY_TXMEASCTRL     0x08
#define HDMI_PHY_CKSYMTXCTRL    0x09
#define HDMI_PHY_CMPSEQCTRL     0x0A
#define HDMI_PHY_CMPPWRCTRL     0x0B
#define HDMI_PHY_CMPMODECTRL    0x0C
#define HDMI_PHY_MEASCTRL       0x0D
#define HDMI_PHY_VLEVCTRL       0x0E
#define HDMI_PHY_D2ACTRL        0x0F
#define HDMI_PHY_CURRCTRL       0x10
#define HDMI_PHY_DRVANACTRL     0x11
#define HDMI_PHY_PLLMEASCTRL    0x12
#define HDMI_PHY_PLLPHBYCTRL    0x13
#define HDMI_PHY_GRP_CTRL       0x14
#define HDMI_PHY_GMPCTRL        0x15
#define HDMI_PHY_MPLLMEASCTRL   0x16
#define HDMI_PHY_MSM_CTRL       0x17
#define HDMI_PHY_SCRPB_STATUS   0x18
#define HDMI_PHY_TXTERM         0x19
#define HDMI_PHY_PTRPT_ENBL     0x1A
#define HDMI_PHY_PATTERNGEN     0x1B
#define HDMI_PHY_SDCAP_MODE     0x1C
#define HDMI_PHY_SCOPEMODE      0x1D
#define HDMI_PHY_DIGTXMODE      0x1E
#define HDMI_PHY_STR_STATUS     0x1F
#define HDMI_PHY_SCOPECNT0      0x20
#define HDMI_PHY_SCOPECNT1      0x21
#define HDMI_PHY_SCOPECNT2      0x22
#define HDMI_PHY_SCOPECNTCLK    0x23
#define HDMI_PHY_SCOPESAMPLE    0x24
#define HDMI_PHY_SCOPECNTMSB01  0x25
#define HDMI_PHY_SCOPECNTMSB2CK 0x26

// HDMI DDC offset
#define HDMI_I2CM_SLAVE                 0x7E00
#define HDMI_I2CM_ADDRESS               0x7E01
#define HDMI_I2CM_DATAO                 0x7E02
#define HDMI_I2CM_DATAI                 0x7E03
#define HDMI_I2CM_OPERATION             0x7E04
#define HDMI_I2CM_INT                   0x7E05
#define HDMI_I2CM_CTLINT                0x7E06
#define HDMI_I2CM_DIV                   0x7E07
#define HDMI_I2CM_SEGADDR               0x7E08
#define HDMI_I2CM_SOFTRSTZ              0x7E09
#define HDMI_I2CM_SEGPTR                0x7E0A
#define HDMI_I2CM_SS_SCL_HCNT_1_ADDR    0x7E0B
#define HDMI_I2CM_SS_SCL_HCNT_0_ADDR    0x7E0C
#define HDMI_I2CM_SS_SCL_LCNT_1_ADDR    0x7E0D
#define HDMI_I2CM_SS_SCL_LCNT_0_ADDR    0x7E0E
#define HDMI_I2CM_FS_SCL_HCNT_1_ADDR    0x7E0F
#define HDMI_I2CM_FS_SCL_HCNT_0_ADDR    0x7E10
#define HDMI_I2CM_FS_SCL_LCNT_1_ADDR    0x7E11
#define HDMI_I2CM_FS_SCL_LCNT_0_ADDR    0x7E12

// DDC Interrupt status
#define I2C_MASTER_ERROR                0x01
#define I2C_MASTER_DONE                 0x02

// HDMI bit configuration
typedef enum {
  DDC_READ_OPERATION = 0x01,
  DDC_READ_EXT_OPERATION = 0x02,
  DDC_WRITE_OPERATION = 0x10,
} DDC_OPERATION;

typedef enum {
  HDMI_DDC_STANDARD_MODE = 0x00,
  HDMI_DDC_FAST_MODE = 0x04,
} DDC_MODE;

#pragma pack(push, 1)

// HDMI_PHY_CONF0 0x0100
typedef union {
  struct {
    UINT8 seldipif : 1;
    UINT8 seldataenpol : 1;
    UINT8 gen2_enhpdrxsense : 1;
    UINT8 gen2_txpwron : 1;
    UINT8 gen2_pddq : 1;
    UINT8 sparectrl : 1;
    UINT8 ENTMDS : 1;
    UINT8 PDZ : 1;
  };
  UINT8 Reg;
}  HDMI_PHY_CONF0_REG;

// HDMI_IH_I2CMPHY_STAT0 0x0108
typedef union {
  struct {
    UINT8 i2cmphyerror : 1;
    UINT8 i2cmphydone : 1;
    UINT8 reserved : 6;
  };
  UINT8 Reg;
}  HDMI_IH_I2CMPHY_STAT0_REG;

// HDMI_FC_AUDSCONF 0x01063
typedef union {
  struct {
    UINT8 aud_packet_layout : 1;
    UINT8 reserved : 4;
    UINT8 aud_packet_sampfit : 4;
  };
  UINT8 Reg;
}  HDMI_FC_AUDSCONF_REG;

// HDMI_PHY_STAT0 0x3004
typedef union {
  struct {
    UINT8 TX_PHY_LOCK : 1;
    UINT8 HPD : 1;
    UINT8 reserved : 2;
    UINT8 RX_SENSE0 : 1;
    UINT8 RX_SENSE1 : 1;
    UINT8 RX_SENSE2 : 1;
    UINT8 RX_SENSE3 : 1;
  };
  UINT8 Reg;
}  HDMI_PHY_STAT0_REG;

// HDMI_PHY_I2CM_OPERATION_ADDR 0x3026
typedef union {
  struct {
    UINT8 read : 1;
    UINT8 reserved0 : 3;
    UINT8 write : 1;
    UINT8 reserved1 : 3;
  };
  UINT8 Reg;
}  HDMI_PHY_I2CM_OPERATION_ADDR_REG;

// HDMI_MC_CLKDIS 0x4001
typedef union {
  struct {
    UINT8 pixelclk_disable : 1;
    UINT8 tmdsclk_disable : 1;
    UINT8 prepclk_disable : 1;
    UINT8 audclk_disable : 1;
    UINT8 cscclk_disable : 1;
    UINT8 cecclk_disable : 1;
    UINT8 hdcpclk_disable : 1;
    UINT8 reserved : 1;
  };
  UINT8 Reg;
} HDMI_MC_CLKDIS_REG;

// HDMI_MC_PHYRSTZ 0x4005
typedef union {
  struct {
    UINT8 phyrstz : 1;
    UINT8 reserved : 7;
  };
  UINT8 Reg;
}  HDMI_MC_PHYRSTZ_REG;

// HDMI_MC_HEACPHY_RST 0x4007
typedef union {
  struct {
    UINT8 heacphyrst : 1;
    UINT8 reserved : 7;
  };
  UINT8 Reg;
}  HDMI_MC_HEACPHY_RST_REG;

// HDMI PHY : HDMI_PHY_CPCE_CTRL 0x06
typedef union {
  struct {
    UINT16 clr_dpth : 2;
    UINT16 pixel_rep : 3;
    UINT16 pll_n_cntrl : 2;
    UINT16 mpll_n_cntrl : 2;
    UINT16 ck_edgerate : 2;
    UINT16 tx_edgerate : 2;
    UINT16 prep_div : 2;
    UINT16 reserved : 1;
  };
  UINT16 Reg;
}  HDMI_PHY_CPCE_CTRL_REG;

// HDMI PHY : HDMI_PHY_CURRCTRL 0x10
typedef union {
  struct {
    UINT16 pll_int_cntrl : 3;
    UINT16 pll_prop_cntrl : 3;
    UINT16 mpll_int_cntrl : 3;
    UINT16 mpll_prop_cntrl : 3;
    UINT16 reserved : 4;
  };
  UINT16 Reg;
}  HDMI_PHY_CURRCTRL_REG;

// HDMI PHY : HDMI_PHY_GMPCTRL 0x15
typedef union {
  struct {
    UINT16 mpll_gmp_cntrl : 2;
    UINT16 pll_gmp_cntrl : 2;
    UINT16 reserved : 12;
  };
  UINT16 Reg;
}  HDMI_PHY_GMPCTRL_REG;

#pragma pack(pop)

typedef struct _PLL_MPLL_CONFIG {
  UINT32 PixelClock;
  UINT8 PixelRepetition;
  UINT8 ColorDepth;
  HDMI_PHY_CPCE_CTRL_REG HdmiPhyCpceCtrl;
  HDMI_PHY_CURRCTRL_REG HdmiPhyCurrctrl;
  HDMI_PHY_GMPCTRL_REG HdmiPhyGmpctrl;
} PLL_MPLL_CONFIG, *PPLL_MPLL_CONFIG;

EFI_STATUS
InitHdmi (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  );

EFI_STATUS
SetHdmiPower (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  BOOLEAN                     PowerState
  );

EFI_STATUS
SetHdmiDisplay (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  IMX_DISPLAY_TIMING          *Timings
  );

EFI_STATUS
HdmiDdcRead (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  UINT8                       SlaveAddress,
  IN  UINT8                       RegisterAddress,
  IN  UINT32                      ReadSize,
  IN  DDC_MODE                    DDCMode,
  IN  UINT8                       *DataReadPtr
  );

#endif  /* _HDMI_H_ */
