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

#ifndef nRF24Z1_H__
#define nRF24Z1_H__

#define AUDIO_STREAMER_I2C_ADDRESS  0x29 

//-----------------------------------------------------------------------------
// nRF24Z1 Register definition
//-----------------------------------------------------------------------------

/* ATX */
#define TXSTA        0x01
#define INTSTA       0x02
#define TXMOD        0x5A
#define TXLAT        0x52
#define INTCF        0x53
#define I2SCNF_IN    0x54
#define TXPWR        0x56
#define TXRESO       0x50

/* ARX */
#define RXMOD        0x4A
#define I2SCNF_OUT   0x44
#define RXPWR        0x49

/* LINK status */
#define LNKSTA       0x03

/* LINK control */
#define CH0          0x0C
#define CH1          0x0D
#define CH2          0x0E
#define CH3          0x0F
#define CH4          0x10
#define CH5          0x11
#define CH6          0x12
#define CH7          0x13
#define CH8          0x14
#define CH9          0x15
#define CH10         0x16
#define CH11         0x17
#define CH12         0x18
#define CH13         0x19
#define CH14         0x1A
#define CH15         0x1B
#define CH16         0x1C
#define CH17         0x1D
#define CH18         0x1E
#define CH19         0x1F
#define CH20         0x20
#define CH21         0x21
#define CH22         0x22
#define CH23         0x23
#define CH24         0x24
#define CH25         0x25
#define CH26         0x26
#define CH27         0x27
#define CH28         0x28
#define CH29         0x29
#define CH30         0x2A
#define CH31         0x2B
#define CH32         0x2C
#define CH33         0x2D
#define CH34         0x2E
#define CH35         0x2F
#define CH36         0x30
#define CH37         0x31
#define NBCH         0x33
#define NACH         0x34 
#define NLCH         0x35
#define LNKMOD       0x36
#define ADDR0        0x39
#define ADDR1        0x3A
#define ADDR2        0x3B
#define ADDR3        0x3C
#define ADDR4        0x3D
#define LNKCSTATE    0x3E
                            
/* TEST */
#define TESTREG      0x7E
#define TESTCH       0x7F
#define REVBYT       0x7D

/* Unsorted */
#define TXDD         0x75
#define TXFMT        0x51
#define TXSTI0       0x57
#define TXSTI1       0x58
#define TXCSTATE     0x5B
#define LNKQ         0x04
#define LNKERR       0x05
#define LNKWTH       0x37
#define LNKETH       0x38
#define RXPIO        0x41
#define RXPWME       0x42
#define TXRXPWMD     0x43
#define RXWAKE       0x45
#define RXSTI0       0x46
#define RXSTI1       0x47
#define RXWTI        0x48
#define RXLTI        0x4C
#define RXRESO       0x40
#define RXCSTATE     0x4B
#define RXDCMD       0x70
#define RXWCNT       0x71
#define RXRCNT       0x72
#define RXBUF0       0x60
#define RXBUF1       0x61
#define RXBUF2       0x62
#define RXBUF3       0x63
#define RXBUF4       0x64
#define RXBUF5       0x65
#define RXBUF6       0x66
#define RXBUF7       0x67
#define RXBUF8       0x68
#define RXBUF9       0x69
#define RXBUF10      0x6A
#define RXBUF11      0x6B
#define RXBUF12      0x6C
#define RXBUF13      0x6D
#define RXBUF14      0x6E
#define RXBUF15      0x6F
#define RXEXEC       0x74

#endif
