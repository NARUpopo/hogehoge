#pragma once

#include <xc.h>
#include "../include/proc/pic16f18346.h"

#define _XTAL_FREQ 16000000

enum
{
    OUTPUT=0,
    INPUT,
    ADC,
    PWM1,
    PWM2,
    PWM3,
    PWM4,
    VDD,
    MCLR,
    TX,
    RX,
    ICSPCLK,
    ICSPDAT,
    VSS,
    
    CCP1 = PWM1,
    CCP2 = PWM2,
    CCP3 = PWM3,
    CCP4 = PWM4,
};

extern unsigned char PIN01_VDD;
extern unsigned char PIN02_RA5;
extern unsigned char PIN03_RA4;
extern unsigned char PIN04_RA3;
extern unsigned char PIN05_RC5;
extern unsigned char PIN06_RC4;
extern unsigned char PIN07_RC3;
extern unsigned char PIN08_RC6;
extern unsigned char PIN09_RC7;
extern unsigned char PIN10_RB7;

extern unsigned char PIN11_RB6;
extern unsigned char PIN12_RB5;
extern unsigned char PIN13_RB4;
extern unsigned char PIN14_RC2;
extern unsigned char PIN15_RC1;
extern unsigned char PIN16_RC0;
extern unsigned char PIN17_RA2;
extern unsigned char PIN18_RA1;
extern unsigned char PIN19_RA0;
extern unsigned char PIN20_VSS;

extern unsigned char INPUT_PULLUP;