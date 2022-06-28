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

#include <Nordic\reg24le1.h>
#include <stdint.h>

#include "config.h"
#include "hal_w2.h"
#include "macro.h"

void main()
{
  initialize_mcu();

  initialize_audio_streamer();
  /* Pair ATX and ARX with random address
  pair_audio_streamer();
  */
  wait_for_audio_link();
  initialize_audio_codec();
  
  /* RF Test modes 
  radio_test_mode_tx_sweep();
  radio_test_mode_rx_sweep();     
  */
  
  DEBUG_PIN = HIGH;
                                                                                             
  while(true)
  {  
    handle_audio_streamer_interrupts();
    /* Example of data transfer
    send_data_example();          
    */
  }
}