#include "main.h"
#include "delay.h"
#include "stm32f4xx_hal.h"

volatile float last_error=4.5;
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

float Error_Calcaulate(uint8_t gray_buffer[])
{		
	int active_sum = 0;
	int active_count = 0;
    float value;
	 for (int i = 0; i < 8; i++) 		
	{
            if (gray_buffer[i]==0)  		//digital 的函数，传入传感器的通道号。
			{
                active_sum +=(i+1); // 累加激活传感器的位置
                active_count++; 	//在循环中，active_count 会在每个激活的传感器时增加 1：
			}
   }
 //0表示黑带 1表示白色 active_count==8表示全白 没有黑带  active_count==0 表示全是黑带
  if (active_count == 0) {
       //全是白色 没有检测到黑带
        value = last_error;  // 或设置为默认值，如 4.5f
    } else {
        // 使用浮点除法，避免整数截断误差
        value = (float)active_sum / active_count; 
    }
   return value;
}

int turn(float target,float now)
{
   static float kp = 11.0f;
   static float ki = 0.0f;
   static float kd = 0.0f;
   static float pre_err = 0.0f;
   static float errorint = 0.0f;
   float speed;
   float error = target - now;

   errorint += error;
   speed = kp*error + ki*errorint + kd*(error-pre_err);
   pre_err =  error;

   if (speed > 90.0f)  speed = 90.0f;
   if (speed < -90.0f) speed = -90.0f;

   return speed;
}