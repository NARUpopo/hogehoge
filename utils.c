#include "common.h"
#include "utils.h"

unsigned char get_port(const unsigned char type, unsigned char* const porta,unsigned char* const portb, unsigned char* const portc)
{
    unsigned char num = 0;
    *porta = 0;
    *portb = 0;
    *portc = 0;
    
    if( PIN02_RA5 == type ) {*porta |= 0b00100000; ++num; }
    if( PIN03_RA4 == type ) {*porta |= 0b00010000; ++num; }
    // RA3 は MCLR/Vpp 固定
    if( PIN17_RA2 == type ) {*porta |= 0b00000100; ++num; }
    // RA1 は ICSPCLK 固定
    // RA0 は ICSPDAT 固定

    if( PIN10_RB7 == type ) {*portb |= 0b10000000; ++num; }
    if( PIN11_RB6 == type ) {*portb |= 0b01000000; ++num; }
    if( PIN12_RB5 == type ) {*portb |= 0b00100000; ++num; }
    if( PIN13_RB4 == type ) {*portb |= 0b00010000; ++num; }
    
    if( PIN09_RC7 == type ) {*portc |= 0b10000000; ++num; }
    if( PIN08_RC6 == type ) {*portc |= 0b01000000; ++num; }
    if( PIN05_RC5 == type ) {*portc |= 0b00100000; ++num; }
    if( PIN06_RC4 == type ) {*portc |= 0b00010000; ++num; }
    if( PIN07_RC3 == type ) {*portc |= 0b00001000; ++num; }
    if( PIN14_RC2 == type ) {*portc |= 0b00000100; ++num; }
    if( PIN15_RC1 == type ) {*portc |= 0b00000010; ++num; }
    if( PIN16_RC0 == type ) {*portc |= 0b00000001; ++num; }
    
    return num;
}

void set_pps( const unsigned char pps, const unsigned char porta,const unsigned char portb, const unsigned char portc)
{
    if( porta & 0b00100000 ) RA5PPS = pps;
    if( porta & 0b00010000 ) RA4PPS = pps;
    // RA3 は MCLR/Vpp 固定
    if( porta & 0b00000100 ) RA2PPS = pps;
    // RA1 は ICSPCLK 固定
    // RA0 は ICSPDAT 固定

    if( portb & 0b10000000 ) RB7PPS = pps;
    if( portb & 0b01000000 ) RB6PPS = pps;
    if( portb & 0b00100000 ) RB5PPS = pps;
    if( portb & 0b00010000 ) RB4PPS = pps;

    if( portc & 0b10000000 ) RC7PPS = pps;
    if( portc & 0b01000000 ) RC6PPS = pps;
    if( portc & 0b00100000 ) RC5PPS = pps;
    if( portc & 0b00010000 ) RC4PPS = pps;
    if( portc & 0b00001000 ) RC3PPS = pps;
    if( portc & 0b00000100 ) RC2PPS = pps;
    if( portc & 0b00000010 ) RC1PPS = pps;
    if( portc & 0b00000001 ) RC0PPS = pps;
}

void set_inputpps( const unsigned char type, const unsigned char porta,const unsigned char portb, const unsigned char portc)
{
    unsigned char pps = 0;
    if( porta & 0b00100000 ) pps = 0b00101;
    if( porta & 0b00010000 ) pps = 0b00100;
    // RA3 は MCLR/Vpp 固定
    if( porta & 0b00000100 ) pps = 0b00010;
    // RA1 は ICSPCLK 固定
    // RA0 は ICSPDAT 固定

    if( portb & 0b10000000 ) pps = 0b01111;
    if( portb & 0b01000000 ) pps = 0b01110;
    if( portb & 0b00100000 ) pps = 0b01101;
    if( portb & 0b00010000 ) pps = 0b01100;

    if( portc & 0b10000000 ) pps = 0b10111;
    if( portc & 0b01000000 ) pps = 0b10110;
    if( portc & 0b00100000 ) pps = 0b10101;
    if( portc & 0b00010000 ) pps = 0b10100;
    if( portc & 0b00001000 ) pps = 0b10011;
    if( portc & 0b00000100 ) pps = 0b10010;
    if( portc & 0b00000010 ) pps = 0b10001;
    if( portc & 0b00000001 ) pps = 0b10000;
    
    if( type == RX ){
        RXPPS = pps;
        TRISA |=  porta;
        TRISB |=  portb;
        TRISC |=  portc;
    }
}