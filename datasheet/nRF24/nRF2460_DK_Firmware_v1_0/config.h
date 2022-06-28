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

#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>

#define END_OF_ARRAY    0xFF // Dummy byte used in the application to indicate end of array during initialization

#define AUDIO_MODE      P00 // Input: Read operation mode of the audio streamer
#define CODEC_PWR       P01 // Output: Control power down of the codec
#define STREAMER_RESET  P02 // Output: Reset the nRF2460 (active high toggle)
#define STREAMER_SADR   P03 // Output: Set 2-wire address of the audio streamer
#define MCU_SCL         P04 // Bidirectional: 2-wire SCL
#define MCU_SDA         P05 // Bidirectional: 2-wire SDA
#define STREAMER_IRQ    !P06 // Input: Interrupt request from the audio streamer
#define DEBUG_PIN       P07 // Output: Debug pin
#define SEND_DATA       !P15 // Input: Initiate data transfer

#define LINK_LOSS       0x40 // Audio streamer interrupt flag: Link loss
#define DATA_SENT       0x10 // Audio streamer interrupt flag: Data sent
#define DATA_RCVD       0x08 // Audio streamer interrupt flag: Data received
#define CLEAR_ALL       0x58 // Audio streamer interrupt flags: Clear all above flags 

#define MAX_SIZE        0x03 // Max number of bytes that can be transferred at any time between ATX and ARX

#define SMUTE_ON        0x31 // Codec: Soft mute on
#define SMUTE_OFF       0x11 // Codec: Soft mute off

#endif