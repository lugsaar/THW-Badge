#ifndef APP_LAUNCHER_H
#define APP_LAUNCHER_H

#include "thw-badge.h"

extern FuncPntr Application[];

extern uint8_t AppSelect;
extern uint8_t Leds;
extern uint8_t Blink;

void App_Launcher(void);
void TimerInt_Launcher(void);
void ButtonInt_Launcher(uint8_t);
void LauncherLeds(void);
void Sleep(void);

#endif

