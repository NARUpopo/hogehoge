#include "common.h"
#include "uart.h"
#include "utils.h"
#include "input.h"

void init_input(void)
{
    unsigned char porta = 0;
    unsigned char portb = 0;
    unsigned char portc = 0;
    
    unsigned char num = get_port(INPUT,&porta,&portb,&portc);
    if( ! num ) return;
    
    TRISA |= porta;
    TRISB |= portb;
    TRISC |= portc;
    
    if( INPUT_PULLUP ){
        WPUA  |= porta;
        WPUB  |= portb;
        WPUC  |= portc;
    }
    
    printf("-----\r\nINPUT:\r\n");
    printf("pull-up = %d, portA = 0x%x, portB = 0x%x, portC = 0x%x\r\n",INPUT_PULLUP,porta,portb,portc);
}