/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * Rev 1.0
 *              
 */

#ifndef AK4642EN_H__
#define AK4642EN_H__

#define CODEC_I2C_ADDRESS 0x12 

//-----------------------------------------------------------------------------
// AK4642EN Register definition
//-----------------------------------------------------------------------------

#define PWR_MAN1        0x00
#define PWR_MAN2        0x01
#define SIG_SEL1        0x02
#define SIG_SEL2        0x03
#define MOD_CON1        0x04
#define MOD_CON2        0x05
#define TIM_SEL         0x06
#define ALC_MOD1        0x07
#define ALC_MOD2        0x08
#define LIN_INP_VOL     0x09
#define LIN_DIG_VOL     0x0A
#define ALC_MOD3        0x0B
#define RIN_INP_VOL     0x0C
#define RIN_DIG_VOL     0x0D
#define MOD_CON3        0x0E
#define MOD_CON4        0x0F    
#define PWR_MAN3        0x10
#define DIG_FIL_SEL     0x11
#define FIL3_COE0       0x12
#define FIL3_COE1       0x13
#define FIL3_COE2       0x14
#define FIL3_COE3       0x15
#define EQ_COE0         0x16
#define EQ_COE1         0x17
#define EQ_COE2         0x18
#define EQ_COE3         0x19
#define EQ_COE4         0x1A
#define EQ_COE5         0x1B
#define FIL1_COE0       0x1C
#define FIL1_COE1       0x1D
#define FIL1_COE2       0x1E
#define FIL1_COE3       0x1F

#endif          
