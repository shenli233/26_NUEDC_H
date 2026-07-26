#ifndef __XUNJI_H
#define __XUNJI_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "pid.h"
#include "delay.h"

/* External global variables */


/* Function prototypes */
void key();
void gray_read(uint8_t a[]);

#endif /* __XUNJI_H */