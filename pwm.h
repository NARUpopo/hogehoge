#pragma once

void init_pwm(void);
void pwm_isr1(void);
void pwm_isr2(void);
void pwm_isr3(void);
void pwm_isr4(void);
void pwm1(const unsigned short period, const unsigned short width);
void pwm2(const unsigned short period, const unsigned short width);
void pwm3(const unsigned short period, const unsigned short width);
void pwm4(const unsigned short period, const unsigned short width);
