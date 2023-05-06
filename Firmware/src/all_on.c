#include "all_on.h"

void App_AllOn(void) {
   initCycleTimer(32);
   while (Stop == 0) {
      activateLeds(0xff);
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
