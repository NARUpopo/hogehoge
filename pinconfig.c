#include "common.h"

void pinconfig(void)
{
    // OUTPUT  : ディジタル出力
    // INPUT   : ディジタル入力 (内部弱プルアップ ON/OFF 切替可)
    // ADC     : アナログ入力 (10 bit)
    // PWM1〜4 : パルス幅変調 (3系統同時出力可)
    // TX      : シリアル通信 送信 (115200ボー)
    // RX      : シリアル通信 受信 (115200ボー)
    //
    PIN01_VDD = VDD;  // VDD 固定
    PIN02_RA5 = OUTPUT;
    PIN03_RA4 = OUTPUT;
    PIN04_RA3 = MCLR; // MCLR/Vpp 固定
    PIN05_RC5 = OUTPUT;
    PIN06_RC4 = OUTPUT;
    PIN07_RC3 = OUTPUT;
    PIN08_RC6 = OUTPUT;
    PIN09_RC7 = OUTPUT;
    PIN10_RB7 = OUTPUT;

    PIN11_RB6 = OUTPUT;
    PIN12_RB5 = OUTPUT;
    PIN13_RB4 = OUTPUT;
    PIN14_RC2 = OUTPUT;
    PIN15_RC1 = OUTPUT;
    PIN16_RC0 = OUTPUT;
    PIN17_RA2 = OUTPUT;
    PIN18_RA1 = ICSPCLK; // ICSPCLK 固定
    PIN19_RA0 = ICSPDAT; // ICSPDAT 固定
    PIN20_VSS = VSS; // VSS 固定

    // ディジタル入力の内部弱プルアップ設定
    // 1 : ON
    // 0 : OFF
    //
    INPUT_PULLUP = 1;
}
