#include <stdint.h>
#include <system.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>
#include "PWM.h"
#include "Camera.h"

#define VGA (uint8_t volatile *)0x00800000

int xCoord, yCoord;

/* Locates horizontal position of black object on white plane */
void getCoords(){
	uint8_t row = 0;				//Only checking middle row for simplicity
	uint8_t col = 0;
	uint8_t gray = 0x28;			//Value of a pixel halfway between black and white
	int xstart = -1;
	int ystart = -1;
	int xend = -1;
	int yend = -1;
	uint8_t pixel;
	while(xstart == -1){
		pixel = IORD_8DIRECT((VGA+((row<<7)+col)),0);
		if(pixel < gray){		//Search for black pixel
			xstart = col;
			ystart = row;
		}
		col++;
		if(col == 80){
			col = 0;
			row++;
		}
		if(row == 60){		//Search yields no black pixel
			xstart = 0;
			ystart = 0;
			xend = 80;
			yend = 60;
		}
	}
	while(row < 60){
		pixel = IORD_8DIRECT((VGA+((row<<7)+col)),0);
		if(pixel < gray){		//Search for white pixel
			xend = col;
			yend = row;
		}
		col++;
		if(col == 80){											//No other white pixel found
			col = 0;
			row++;
		}
	}
	xCoord = (xstart+xend)/2;										//Center of object = midway between first and last black pixel
	yCoord = (ystart+yend)/2;
}

void setPosition(){
	if(abs(xCoord-40)>2){
		pwmMotor1(readPWMA()+((xCoord-40)*0.02));				//New position = old position + (Difference between current location and middle)/40
	}
	if(abs(yCoord-30)>2){
		pwmMotor2(readPWMB()+((yCoord-30)*0.015));
	}
}

int main(void){
	pwmMotor1(7);											//Initiate motors to centered position
	pwmMotor2(7);
	camInit();												//Initialize camera settings
	char input[30];
	char cmd[30];											//Variables for user input
	int val1;
	int val2;
	while(1){
		val1 = 0;
		val2 = 0;
		fgets(input, 30, stdin);
		sscanf(input, "%s %x %x", cmd, &val1, &val2);			//Correct input: cmd val1 val2
		uint8_t *adr1 = val1;
		uint8_t *adr2 = val2;

		/* RD and WR to/from memory */
		if(strcmp(cmd, "RD")==0){								//User input RD command
			if(adr1 == 0){
				printf("Invalid Command: Please enter a valid address\n");		//User failed to enter a valid address
			}else{
				if(adr2 == 0){													//Read from single address
					printf("0x%x: 0x%x\n", adr1, *adr1);
				}else{															//Else read from block of addresses
					if(adr2>adr1){
						int lines = ((adr2-adr1)/16);							//16 bytes per line of output
						while(lines>0){
							printf("0x%x: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n", adr1, *(adr1+0), *(adr1+1), *(adr1+2),
									*(adr1+3), *(adr1+4), *(adr1+5), *(adr1+6), *(adr1+7), *(adr1+8), *(adr1+9), *(adr1+10),
									*(adr1+11), *(adr1+12), *(adr1+13), *(adr1+14), *(adr1+15));
							adr1+=16;
							lines--;
						}
					}else{
						printf("End address must be greater than start address\n");		//If addresses were put in wrong order
					}
				}
			}
		}else if(strcmp(cmd, "WR")==0){											//User input WR command
			if(adr1 == 0){														//Invalid write address
				printf("Invalid Command: Please enter a valid address\n");
			}else{
				*adr1 = adr2;													//Assign data of address the specified value
			}

		/*Pan and Tilt motor*/
		}else if(strcmp(cmd, "PAN")==0){										//User input PAN command
			pwmMotor1(val1);													//Set PWM output
		}else if(strcmp(cmd, "TILT")==0){										//User input WR command
			pwmMotor2(val1);		//Set PWM output

		/* RD/WR to camera */
		}else if(strcmp(cmd, "CamRead")==0){
			uint8_t data = camRead(val1);
			printf("The value at Camera sub-address 0x%x is 0x%x", val1, data);
		}else if(strcmp(cmd, "CamWrite")==0){
			camWrite(val1, val2);

		/* Camera image to VGA */
		}else if(strcmp(cmd, "Capture")==0){
			while(1){
				capture();				//Camera pixels set to VGA memory
			}

		/* Camera tracking horizontal axis */
		}else if(strcmp(cmd, "Track")==0){
			while(1){
				capture();				//Camera pixels set to VGA memory
				getCoords();			//Search VGA memory for object
				setPosition();			//Adjust motors based on Coords
			}

		/*Otherwise, command is invalid */
		}else{
			printf("Invalid Command: Unknown command\n");
		}
	}
}
