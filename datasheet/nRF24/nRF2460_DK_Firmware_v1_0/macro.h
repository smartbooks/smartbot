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

#ifndef MACROS_H__
#define MACROS_H__

#define HIGH      1
#define ON        1
#define ATX       1

#define LOW       0
#define OFF       0
#define ARX       0

extern uint8_t tx_buf[];
extern uint8_t rx_buf[];
extern code const uint8_t codec_init[];
extern code const uint8_t audio_streamer_init[];
extern code const uint8_t audio_streamer_tx_sweep[];
extern code const uint8_t audio_streamer_rx_sweep[];

void initialize_mcu(void);
void initialize_audio_codec(void);
void initialize_audio_streamer(void);
void wait_for_audio_link(void);
void pair_audio_streamer(void);
void handle_audio_streamer_interrupts(void);
void send_data_example(void);
void delay_ms(uint16_t time_ms);

void radio_test_mode_rx_sweep(void);
void radio_test_mode_tx_sweep(void);

#endif