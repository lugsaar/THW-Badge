#ifndef APP_WHEEL_H
#define APP_WHEEL_H

#include "thw-badge.h"



extern uint16_t WheelSpeed;
extern Direction_t Direction;

void App_Wheel(void);
void TimerInt_Wheel(void);
void ButtonInt_Wheel(uint8_t);

#endif

