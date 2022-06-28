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

#include "macro.h"
#include "hal_w2.h"
#include "config.h"
#include "AK4642EN.h"
#include "nordic_common.h"

/* Function that initialize mcu */
void initialize_mcu(void)
{
  P0DIR         = 0x71; 
  P1CON         = 0x55;
  CODEC_PWR     = OFF;
  STREAMER_SADR = LOW;
  DEBUG_PIN     = LOW;
  STREAMER_RESET= LOW;                                 
  hal_w2_configure_master();
  delay_ms(200);
}

/* Function that initialize codec */
void initialize_audio_codec(void)
{
  CODEC_PWR     = ON;
  write_array(codec_init);  
  delay_ms(200);
  write_byte(CODEC_I2C_ADDRESS, MOD_CON3, SMUTE_OFF);   
}

/* Function that setup test mode tx sweep */
void radio_test_mode_tx_sweep(void)
{
  write_array(audio_streamer_tx_sweep);  
}

/* Function that setup test mode rx sweep */
void radio_test_mode_rx_sweep(void)
{
  write_array(audio_streamer_rx_sweep);  
}

/* Function that wait initialize audio streamer */
void initialize_audio_streamer(void)
{
  write_array(audio_streamer_init);  
}
/* Function that check if a new random number is generated */
bool hal_rng_data_ready(void)
{
  return((RNGCTL & BIT_5) > 0);     
}

/* Function that pair up a ATX and ARX with a random address */
void pair_audio_streamer(void)
{
  uint8_t RND_ADDR[5], i;
  
  /* Power up the random number generator, and create a random address */
  RNGCTL = 0xC0;    
  for(i=0;i<5;i++)
  {
    while(!hal_rng_data_ready())
      ;
    RND_ADDR[i] = RNGDAT;
  }
  
  /* Wait for audio streamer to link up */
  while(!read_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKSTA))
    ;
      
  /* Initiate address change from ATX side */
  if(AUDIO_MODE == ATX)
  {
    /* Use the 5byte random address */
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, ADDR0, RND_ADDR[0]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, ADDR1, RND_ADDR[1]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, ADDR2, RND_ADDR[2]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, ADDR3, RND_ADDR[3]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, ADDR4, RND_ADDR[4]);
  
    /* Transfer registers, and wait for transfer complete */
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKCSTATE, 0x01);  
    while(read_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKCSTATE)==0x01) 
      ;

    /* Enable the new address, and wait for re-link */
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKMOD, 0x10);  
    delay_ms(100);
    while(!read_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKSTA)) 
      ;                  
  }

  /* Suggested improvements
  The MCU (on both sides) can use the new ADDR0..4 register values to 
  also set new frequency hopping positions for the audio streaming
  based on the random ADDR0..4 values.
  E.g. CH0 = ADDR0 % 78 + 3; // CH0 is now in the range of 3-80.
  E.g. CH1 = ADDR1 % 78 + 3; // CH1 is now in the range of 3-80.
  ..
  E.g. CH8 = (ADDR2 + ADDR4) % 78 + 3; // CH8 is now in the range of 3-80.   
  */
}

/* Function that wait for audio streamer to link up */
void wait_for_audio_link(void)
{
  while(!read_byte(AUDIO_STREAMER_I2C_ADDRESS, LNKSTA))
      ;
}

/* Function that handle audio streamer interrupts */
void handle_audio_streamer_interrupts(void)
{
  uint8_t interrupt_source = 0;  

  /* Check if there is an interrupt */
  if(STREAMER_IRQ)
  {
    /* Check interrupt source */
    interrupt_source = read_byte(AUDIO_STREAMER_I2C_ADDRESS, INTSTA);
    /* Clear interrupt flags */
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, INTSTA, CLEAR_ALL);
    
    switch(interrupt_source)
    {
      /* Link loss interrupt */
      case LINK_LOSS:
      /* Mute audio until link is up again */        
        write_byte(CODEC_I2C_ADDRESS, MOD_CON3, SMUTE_ON);         
        DEBUG_PIN = LOW;
        wait_for_audio_link();
        write_byte(CODEC_I2C_ADDRESS, MOD_CON3, SMUTE_OFF);                        
        DEBUG_PIN = HIGH;
        break;
      /* Data sent interrupt */
      case DATA_SENT:
        DEBUG_PIN = !DEBUG_PIN;
        break;
      /* Data received interrupt */
      case DATA_RCVD:              
        rx_buf[0] = read_byte(AUDIO_STREAMER_I2C_ADDRESS, RXBUF0);
        rx_buf[1] = read_byte(AUDIO_STREAMER_I2C_ADDRESS, RXBUF1);
        rx_buf[2] = read_byte(AUDIO_STREAMER_I2C_ADDRESS, RXBUF2);        
        DEBUG_PIN = !DEBUG_PIN;
        break;
      /* Default */
      default: 
        break;
    }                                                           
  }    
}

/* Function to send 3 bytes of data if SEND_DATA button is pressed */
void send_data_example(void)
{
  if(SEND_DATA)
  {
    /* Wait until button is released, then send 3 bytes of data */
    while(SEND_DATA)
      ; 
    /* Insert a short delay to avoid that GPIO toggling when button is released cause the function to run more than one time */
    delay_ms(10); 
    /* Transmit 3 bytes of data */    
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, TXBUF0, tx_buf[0]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, TXBUF1, tx_buf[1]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, TXBUF2, tx_buf[2]);
    write_byte(AUDIO_STREAMER_I2C_ADDRESS, TXCOUNT, MAX_SIZE);     
  }
}

/* Simple delay function */
void delay_ms(uint16_t time_ms)
{
  xdata uint32_t delvar = (uint32_t)time_ms*272;
  while(delvar--)
    ;
}