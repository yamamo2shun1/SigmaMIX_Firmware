/*
 * File:           C:\Users\shun\SimplicityStudio\v4_workspace\SigmaMIX\SigmaMIX_IC_2_PARAM.h
 *
 * Created:        Saturday, August 19, 2017 1:40:39 AM
 * Description:    SigmaMIX_4:IC 2 parameter RAM definitions.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2017 Analog Devices, Inc. All rights reserved.
 */
#ifndef __SIGMAMIX_IC_2_PARAM_H__
#define __SIGMAMIX_IC_2_PARAM_H__


/* Module DC1 - DC Input Entry*/
#define MOD_DC1_COUNT                                  1
#define MOD_DC1_DEVICE                                 "IC2"
#define MOD_DC1_DCINPALG1_ADDR                         0
#define MOD_DC1_DCINPALG1_FIXPT                        0x00400000
#define MOD_DC1_DCINPALG1_VALUE                        SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_DC1_DCINPALG1_TYPE                         SIGMASTUDIOTYPE_FIXPOINT

/* Module DC2 - DC Input Entry*/
#define MOD_DC2_COUNT                                  1
#define MOD_DC2_DEVICE                                 "IC2"
#define MOD_DC2_DCINPALG2_ADDR                         1
#define MOD_DC2_DCINPALG2_FIXPT                        0x00400000
#define MOD_DC2_DCINPALG2_VALUE                        SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_DC2_DCINPALG2_TYPE                         SIGMASTUDIOTYPE_FIXPOINT

/* Module mix rate - DC Input Entry*/
#define MOD_MIXRATE_COUNT                              1
#define MOD_MIXRATE_DEVICE                             "IC2"
#define MOD_MIXRATE_DCINPALG3_ADDR                     2
#define MOD_MIXRATE_DCINPALG3_FIXPT                    0x00800000
#define MOD_MIXRATE_DCINPALG3_VALUE                    SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_MIXRATE_DCINPALG3_TYPE                     SIGMASTUDIOTYPE_FIXPOINT

/* Module DC4 - DC Input Entry*/
#define MOD_DC4_COUNT                                  1
#define MOD_DC4_DEVICE                                 "IC2"
#define MOD_DC4_DCINPALG4_ADDR                         3
#define MOD_DC4_DCINPALG4_FIXPT                        0x00800000
#define MOD_DC4_DCINPALG4_VALUE                        SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_DC4_DCINPALG4_TYPE                         SIGMASTUDIOTYPE_FIXPOINT

/* Module DC1_2 - DC Input Entry*/
#define MOD_DC1_2_COUNT                                1
#define MOD_DC1_2_DEVICE                               "IC2"
#define MOD_DC1_2_DCINPALG7_ADDR                       4
#define MOD_DC1_2_DCINPALG7_FIXPT                      0x00800000
#define MOD_DC1_2_DCINPALG7_VALUE                      SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_DC1_2_DCINPALG7_TYPE                       SIGMASTUDIOTYPE_FIXPOINT

/* Module DC2_2 - DC Input Entry*/
#define MOD_DC2_2_COUNT                                1
#define MOD_DC2_2_DEVICE                               "IC2"
#define MOD_DC2_2_DCINPALG8_ADDR                       5
#define MOD_DC2_2_DCINPALG8_FIXPT                      0x00000000
#define MOD_DC2_2_DCINPALG8_VALUE                      SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0)
#define MOD_DC2_2_DCINPALG8_TYPE                       SIGMASTUDIOTYPE_FIXPOINT

/* Module Count - DC Input Entry*/
#define MOD_COUNT_COUNT                                1
#define MOD_COUNT_DEVICE                               "IC2"
#define MOD_COUNT_DCINPALG9_ADDR                       6
#define MOD_COUNT_DCINPALG9_FIXPT                      0x00000005
#define MOD_COUNT_DCINPALG9_VALUE                      SIGMASTUDIOTYPE_INTEGER_CONVERT(5)
#define MOD_COUNT_DCINPALG9_TYPE                       SIGMASTUDIOTYPE_INTEGER

/* Module DC3 - DC Input Entry*/
#define MOD_DC3_COUNT                                  1
#define MOD_DC3_DEVICE                                 "IC2"
#define MOD_DC3_DCINPALG10_ADDR                        7
#define MOD_DC3_DCINPALG10_FIXPT                       0x00000005
#define MOD_DC3_DCINPALG10_VALUE                       SIGMASTUDIOTYPE_INTEGER_CONVERT(5)
#define MOD_DC3_DCINPALG10_TYPE                        SIGMASTUDIOTYPE_INTEGER

/* Module Delay Length - DC Input Entry*/
#define MOD_DELAYLENGTH_COUNT                          1
#define MOD_DELAYLENGTH_DEVICE                         "IC2"
#define MOD_DELAYLENGTH_DCINPALG11_ADDR                8
#define MOD_DELAYLENGTH_DCINPALG11_FIXPT               0x00000730
#define MOD_DELAYLENGTH_DCINPALG11_VALUE               SIGMASTUDIOTYPE_INTEGER_CONVERT(1840)
#define MOD_DELAYLENGTH_DCINPALG11_TYPE                SIGMASTUDIOTYPE_INTEGER

/* Module SW vol 1 - Single slew ext vol*/
#define MOD_SWVOL1_COUNT                               1
#define MOD_SWVOL1_DEVICE                              "IC2"
#define MOD_SWVOL1_EXTSWGAINDB2STEP_ADDR               9
#define MOD_SWVOL1_EXTSWGAINDB2STEP_FIXPT              0x00010000
#define MOD_SWVOL1_EXTSWGAINDB2STEP_VALUE              SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0078125)
#define MOD_SWVOL1_EXTSWGAINDB2STEP_TYPE               SIGMASTUDIOTYPE_FIXPOINT

/* Module SW vol 2 - Single slew ext vol*/
#define MOD_SWVOL2_COUNT                               1
#define MOD_SWVOL2_DEVICE                              "IC2"
#define MOD_SWVOL2_EXTSWGAINDB3STEP_ADDR               10
#define MOD_SWVOL2_EXTSWGAINDB3STEP_FIXPT              0x00010000
#define MOD_SWVOL2_EXTSWGAINDB3STEP_VALUE              SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0078125)
#define MOD_SWVOL2_EXTSWGAINDB3STEP_TYPE               SIGMASTUDIOTYPE_FIXPOINT

/* Module Delay1 - Delay*/
#define MOD_DELAY1_COUNT                               1
#define MOD_DELAY1_DEVICE                              "IC2"
#define MOD_DELAY1_DELAYAMT_ADDR                       11
#define MOD_DELAY1_DELAYAMT_FIXPT                      0x00000006
#define MOD_DELAY1_DELAYAMT_VALUE                      SIGMASTUDIOTYPE_INTEGER_CONVERT(6)
#define MOD_DELAY1_DELAYAMT_TYPE                       SIGMASTUDIOTYPE_INTEGER

/* Module ch sel sw - Stereo Switch Nx2*/
#define MOD_CHSELSW_COUNT                              1
#define MOD_CHSELSW_DEVICE                             "IC2"
#define MOD_CHSELSW_STEREOSWSLEW_ADDR                  12
#define MOD_CHSELSW_STEREOSWSLEW_FIXPT                 0x00000000
#define MOD_CHSELSW_STEREOSWSLEW_VALUE                 SIGMASTUDIOTYPE_INTEGER_CONVERT(0)
#define MOD_CHSELSW_STEREOSWSLEW_TYPE                  SIGMASTUDIOTYPE_INTEGER

/* Module SW vol 5 - Single slew ext vol*/
#define MOD_SWVOL5_COUNT                               1
#define MOD_SWVOL5_DEVICE                              "IC2"
#define MOD_SWVOL5_EXTSWGAINDB1STEP_ADDR               13
#define MOD_SWVOL5_EXTSWGAINDB1STEP_FIXPT              0x00004000
#define MOD_SWVOL5_EXTSWGAINDB1STEP_VALUE              SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.001953125)
#define MOD_SWVOL5_EXTSWGAINDB1STEP_TYPE               SIGMASTUDIOTYPE_FIXPOINT

/* Module Delay2 - Delay*/
#define MOD_DELAY2_COUNT                               1
#define MOD_DELAY2_DEVICE                              "IC2"
#define MOD_DELAY2_DELAYAMT_ADDR                       14
#define MOD_DELAY2_DELAYAMT_FIXPT                      0x00000006
#define MOD_DELAY2_DELAYAMT_VALUE                      SIGMASTUDIOTYPE_INTEGER_CONVERT(6)
#define MOD_DELAY2_DELAYAMT_TYPE                       SIGMASTUDIOTYPE_INTEGER

/* Module Mg5 - Signal Merger*/
#define MOD_MG5_COUNT                                  1
#define MOD_MG5_DEVICE                                 "IC2"
#define MOD_MG5_SINGLECTRLMIXER194010_ADDR             15
#define MOD_MG5_SINGLECTRLMIXER194010_FIXPT            0x00400000
#define MOD_MG5_SINGLECTRLMIXER194010_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_MG5_SINGLECTRLMIXER194010_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module Mg6 - Signal Merger*/
#define MOD_MG6_COUNT                                  1
#define MOD_MG6_DEVICE                                 "IC2"
#define MOD_MG6_SINGLECTRLMIXER194011_ADDR             16
#define MOD_MG6_SINGLECTRLMIXER194011_FIXPT            0x00400000
#define MOD_MG6_SINGLECTRLMIXER194011_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_MG6_SINGLECTRLMIXER194011_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module SW vol 6 - Single slew ext vol*/
#define MOD_SWVOL6_COUNT                               1
#define MOD_SWVOL6_DEVICE                              "IC2"
#define MOD_SWVOL6_EXTSWGAINDB4STEP_ADDR               17
#define MOD_SWVOL6_EXTSWGAINDB4STEP_FIXPT              0x00004000
#define MOD_SWVOL6_EXTSWGAINDB4STEP_VALUE              SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.001953125)
#define MOD_SWVOL6_EXTSWGAINDB4STEP_TYPE               SIGMASTUDIOTYPE_FIXPOINT

/* Module Delay Ch - Stereo Switch Nx2*/
#define MOD_DELAYCH_COUNT                              1
#define MOD_DELAYCH_DEVICE                             "IC2"
#define MOD_DELAYCH_STEREOSWSLEW_ADDR                  18
#define MOD_DELAYCH_STEREOSWSLEW_FIXPT                 0x00000000
#define MOD_DELAYCH_STEREOSWSLEW_VALUE                 SIGMASTUDIOTYPE_INTEGER_CONVERT(0)
#define MOD_DELAYCH_STEREOSWSLEW_TYPE                  SIGMASTUDIOTYPE_INTEGER

/* Module Delay3 - Delay*/
#define MOD_DELAY3_COUNT                               1
#define MOD_DELAY3_DEVICE                              "IC2"
#define MOD_DELAY3_DELAYAMT_ADDR                       19
#define MOD_DELAY3_DELAYAMT_FIXPT                      0x00000006
#define MOD_DELAY3_DELAYAMT_VALUE                      SIGMASTUDIOTYPE_INTEGER_CONVERT(6)
#define MOD_DELAY3_DELAYAMT_TYPE                       SIGMASTUDIOTYPE_INTEGER

/* Module Mg8 - Signal Merger*/
#define MOD_MG8_COUNT                                  1
#define MOD_MG8_DEVICE                                 "IC2"
#define MOD_MG8_SINGLECTRLMIXER194013_ADDR             20
#define MOD_MG8_SINGLECTRLMIXER194013_FIXPT            0x00400000
#define MOD_MG8_SINGLECTRLMIXER194013_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_MG8_SINGLECTRLMIXER194013_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module Mg7 - Signal Merger*/
#define MOD_MG7_COUNT                                  1
#define MOD_MG7_DEVICE                                 "IC2"
#define MOD_MG7_SINGLECTRLMIXER194012_ADDR             21
#define MOD_MG7_SINGLECTRLMIXER194012_FIXPT            0x00400000
#define MOD_MG7_SINGLECTRLMIXER194012_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_MG7_SINGLECTRLMIXER194012_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module Delay Mute - Mute*/
#define MOD_DELAYMUTE_COUNT                            2
#define MOD_DELAYMUTE_DEVICE                           "IC2"
#define MOD_DELAYMUTE_ALG0_MUTEONOFF_ADDR              22
#define MOD_DELAYMUTE_ALG0_MUTEONOFF_FIXPT             0x00000000
#define MOD_DELAYMUTE_ALG0_MUTEONOFF_VALUE             SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0)
#define MOD_DELAYMUTE_ALG0_MUTEONOFF_TYPE              SIGMASTUDIOTYPE_FIXPOINT
#define MOD_DELAYMUTE_ALG0_STEP_ADDR                   23
#define MOD_DELAYMUTE_ALG0_STEP_FIXPT                  0x00002000
#define MOD_DELAYMUTE_ALG0_STEP_VALUE                  SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0009765625)
#define MOD_DELAYMUTE_ALG0_STEP_TYPE                   SIGMASTUDIOTYPE_FIXPOINT

/* Module monitor gain - Single SW slew vol (adjustable)*/
#define MOD_MONITORGAIN_COUNT                          2
#define MOD_MONITORGAIN_DEVICE                         "IC2"
#define MOD_MONITORGAIN_ALG0_TARGET_ADDR               24
#define MOD_MONITORGAIN_ALG0_TARGET_FIXPT              0x00800000
#define MOD_MONITORGAIN_ALG0_TARGET_VALUE              SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_MONITORGAIN_ALG0_TARGET_TYPE               SIGMASTUDIOTYPE_FIXPOINT
#define MOD_MONITORGAIN_ALG0_STEP_ADDR                 25
#define MOD_MONITORGAIN_ALG0_STEP_FIXPT                0x00008000
#define MOD_MONITORGAIN_ALG0_STEP_VALUE                SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.00390625)
#define MOD_MONITORGAIN_ALG0_STEP_TYPE                 SIGMASTUDIOTYPE_FIXPOINT

/* Module Delay4 - Delay*/
#define MOD_DELAY4_COUNT                               1
#define MOD_DELAY4_DEVICE                              "IC2"
#define MOD_DELAY4_DELAYAMT_ADDR                       26
#define MOD_DELAY4_DELAYAMT_FIXPT                      0x00000006
#define MOD_DELAY4_DELAYAMT_VALUE                      SIGMASTUDIOTYPE_INTEGER_CONVERT(6)
#define MOD_DELAY4_DELAYAMT_TYPE                       SIGMASTUDIOTYPE_INTEGER

/* Module mg4_2 - Signal Merger*/
#define MOD_MG4_2_COUNT                                1
#define MOD_MG4_2_DEVICE                               "IC2"
#define MOD_MG4_2_SINGLECTRLMIXER19405_ADDR            27
#define MOD_MG4_2_SINGLECTRLMIXER19405_FIXPT           0x00400000
#define MOD_MG4_2_SINGLECTRLMIXER19405_VALUE           SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.5)
#define MOD_MG4_2_SINGLECTRLMIXER19405_TYPE            SIGMASTUDIOTYPE_FIXPOINT

/* Module Fb Mix - Multiple Control Mixer*/
#define MOD_FBMIX_COUNT                                2
#define MOD_FBMIX_DEVICE                               "IC2"
#define MOD_FBMIX_ALG0_STAGE0_VOLUME_ADDR              28
#define MOD_FBMIX_ALG0_STAGE0_VOLUME_FIXPT             0x00800000
#define MOD_FBMIX_ALG0_STAGE0_VOLUME_VALUE             SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_FBMIX_ALG0_STAGE0_VOLUME_TYPE              SIGMASTUDIOTYPE_FIXPOINT
#define MOD_FBMIX_ALG0_STAGE1_VOLUME_ADDR              29
#define MOD_FBMIX_ALG0_STAGE1_VOLUME_FIXPT             0x002D6A86
#define MOD_FBMIX_ALG0_STAGE1_VOLUME_VALUE             SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.354813389233575)
#define MOD_FBMIX_ALG0_STAGE1_VOLUME_TYPE              SIGMASTUDIOTYPE_FIXPOINT

/* Module Gen Filter1 - General (2nd order)*/
#define MOD_GENFILTER1_COUNT                           5
#define MOD_GENFILTER1_DEVICE                          "IC2"
#define MOD_GENFILTER1_ALG0_STAGE0_B0_ADDR             30
#define MOD_GENFILTER1_ALG0_STAGE0_B0_FIXPT            0x0005653F
#define MOD_GENFILTER1_ALG0_STAGE0_B0_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0421523953718669)
#define MOD_GENFILTER1_ALG0_STAGE0_B0_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER1_ALG0_STAGE0_B1_ADDR             31
#define MOD_GENFILTER1_ALG0_STAGE0_B1_FIXPT            0x000ACA7F
#define MOD_GENFILTER1_ALG0_STAGE0_B1_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0843047907437338)
#define MOD_GENFILTER1_ALG0_STAGE0_B1_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER1_ALG0_STAGE0_B2_ADDR             32
#define MOD_GENFILTER1_ALG0_STAGE0_B2_FIXPT            0x0005653F
#define MOD_GENFILTER1_ALG0_STAGE0_B2_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0421523953718669)
#define MOD_GENFILTER1_ALG0_STAGE0_B2_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER1_ALG0_STAGE0_A0_ADDR             33
#define MOD_GENFILTER1_ALG0_STAGE0_A0_FIXPT            0x00B31B84
#define MOD_GENFILTER1_ALG0_STAGE0_A0_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1.39927729259308)
#define MOD_GENFILTER1_ALG0_STAGE0_A0_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER1_ALG0_STAGE0_A1_ADDR             34
#define MOD_GENFILTER1_ALG0_STAGE0_A1_FIXPT            0xFFB6081C
#define MOD_GENFILTER1_ALG0_STAGE0_A1_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(-0.57787761988426)
#define MOD_GENFILTER1_ALG0_STAGE0_A1_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module VC Delay1 - Multi-Tap Voltage Controlled Delay*/
#define MOD_VCDELAY1_COUNT                             1
#define MOD_VCDELAY1_DEVICE                            "IC2"
#define MOD_VCDELAY1_VOLTAGECONTROLLEDDELAY1940NOZIPALGWLIMIT1BUFFERSIZE_ADDR 35
#define MOD_VCDELAY1_VOLTAGECONTROLLEDDELAY1940NOZIPALGWLIMIT1BUFFERSIZE_FIXPT 0x00000730
#define MOD_VCDELAY1_VOLTAGECONTROLLEDDELAY1940NOZIPALGWLIMIT1BUFFERSIZE_VALUE SIGMASTUDIOTYPE_INTEGER_CONVERT(1840)
#define MOD_VCDELAY1_VOLTAGECONTROLLEDDELAY1940NOZIPALGWLIMIT1BUFFERSIZE_TYPE SIGMASTUDIOTYPE_INTEGER

/* Module Gen Filter2 - General (2nd order)*/
#define MOD_GENFILTER2_COUNT                           5
#define MOD_GENFILTER2_DEVICE                          "IC2"
#define MOD_GENFILTER2_ALG0_STAGE0_B0_ADDR             36
#define MOD_GENFILTER2_ALG0_STAGE0_B0_FIXPT            0x0003D58D
#define MOD_GENFILTER2_ALG0_STAGE0_B0_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0299545822080925)
#define MOD_GENFILTER2_ALG0_STAGE0_B0_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER2_ALG0_STAGE0_B1_ADDR             37
#define MOD_GENFILTER2_ALG0_STAGE0_B1_FIXPT            0x0007AB1A
#define MOD_GENFILTER2_ALG0_STAGE0_B1_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0599091644161849)
#define MOD_GENFILTER2_ALG0_STAGE0_B1_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER2_ALG0_STAGE0_B2_ADDR             38
#define MOD_GENFILTER2_ALG0_STAGE0_B2_FIXPT            0x0003D58D
#define MOD_GENFILTER2_ALG0_STAGE0_B2_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0299545822080925)
#define MOD_GENFILTER2_ALG0_STAGE0_B2_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER2_ALG0_STAGE0_A0_ADDR             39
#define MOD_GENFILTER2_ALG0_STAGE0_A0_FIXPT            0x00BA24A7
#define MOD_GENFILTER2_ALG0_STAGE0_A0_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1.45424358625158)
#define MOD_GENFILTER2_ALG0_STAGE0_A0_TYPE             SIGMASTUDIOTYPE_FIXPOINT
#define MOD_GENFILTER2_ALG0_STAGE0_A1_ADDR             40
#define MOD_GENFILTER2_ALG0_STAGE0_A1_FIXPT            0xFFB68524
#define MOD_GENFILTER2_ALG0_STAGE0_A1_VALUE            SIGMASTUDIOTYPE_FIXPOINT_CONVERT(-0.574061915083955)
#define MOD_GENFILTER2_ALG0_STAGE0_A1_TYPE             SIGMASTUDIOTYPE_FIXPOINT

/* Module St Mixer1 - Stereo Mixer*/
#define MOD_STMIXER1_COUNT                             2
#define MOD_STMIXER1_DEVICE                            "IC2"
#define MOD_STMIXER1_ALG0_STAGE0_VOLUME_ADDR           41
#define MOD_STMIXER1_ALG0_STAGE0_VOLUME_FIXPT          0x00800000
#define MOD_STMIXER1_ALG0_STAGE0_VOLUME_VALUE          SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_STMIXER1_ALG0_STAGE0_VOLUME_TYPE           SIGMASTUDIOTYPE_FIXPOINT
#define MOD_STMIXER1_ALG0_STAGE1_VOLUME_ADDR           42
#define MOD_STMIXER1_ALG0_STAGE1_VOLUME_FIXPT          0x00800000
#define MOD_STMIXER1_ALG0_STAGE1_VOLUME_VALUE          SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_STMIXER1_ALG0_STAGE1_VOLUME_TYPE           SIGMASTUDIOTYPE_FIXPOINT

/* Module master gain - Single SW slew vol (adjustable)*/
#define MOD_MASTERGAIN_COUNT                           2
#define MOD_MASTERGAIN_DEVICE                          "IC2"
#define MOD_MASTERGAIN_ALG0_TARGET_ADDR                43
#define MOD_MASTERGAIN_ALG0_TARGET_FIXPT               0x0039D66A
#define MOD_MASTERGAIN_ALG0_TARGET_VALUE               SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.451855944374923)
#define MOD_MASTERGAIN_ALG0_TARGET_TYPE                SIGMASTUDIOTYPE_FIXPOINT
#define MOD_MASTERGAIN_ALG0_STEP_ADDR                  44
#define MOD_MASTERGAIN_ALG0_STEP_FIXPT                 0x00008000
#define MOD_MASTERGAIN_ALG0_STEP_VALUE                 SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.00390625)
#define MOD_MASTERGAIN_ALG0_STEP_TYPE                  SIGMASTUDIOTYPE_FIXPOINT

#endif
