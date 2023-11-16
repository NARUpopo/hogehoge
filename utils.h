#pragma once

unsigned char get_port(unsigned char type, unsigned char* const porta,unsigned char* const portb, unsigned char* const portc);
void set_pps(const unsigned char pps, const unsigned char porta,const unsigned char portb, const unsigned char portc);
void set_inputpps( const unsigned char type, const unsigned char porta,const unsigned char portb, const unsigned char portc);
