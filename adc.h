#pragma once

void init_adc(void);
void adc_isr(void);

extern volatile unsigned short ANA5;
extern volatile unsigned short ANA4;
extern volatile unsigned short ANA2;

extern volatile unsigned short ANB7;
extern volatile unsigned short ANB6;
extern volatile unsigned short ANB5;
extern volatile unsigned short ANB4;

extern volatile unsigned short ANC7;
extern volatile unsigned short ANC6;
extern volatile unsigned short ANC5;
extern volatile unsigned short ANC4;
extern volatile unsigned short ANC3;
extern volatile unsigned short ANC2;
extern volatile unsigned short ANC1;
extern volatile unsigned short ANC0;