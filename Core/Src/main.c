/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Emm_V5.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "pid.h"
#include "xunji.h"
#include "delay.h"
#include "oled.h"
#include "time.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t rxCmd1[50];
uint8_t rxCmd2[50];
uint32_t pos2 = 0;
uint32_t pos1 = 0;
volatile float Motor_Cur_Pos1 = 0.0f;
volatile float Motor_Cur_Pos2 = 0.0f;
float Motor_cache_Pos1 = 0.0f;//记录当前电机位置
float Motor_cache_Pos2 = 0.0f;

uint8_t gray_buffer[8] = {0};
float turnerror_now = 0.0f;
float turnerror_past = 0.0f;
float alpha = 0.5;

float turnspeed;  //turn>0表示左转 error>0小车应该左转
float turnerror;
float vel_1;
float vel_2;

uint8_t dir_m1 = 1, dir_m2 = 0;//2的角度为正，1的角度为负
volatile uint8_t state = 0; //状态机
uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint8_t flag3 = 0;
uint8_t flag4 = 0;
uint8_t flag5 = 0;

uint8_t raw_data[11];
short x_raw, y_raw, z_raw;
float x, y, z;
uint8_t rxData[11];
uint8_t command[50];
char text[50];
char text0[50] = "start\r\n";

uint32_t now_ticks;
volatile uint32_t      start_tick  = 0;            /* 计时开始时刻 (ms)  */
volatile uint32_t      stop_tick   = 0;            /* 计时停止时刻 (ms)  */
static uint8_t         display_dirty = 1;
volatile uint8_t flag_time = 0;
volatile uint8_t cancel_request = 0;

PID pid;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//printf重定向到串口1
int __io_putchar(int ch){
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

void Motor_Stop(PID *pid){
  pid->Angle_Error = 0.0f;
  pid->Real_Speed[0] = 0.0f;
  pid->Real_Speed[1] = 0.0f;
  pid->Turn_Out = 0.0f;
  Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
  Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
8  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_UART5_Init();
  MX_UART4_Init();
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(3000);
  //初始化OLED
  OLED_Init();
  OLED_DisPlay_On();
  OLED_NewFrame();
  draw_ready_screen();
  OLED_ShowFrame();
  //两个电机开始发送位置数据（uart2对应地址1，uart4对应地址2）
  Emm_V5_Auto_Return_Sys_Params_Timed_2(2, S_CPOS, 10);
  Emm_V5_Auto_Return_Sys_Params_Timed_1(1, S_CPOS, 10);
  HAL_Delay(100);
  //开始接收两个电机的位置数据
  HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rxCmd2, sizeof(rxCmd2));
  __HAL_DMA_DISABLE_IT(huart4.hdmarx, DMA_IT_HT);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxCmd1, sizeof(rxCmd1));
  __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
  //开始接收IMU数据
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxData, sizeof(rxData));
  __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
  //开始接收串口屏数据
  HAL_UARTEx_ReceiveToIdle_DMA(&huart5, command, sizeof(command));
  __HAL_DMA_DISABLE_IT(huart5.hdmarx, DMA_IT_HT);
  //调试正常启动标志
  printf("System Start OK!\r\n");

  // HAL_TIM_Base_Start_IT(&htim2);

  // //初始化PID
  // PID_Init(&pid);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
    now_ticks = HAL_GetTick();
    if (flag_time == 1){
      static uint32_t last_draw_tick = 0;
      uint32_t elapsed = now_ticks - start_tick;
      /* 每 100ms 刷新显示 (与 0.1s 精度匹配) */
      if (display_dirty || (elapsed / 100) != (last_draw_tick / 100)){
        display_dirty = 0;
        last_draw_tick = elapsed;
        OLED_NewFrame();
        draw_running_screen(elapsed);
        OLED_ShowFrame();
      }
    }

    if (cancel_request){
      state = 0;
      cancel_request = 0;
      flag_time = 0;
      flag1 = 0;
      flag2 = 0;
      flag3 = 0;
      flag4 = 0;
      flag5 = 0;
      OLED_NewFrame();
      draw_ready_screen();
      OLED_ShowFrame();
    }

    switch (state) {
      case 0:
        break;
      case 1:
        gray_read(gray_buffer);
        if((-Motor_Cur_Pos1 + Motor_Cur_Pos2)/2.0f >= (10996.5f - 120.0f - 286.5f)){
          state = 0;
          Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
          Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
        }
        turnerror_now = Error_Calcaulate(gray_buffer);
        if (turnerror_past != 0){
          turnerror = alpha * turnerror_now + (1.0f - alpha) * turnerror_past;
        }
        turnerror_past = turnerror_now;
        
        turnspeed = turn(4.5f, turnerror);
        vel_1 = 100 + turnspeed;
        vel_2 = 100 - turnspeed;
        if (vel_2 < 0){
          dir_m2 = 1;
          vel_2 = -vel_2;
        } else {
          dir_m2 = 0;
        }
        Emm_V5_Vel_Control_1(dir_m1, vel_1, 40, 0);
        Emm_V5_Vel_Control_2(dir_m2, vel_2, 40, 0);
        HAL_Delay(10);
        break;
      case 2:
        if (flag1 == 0){
          start_tick = now_ticks;
          flag_time = 1;
          display_dirty = 1;
          Emm_V5_Vel_Control_1(dir_m1, 100, 40, 0);
          Emm_V5_Vel_Control_2(dir_m2, 100, 40, 0);
          flag1 = 1;
        }
        if (flag1 == 1 && flag2 == 0){
          if(Motor_Cur_Pos2 >= (2686.0f - 120.0f - 161.1f)){
          Emm_V5_Vel_Control_1(dir_m1, 78, 40, 0);
          Emm_V5_Vel_Control_2(dir_m2, 122, 40, 0);
          flag2 = 1;
          }
        }
        if (flag1 == 1 && flag2 == 1 && flag3 == 0){
          if(Motor_Cur_Pos2 >= (6117.0f - 120.0f - 161.1f)){
          Motor_cache_Pos1 = Motor_Cur_Pos1;
          Motor_cache_Pos2 = Motor_Cur_Pos2;
          Emm_V5_Vel_Control_1(dir_m1, 100, 40, 0);
          Emm_V5_Vel_Control_2(dir_m2, 100, 40, 0);
          flag3 = 1;
          }
        }
        if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 0) {
          if (Motor_Cur_Pos2 >= (8802.7f - 280.0f - 161.1f)) {
            Emm_V5_Vel_Control_1(dir_m1, 78, 40, 0);
            Emm_V5_Vel_Control_2(dir_m2, 122, 40, 0);
            flag4 = 1;
          }
        }
        if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1 && flag5 == 0) {
          if (Motor_Cur_Pos2 >= (12234.0f - 280.0f -72.0f)) {
            Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
            Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
            stop_tick = now_ticks;
            flag_time = 0;
            OLED_NewFrame();
            draw_stopped_screen(stop_tick - start_tick);
            OLED_ShowFrame();
            HAL_Delay(10);  // 等停止指令发送完成
            flag5 = 1;
            state = 0;
            flag1 = 0;
            flag2 = 0;
            flag3 = 0;
            flag4 = 0;
            flag5 = 0;
            Emm_V5_Reset_CurPos_To_Zero_1();
            Emm_V5_Reset_CurPos_To_Zero_2();
            HAL_Delay(20);  // 等清零指令执行并收到新的位置数据
          }
        }
        break;
      case 3:
        //上位机启动第三项测试
        printf("1111");
        HAL_UART_Transmit_DMA(&huart6, (uint8_t *)text0, strlen(text0));
        state = 0;
        break;
      case 4:
        if (flag1 == 0){
          Emm_V5_Vel_Control_1(dir_m1, 65, 10, 0);
          Emm_V5_Vel_Control_2(dir_m2, 65, 10, 0);
          flag1 = 1;
        }
        if (flag1 == 1 && flag2 == 0){
          if((-Motor_Cur_Pos1 + Motor_Cur_Pos2)/2.0f >= (2686.0f - 161.1f)){
          Emm_V5_Vel_Control_1(dir_m1, 43, 10, 0);
          Emm_V5_Vel_Control_2(dir_m2, 87, 10, 0);
          flag2 = 1;
          }
        }
        if (flag1 == 1 && flag2 == 1 && flag3 == 0){
          if(Motor_Cur_Pos2 >= (3500.0f - 120.0f - 161.1f)){
          Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
          Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
          HAL_Delay(10);  // 等停止指令发送完成
          flag3 = 1;
          state = 0;
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          Emm_V5_Reset_CurPos_To_Zero_1();
          Emm_V5_Reset_CurPos_To_Zero_2();
          HAL_Delay(20);  // 等清零指令执行并收到新的位置数据
          }
        }
        break;
      case 5:
        if (flag1 == 0){
          Emm_V5_Vel_Control_1(dir_m1, 70, 20, 0);
          Emm_V5_Vel_Control_2(dir_m2, 70, 20, 0);
          flag1 = 1;
        }
        if (flag1 == 1 && flag2 == 0){
          if((-Motor_Cur_Pos1 + Motor_Cur_Pos2)/2.0f >= (2686.0f - 161.1f)){
          Emm_V5_Vel_Control_1(dir_m1, 55, 20, 0);
          Emm_V5_Vel_Control_2(dir_m2, 85, 20, 0);
          flag2 = 1;
          }
        }
        if (flag1 == 1 && flag2 == 1 && flag3 == 0){
          if(Motor_Cur_Pos2 >= (6117.0f - 120.0f - 161.1f)){
            Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
            Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
            HAL_Delay(10);  // 等停止指令发送完成
            flag5 = 1;
            state = 0;
            flag1 = 0;
            flag2 = 0;
            flag3 = 0;
            flag4 = 0;
            flag5 = 0;
            Emm_V5_Reset_CurPos_To_Zero_1();
            Emm_V5_Reset_CurPos_To_Zero_2();
            HAL_Delay(20);  // 等清零指令执行并收到新的位置数据
          // Emm_V5_Vel_Control_1(dir_m1, 70, 40, 0);
          // Emm_V5_Vel_Control_2(dir_m2, 70, 40, 0);
          // flag3 = 1;
          }
        }
        // if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 0) {
        //   if (Motor_Cur_Pos2 >= (8802.7f - 280.0f - 161.1f)) {
        //     Emm_V5_Vel_Control_1(dir_m1, 48, 40, 0);
        //     Emm_V5_Vel_Control_2(dir_m2, 92, 40, 0);
        //     flag4 = 1;
        //   }
        // }
        // if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1 && flag5 == 0) {
        //   if (Motor_Cur_Pos2 >= (12234.0f - 280.0f -72.0f)) {
        //     Emm_V5_Stop_Now_1(0); // Immediate stop left wheel with sync flag
        //     Emm_V5_Stop_Now_2(0); // Immediate stop right wheel with sync flag
        //     HAL_Delay(10);  // 等停止指令发送完成
        //     flag5 = 1;
        //     state = 0;
        //     flag1 = 0;
        //     flag2 = 0;
        //     flag3 = 0;
        //     flag4 = 0;
        //     flag5 = 0;
        //     Emm_V5_Reset_CurPos_To_Zero_1();
        //     Emm_V5_Reset_CurPos_To_Zero_2();
        //     HAL_Delay(20);  // 等清零指令执行并收到新的位置数据
        //   }
        // }
        break;
      default:
        break;
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//   if (htim == &htim2) {
//     if(pid.flag == 1){
//       PID_Run(&pid, yaw);
//       Emm_V5_Vel_Control_1(dir_m1, pid.Real_Speed[1], 50, 0);
//       Emm_V5_Vel_Control_2(dir_m2, pid.Real_Speed[0], 50, 0);
//     }
//   }
// }

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
  if (huart == &huart4) {
    if(rxCmd2[0] == 2 && rxCmd2[1] == 0x36 && Size == 8){
    // 拼接成uint32_t类型
      pos2 = (uint32_t)(
                      ((uint32_t)rxCmd2[3] << 24)    |
                      ((uint32_t)rxCmd2[4] << 16)    | 
                      ((uint32_t)rxCmd2[5] << 8)     |
                      ((uint32_t)rxCmd2[6] << 0)
                    );
      Motor_Cur_Pos2 = (float)pos2 * 360.0f / 65536.0f;
      if(rxCmd2[2]) { Motor_Cur_Pos2 = -Motor_Cur_Pos2; }

      // printf("Motor2: %.2f degrees\r\n", Motor_Cur_Pos2);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rxCmd2, sizeof(rxCmd2));
    __HAL_DMA_DISABLE_IT(huart4.hdmarx, DMA_IT_HT);
  }

  if (huart == &huart2) {
    if(rxCmd1[0] == 1 && rxCmd1[1] == 0x36 && Size == 8){
    // 拼接成uint32_t类型
      pos1 = (uint32_t)(
                      ((uint32_t)rxCmd1[3] << 24)    |
                      ((uint32_t)rxCmd1[4] << 16)    | 
                      ((uint32_t)rxCmd1[5] << 8)     |
                      ((uint32_t)rxCmd1[6] << 0)
                    );
      Motor_Cur_Pos1 = (float)pos1 * 360.0f / 65536.0f;
      if(rxCmd1[2]) { Motor_Cur_Pos1 = -Motor_Cur_Pos1; }

      // printf("Motor1: %.2f degrees\r\n", Motor_Cur_Pos1);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxCmd1, sizeof(rxCmd1));
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
  }

  if (huart == &huart3) {
    if(rxData[0] == 0x55 && rxData[1] == 0x51 && rxData[10] == (uint8_t)(rxData[0] + rxData[1] + rxData[2] + rxData[3] + rxData[4] + rxData[5] + rxData[6] + rxData[7] + rxData[8] + rxData[9])) {

			x_raw = (short)(rxData[3] << 8 | rxData[2]);
		  y_raw = (short)(rxData[5] << 8 | rxData[4]);
		  z_raw = (short)(rxData[7] << 8 | rxData[6]);

		  x = (float)x_raw  / 32768.0f * 156.8f;
		  y = (float)y_raw / 32768.0f * 156.8f;
		  z = (float)z_raw   / 32768.0f * 156.8f;

      // sprintf(text,"x:%.2f,y:%.2f,z:%.2f",x,y,z);
      // HAL_UART_Transmit_DMA(&huart6, (uint8_t *)text, sizeof(text));
      // printf("z: %.2f\r\n", z);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxData, sizeof(rxData));
    __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
  }

  if (huart == &huart5) {
    printf("command:%x,%x",command[0],command[1]);
    if(command[0] == 0xaa) {
      switch (command[1]) {
        case 0x02:
          state = 2;
          break;
        case 0x03:
          state = 3;
          break;
        case 0x04:
          state = 4;
          break;
        case 0x05:
          state = 5;
          break;
        case 0x06:
          state = 5;
          break;
        case 0x07:
          cancel_request = 1;
          break;
        default:
          break;
      }
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, command, sizeof(command));
    __HAL_DMA_DISABLE_IT(huart5.hdmarx, DMA_IT_HT);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
