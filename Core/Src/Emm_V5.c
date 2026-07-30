#include "Emm_V5.h"
#include "stdbool.h"
/**********************************************************
***	Emm_V5.0�����ջ���������
***	��д���ߣ�ZHANGDATOU
***	����֧�֣��Ŵ�ͷ�ջ��ŷ�
***	�Ա����̣�https://zhangdatou.taobao.com
***	CSDN���ͣ�http s://blog.csdn.net/zhangdatou666
***	qq����Ⱥ��262438510
**********************************************************/

__IO uint16_t MMCL_count = 0, MMCL_cmd[MMCL_LEN] = {0};

/**********************************************************
*** ������������
**********************************************************/
/**
  * @brief    ����������У׼
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Trig_Encoder_Cal(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x06;                       // ������
  cmd[2] =  0x45;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ���������Y42��
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Reset_Motor(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x08;                       // ������
  cmd[2] =  0x97;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ����ǰλ������
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Reset_CurPos_To_Zero_1()
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  0x01;                       // ��ַ
  cmd[1] =  0x0A;                       // ������
  cmd[2] =  0x6D;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

void Emm_V5_Reset_CurPos_To_Zero_2()
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  0x02;                       // ��ַ
  cmd[1] =  0x0A;                       // ������
  cmd[2] =  0x6D;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
	HAL_UART_Transmit_DMA(&huart4, (uint8_t *)cmd, 4);
}

/**
  * @brief    �����ת����??
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Reset_Clog_Pro(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0E;                       // ������
  cmd[2] =  0x52;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    �ָ���������
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Restore_Motor(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0F;                       // ������
  cmd[2] =  0x5F;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**********************************************************
*** �˶���������
**********************************************************/
/**
  * @brief    �������Y42��
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Multi_Motor_Cmd(uint8_t addr)
{
  uint16_t i = 0, j = 0, len = 0; __IO static uint8_t cmd[MMCL_LEN] = {0};
  
	// ��������ȴ���0
	if(MMCL_count > 0)
	{
		// ������������ֽ���??
		len = MMCL_count + 5;
		
		// װ������
		cmd[0] = addr;                       // ��ַ
		cmd[1] = 0xAA;                       // ������
		cmd[2] = (uint8_t)(len >> 8);				 // ���ֽ�����8λ
		cmd[3] = (uint8_t)(len); 		 				 // ���ֽ�����8λ
		for(i=0,j=4; i < MMCL_count; i++,j++) { cmd[j] = MMCL_cmd[i]; }
		cmd[j] = 0x6B; ++j;                  // У���ֽ�
		
		// ��������
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, j); MMCL_count = 0;
	}
	else
	{
		MMCL_count = 0;
	}
}

/**
  * @brief    ʹ���źſ���
  * @param    addr  ��������?
  * @param    state ��ʹ��״̬     ��trueΪʹ�ܵ����falseΪ�رյ��??
  * @param    snF   �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_En_Control(uint8_t addr, bool state, bool snF)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF3;                       // ������
  cmd[2] =  0xAB;                       // ������
  cmd[3] =  (uint8_t)state;             // ʹ��״̬
  cmd[4] =  snF;                        // ���ͬ���˶����?
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �ٶ�ģʽ
  * @param    addr��������?
  * @param    dir ������       ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�       ����Χ0 - 5000RPM
  * @param    acc �����ٶ�     ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    snF �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Vel_Control_1(uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  __IO static uint8_t cmd[16] = {0};

  // װ������
  cmd[0] =  0x01;                       // ��ַ
  cmd[1] =  0xF6;                       // ������
  cmd[2] =  dir;                        // ����
  cmd[3] =  (uint8_t)(vel >> 8);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[4] =  (uint8_t)(vel >> 0);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[5] =  acc;                        // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6] =  snF;                        // ���ͬ���˶����?
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 8);
}

void Emm_V5_Vel_Control_2(uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  __IO static uint8_t cmd[16] = {0};

  // װ������
  cmd[0] =  0x02;                       // ��ַ
  cmd[1] =  0xF6;                       // ������
  cmd[2] =  dir;                        // ����
  cmd[3] =  (uint8_t)(vel >> 8);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[4] =  (uint8_t)(vel >> 0);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[5] =  acc;                        // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6] =  snF;                        // ���ͬ���˶����?
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart4, (uint8_t *)cmd, 8);
}

/**
  * @brief    λ��ģʽ
  * @param    addr��������?
  * @param    dir ������        ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    clk ��������      ����Χ0- (2^32 - 1)��
  * @param    raF ���˶���־��0Ϊ�����һ����Ŀ��λ�ý������λ���˶���1Ϊ����ֵ�˶���2��Ե�ǰ���ʵʱλ�ý������λ���˶�??
  * @param    snF �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, uint8_t raF, bool snF)
{
  __IO static uint8_t cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFD;                      // ������
  cmd[2]  =  dir;                       // ����
  cmd[3]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[4]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[5]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[10] =  raF;                       // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[11] =  snF;                       // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[12] =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 13);
}

/**
  * @brief    ���ÿ���λ��ģʽ���˶�����
  * @param    addr��������?
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    raF ���˶���־��0Ϊ�����һ����Ŀ��λ�ý������λ���˶���1Ϊ����ֵ�˶���2��Ե�ǰ���ʵʱλ�ý������λ���˶�??
  * @param    snF �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Set_QPos_Params(uint8_t addr, uint16_t vel, uint8_t acc, uint8_t raF, bool snF)
{
  __IO static uint8_t cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xF1;                      // ������
  cmd[2]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[3]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[4]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[5] =  raF;                        // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[6] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 8);
}

/**
  * @brief    ����λ��ģʽ
  * @param    addr��������?
  * @param    clk ���������������ţ���Ĭ��16ϸ����+3200��������תһȦ����-3200�ͷ�����תһȦ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_QPos_Control(uint8_t addr, int32_t clk)
{
  __IO static uint8_t cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFC;                      // ������
  cmd[2]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[3]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[4]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[5]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[6] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 7);
}

/**
  * @brief    ����ֹͣ
  * @param    addr  ��������?
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Stop_Now_1(bool snF)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  0x01;                       // ��ַ
  cmd[1] =  0xFE;                       // ������
  cmd[2] =  0x98;                       // ������
  cmd[3] =  snF;                        // ���ͬ���˶����?
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 5);
  
}

void Emm_V5_Stop_Now_2(bool snF)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  0x02;                       // ��ַ
  cmd[1] =  0xFE;                       // ������
  cmd[2] =  0x98;                       // ������
  cmd[3] =  snF;                        // ���ͬ���˶����?
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart4, (uint8_t *)cmd, 5);
  
}

/**
  * @brief    ���ͬ���˶�??
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Synchronous_motion(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xFF;                       // ������
  cmd[2] =  0x66;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**********************************************************
*** ԭ���������??
**********************************************************/
/**
  * @brief    ���õ�Ȧ��������λ��
  * @param    addr  ��������?
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Set_O(uint8_t addr, bool svF)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x93;                       // ������
  cmd[2] =  0x88;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 5);
}

/**
  * @brief    ��������
  * @param    addr   ��������?
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9A;                       // ������
  cmd[2] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[3] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 5);
}

/**
  * @brief    ǿ���жϲ��˳�����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Interrupt(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9C;                       // ������
  cmd[2] =  0x48;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ��ȡ�������??
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Read_Params(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x22;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸Ļ������??
  * @param    addr  ��������?
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    o_dir  �����㷽��0ΪCW������ֵΪCCW
  * @param    o_vel  �������ٶȣ���λ��RPM��ת/���ӣ�
  * @param    o_tm   �����㳬ʱʱ�䣬��λ������
  * @param    sl_vel ������λ��ײ������ת�٣���λ��RPM��ת/���ӣ�
  * @param    sl_ma  ������λ��ײ�������������λ��Ma��������
  * @param    sl_ms  ������λ��ײ������ʱ�䣬��λ��Ms�����룩
  * @param    potF   ���ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Modify_Params(uint8_t addr, bool svF, uint8_t o_mode, uint8_t o_dir, uint16_t o_vel, uint32_t o_tm, uint16_t sl_vel, uint16_t sl_ma, uint16_t sl_ms, bool potF)
{
  __IO static uint8_t cmd[32] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x4C;                       // ������
  cmd[2] =  0xAE;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[5] =  o_dir;                      // ���㷽��
  cmd[6]  =  (uint8_t)(o_vel >> 8);     // �����ٶ�(RPM)��8λ�ֽ�
  cmd[7]  =  (uint8_t)(o_vel >> 0);     // �����ٶ�(RPM)��8λ�ֽ� 
  cmd[8]  =  (uint8_t)(o_tm >> 24);     // ���㳬ʱʱ��(bit24 - bit31)
  cmd[9]  =  (uint8_t)(o_tm >> 16);     // ���㳬ʱʱ��(bit16 - bit23)
  cmd[10] =  (uint8_t)(o_tm >> 8);      // ���㳬ʱʱ��(bit8  - bit15)
  cmd[11] =  (uint8_t)(o_tm >> 0);      // ���㳬ʱʱ��(bit0  - bit7 )
  cmd[12] =  (uint8_t)(sl_vel >> 8);    // ����λ��ײ������ת��(RPM)��8λ�ֽ�
  cmd[13] =  (uint8_t)(sl_vel >> 0);    // ����λ��ײ������ת��(RPM)��8λ�ֽ� 
  cmd[14] =  (uint8_t)(sl_ma >> 8);     // ����λ��ײ���������??(Ma)��8λ�ֽ�
  cmd[15] =  (uint8_t)(sl_ma >> 0);     // ����λ��ײ���������??(Ma)��8λ�ֽ� 
  cmd[16] =  (uint8_t)(sl_ms >> 8);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[17] =  (uint8_t)(sl_ms >> 0);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[18] =  potF;                      // �ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  cmd[19] =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 20);
}

/**
  * @brief    ��ȡ��ײ���㷵�ؽǶȣ�X42S/Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void X_V2_Origin_Read_SL_RP(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x3F;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸���ײ���㷵�ؽǶȣ�X42S/Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    sl_rp 	 ����ײ���㷵�ؽǶȣ���λ0.1�㣬����40������4.0��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void X_V2_Origin_Modify_SL_RP(uint8_t addr, bool svF, uint16_t sl_rp)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0x5C;                      // ������
  cmd[2]  =  0xAC;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(sl_rp >> 8);			// ��ײ���㷵�ؽǶȣ���λ0.1��
	cmd[5]  =  (uint8_t)(sl_rp >> 0);
  cmd[6]  =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 7);
}

/**********************************************************
*** ��ȡϵͳ��������
**********************************************************/
/**
  * @brief    ��ʱ������Ϣ���Y42��
  * @param    addr  	��������?
  * @param    s     	��ϵͳ��������
	* @param    time_ms ����ʱʱ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Auto_Return_Sys_Params_Timed_2(uint8_t addr, SysParams_t s, uint16_t time_ms)
{
  uint8_t i = 0; __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  cmd[i] = 0x11; ++i;                   // ������

  cmd[i] = 0x18; ++i;                   // ������

  switch(s)                             // ��Ϣ������
  {
    case S_VBUS : cmd[i] = 0x24; ++i; break;	// ��ȡ���ߵ�ѹ
		case S_CBUS : cmd[i] = 0x26; ++i; break;	// ��ȡ���ߵ���
    case S_CPHA : cmd[i] = 0x27; ++i; break;	// ��ȡ�����??
		case S_ENCO : cmd[i] = 0x29; ++i; break;	// ��ȡ������ԭʼֵ
		case S_CLKC : cmd[i] = 0x30; ++i; break;	// ��ȡʵʱ������
    case S_ENCL : cmd[i] = 0x31; ++i; break;	// ��ȡ�������Ի�У׼��ı������?
		case S_CLKI : cmd[i] = 0x32; ++i; break;	// ��ȡ����������
    case S_TPOS : cmd[i] = 0x33; ++i; break;	// ��ȡ���Ŀ��λ��??
    case S_SPOS : cmd[i] = 0x34; ++i; break;	// ��ȡ���ʵʱ�趨��Ŀ��λ��??
		case S_VEL  : cmd[i] = 0x35; ++i; break;	// ��ȡ���ʵʱת��??
    case S_CPOS : cmd[i] = 0x36; ++i; break;	// ��ȡ���ʵʱλ��??
    case S_PERR : cmd[i] = 0x37; ++i; break;	// ��ȡ���λ�����
		case S_VBAT : cmd[i] = 0x38; ++i; break;	// ��ȡ��Ȧ��������ص�ѹ��Y42��
		case S_TEMP : cmd[i] = 0x39; ++i; break;	// ��ȡ���ʵʱ�¶ȣ�Y42��
    case S_FLAG : cmd[i] = 0x3A; ++i; break;	// ��ȡ���״̬��־�?
    case S_OFLAG: cmd[i] = 0x3B; ++i; break;	// ��ȡ����״̬��־λ
		case S_OAF  : cmd[i] = 0x3C; ++i; break;	// ��ȡ���״̬��־�? + ����״̬��־λ��Y42��
		case S_PIN  : cmd[i] = 0x3D; ++i; break;	// ��ȡ����״̬��Y42��
    default: break;
  }
	
	cmd[i] = (uint8_t)(time_ms >> 8);  ++i;	// ��ʱʱ��
	cmd[i] = (uint8_t)(time_ms >> 0);  ++i;

  cmd[i] = 0x6B; ++i;                   	// У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart4, (uint8_t *)cmd, i);
}

void Emm_V5_Auto_Return_Sys_Params_Timed_1(uint8_t addr, SysParams_t s, uint16_t time_ms)
{
  uint8_t i = 0; __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  cmd[i] = 0x11; ++i;                   // ������

  cmd[i] = 0x18; ++i;                   // ������

  switch(s)                             // ��Ϣ������
  {
    case S_VBUS : cmd[i] = 0x24; ++i; break;	// ��ȡ���ߵ�ѹ
		case S_CBUS : cmd[i] = 0x26; ++i; break;	// ��ȡ���ߵ���
    case S_CPHA : cmd[i] = 0x27; ++i; break;	// ��ȡ�����??
		case S_ENCO : cmd[i] = 0x29; ++i; break;	// ��ȡ������ԭʼֵ
		case S_CLKC : cmd[i] = 0x30; ++i; break;	// ��ȡʵʱ������
    case S_ENCL : cmd[i] = 0x31; ++i; break;	// ��ȡ�������Ի�У׼��ı������?
		case S_CLKI : cmd[i] = 0x32; ++i; break;	// ��ȡ����������
    case S_TPOS : cmd[i] = 0x33; ++i; break;	// ��ȡ���Ŀ��λ��??
    case S_SPOS : cmd[i] = 0x34; ++i; break;	// ��ȡ���ʵʱ�趨��Ŀ��λ��??
		case S_VEL  : cmd[i] = 0x35; ++i; break;	// ��ȡ���ʵʱת��??
    case S_CPOS : cmd[i] = 0x36; ++i; break;	// ��ȡ���ʵʱλ��??
    case S_PERR : cmd[i] = 0x37; ++i; break;	// ��ȡ���λ�����
		case S_VBAT : cmd[i] = 0x38; ++i; break;	// ��ȡ��Ȧ��������ص�ѹ��Y42��
		case S_TEMP : cmd[i] = 0x39; ++i; break;	// ��ȡ���ʵʱ�¶ȣ�Y42��
    case S_FLAG : cmd[i] = 0x3A; ++i; break;	// ��ȡ���״̬��־�?
    case S_OFLAG: cmd[i] = 0x3B; ++i; break;	// ��ȡ����״̬��־λ
		case S_OAF  : cmd[i] = 0x3C; ++i; break;	// ��ȡ���״̬��־�? + ����״̬��־λ��Y42��
		case S_PIN  : cmd[i] = 0x3D; ++i; break;	// ��ȡ����״̬��Y42��
    default: break;
  }
	
	cmd[i] = (uint8_t)(time_ms >> 8);  ++i;	// ��ʱʱ��
	cmd[i] = (uint8_t)(time_ms >> 0);  ++i;

  cmd[i] = 0x6B; ++i;                   	// У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, i);
}

/**
  * @brief    ��ȡϵͳ����
  * @param    addr  ��������?
  * @param    s     ��ϵͳ��������
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
  uint8_t i = 0; __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  switch(s)                             // ������
  {
    case S_VBUS : cmd[i] = 0x24; ++i; break;	// ��ȡ���ߵ�ѹ
		case S_CBUS : cmd[i] = 0x26; ++i; break;	// ��ȡ���ߵ���
    case S_CPHA : cmd[i] = 0x27; ++i; break;	// ��ȡ�����??
		case S_ENCO : cmd[i] = 0x29; ++i; break;	// ��ȡ������ԭʼֵ
		case S_CLKC : cmd[i] = 0x30; ++i; break;	// ��ȡʵʱ������
    case S_ENCL : cmd[i] = 0x31; ++i; break;	// ��ȡ�������Ի�У׼��ı������?
		case S_CLKI : cmd[i] = 0x32; ++i; break;	// ��ȡ����������
    case S_TPOS : cmd[i] = 0x33; ++i; break;	// ��ȡ���Ŀ��λ��??
    case S_SPOS : cmd[i] = 0x34; ++i; break;	// ��ȡ���ʵʱ�趨��Ŀ��λ��??
		case S_VEL  : cmd[i] = 0x35; ++i; break;	// ��ȡ���ʵʱת��??
    case S_CPOS : cmd[i] = 0x36; ++i; break;	// ��ȡ���ʵʱλ��??
    case S_PERR : cmd[i] = 0x37; ++i; break;	// ��ȡ���λ�����
		case S_VBAT : cmd[i] = 0x38; ++i; break;	// ��ȡ��Ȧ��������ص�ѹ��Y42��
		case S_TEMP : cmd[i] = 0x39; ++i; break;	// ��ȡ���ʵʱ�¶ȣ�Y42��
    case S_FLAG : cmd[i] = 0x3A; ++i; break;	// ��ȡ���״̬��־�?
    case S_OFLAG: cmd[i] = 0x3B; ++i; break;	// ��ȡ����״̬��־λ
		case S_OAF  : cmd[i] = 0x3C; ++i; break;	// ��ȡ���״̬��־�? + ����״̬��־λ��Y42��
		case S_PIN  : cmd[i] = 0x3D; ++i; break;	// ��ȡ����״̬��Y42��
    default: break;
  }

  cmd[i] = 0x6B; ++i;                   // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, i);
}

/**********************************************************
*** ��д������������
**********************************************************/
/**
  * @brief    �޸ĵ��ID��ַ
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    id			 ��Ĭ�ϵ��IDΪ1�����޸�Ϊ1-255��0Ϊ�㲥��ַ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Motor_ID(uint8_t addr, bool svF, uint8_t id)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xAE;                       // ������
  cmd[2] =  0x4B;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  id;                  				// Ĭ�ϵ��IDΪ1�����޸�Ϊ1-255��0Ϊ�㲥��ַ
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸�ϸ��ֵ
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    mstep		 ��Ĭ��ϸ��Ϊ16�����޸�Ϊ1-2556��0Ϊ256ϸ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_MicroStep(uint8_t addr, bool svF, uint8_t mstep)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x84;                       // ������
  cmd[2] =  0x8A;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  mstep;                 	 		// Ĭ��ϸ��Ϊ16�����޸�Ϊ1-2556��0Ϊ256ϸ��
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸ĵ�����?
  * @param    addr     ��������?
  * @param    pdf		 	 ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_PDFlag(uint8_t addr, bool pdf)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x50;                       // ������
  cmd[2] =  pdf;                 	 			// ������?
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ��ȡѡ�����״̬��Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Opt_Param_Sta(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x1A;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸ĵ�����ͣ�Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    mottype	 ��������ͣ�Ĭ���?0��0��ʾ1.8�㲽�������??1��ʾ0.9�㲽�����??
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Motor_Type(uint8_t addr, bool svF, bool mottype)
{
  __IO static uint8_t cmd[16] = {0}; uint8_t MotType = 0;
  
	if(mottype) { MotType = 25; } else { MotType = 50; }
	
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xD7;                       // ������
  cmd[2] =  0x35;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  MotType;                 	 	// ������ͣ�??0��ʾ0.9�㲽�������??1��ʾ1.8�㲽�����??
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸Ĺ̼����ͣ�Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    fwtype	 ���̼����ͣ�Ĭ��Ϊ0��0ΪX�̼���1ΪEmm�̼�
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Firmware_Type(uint8_t addr, bool svF, bool fwtype)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xD5;                       // ������
  cmd[2] =  0x69;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  fwtype;                 	 	// ������ͣ�??25��ʾ0.9�㲽�������??50��ʾ1.8�㲽�����??
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸Ŀ���/�ջ�����ģʽ��Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    ctrl_mode������ģʽ��Ĭ��Ϊ1,0Ϊ����ģʽ��1Ϊ�ջ�FOCģʽ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Ctrl_Mode(uint8_t addr, bool svF, bool ctrl_mode)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x46;                       // ������
  cmd[2] =  0x69;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  ctrl_mode;                  // ����ģʽ��Ĭ��Ϊ1,0Ϊ����ģʽ��1Ϊ�ջ�FOCģʽ
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸ĵ���˶�������Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    dir			 ������˶�������Ĭ��ΪCW��0ΪCW��˳ʱ�뷽�򣩣�1ΪCCW
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Motor_Dir(uint8_t addr, bool svF, bool dir)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xD4;                       // ������
  cmd[2] =  0x60;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  dir;                  			// ����˶�������Ĭ��ΪCW��0ΪCW��˳ʱ�뷽�򣩣�1ΪCCW
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸������������ܣ�Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    lock		 �������������ܣ�Ĭ��ΪDisable��0ΪDisable��1ΪEnable
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Lock_Btn(uint8_t addr, bool svF, bool lock)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xD0;                       // ������
  cmd[2] =  0xB3;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  lock;                  			// �����������ܣ�Ĭ��ΪDisable��0ΪDisable����1ΪEnable
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸������ٶ�ֵ�Ƿ���С10�����루Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    s_vel		 �������ٶ�ֵ�Ƿ���С10�����룬Ĭ��ΪDisable��0ΪDisable��1ΪEnable
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_S_Vel(uint8_t addr, bool svF, bool s_vel)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x4F;                       // ������
  cmd[2] =  0x71;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  s_vel;                  		// �����ٶ�ֵ�Ƿ���С10�����룬Ĭ��ΪDisable��0ΪDisable��1ΪEnable
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 6);
}

/**
  * @brief    �޸Ŀ���ģʽ��������
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    om_ma 	 ������ģʽ������������λmA
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_OM_mA(uint8_t addr, bool svF, uint16_t om_ma)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x44;                       // ������
  cmd[2] =  0x33;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  (uint8_t)(om_ma >> 8);			// ����ģʽ������������λmA
	cmd[5] =  (uint8_t)(om_ma >> 0);
  cmd[6] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 7);
}

/**
  * @brief    �޸ıջ�ģʽ������
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    foc_mA 	 ���ջ�ģʽ����������λmA
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_FOC_mA(uint8_t addr, bool svF, uint16_t foc_mA)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x45;                       // ������
  cmd[2] =  0x66;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  (uint8_t)(foc_mA >> 8);			// �ջ�ģʽ����������λmA
	cmd[5] =  (uint8_t)(foc_mA >> 0);
  cmd[6] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 7);
}

/**
  * @brief    ��ȡPID����
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_PID_Params(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x21;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸�PID����
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    kp 	 		 ������ϵ����Ĭ��ΪY42/18000
	* @param    ki 	 		 ������ϵ����Ĭ��ΪY42/10
	* @param    kd 	 		 ��΢��ϵ����Ĭ��ΪY42/18000
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_PID_Params(uint8_t addr, bool svF, uint32_t kp, uint32_t ki, uint32_t kd)
{
  __IO static uint8_t cmd[20] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0x4A;                      // ������
  cmd[2]  =  0xC3;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(kp >> 24);				// kp
	cmd[5]  =  (uint8_t)(kp >> 16);
	cmd[6]  =  (uint8_t)(kp >> 8);
	cmd[7]  =  (uint8_t)(kp >> 0);
	cmd[8]  =  (uint8_t)(ki >> 24);				// ki
	cmd[9]  =  (uint8_t)(ki >> 16);
	cmd[10] =  (uint8_t)(ki >> 8);
	cmd[11] =  (uint8_t)(ki >> 0);
	cmd[12] =  (uint8_t)(kd >> 24);				// kd
	cmd[13] =  (uint8_t)(kd >> 16);
	cmd[14] =  (uint8_t)(kd >> 8);
	cmd[15] =  (uint8_t)(kd >> 0);
  cmd[16] =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 17);
}

/**
  * @brief    ��ȡDMX512Э�������Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_DMX512_Params(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x49;                       // ������
	cmd[2] =  0x78;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ��ȡDMX512Э�������Y42��
  * @param    addr  		��������?
  * @param    svF   		���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    tch				����ͨ������Ĭ��Ϊ192����ֵҪ������ DMX512 ����������ͨ����һ��
	* @param    nch				��ÿ�����ռ�õ�ͨ������Ĭ���?1��1Ϊ��ͨ��ģʽ,2Ϊ˫ͨ��ģʽ
	* @param    mode			���˶�ģʽ��Ĭ��Ϊ1��0��ʾ���λ��ģʽ�˶���??1��ʾ��������ʽλ���˶�
	* @param    vel				����ͨ��ģʽ���˶��ٶȣ�Ĭ��ֵΪ1000�� ��λRPM�� ��1000RPM��
	* @param    acc				�����ٶȣ�acc=������ֵ/8=125������ʱ���˵����??5.3.12 λ��ģʽ���ƣ�Emm����
	* @param    vel_step	��˫ͨ��ģʽ�ٶȲ�����Ĭ��ֵΪ 10�� ������˶��ٶ��?(ͨ��ֵ * 10)RPM
	* @param    pos_step	��˫ͨ��ģʽ�˶�������Ĭ��ֵΪ 100�� �����ת���Ƕ��?(ͨ��ֵ * 10.0)��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_DMX512_Params(uint8_t addr, bool svF, uint16_t tch, uint8_t nch, uint8_t mode, uint16_t vel, uint16_t acc, uint16_t vel_step, uint32_t pos_step)
{
  __IO static uint8_t cmd[32] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xD9;                      // ������
  cmd[2]  =  0x90;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(tch >> 8);     	// ��ͨ����
  cmd[5]  =  (uint8_t)(tch >> 0);
	cmd[6]  =  nch;                       // ÿ�����ռ�õ�ͨ����??
	cmd[7]  =  mode;                      // �˶�ģʽ
	cmd[8]  =  (uint8_t)(vel >> 8);     	// ��ͨ��ģʽ���˶��ٶ�
  cmd[9]  =  (uint8_t)(vel >> 0);
	cmd[10] =  (uint8_t)(acc >> 8);     	// ˫ͨ��ģʽ�ٶȲ���
  cmd[11] =  (uint8_t)(acc >> 0);
	cmd[12] =  (uint8_t)(vel_step >> 8);  // ˫ͨ��ģʽ�ٶȲ���
  cmd[13] =  (uint8_t)(vel_step >> 0);
  cmd[14]  = (uint8_t)(pos_step >> 24);	// ˫ͨ��ģʽ�˶�����
  cmd[15]  = (uint8_t)(pos_step >> 16);
  cmd[16] =  (uint8_t)(pos_step >> 8);
  cmd[17] =  (uint8_t)(pos_step >> 0);
  cmd[18] =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 19);
}

/**
  * @brief    ��ȡλ�õ��ﴰ�ڣ�Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Pos_Window(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x41;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸�λ�õ��ﴰ�ڣ�Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    prw 	 	 ��λ�õ��ﴰ�ڣ�Ĭ��ֵΪ8����0.8��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Pos_Window(uint8_t addr, bool svF, uint16_t prw)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xD1;                       // ������
  cmd[2] =  0x07;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  (uint8_t)(prw >> 8);				// λ�õ��ﴰ�ڣ�Ĭ��ֵΪ8����0.8��
	cmd[5] =  (uint8_t)(prw >> 0);
  cmd[6] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 7);
}

/**
  * @brief    ��ȡ���ȹ������������ֵ��Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Otocp(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x13;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸Ĺ��ȹ������������ֵ��Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    otp 	 	 �����ȱ��������ֵ��Ĭ��??100��
	* @param    ocp 	 	 ���������������ֵ��Ĭ��??6600mA
	* @param    time_ms  �����ȹ������ʱ�䣬Ĭ��??1000ms
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Otocp(uint8_t addr, bool svF, uint16_t otp, uint16_t ocp, uint16_t time_ms)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xD3;                      // ������
  cmd[2]  =  0x56;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(otp >> 8);				// ���ȱ���������?
	cmd[5]  =  (uint8_t)(otp >> 0);
	cmd[6]  =  (uint8_t)(ocp >> 8);				// ��������������?
	cmd[7]  =  (uint8_t)(ocp >> 0);
	cmd[8]  =  (uint8_t)(time_ms >> 8);		// ���ȹ������ʱ��??
	cmd[9]  =  (uint8_t)(time_ms >> 0);
  cmd[10] =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 11);
}

/**
  * @brief    ��ȡ������������ʱ�䣨Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Heart_Protect(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x16;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸�������������ʱ�䣨Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    hp 	 	 	 ����������ʱ�䣬��λ��ms
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Heart_Protect(uint8_t addr, bool svF, uint32_t hp)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0x68;                      // ������
  cmd[2]  =  0x38;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(hp >> 24);				// ��������ʱ�䣬��λ��ms
	cmd[5]  =  (uint8_t)(hp >> 16);
	cmd[6]  =  (uint8_t)(hp >> 8);
	cmd[7]  =  (uint8_t)(hp >> 0);
  cmd[8]  =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 9);
}

/**
  * @brief    ��ȡ�����޷�/����ϵ����Y42��
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Integral_Limit(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x23;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 3);
}

/**
  * @brief    �޸Ļ����޷�/����ϵ����Y42��
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    il 	 	 	 �������޷���Ĭ��ֵΪ65535
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Integral_Limit(uint8_t addr, bool svF, uint32_t il)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0x4B;                      // ������
  cmd[2]  =  0x57;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(il >> 24);				// ��������ʱ�䣬��λ��ms
	cmd[5]  =  (uint8_t)(il >> 16);
	cmd[6]  =  (uint8_t)(il >> 8);
	cmd[7]  =  (uint8_t)(il >> 0);
  cmd[8]  =  0x6B;                      // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 9);
}

/**********************************************************
*** ��ȡ����������������
**********************************************************/
/**
  * @brief    ��ȡϵͳ״̬����
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_System_State_Params(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x43;                       // ������
	cmd[2] =  0x7A;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}

/**
  * @brief    ��ȡ�������ò���
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Motor_Conf_Params(uint8_t addr)
{
  __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x42;                       // ������
	cmd[2] =  0x6C;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)cmd, 4);
}



/**
***********************************************************
***********************************************************
*** 
***
*** @brief	�����ǰ���Ӧ������ص�Y42���������ϵĺ�����Y42��
***
*** 
***********************************************************
***********************************************************
***/
/**********************************************************
*** ������������
**********************************************************/
/**
  * @brief    ����������У׼ - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Trig_Encoder_Cal(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x06;                       // ������
  cmd[2] =  0x45;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ���������Y42�� - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Reset_Motor(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x08;                       // ������
  cmd[2] =  0x97;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ����ǰλ������ - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Reset_CurPos_To_Zero(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0A;                       // ������
  cmd[2] =  0x6D;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �����ת����?? - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Reset_Clog_Pro(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0E;                       // ������
  cmd[2] =  0x52;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �ָ��������� - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Restore_Motor(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0F;                       // ������
  cmd[2] =  0x5F;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**********************************************************
*** �˶���������
**********************************************************/
/**
  * @brief    ʹ���źſ��� - ���ص�����ָ����
  * @param    addr  ��������?
  * @param    state ��ʹ��״̬     ��trueΪʹ�ܵ����falseΪ�رյ��??
  * @param    snF   �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_En_Control(uint8_t addr, bool state, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF3;                       // ������
  cmd[2] =  0xAB;                       // ������
  cmd[3] =  (uint8_t)state;             // ʹ��״̬
  cmd[4] =  snF;                        // ���ͬ���˶����?
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 6; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �ٶ�ģʽ - ���ص�����ָ����
  * @param    addr��������?
  * @param    dir ������       ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�       ����Χ0 - 5000RPM
  * @param    acc �����ٶ�     ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    snF �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};

  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF6;                       // ������
  cmd[2] =  dir;                        // ����
  cmd[3] =  (uint8_t)(vel >> 8);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[4] =  (uint8_t)(vel >> 0);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[5] =  acc;                        // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6] =  snF;                        // ���ͬ���˶����?
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 8; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    λ��ģʽ - ���ص�����ָ����
  * @param    addr��������?
  * @param    dir ������        ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    clk ��������      ����Χ0- (2^32 - 1)��
  * @param    raF ���˶���־��0Ϊ�����һ����Ŀ��λ�ý������λ���˶���1Ϊ����ֵ�˶���2��Ե�ǰ���ʵʱλ�ý������λ���˶�??
  * @param    snF �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, uint8_t raF, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFD;                      // ������
  cmd[2]  =  dir;                       // ����
  cmd[3]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[4]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[5]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[10] =  raF;                       // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[11] =  snF;                       // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[12] =  0x6B;                      // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 13; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ���ÿ���λ��ģʽ���˶�����
  * @param    addr��������?
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    raF ���˶���־��0Ϊ�����һ����Ŀ��λ�ý������λ���˶���1Ϊ����ֵ�˶���2��Ե�ǰ���ʵʱλ�ý������λ���˶�??
  * @param    snF �����ͬ�����? ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Set_QPos_Params(uint8_t addr, uint16_t vel, uint8_t acc, uint8_t raF, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xF1;                      // ������
  cmd[2]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[3]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[4]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[5] =  raF;                        // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[6] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 8; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ����λ��ģʽ
  * @param    addr��������?
  * @param    clk ���������������ţ���Ĭ��16ϸ����+3200��������תһȦ����-3200�ͷ�����תһȦ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_QPos_Control(uint8_t addr, int32_t clk)
{
  uint8_t j = 0, cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFC;                      // ������
  cmd[2]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[3]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[4]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[5]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[6] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 7; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ����ֹͣ - ���ص�����ָ����
  * @param    addr  ��������?
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Stop_Now(uint8_t addr, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xFE;                       // ������
  cmd[2] =  0x98;                       // ������
  cmd[3] =  snF;                        // ���ͬ���˶����?
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 5; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ���ͬ���˶�?? - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Synchronous_motion(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xFF;                       // ������
  cmd[2] =  0x66;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**********************************************************
*** ԭ���������??
**********************************************************/
/**
  * @brief    ���õ�Ȧ��������λ�� - ���ص�����ָ����
  * @param    addr  ��������?
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Origin_Set_O(uint8_t addr, bool svF)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x93;                       // ������
  cmd[2] =  0x88;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 5; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �������� - ���ص�����ָ����
  * @param    addr   ��������?
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9A;                       // ������
  cmd[2] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[3] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 5; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ǿ���жϲ��˳����� - ���ص�����ָ����
  * @param    addr  ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Origin_Interrupt(uint8_t addr)
{
  uint8_t j = 0, cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9C;                       // ������
  cmd[2] =  0x48;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 4; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �޸Ļ������?? - ���ص�����ָ����
  * @param    addr  ��������?
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    o_dir  �����㷽��0ΪCW������ֵΪCCW
  * @param    o_vel  �������ٶȣ���λ��RPM��ת/���ӣ�
  * @param    o_tm   �����㳬ʱʱ�䣬��λ������
  * @param    sl_vel ������λ��ײ������ת�٣���λ��RPM��ת/���ӣ�
  * @param    sl_ma  ������λ��ײ�������������λ��Ma��������
  * @param    sl_ms  ������λ��ײ������ʱ�䣬��λ��Ms�����룩
  * @param    potF   ���ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Origin_Modify_Params(uint8_t addr, bool svF, uint8_t o_mode, uint8_t o_dir, uint16_t o_vel, uint32_t o_tm, uint16_t sl_vel, uint16_t sl_ma, uint16_t sl_ms, bool potF)
{
  uint8_t j = 0, cmd[32] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x4C;                       // ������
  cmd[2] =  0xAE;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ�������??2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[5] =  o_dir;                      // ���㷽��
  cmd[6]  =  (uint8_t)(o_vel >> 8);     // �����ٶ�(RPM)��8λ�ֽ�
  cmd[7]  =  (uint8_t)(o_vel >> 0);     // �����ٶ�(RPM)��8λ�ֽ� 
  cmd[8]  =  (uint8_t)(o_tm >> 24);     // ���㳬ʱʱ��(bit24 - bit31)
  cmd[9]  =  (uint8_t)(o_tm >> 16);     // ���㳬ʱʱ��(bit16 - bit23)
  cmd[10] =  (uint8_t)(o_tm >> 8);      // ���㳬ʱʱ��(bit8  - bit15)
  cmd[11] =  (uint8_t)(o_tm >> 0);      // ���㳬ʱʱ��(bit0  - bit7 )
  cmd[12] =  (uint8_t)(sl_vel >> 8);    // ����λ��ײ������ת��(RPM)��8λ�ֽ�
  cmd[13] =  (uint8_t)(sl_vel >> 0);    // ����λ��ײ������ת��(RPM)��8λ�ֽ� 
  cmd[14] =  (uint8_t)(sl_ma >> 8);     // ����λ��ײ���������??(Ma)��8λ�ֽ�
  cmd[15] =  (uint8_t)(sl_ma >> 0);     // ����λ��ײ���������??(Ma)��8λ�ֽ� 
  cmd[16] =  (uint8_t)(sl_ms >> 8);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[17] =  (uint8_t)(sl_ms >> 0);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[18] =  potF;                      // �ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  cmd[19] =  0x6B;                      // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 20; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ��ȡ��ײ���㷵�ؽǶȣ�X42S/Y42�� - ���ص�����ָ����
  * @param    addr     ��������?
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void X_V2_MMCL_Origin_Read_SL_RP(uint8_t addr)
{
  uint8_t j = 0; __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x3F;                       // ������
  cmd[2] =  0x6B;                       // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 3; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    �޸���ײ���㷵�ؽǶȣ�X42S/Y42�� - ���ص�����ָ����
  * @param    addr     ��������?
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    sl_rp 	 ����ײ���㷵�ؽǶȣ���λ0.1�㣬����40������4.0��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void X_V2_MMCL_Origin_Modify_SL_RP(uint8_t addr, bool svF, uint16_t sl_rp)
{
  uint8_t j = 0; __IO static uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0x5C;                      // ������
  cmd[2]  =  0xAC;                      // ������
  cmd[3]  =  svF;                       // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4]  =  (uint8_t)(sl_rp >> 8);			// ��ײ���㷵�ؽǶȣ���λ0.1��
	cmd[5]  =  (uint8_t)(sl_rp >> 0);
  cmd[6]  =  0x6B;                      // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < 7; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**********************************************************
*** ��ȡϵͳ��������
**********************************************************/
/**********************************************************
*** ��ȡϵͳ��������
**********************************************************/
/**
  * @brief    ��ʱ������Ϣ���Y42��
  * @param    addr  	��������?
  * @param    s     	��ϵͳ��������
	* @param    time_ms ����ʱʱ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Auto_Return_Sys_Params_Timed(uint8_t addr, SysParams_t s, uint16_t time_ms)
{
  uint8_t i = 0, j = 0; uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  cmd[i] = 0x11; ++i;                   // ������

  cmd[i] = 0x18; ++i;                   // ������

  switch(s)                             // ��Ϣ������
  {
    case S_VBUS : cmd[i] = 0x24; ++i; break;	// ��ȡ���ߵ�ѹ
		case S_CBUS : cmd[i] = 0x26; ++i; break;	// ��ȡ���ߵ���
    case S_CPHA : cmd[i] = 0x27; ++i; break;	// ��ȡ�����??
		case S_ENCO : cmd[i] = 0x29; ++i; break;	// ��ȡ������ԭʼֵ
		case S_CLKC : cmd[i] = 0x30; ++i; break;	// ��ȡʵʱ������
    case S_ENCL : cmd[i] = 0x31; ++i; break;	// ��ȡ�������Ի�У׼��ı������?
		case S_CLKI : cmd[i] = 0x32; ++i; break;	// ��ȡ����������
    case S_TPOS : cmd[i] = 0x33; ++i; break;	// ��ȡ���Ŀ��λ��??
    case S_SPOS : cmd[i] = 0x34; ++i; break;	// ��ȡ���ʵʱ�趨��Ŀ��λ��??
		case S_VEL  : cmd[i] = 0x35; ++i; break;	// ��ȡ���ʵʱת��??
    case S_CPOS : cmd[i] = 0x36; ++i; break;	// ��ȡ���ʵʱλ��??
    case S_PERR : cmd[i] = 0x37; ++i; break;	// ��ȡ���λ�����
		case S_VBAT : cmd[i] = 0x38; ++i; break;	// ��ȡ��Ȧ��������ص�ѹ��Y42��
		case S_TEMP : cmd[i] = 0x39; ++i; break;	// ��ȡ���ʵʱ�¶ȣ�Y42��
    case S_FLAG : cmd[i] = 0x3A; ++i; break;	// ��ȡ���״̬��־�?
    case S_OFLAG: cmd[i] = 0x3B; ++i; break;	// ��ȡ����״̬��־λ
		case S_OAF  : cmd[i] = 0x3C; ++i; break;	// ��ȡ���״̬��־�? + ����״̬��־λ��Y42��
		case S_PIN  : cmd[i] = 0x3D; ++i; break;	// ��ȡ����״̬��Y42��
    default: break;
  }
	
	cmd[i] = (uint8_t)(time_ms >> 8);  ++i;	// ��ʱʱ��
	cmd[i] = (uint8_t)(time_ms >> 0);  ++i;

  cmd[i] = 0x6B; ++i;                   	// У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < i; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**
  * @brief    ��ȡϵͳ���� - ���ص�����ָ����
  * @param    addr  ��������?
  * @param    s     ��ϵͳ��������
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_MMCL_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
  uint8_t i = 0, j = 0; uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  switch(s)                             // ������
  {
    case S_VBUS : cmd[i] = 0x24; ++i; break;	// ��ȡ���ߵ�ѹ
		case S_CBUS : cmd[i] = 0x26; ++i; break;	// ��ȡ���ߵ���
    case S_CPHA : cmd[i] = 0x27; ++i; break;	// ��ȡ�����??
		case S_ENCO : cmd[i] = 0x29; ++i; break;	// ��ȡ������ԭʼֵ
		case S_CLKC : cmd[i] = 0x30; ++i; break;	// ��ȡʵʱ������
    case S_ENCL : cmd[i] = 0x31; ++i; break;	// ��ȡ�������Ի�У׼��ı������?
		case S_CLKI : cmd[i] = 0x32; ++i; break;	// ��ȡ����������
    case S_TPOS : cmd[i] = 0x33; ++i; break;	// ��ȡ���Ŀ��λ��??
    case S_SPOS : cmd[i] = 0x34; ++i; break;	// ��ȡ���ʵʱ�趨��Ŀ��λ��??
		case S_VEL  : cmd[i] = 0x35; ++i; break;	// ��ȡ���ʵʱת��??
    case S_CPOS : cmd[i] = 0x36; ++i; break;	// ��ȡ���ʵʱλ��??
    case S_PERR : cmd[i] = 0x37; ++i; break;	// ��ȡ���λ�����
		case S_VBAT : cmd[i] = 0x38; ++i; break;	// ��ȡ��Ȧ��������ص�ѹ��Y42��
		case S_TEMP : cmd[i] = 0x39; ++i; break;	// ��ȡ���ʵʱ�¶ȣ�Y42��
    case S_FLAG : cmd[i] = 0x3A; ++i; break;	// ��ȡ���״̬��־�?
    case S_OFLAG: cmd[i] = 0x3B; ++i; break;	// ��ȡ����״̬��־λ
		case S_OAF  : cmd[i] = 0x3C; ++i; break;	// ��ȡ���״̬��־�? + ����״̬��־λ��Y42��
		case S_PIN  : cmd[i] = 0x3D; ++i; break;	// ��ȡ����״̬��Y42��
    default: break;
  }

  cmd[i] = 0x6B; ++i;                   // У���ֽ�
  
  // ���ص�ǰ�������������
  for(j=0; j < i; j++) { MMCL_cmd[MMCL_count] = cmd[j]; ++MMCL_count; }
}

/**********************************************************
*** ��д������������
**********************************************************/
