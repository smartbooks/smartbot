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

#include <stdint.h>

#include "nRF2460.h"
#include "AK4642EN.h"
#include "config.h"

/* Array used as TX buffer */
uint8_t tx_buf[] = {
  0x00, 0x00, 0x00
};

/* Array used as RX buffer */
uint8_t rx_buf[] = {
  0x00, 0x00, 0x00
};

/* Array that initialize codec */
code const uint8_t codec_init[] = {                                                        
  
  /* Chip address */
  CODEC_I2C_ADDRESS,
  
  /* Register,  Value */
  MOD_CON1,     0x03, // I2S format
  MOD_CON2,     0x00, // 256fs
  PWR_MAN1,     0x40, // VCOM Power up
  MOD_CON4,     0x09, // DAC to Headphone 
  LIN_INP_VOL,  0x41, // Input dig. volume control 
  LIN_DIG_VOL,  0x18, // Output dig. volume control   
  MOD_CON3,     0x31, // Soft mute on
  PWR_MAN1,     0x45, // Power up DAC 
  PWR_MAN3,     0x21, // HP gain on, Mic-Amp on and ADC Left channel power management
  PWR_MAN2,     0x70, // Power up Headphone Left and right  
  
  /* Set Microphone input instead of line-in */
  /*
  SIG_SEL1,     0x05, // Microphone power pin
  PWR_MAN3,     0x28, // Microphone input (mono)
  TIM_SEL,      0x3C, // Timer select for ALC
  ALC_MOD2,     0xE1, // Ref select for ALC
  ALC_MOD3,     0x00, // Set gain
  ALC_MOD1,     0x21, // Set gain
  */  

  /* End of array */
  END_OF_ARRAY 
 };

/* Array that initialize audio streamer */
code const uint8_t audio_streamer_init[] = {                                                        
  
  /* Chip address */
  AUDIO_STREAMER_I2C_ADDRESS, 
                                    
  /* Register,  Value */
  TXSTA       , 0x50, // 32kHz
  TXLAT       , 0x06, // Latency settings
  I2SCNF_IN   , 0x80, // I2S Master mode
  TXPWR       , 0x03, // 0dBm output power
  CH0         , 31,   // Randomly generated 
  CH1         , 59,   // Randomly generated 
  CH2         , 17,   // Randomly generated 
  CH3         , 3,    // Randomly generated 
  CH4         , 45,   // Randomly generated 
  CH5         , 73,   // Randomly generated 
  NBCH        , 0x02, // Maximum number of channels that are not used at any time
  NACH        , 0x06, // Maximum number of channels that are used during streaming at any time
  NLCH        , 0x06, // Maximum number of channels that are used during link up phase
  
  ADDR0       , 0x15, // Radio address byte #1
  ADDR1       , 0x36, // Radio address byte #2
  ADDR2       , 0x57, // Radio address byte #3
  ADDR3       , 0x78, // Radio address byte #4
  ADDR4       , 0x99, // Radio address byte #5
  
  INTCF       , 0x58, // Enable all interrupts
  I2SCNF_OUT  , 0x00, // I2S mode
  RXPWR       , 0x03, // 0dBm output power
  RXMOD       , 0x20, // Enable RF
  TXMOD       , 0x83, // Enable RF and MCLK  

  /* End of array */
  END_OF_ARRAY 
  };

/* Array that setup test mode tx sweep */
code const uint8_t audio_streamer_tx_sweep[] = {                                                        
  
  /* Chip address */
  AUDIO_STREAMER_I2C_ADDRESS,
  
  /* Register,  Value */
  TESTREG     , 0x73, // Test reg
  TESTCH      , 0x84, // Test reg
  LNKMOD      , 0x10, // Enable test
  
  /* End of array */
  END_OF_ARRAY 
 };

/* Array that setup test mode rx sweep */
code const uint8_t audio_streamer_rx_sweep[] = {                                                        
  
  /* Chip address */
  AUDIO_STREAMER_I2C_ADDRESS,
  
  /* Register,  Value */
  TESTREG     , 0x73, // Test reg
  TESTCH      , 0x04, // Test reg
  LNKMOD      , 0x10, // Enable test
  
  /* End of array */
  END_OF_ARRAY 
 };
