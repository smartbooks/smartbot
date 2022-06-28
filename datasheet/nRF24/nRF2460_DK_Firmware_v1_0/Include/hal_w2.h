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

#ifndef HAL_W2_MOD_H__
#define HAL_W2_MOD_H__

#include <stdint.h>
#include <stdbool.h>

#define W2CON0_FLAG_X_STOP      0x20
#define W2CON0_FLAG_X_START     0x10

#define W2CON1_FLAG_NACK        0x02
#define W2CON1_FLAG_DATA_READY  0x01

#define HAL_W2_ISSUE_START_COND (W2CON0 |= W2CON0_FLAG_X_START)
#define HAL_W2_ISSUE_STOP_COND (W2CON0 |= W2CON0_FLAG_X_STOP)
#define HAL_W2_WAIT_FOR_INTERRUPT {while(!SPIF); SPIF = 0;}
#define HAL_W2_WRITE(a) W2DAT = (a)
#define HAL_W2_READ() W2DAT

void hal_w2_configure_master();
uint8_t write_byte(uint8_t chip_adress, uint8_t address, uint8_t databyte);
uint8_t read_byte(uint8_t chip_address, uint8_t address );
void write_array(uint8_t *register_array);

#endif
/** @}  */
