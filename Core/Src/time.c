#include "time.h"

/**
  * @brief  将毫秒计数值转换为 "MM:SS.T" 格式字符串
  * @param  elapsed_ms : 已计时的毫秒数
  * @param  buf        : 输出缓冲区 (至少 8 字节)
  */
static void time_to_str(uint32_t elapsed_ms, char *buf)
{
  uint32_t total_tenths = elapsed_ms / 100;      /* 总共十分之一秒 */
  uint8_t  tenths       = total_tenths % 10;     /* 十分位 (0-9)   */
  uint32_t total_secs   = total_tenths / 10;     /* 总共秒数        */
  uint8_t  seconds      = total_secs % 60;       /* 秒 (0-59)       */
  uint8_t  minutes      = total_secs / 60;       /* 分 (0-99)       */

  buf[0] = '0' + (minutes / 10);
  buf[1] = '0' + (minutes % 10);
  buf[2] = ':';
  buf[3] = '0' + (seconds / 10);
  buf[4] = '0' + (seconds % 10);
  buf[5] = '.';
  buf[6] = '0' + tenths;
  buf[7] = '\0';
}

/**
  * @brief  绘制时间字符串 (MM:SS.T)
  * @note   数字使用 font16x24 (16×24), 分隔符使用 afont24x12 (12×24)
  *         二者高度均为 24px, 视觉对齐
  */
static void draw_time(uint8_t x, uint8_t y, const char *time_str)
{
  for (uint8_t i = 0; i < 7; i++)
  {
    if (time_str[i] >= '0' && time_str[i] <= '9')
    {
      /* 数字: font16x24 (16×24 中文数字字库) */
      char digit[2] = {time_str[i], '\0'};
      OLED_PrintString(x, y, digit, &font16x24, OLED_COLOR_NORMAL);
      x += 16;
    }
    else
    {
      /* 分隔符 : . : afont24x12 (12×24, 等高于中文数字) */
      OLED_PrintASCIIChar(x, y, time_str[i], &afont24x12, OLED_COLOR_NORMAL);
      x += 12;
    }
  }
}

/**
  * @brief  STATE_READY 画面 — 全零显示, 等待开始
  */
void draw_ready_screen(void)
{
  /* 状态标签 */
  OLED_PrintASCIIString(CENTER_X_6(13), 2, "--- READY ---",
                        &afont12x6, OLED_COLOR_NORMAL);

  /* 时间: 00:00.0 (数字=font16x24, 分隔符=afont24x12) */
  draw_time(TIME_CENTER_X, 18, "00:00.0");

  /* 底部提示 */
  OLED_PrintASCIIString(CENTER_X_6(16), 52, "[START] to begin",
                        &afont12x6, OLED_COLOR_NORMAL);
}

/**
  * @brief  STATE_RUNNING 画面 — 实时显示计时
  */
void draw_running_screen(uint32_t elapsed_ms)
{
  char time_buf[8];
  time_to_str(elapsed_ms, time_buf);

  /* 状态标签 + 运行指示器 */
  OLED_PrintASCIIString(CENTER_X_6(13), 2, ">> RUNNING <<",
                        &afont12x6, OLED_COLOR_NORMAL);

  /* 时间显示 */
  draw_time(TIME_CENTER_X, 18, time_buf);

  /* 底部提示 */
  OLED_PrintASCIIString(CENTER_X_6(14), 52, "[STOP] to stop",
                        &afont12x6, OLED_COLOR_NORMAL);
}

/**
  * @brief  STATE_STOPPED 画面 — 显示停止时刻的时间
  */
void draw_stopped_screen(uint32_t elapsed_ms)
{
  char time_buf[8];
  time_to_str(elapsed_ms, time_buf);

  /* 状态标签 */
  OLED_PrintASCIIString(CENTER_X_6(13), 2, "## STOPPED ##",
                        &afont12x6, OLED_COLOR_NORMAL);

  /* 最终时间 */
  draw_time(TIME_CENTER_X, 18, time_buf);

  /* 底部提示 */
  OLED_PrintASCIIString(CENTER_X_6(18), 52, "[RESET] to restart",
                        &afont12x6, OLED_COLOR_NORMAL);
}