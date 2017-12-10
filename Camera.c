#include <system.h>
#include <stdio.h>
#include <io.h>
#include "Camera.h"

#define SLA_W (uint8_t volatile) 0xC0
#define SLA_R (uint8_t volatile) 0xC1

void camInit(){
	IOWR_8DIRECT(CAMERA, 2, 0x80);						//set EN bit
	camWrite(0x11, 0x08);								//Reduce pixel rate to 985kHz
	camWrite(0x14, 0x20);								//Reduce resolution to 176x144
	camWrite(0x39, 0x40);								//PCLK generated only when HREF=1
}

uint8_t camRead(int reg){
	IOWR_8DIRECT(CAMERA, 3, SLA_W);						//write 0xC0 to Transmit Register
	IOWR_8DIRECT(CAMERA, 4, 0x90);						//set STA and WR bit
	while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
		//Wait for TIP to clear
	}
	if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
		printf("Device not found\n");						//RXACK = 1 signals that device can not be found to read from
	}else{
		IOWR_8DIRECT(CAMERA, 3, reg);					//write sub-address to Transmit Register
		IOWR_8DIRECT(CAMERA, 4, 0x50);					//set WR and STO bit
		while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
			//Wait for TIP to clear
		}
		if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
			printf("Sub address not found\n");				//RXACK = 1 signals that sub address can not be found
		}else{
			IOWR_8DIRECT(CAMERA, 3, SLA_R);						//write 0xC1 to Transmit Register
			IOWR_8DIRECT(CAMERA, 4, 0x90);						//set STA and WR bit
			while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
				//Wait for TIP to clear
			}
			if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
				printf("Sub address not able to be written to\n");//RXACK = 1 signals that sub address can not receive data
			}else{
				IOWR_8DIRECT(CAMERA, 4, 0x64);					//set RD, STO and ACK bit
				while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
					//Wait for TIP to clear
				}
				return IORD_8DIRECT(CAMERA, 3);
			}
		}
	}
}

void camWrite(int reg, int data){
	IOWR_8DIRECT(CAMERA, 3, SLA_W);						//write 0xC0 to Transmit Register
	IOWR_8DIRECT(CAMERA, 4, 0x90);						//set STA and WR bit
	while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
		//Wait for TIP to clear
	}
	if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
		printf("Device not found\n");						//RXACK = 1 signals that device can not be found to read from
	}else{
		IOWR_8DIRECT(CAMERA, 3, reg);					//write sub-address to Transmit Register
		IOWR_8DIRECT(CAMERA, 4, 0x10);					//set WR bit
		while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
			//Wait for TIP to clear
		}
		if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
			printf("Sub address not found\n");				//RXACK = 1 signals that sub address can not be found
		}else{
			IOWR_8DIRECT(CAMERA, 3, data);						//write data to Transmit Register
			IOWR_8DIRECT(CAMERA, 4, 0x50);						//set WR and STO bit
			while((IORD_8DIRECT(CAMERA, 4)>>1)&0x01){
				//Wait for TIP to clear
			}
			if((IORD_8DIRECT(CAMERA,4)>>7)&0x01){				//read RXACK (Should be 0)
				printf("Sub address not able to be written to\n");//RXACK = 1 signals that sub address can not receive data
			}
		}
	}
}

void capture(){
	int i,j;
	uint8_t row,col;

	/* VSYNC PULSE*/
	while(!((IORD_8DIRECT(CAMCTRL, 0)>>2)&0x01)){
		//Wait for VSYNC to pulse high
	}
	while(!((IORD_8DIRECT(CAMCTRL, 0)>>2)&0x01)){
		//Wait for VSYNC to go low
	}

	/*SKIP FIRST 8 ROWS*/
	for(i=0; i<8; i++){
		while(!((IORD_8DIRECT(CAMCTRL, 0)>>1)&0x01)){
			//Wait for HREF to pulse high
		}
		while((IORD_8DIRECT(CAMCTRL, 0)>>1)&0x01){
			//Wait for HREF to go low
		}
	}
	for(row=0; row<60; row++){									//For each row of VGA
		while(!((IORD_8DIRECT(CAMCTRL, 0)>>1)&0x01)){
			//Wait for HREF to pulse high
		}
		for(j=0; j<12; j++){									//Skip first 12 pixels
			while(!((IORD_8DIRECT(CAMCTRL, 0))&0x01)){
				//Wait for PCLK to pulse high
			}
			while((IORD_8DIRECT(CAMCTRL, 0))&0x01){
				//Wait for PCLK to go low
			}
		}
		for(col=0; col<80; col++){								//For each column of VGA
			while(!((IORD_8DIRECT(CAMCTRL, 0))&0x01)){
				//Wait for PCLK to pulse high
			}
			uint16_t coords = ((59-row)<<7)+(79-col);			//Subtract row/col from max value to flip image
			uint8_t data = IORD_8DIRECT(PXLPORT, 0);
			IOWR_8DIRECT((VGA+coords),0,data);	//Write PixelPort data to correct VGA coordinates
			while((IORD_8DIRECT(CAMCTRL, 0))&0x01){
				//Wait for PCLK to go low
			}
			while(!((IORD_8DIRECT(CAMCTRL, 0))&0x01)){						//Skip a pixel
				//Wait for PCLK to pulse high
			}
			while((IORD_8DIRECT(CAMCTRL, 0))&0x01){
				//Wait for PCLK to go low
			}
		}
		while(((IORD_8DIRECT(CAMCTRL, 0)>>1)&0x01)){
			//Wait for HREF to pulse low
		}
	}
}
