#include "common.h"
#include "uart.h"
#include "utils.h"
#include "pwm.h"

enum
{
    PWM_NUM = 4,
    
    PWM_DISABLE = 255,
    PWM_LOW = 0,
    PWM_HIGH = 1,
    
    PWM_DEFAULT_PERIOD = 15000, // デフォルト周期(マイクロ秒)
    PWM_DEFAULT_WIDTH = 5000 // デフォルト幅(マイクロ秒)
};

volatile unsigned char pwm_status[PWM_NUM+1];
volatile unsigned short pwm_period_low[PWM_NUM+1];
volatile unsigned short pwm_period_high[PWM_NUM+1];
volatile unsigned char pwm_timer1 = 0;
volatile unsigned char pwm_timer3 = 0;
volatile unsigned char pwm_timer5 = 0;

void pwm_isr1()
{
    const unsigned char no = 1;
    if( pwm_status[no] == PWM_HIGH ){
        CCPR1 = pwm_period_low[no];
        pwm_status[no] = PWM_LOW;
    }
    else{
        CCPR1 = pwm_period_high[no];
        pwm_status[no] = PWM_HIGH;
    }
}

void pwm_isr2()
{
    const unsigned char no = 2;
    if( pwm_status[no] == PWM_HIGH ){
        CCPR2 = pwm_period_low[no];
        pwm_status[no] = PWM_LOW;
    }
    else{
        CCPR2 = pwm_period_high[no];
        pwm_status[no] = PWM_HIGH;
    }
}

void pwm_isr3()
{
    const unsigned char no = 3;
    if( pwm_status[no] == PWM_HIGH ){
        CCPR3 = pwm_period_low[no];
        pwm_status[no] = PWM_LOW;
    }
    else{
        CCPR3 = pwm_period_high[no];
        pwm_status[no] = PWM_HIGH;
    }
}

void pwm_isr4()
{
    const unsigned char no = 4;
    if( pwm_status[no] == PWM_HIGH ){
        CCPR4 = pwm_period_low[no];
        pwm_status[no] = PWM_LOW;
    }
    else{
        CCPR4 = pwm_period_high[no];
        pwm_status[no] = PWM_HIGH;
    }
}

void init_ccp1(const unsigned char tsel)
{
    CCPR1 = pwm_period_low[1];
    CCP1CONbits.CCP1MODE = 0b0001 ; // コンペア・トグルモード、タイマーカウンタークリア
    CCPTMRSbits.C1TSEL = tsel; // タイマー接続
    PIR4bits.CCP1IF = 0; // 割り込みフラグリセット
    CCP1CONbits.CCP1EN = 1; // CCPモジュール有効化
    PIE4bits.CCP1IE = 1; // 割り込み許可
}

void init_ccp2(const unsigned char tsel)
{
    CCPR2 = pwm_period_low[2];
    CCP2CONbits.CCP2MODE = 0b0001 ; // コンペア・トグルモード、タイマーカウンタークリア
    CCPTMRSbits.C2TSEL = tsel; // タイマー接続
    PIR4bits.CCP2IF = 0; // 割り込みフラグリセット
    CCP2CONbits.CCP2EN = 1; // CCPモジュール有効化    
    PIE4bits.CCP2IE = 1; // 割り込み許可
}

void init_ccp3(const unsigned char tsel)
{
    CCPR3 = pwm_period_low[3];
    CCP3CONbits.CCP3MODE = 0b0001 ; // コンペア・トグルモード、タイマーカウンタークリア
    CCPTMRSbits.C3TSEL = tsel; // タイマー接続
    PIR4bits.CCP3IF = 0; // 割り込みフラグリセット
    CCP3CONbits.CCP3EN = 1; // CCPモジュール有効化        
    PIE4bits.CCP3IE = 1; // 割り込み許可
}

void init_ccp4(const unsigned char tsel)
{
    CCPR4 = pwm_period_low[4];
    CCP4CONbits.CCP4MODE = 0b0001 ; // コンペア・トグルモード、タイマーカウンタークリア
    CCPTMRSbits.C4TSEL = tsel; // タイマー接続
    PIR4bits.CCP4IF = 0; // 割り込みフラグリセット
    CCP4CONbits.CCP4EN = 1; // CCPモジュール有効化    
    PIE4bits.CCP4IE = 1; // 割り込み許可
}

// (注意) コンペア・トグルモードの場合は prescale = 1 にする必要あり
// 16Mhz で駆動しているので
// タイマーのカウントアップ間隔 = prescale/(FOSC/4) = 1/(16/4*10^6) = 1/4 マイクロ秒
void start_timer1(void)
{
    T1CONbits.TMR1CS = 0b00; // クロック : FOSC/4
    T1CONbits.T1CKPS = 0b00; // prescale ; x1
    TMR1 = 0;
    T1CONbits.TMR1ON = 1;
}

void start_timer3(void)
{
    T3CONbits.TMR3CS = 0b00; // クロック : FOSC/4
    T3CONbits.T3CKPS = 0b00; // prescale ; x1
    TMR3 = 0;
    T3CONbits.TMR3ON = 1; 
}

void start_timer5(void)
{
    T5CONbits.TMR5CS = 0b00; // クロック : FOSC/4
    T5CONbits.T5CKPS = 0b00; // prescale ; x1
    TMR5 = 0;
    T5CONbits.TMR5ON = 1;     
}

void init_pwm_impl(const unsigned char no)
{ 
    unsigned char porta = 0;
    unsigned char portb = 0;
    unsigned char portc = 0;
    
    unsigned char type = PWM1+no-1;
    
    pwm_status[no] = PWM_DISABLE;
    pwm_period_low[no] = (unsigned short)(PWM_DEFAULT_PERIOD-PWM_DEFAULT_WIDTH)*4;
    pwm_period_high[no] = (unsigned short)PWM_DEFAULT_WIDTH*4;

    unsigned char timer = 0;
    if( ! pwm_timer1 ) timer = 1;
    else if( ! pwm_timer3 ) timer = 3;
    else if( ! pwm_timer5 ) timer = 5;
    else return;

    unsigned char num = get_port(type,&porta,&portb,&portc);
    if( ! num ) return;

    pwm_status[no] = PWM_LOW;
    
    unsigned char pps = 0;    
    if( no == 1 ) pps = 0b01100;
    if( no == 2 ) pps = 0b01101;
    if( no == 3 ) pps = 0b01110;
    if( no == 4 ) pps = 0b01111;
    
    unsigned char tsel = 0;
    if( timer == 1 ){
        pwm_timer1 = no;
        tsel = 0b01;
    }
    if( timer == 3 ){
        pwm_timer3 = no;
        tsel = 0b10;
    }
    if( timer == 5 ){
        pwm_timer5 = no;
        tsel = 0b11;
    }
    
    printf("-----\r\nPWM%d:\r\n",no);
    printf("timer = %d, portA = 0x%x, portB = 0x%x, portC = 0x%x\r\n",timer,porta,portb,portc);
    
    // 全割り込み処理不可
    INTCONbits.GIE = 0;

    set_pps(pps,porta,portb,portc);

    if( no == 1 ) init_ccp1(tsel);
    if( no == 2 ) init_ccp2(tsel);
    if( no == 3 ) init_ccp3(tsel);
    if( no == 4 ) init_ccp4(tsel);

    if( timer == 1 ) start_timer1();
    if( timer == 3 ) start_timer3();
    if( timer == 5 ) start_timer5();
    
    // 周辺割り込み許可
    INTCONbits.PEIE = 1;
    
    // 全割り込み処理許可
    INTCONbits.GIE = 1;
}

void init_pwm(void)
{
    for(unsigned char i=1; i<=PWM_NUM; ++i ) init_pwm_impl(i);
    
    // コンパイル時の warning 対策
    pwm1(PWM_DEFAULT_PERIOD,PWM_DEFAULT_WIDTH);
    pwm2(PWM_DEFAULT_PERIOD,PWM_DEFAULT_WIDTH);
    pwm3(PWM_DEFAULT_PERIOD,PWM_DEFAULT_WIDTH);
    pwm4(PWM_DEFAULT_PERIOD,PWM_DEFAULT_WIDTH);
}

void pwm_impl(const unsigned char no, const unsigned short period, const unsigned short width)
{
    if( pwm_status[no] == PWM_DISABLE ) return;

    unsigned short period2 = (period > 16383 ? 16383 : period);
    if( period2 <= width ) return;
    
    INTCONbits.GIE = 0;
    pwm_period_high[no] = width*4;
    pwm_period_low[no] = ( period2 - width)*4;
    INTCONbits.GIE = 1;
}

void pwm1(const unsigned short period, const unsigned short width)
{
    pwm_impl(1,period,width);
}

void pwm2(const unsigned short period, const unsigned short width)
{
    pwm_impl(2,period,width);
}

void pwm3(const unsigned short period, const unsigned short width)
{
    pwm_impl(3,period,width);
}

void pwm4(const unsigned short period, const unsigned short width)
{
    pwm_impl(4,period,width);
}
