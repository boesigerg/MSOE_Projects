#include <system.h>
#include <stdio.h>
#include <io.h>
#include "PWM.h"

double dutyConvert = 200;		//Convert Duty % to number

void pwmMotor1(double duty){
	if(duty>12){					//Ensure that value falls within range
		duty = 12;
	}else if(duty<2){
		duty=2;
	}
	duty *= dutyConvert;			//Convert duty from percent to number
	IOWR_16DIRECT(PWM,0, duty);
}

void pwmMotor2(double duty){
	if(duty>9){						//Ensure that value falls within range
		duty = 9;
	}else if(duty<2){
		duty=2;
	}
	duty *= dutyConvert;			//Convert duty from percent to number
	IOWR_16DIRECT(PWMB,0, duty);
}

double readPWMA(){
	return IORD_16DIRECT(PWM,0)/dutyConvert;
}

double readPWMB(){
	return IORD_16DIRECT(PWMB,0)/dutyConvert;
}
