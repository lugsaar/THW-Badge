#include "launcher.h"

uint8_t AppSelect;
uint8_t Leds;
uint8_t Blink;

void App_Launcher(void) {
   initCycleTimer(32);
   initAppTimer(3000);
   while (1) {
      if (AppNumber == 0) {
         if (Stop == 1) {
           Sleep();
         }
      }
      else {
         Application[AppNumber]();
         AppNumber = 0;
         Stop = 0;
         initAppTimer(3000);
      }
      LauncherLeds();
   }
   return;
}

void TimerInt_Launcher(void) {
   Blink = ~Blink;
   return;
}

void ButtonInt_Launcher(uint8_t _Buttons) {
   Stop = 0;
   if (BUTTON1_PRESSED) {
      AppSelect--;
   }
   if (BUTTON2_PRESSED) {
      AppSelect++;
   }
   if (AppSelect > 2) {
      AppSelect = 0;
      activateLeds(0);
      AppNumber = 0;
      Stop = 1;
      return;
   }
   if (BUTTON3_PRESSED) {
      AppNumber = AppSelect;
      if (AppSelect == 0) {
         AppSelect = 1;
      }
   }
   return;
}

void LauncherLeds(void) {
   if (AppSelect > 0) {
      Leds = (1 << (AppSelect - 1));
   }
   else {
      Leds = 0;
   }
   activateLeds(Leds & Blink);
   return;
}

void Sleep(void) {
   activateLeds(0);
   sleep_mode();
   return;
}
