#define CPU_TYPE    2	//定义CPU的类型
	#define P80CL32     1
	#define W78E812     2
/*  BYTE Registers  */
sfr P0   = 0x80;
sfr P1   = 0x90;
sfr P2   = 0xA0;
sfr P3   = 0xB0;
sfr P4	 = 0xD8;
sfr PSW  = 0xD0;
sfr ACC  = 0xE0;
sfr B    = 0xF0;
sfr SP   = 0x81;
sfr16 DPTR = 0x82;
sfr DPL  = 0x82;
sfr DPH  = 0x83;
sfr PCON = 0x87;
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr IE   = 0xA8;
sfr IP   = 0xB8;
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr T2CON  = 0xC8;
sfr T2MOD  = 0xC9;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;

/*  BIT Registers  */
/*  PSW   */
sbit CY   = 0xD7;
sbit AC   = 0xD6;
sbit F0   = 0xD5;
sbit RS1  = 0xD4;
sbit RS0  = 0xD3;
sbit OV   = 0xD2;
sbit P    = 0xD0;
/*  P1   */
sbit P1_0 = 0x90;
sbit P1_1 = 0x91;
sbit P1_2 = 0x92;
sbit P1_3 = 0x93;
sbit P1_4 = 0x94;
sbit P1_5 = 0x95;
sbit P1_6 = 0x96;
sbit P1_7 = 0x97;

/*  P3   */
sbit P3_0 = 0xb0;
sbit P3_1 = 0xb1;
sbit P3_2 = 0xb2;
sbit P3_3 = 0xb3;
sbit P3_4 = 0xb4;
sbit P3_5 = 0xb5;
sbit P3_6 = 0xb6;
sbit P3_7 = 0xb7;
/*  TCON  */
sbit TF1  = 0x8F;
sbit TR1  = 0x8E;
sbit TF0  = 0x8D;
sbit TR0  = 0x8C;
sbit IE1  = 0x8B;
sbit IT1  = 0x8A;
sbit IE0  = 0x89;
sbit IT0  = 0x88;

/*  IE   */
sbit EA   = 0xAF;
sbit ET2  = 0xAD;
sbit ES   = 0xAC;
sbit ET1  = 0xAB;
sbit EX1  = 0xAA;
sbit ET0  = 0xA9;
sbit EX0  = 0xA8;

/*  IP   */
#if CPU_TYPE==W78E812
 
    sbit PS1  = 0xBE; //W78E812
	sfr CHPENR   = 0xF6;
	sfr CHPCON   = 0xBF;
#endif
sbit PT2  = 0xBD;
sbit PS   = 0xBC;
sbit PT1  = 0xBB;
sbit PX1  = 0xBA;
sbit PT0  = 0xB9;
sbit PX0  = 0xB8;

/*  P3  */
sbit T1   = 0xB5;
sbit T0   = 0xB4;
sbit INT1 = 0xB3;
sbit INT0 = 0xB2;
sbit TXD  = 0xB1;
sbit RXD  = 0xB0;

/*  SCON  */
sbit SM0  = 0x9F;
sbit SM1  = 0x9E;
sbit SM2  = 0x9D;
sbit REN  = 0x9C;
sbit TB8  = 0x9B;
sbit RB8  = 0x9A;
sbit TI   = 0x99;
sbit RI   = 0x98;

/*  T2CON  */
sbit TF2   = 0xCF;
sbit EXF2  = 0xCE;
sbit RCLK  = 0xCD;
sbit TCLK  = 0xCC;
sbit EXEN2 = 0xCB;
sbit TR2   = 0xCA;
sbit C_T2  = 0xC9;
sbit CP_RL2= 0xC8;
/*------------------------------------------------
P2 Bit Registers
------------------------------------------------*/
sbit P2_0 = 0xA0;
sbit P2_1 = 0xA1;
sbit P2_2 = 0xA2;
sbit P2_3 = 0xA3;
sbit P2_4 = 0xA4;
sbit P2_5 = 0xA5;
sbit P2_6 = 0xA6;
sbit P2_7 = 0xA7;
/*------------------------------------------------
P0 Bit Registers
------------------------------------------------*/
sbit P0_0 = 0x80;
sbit P0_1 = 0x81;
sbit P0_2 = 0x82;
sbit P0_3 = 0x83;
sbit P0_4 = 0x84;
sbit P0_5 = 0x85;
sbit P0_6 = 0x86;
sbit P0_7 = 0x87;
/*  P4   */
sbit P4_0   = 0xD8;
sbit P4_1   = 0xD9;
sbit P4_2   = 0xDA;
sbit P4_3   = 0xDB;
sbit P4_4   = 0xDD;
sbit P4_5   = 0xDE;
/*  IEN   */
sbit EX2    = 0xE8;
sbit EX3    = 0xE9;
sbit EX4    = 0xEA;
sbit EX5    = 0xEB;
sbit EX6    = 0xEC;
sbit EX7    = 0xED;
sbit EX8    = 0xEE;
sbit EX9    = 0xEF;
/*  IPN   */
sbit PX2    = 0xF8;
sbit PX3    = 0xF9;
sbit PX4    = 0xFA;
sbit PX5    = 0xFB;
sbit PX6    = 0xFC;
sbit PX7    = 0xFD;
sbit PX8    = 0xFE;
sbit PX9    = 0xFF;
/*  IRQ   */
sbit IQ2    = 0xC0;
sbit IQ3    = 0xC1;
sbit IQ4    = 0xC2;
sbit IQ5    = 0xC3;
sbit IQ6    = 0xC4;
sbit IQ7    = 0xC5;
sbit IQ8    = 0xC6;
sbit IQ9    = 0xC7;

/* Define interrupt vector address  */
/* 00 -- 0003H     01 -- 000BH     02 -- 0013H     03 -- 001BH
   04 -- 0023H     05 -- 002BH     06 -- 0033H     07 -- 003BH
   08 -- 0043H     09 -- 004BH     10 -- 0053H     11 -- 005BH
   12 -- 0063H     13 -- 006BH     14 -- 0073H     15 -- 007BH
   16 -- 0083H     17 -- 008BH     18 -- 0093H     19 -- 009BH   */
#define EXT_INT0	00
#define	TIME0_INT   01
#define EXT_INT1    02
#define TIME1_INT   03
#define UART_INT    04
#if  CPU_TYPE==P80CL32
	#define TIME2_INT   05
#elif  CPU_TYPE==W78E812
	#define TIME2_INT   05
	#define EXT_INT2    07
	#define EXT_INT3    08
	#define EXT_INT4    09
	#define EXT_INT5    10
	#define EXT_INT6    11
	#define EXT_INT7    12
	#define EXT_INT8    13
	#define EXT_INT9    14
#endif
//Regist DESCRIPTION for 78LE812
//<1>Port Options Register
//	Bit: 7   6  5  4   3   2   1   0
//		EP6 EP5 - HD7 HD6 HD5 HD4 PUP
//	Mnemonic: POR Address: 86H
//	PUP : Enable Port 0 weak pull-up.
//	HD4-7 : Enable pins P3.4 to P3.7 individually with High Drive outputs.
//	EP5 : Enable P4.5. To set this bit shifts ALE pin to the alternate function P4.5.
//	EP6 : Enable P4.6. To set this bit shifts PSEN pin to the alternate function P4.6
//<2>Watchdog Timer Control Register
//	Bit: 7    6    5  4 3  2   1   0
//		ENW CLRW WIDL - - PS2 PS1 PS0
//	Mnemonic: WDTC Address: 8FH
//	ENW : Enable watch-dog if set.
//	CLRW : Clear watch-dog timer and prescaler if set. This flag will be cleared automatically
//	WIDL : If this bit is set, watch-dog is enabled under IDLE mode. If cleared, watch-dog is disabled
//			under IDLE mode. Default is cleared.
