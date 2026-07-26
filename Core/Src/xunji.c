#include "main.h"
#include "delay.h"
#include "stm32f4xx_hal.h"

void key(){
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12) == GPIO_PIN_SET){
        HAL_GPIO_WritePin(XJ_KEY_GPIO_Port, XJ_KEY_Pin, GPIO_PIN_RESET);
    }
}

void gray_read(uint8_t a[])
{
    for (int i = 0; i < 8; ++i) 
    {
        HAL_GPIO_WritePin(XJ_CLK_GPIO_Port, XJ_CLK_Pin, GPIO_PIN_RESET);
        Delay_us(2);
        a[i] = (HAL_GPIO_ReadPin(XJ_DAT_GPIO_Port, XJ_DAT_Pin) == GPIO_PIN_SET) ? 1 : 0;
        HAL_GPIO_WritePin(XJ_CLK_GPIO_Port, XJ_CLK_Pin, GPIO_PIN_SET);
        Delay_us(5);
    }
}
