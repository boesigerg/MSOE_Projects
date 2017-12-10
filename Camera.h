#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdint.h>
#include <system.h>
#include <unistd.h>

#define VGA (uint8_t volatile *)0x00800000
#define CAMCTRL (uint8_t volatile *)0x00802000
#define PXLPORT (uint16_t volatile *)0x00802010
#define CAMERA (uint16_t volatile *)0x00802020


void camInit();

uint8_t camRead(int reg);

void camWrite(int reg, int data);

void capture();

#endif
