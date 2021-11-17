#ifndef __VOLUME_H
#define __VOLUME_H
#include "main.h"

void Vol_Step(bool up);//up or down;
void Vol_Save(uint8_t slot);
void Vol_Read(uint8_t slot);
void Vol_Down_Temp(float relative_db_volume);

void Vol_In_While();
void Vol_In_Timer_2us();
void Vol_In_Timer_1ms();
void Vol_Init();

#endif