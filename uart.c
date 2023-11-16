#include "common.h"
#include "utils.h"
#include "uart.h"

enum
{
    TX_DISABLE = 0,
    TX_ENABLE = 1,

    RX_DISABLE = 0,
    RX_ENABLE = 1,
};

unsigned char tx_status;
unsigned char rx_status;

// UART 初期化
void init_uart(void)
{
    unsigned char tx_porta = 0;
    unsigned char tx_portb = 0;
    unsigned char tx_portc = 0;

    unsigned char rx_porta = 0;
    unsigned char rx_portb = 0;
    unsigned char rx_portc = 0;

    tx_status = TX_DISABLE;
    rx_status = RX_DISABLE;

    TX1STA = 0x00; // Transmit Status and Control
    RC1STA = 0x00; // Receive  Status and Control
    BAUD1CON = 0x00; // Baud Rate Control

    // コンパイル時の warning 対策
    getch();

    // TX設定
    unsigned char num = get_port(TX,&tx_porta,&tx_portb,&tx_portc);
    if( num ){
        set_pps(0b10100,tx_porta,tx_portb,tx_portc);
        tx_status = TX_ENABLE;
    }

    // RX設定
    num = get_port(RX,&rx_porta,&rx_portb,&rx_portc);
    if( num == 1 ){
        set_inputpps(RX,rx_porta,rx_portb,rx_portc);
        rx_status = RX_ENABLE;
    }

    if( tx_status == TX_DISABLE && rx_status == RX_DISABLE ) return;
    
    // ボーレート設定
    // SYNC=0, BRG16=1, BRGH=1  の場合 baud rate = FOSC/( 4*([SP1BRGH:SP1BRGL]+1) )
    // 詳しくは 389p の TABLE 31-3 参照
    TX1STAbits.SYNC = 0; // 非同期(Asynchronous)モード
    BAUD1CONbits.BRG16 = 1; // 16bit ボーレートジェネレータ使用
    TX1STAbits.BRGH = 1; // 高速モード
    SP1BRG = 34; // 通信速度 = (16*10^6)/(4*(34+1)) = 114285.7 ≒ 115200

    // 送信設定
    if( tx_status == TX_ENABLE ){
        TX1STAbits.TX9 = 0;  // 8bit送信
        TX1STAbits.TXEN = 1; // 送信有効化
        PIE1bits.TXIE = 0; // 送信割り込み無し
    }

    // 受信設定
    if( rx_status == RX_ENABLE ){
        RC1STAbits.RX9 = 0;  // 8bit受信
        RC1STAbits.CREN = 1; // 連続受信有効化
        PIE1bits.RCIE = 0; // 受信割り込み無し
    }

    RC1STAbits.SPEN=1; // シリアルポート有効化

    printf("-----\r\n");
    printf("-----\r\n");
    printf("Welcome to template_16f18346\r\n");
    
    printf("-----\r\nUART:\r\n");
    if( tx_status == TX_ENABLE ){
        printf("TX: portA = 0x%x, portB = 0x%x, portC = 0x%x\r\n",tx_porta,tx_portb,tx_portc);
    }
    if( rx_status == RX_ENABLE ){
        printf("RX: portA = 0x%x, portB = 0x%x, portC = 0x%x\r\n",rx_porta,rx_portb,rx_portc);
    }
}

// USART で 1 byte 送信
void putch(char data)
{
    if( tx_status == TX_DISABLE ) return;
    
    while(!PIR1bits.TXIF) continue; // 送信可まで待つ

    TX1REG = data; // 1文字送信
}

// USART で 1 byte 受信
// 文字を受信していない時やエラーの時は EOF を返す
int getch(void)
{
    if( rx_status == RX_DISABLE ) return EOF;
    if(!PIR1bits.RCIF) return EOF; // 受信バッファが空

    // エラーが起きたらリセット
    if( RC1STAbits.FERR==1 || RC1STAbits.OERR==1){
        RC1STAbits.CREN=0;
        NOP();
        NOP();
        RC1STAbits.CREN=1;
        return EOF;
    }
    
    return RC1REG;
}
