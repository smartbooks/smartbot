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
#include <Nordic\reg24le1.h>

#include "nordic_common.h"
#include "hal_w2.h"
#include "config.h"
#include "macro.h"

/* Setup 2-wire */
void hal_w2_configure_master(void)
{
  W2CON0 =0x87;                           
  INTEXP |= 0x04;                         
  W2CON1 = ~(BIT_5);  
}

/* Write several bytes on 2-wire */
uint8_t hal_w2_write_bytes( uint8_t address, uint8_t *data_ptr, uint8_t data_len )
{
  uint8_t nack_received = 0;
  HAL_W2_ISSUE_START_COND;
  HAL_W2_WRITE(address << 1);
  HAL_W2_WAIT_FOR_INTERRUPT;
  if( W2CON1 & W2CON1_FLAG_NACK ) nack_received = 1; //NACK
  while(data_len-- && !nack_received)
  {
    HAL_W2_WRITE(*data_ptr++);
    HAL_W2_WAIT_FOR_INTERRUPT;
    if( W2CON1 & W2CON1_FLAG_NACK ) nack_received = 1; 
  }
  HAL_W2_ISSUE_STOP_COND;
  return !nack_received;
}

/* Read several bytes on 2-wire */
uint8_t hal_w2_write_and_read_bytes( uint8_t address, uint8_t *wr_data_ptr, uint8_t wr_data_len, uint8_t *rd_data_ptr, uint8_t rd_data_len )
{
  uint8_t nack_received = 0, status;
  HAL_W2_ISSUE_START_COND;
  HAL_W2_WRITE(address << 1);
  HAL_W2_WAIT_FOR_INTERRUPT;
  if( W2CON1 & W2CON1_FLAG_NACK ) nack_received = 1; //NACK
  while(wr_data_len-- && !nack_received)
  {
    HAL_W2_WRITE(*wr_data_ptr++);
    HAL_W2_WAIT_FOR_INTERRUPT;
    if( W2CON1 & W2CON1_FLAG_NACK ) nack_received = 1; 
  }
  // Abort if NACK received
  if( nack_received )
  {
    HAL_W2_ISSUE_STOP_COND;
    return 0;
  }
  // If NACK not received, issue repeated start and read command
  HAL_W2_ISSUE_START_COND;
  HAL_W2_WRITE(address << 1 | 0x01);
  HAL_W2_WAIT_FOR_INTERRUPT;
  if( W2CON1 & W2CON1_FLAG_NACK ) 
    nack_received = 1;  
  while(rd_data_len-- && !nack_received)
  {
    if( rd_data_len == 0 ) HAL_W2_ISSUE_STOP_COND;
    do
    {
      HAL_W2_WAIT_FOR_INTERRUPT;
      status = W2CON1;
    }while( (status & 0x01) == 0 );
    *rd_data_ptr++ = HAL_W2_READ();
    if(status & W2CON1_FLAG_NACK) nack_received = 1;  
  }
  return !nack_received;  
}

/* Write single byte on 2-wire */
uint8_t write_byte(uint8_t chip_adress, uint8_t address, uint8_t databyte )
{
  uint8_t data_string[2];
  data_string[0] = address;
  data_string[1] = databyte;  
  return hal_w2_write_bytes( chip_adress, data_string, 2 );
}

/* Read single byte on 2-wire */
uint8_t read_byte(uint8_t chip_address,  uint8_t address )
{
  uint8_t return_data;  
  hal_w2_write_and_read_bytes( chip_address, &address, 1, &return_data, 1 );
  return return_data;
}

/* Write bytes from array on 2-wire */
void write_array(uint8_t *register_array)
{
  uint8_t i = 1, readback;  
  uint8_t chip_address = register_array[0]; 

  while(register_array[i] != END_OF_ARRAY)
  {
    write_byte( chip_address, register_array[i], register_array[i+1] );    
    readback = read_byte( chip_address, register_array[i] );

    if(readback == register_array[i+1])
      i=i+2;    
  }  
}