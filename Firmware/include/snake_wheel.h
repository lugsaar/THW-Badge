#ifndef SNAKE_WHEEL_H
#define SNAKE_WHEEL_H

#include "thw-badge.h"

extern uint16_t SnakeWheelSpeed;

void App_SnakeWheel(void);
void TimerInt_SnakeWheel(void);
void ButtonInt_SnakeWheel(uint8_t);

#endif

