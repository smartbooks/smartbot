#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"

// ��ʱ������ѡ���ѡ���ڲ�DCO���ⲿ��Ƶ�������֮һ
//#define  INTERNAL_HF_OSC                                                     // �ڲ�DCOʱ��                   
#define  EXTERNAL_HF_OSC                                                   // �ⲿ��Ƶʱ��

#ifdef   INTERNAL_HF_OSC
#define  INTERNAL_HF_OSC
#endif

#ifdef   EXTERNAL_HF_OSC 
#define  EXTERNAL_HF_OSC
#endif


#define  FLL_FACTOR     549                                                  // FLL_FACTOR: DCO��Ƶϵ��    


//***************************************************************************//
//                                                                           //
//                       ��ʼ����ʱ��: MCLK = XT2                            //
//                                                                           //
//***************************************************************************//
#ifdef   EXTERNAL_HF_OSC
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                                             ; // �ؿ��Ź�
  P5SEL     |= 0x0C                                                        ; // �˿ڹ���ѡ������
  UCSCTL6   &= ~XT2OFF                                                     ; // ����ʹ��
  UCSCTL3   |= SELREF_2                                                    ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                                      ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG)                 ; // ��� XT2,XT1,DCO �����־                                                          
    SFRIFG1 &= ~OFIFG                                                      ; 
  }while(SFRIFG1&OFIFG)                                                    ; // ������������־
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                                       ; // XT2 ����ģʽ 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                                             ; // SMCLK = MCLK = XT2
}
#endif

//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                   //
//                                                                           //
//***************************************************************************//
#ifdef   INTERNAL_HF_OSC
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                                             ; // �ؿ��Ź�
  P7SEL     |= 0x03                                                        ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                                      ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                                      ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                                                    ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                                      ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                                  ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                                      ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                                                   ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                                        ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (549 + 1) * 32768 = 18.0224MHz
  __bic_SR_register(SCG0)                                                  ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                                                  ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG)                 ; // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &= ~OFIFG                                                      ; 
  }while(SFRIFG1&OFIFG)                                                    ; // ������������־ 
}
#endif

void Uart_Init(void)
{
  P10SEL |= BIT4 + BIT5;
  P10DIR |= BIT4       ;
  UCA3CTL1  = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1 |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0   = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1   = 0x00                                           ; 
  UCA3MCTL  = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1 &= ~UCSWRST                                       ; // ����״̬��
  UCA3IE   |= UCRXIE                                         ; // ��������ж�  
}

//***************************************************************************//
//                                                                           //
//                 ������: ����ʱ�Ӳ������P11.0~2                           //
//                                                                           //
//***************************************************************************//

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                                 ; // �رտ��Ź�
  Init_CLK()                                                               ;
  Uart_Init();

  P1DS  = BIT0 + BIT6           ;// ѡ������ǿ��
  P1SEL = BIT0 + BIT6           ;//ѡ�����Ź��ܣ�ACK��MCK��SMCK
  P1DIR = BIT0 + BIT6           ;//��������IO����Ϊ���
  
  //_EINT();
  __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
  __no_operation()                  ; // For debugger
  
  while(1)                          ;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA3IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA3TXBUF = UCA3RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}

// end of file