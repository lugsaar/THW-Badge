#include "all_on.h"

void App_AllOn(void) {
   initCycleTimer(16);
   while (Stop == 0) {
      activateLeds(0x3ff);
   }
   return;
}

void TimerInt_AllOn(void) {
   return;
}

void ButtonInt_AllOn(uint8_t _Buttons) {
   if (BUTTON3_PRESSED) {
      Stop = 1;
   }
   return;
}
