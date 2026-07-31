#ifndef __TIME_H__
#define __TIME_H__

#include "oled.h"

#define CENTER_X_6(len)  ((128 - (len) * 6) / 2)   /* afont8x6/12x6 居中 */
#define CENTER_X_8(len)  ((128 - (len) * 8) / 2)   /* afont16x8 居中 */
#define TIME_STR_WIDTH   104
#define TIME_CENTER_X    ((128 - TIME_STR_WIDTH) / 2)

void draw_ready_screen(void);
void draw_running_screen(uint32_t elapsed_ms);
void draw_stopped_screen(uint32_t elapsed_ms);

#endif // __TIME_H__