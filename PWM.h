#ifndef PWM_h
#define PWM_h

#include <stdint.h>
#include <system.h>
#include <unistd.h>

#define PWM (uint16_t volatile *)0x00802028
#define PWMB (uint16_t volatile *)0x0080202a

void pwmMotor1(double duty);

void pwmMotor2(double duty);

double readPWMA();

double readPWMB();

#endif
