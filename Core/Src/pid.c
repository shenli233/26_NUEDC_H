#include "pid.h"
#include "main.h"
#include <stdio.h>
#include "tim.h"

void PID_Init(PID* pid){
  pid->flag = 0;
  pid->Speed[0] = 0.0f;
  pid->Speed[1] = 0.0f;
  pid->Real_Speed[0] = 0.0f;
  pid->Real_Speed[1] = 0.0f;
  pid->Target_Yaw = 0.0f;
  pid->Kp_turn = 3.0f;
  //pid->Kd_turn = 0.005f;
  pid->Angle_Error = 0.0f;
  pid->Turn_Out = 0.0f;
}

void PID_Start(PID *pid){
  HAL_TIM_Base_Start_IT(&htim2);
  pid->flag = 1;
}

void PID_Stop(PID *pid){
  HAL_TIM_Base_Stop_IT(&htim2);
  pid->flag = 0;
}

void PID_Run(PID *pid, float yaw){
    pid->Angle_Error = pid->Target_Yaw - yaw;

    if (pid->Angle_Error > 180.0f)  pid->Angle_Error -= 360.0f;
    if (pid->Angle_Error < -180.0f) pid->Angle_Error += 360.0f;

    pid->Turn_Out = pid->Kp_turn * pid->Angle_Error;// + pid->Kd_turn * (pid->Angle_Error - pid->Last_Angle_Error);
    //Last_Angle_Error = Angle_Error;

    if (pid->Turn_Out > 30.0f)  pid->Turn_Out = 30.0f;
    if (pid->Turn_Out < -30.0f) pid->Turn_Out = -30.0f;
      
    pid->Real_Speed[0] = pid->Speed[0] - pid->Turn_Out; // Left wheel target RPM
    pid->Real_Speed[1] = pid->Speed[1] + pid->Turn_Out; // Right wheel target RPM
}
