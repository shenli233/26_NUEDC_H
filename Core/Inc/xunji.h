#ifndef __XUNJI_H
#define __XUNJI_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "pid.h"
#include "delay.h"
#include <stdint.h>

/* External global variables */


/* Function prototypes */
void key();
void gray_read(uint8_t a[]);
float Error_Calcaulate(uint8_t gray_buffer[]);
int turn(float target,float now);

#endif /* __XUNJI_H */