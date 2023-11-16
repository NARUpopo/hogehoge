#include "common.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "define_hoge.h"

void run(void) 
{

    double ave=0;  //閾値の変数宣言
    char sensor;
    
    RB6 = HIGH; //redLED turn on

    while(1){
        /*
        // 例2. ディジタル入力
        if(RA4 == 0) RA5 = 1;
        else RA5 = 0;
        */

        /*
        // 例3. アナログ入力
        if(ANC5 > 512) RA5 = 1;
        else RA5 = 0;
        */

        if(RC3 == HIGH){
        
            RB6 = LOW; // redLED turn off
            RB7 = HIGH;  //greenLED turn on
        
            while (1) {        
            
                //ADC start
                ave = (RC0 + RC1)/2;  //閾値の決定
            
                while(RC0 < ave){
                    RB4 = LOW;   //右輪停止
                    RB5 = HIGH;  //左輪稼働
                
                    //scan wall
                    if(RC2 == HIGH){
                        RB4 = LOW;  //moter stop
                        RB5 = LOW;  //moter stop
                        RB6 = HIGH; //redLED turn on
                        RB7 = LOW;  //greenLED turn off
                    
                        break;
                    }
                }
            
            while(RC0 >= ave){
                RB5 = LOW;   //左輪停止
                RB4 = HIGH;  //右輪稼働
                
                //scan wall
                if(RC2 == HIGH){
                    RB4 = LOW;  //moter stop
                    RB5 = LOW;  //moter stop
                    RB6 = HIGH; //redLED turn on
                    RB7 = LOW;  //greenLED turn off
                    
                    break;
                }
            }
            
            /*
            //scan wall
            if(RC2 == HIGH){
                
                RB4 = LOW;  //moter stop
                RB5 = LOW;  //moter stop
                RB6 = HIGH; //redLED turn on
                RB7 = LOW;  //greenLED turn off
            }
            */
            

         }

       


    }

    return;
}
