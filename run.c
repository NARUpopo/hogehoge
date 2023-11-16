#include "common.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"

void run(void) 
{
    while(1){
                 
        
        // 例1. LED点滅 
        RA5 = 1;
        __delay_ms(500);
        RA5 = 0;
        __delay_ms(500);
        

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

        /*       
        // 例4. PWM
        // pwm1〜4(周期,幅)
        // 周期と幅はマイクロ秒で指定(最大 16383 マイクロ秒)
        pwm1(1000,500);
        __delay_ms(1000);
        pwm1(2000,1000);
        __delay_ms(1000);
        */

        /* 
        // 例5. シリアル通信
        int c = getch();
        if( c == 'a'){
            RA5 = 1;
            printf("ON\r\n");
        }
        if( c == 'b'){
            RA5 = 0;
            printf("OFF\r\n");
        }
        */
    }

    return;
}
