#ifndef __LED_H
#define __LED_H

#include "main.h"

extern uint16_t g_cnt_led_show;
extern uint16_t g_cnt_led_bring;
void LED_Init();
void Led_In_While();
void LED_In_Timer();
void LED_Update(const char *string);

#endif