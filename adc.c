#include "common.h"
#include "uart.h"
#include "utils.h"
#include "adc.h"

enum
{
    ADC_DISABLE = 255,
    ADC_CHARGE = 0,
    ADC_GO = 1,
    
    ADC_INTERVAL = 10000,  // サンプリング間隔(マイクロ秒)
};

volatile unsigned short ANA5 = 0;
volatile unsigned short ANA4 = 0;
volatile unsigned short ANA2 = 0;

volatile unsigned short ANB7 = 0;
volatile unsigned short ANB6 = 0;
volatile unsigned short ANB5 = 0;
volatile unsigned short ANB4 = 0;

volatile unsigned short ANC7 = 0;
volatile unsigned short ANC6 = 0;
volatile unsigned short ANC5 = 0;
volatile unsigned short ANC4 = 0;
volatile unsigned short ANC3 = 0;
volatile unsigned short ANC2 = 0;
volatile unsigned short ANC1 = 0;
volatile unsigned short ANC0 = 0;

volatile unsigned char adc_status;
volatile unsigned char adc_idx;
volatile unsigned char adc_maxidx;
volatile unsigned char adc_chn[21];
volatile unsigned short* volatile adc_pin[21];

void adc_isr(void)
{
    if( adc_status == ADC_GO ){
            
        if( ADCON0bits.GO_nDONE == 0 ){

            // 結果取得
            *adc_pin[adc_idx] = ADRES;
            
            ++adc_idx;
            if(adc_idx >= adc_maxidx) adc_idx = 0;
        
            // チャンネル変更
            ADCON0bits.CHS = adc_chn[adc_idx];
            
            adc_status = ADC_CHARGE;
        }
    }
    else {
        
        // ADC 開始
        ADCON0bits.GO_nDONE = 1 ;

        adc_status = ADC_GO;
    }
}

void init_adc(void)
{
    unsigned char porta = 0;
    unsigned char portb = 0;
    unsigned char portc = 0;
    
    ANA5 = 0;
    ANA4 = 0;
    ANA2 = 0;
    
    ANB7 = 0;
    ANB6 = 0;
    ANB5 = 0;
    ANB4 = 0;

    ANC7 = 0;
    ANC6 = 0;
    ANC5 = 0;
    ANC4 = 0;
    ANC3 = 0;
    ANC2 = 0;
    ANC1 = 0;
    ANC0 = 0;

    adc_status = ADC_DISABLE;
    adc_idx = 0;
    adc_maxidx = 0;
    
    unsigned char num = get_port(ADC,&porta,&portb,&portc);
    if( num ) adc_status = ADC_GO;
    else return;
    
    if( porta & 0b00100000 ){ adc_chn[adc_maxidx] = 0b000101; adc_pin[adc_maxidx++] = &ANA5; }
    if( porta & 0b00010000 ){ adc_chn[adc_maxidx] = 0b000100; adc_pin[adc_maxidx++] = &ANA4; }
    // RA3 は MCLR/Vpp 固定
    if( porta & 0b00000100 ){ adc_chn[adc_maxidx] = 0b000010; adc_pin[adc_maxidx++] = &ANA2; }
    // RA1 は ICSPCLK 固定
    // RA0 は ICSPDAT 固定
    
    if( portb & 0b10000000 ){ adc_chn[adc_maxidx] = 0b001111; adc_pin[adc_maxidx++] = &ANB7; }
    if( portb & 0b01000000 ){ adc_chn[adc_maxidx] = 0b001110; adc_pin[adc_maxidx++] = &ANB6; }
    if( portb & 0b00100000 ){ adc_chn[adc_maxidx] = 0b001101; adc_pin[adc_maxidx++] = &ANB5; }
    if( portb & 0b00010000 ){ adc_chn[adc_maxidx] = 0b001100; adc_pin[adc_maxidx++] = &ANB4; }

    if( portc & 0b10000000 ){ adc_chn[adc_maxidx] = 0b010111; adc_pin[adc_maxidx++] = &ANC7; }
    if( portc & 0b01000000 ){ adc_chn[adc_maxidx] = 0b010110; adc_pin[adc_maxidx++] = &ANC6; }
    if( portc & 0b00100000 ){ adc_chn[adc_maxidx] = 0b010101; adc_pin[adc_maxidx++] = &ANC5; }
    if( portc & 0b00010000 ){ adc_chn[adc_maxidx] = 0b010100; adc_pin[adc_maxidx++] = &ANC4; }
    if( portc & 0b00001000 ){ adc_chn[adc_maxidx] = 0b010011; adc_pin[adc_maxidx++] = &ANC3; }
    if( portc & 0b00000100 ){ adc_chn[adc_maxidx] = 0b010010; adc_pin[adc_maxidx++] = &ANC2; }
    if( portc & 0b00000010 ){ adc_chn[adc_maxidx] = 0b010001; adc_pin[adc_maxidx++] = &ANC1; }
    if( portc & 0b00000001 ){ adc_chn[adc_maxidx] = 0b010000; adc_pin[adc_maxidx++] = &ANC0; }

    TRISA |=  porta;
    ANSELA |= porta;
    TRISB |=  portb;
    ANSELB |= portb;
    TRISC |=  portc;
    ANSELC |= portc;
    
    printf("-----\r\nADC:\r\n");
    printf("portA = 0x%x, portB = 0x%x, portC = 0x%x\r\n",porta,portb,portc);

    // 全割り込み処理不可
    INTCONbits.GIE = 0;

    ADRES = 0;
    
    // ADC 結果右詰め
    ADCON1bits.ADFM = 1;

    // ADC クロック設定
    // 早すぎると正しく変換できないので、238p 表22-1 で影が入ってないマス目のスピードを選ぶ
    ADCON1bits.ADCS = 0b010;  // FOSC=16MHzなので FOSC/32 を選択

    // ADC 割り込み不許可
    PIE1bits.ADIE = 0;
    
    // ADC 有効化
    ADCON0bits.ADON = 1;

    // タイマー2のクロックは FOSC/4 固定

    // タイマー2 prescale
    // 0b11 : x64
    T2CONbits.T2CKPS = 0b11;
    
    // タイマー2 postscale
    // 0b1111 = 1:16
    T2CONbits.T2OUTPS = 0b1111;
    
    // 16Mhz で駆動しているので
    // タイマーのカウントアップ間隔 = prescale/(FOSC/4) = 64/(16/4*10^6) = 16 マイクロ秒
    // タイマーリセット間隔
    // 割り込み間隔 =  16 * PR2 * postscale =  256 * PR2 マイクロ秒 
    PR2 = ADC_INTERVAL/2/256;
    
    // タイマー2 カウンターリセット
    TMR2 = 0;

    // タイマー2 割り込みフラグリセット
    PIR1bits.TMR2IF = 0;
        
    // タイマー2 割り込み許可
    PIE1bits.TMR2IE = 1;

    // タイマー2 有効化
    T2CONbits.TMR2ON = 1;

    // 周辺割り込み許可
    INTCONbits.PEIE = 1;
    
    // 全割り込み処理許可
    INTCONbits.GIE = 1; 
}
