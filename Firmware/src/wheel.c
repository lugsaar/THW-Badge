#include "wheel.h"

uint16_t WheelSpeed;
Direction_t Direction;

void App_Wheel(void) {
   WheelSpeed = 800;
   Direction.Forward = 0;
   initCycleTimer(16);
   initAppTimer(WheelSpeed);

   while (Stop == 0) {
      activateLeds(Leds);
   }
   return;
}

void TimerInt_Wheel(void) {
   if (Direction.Forward == 1) {
      Leds <<= 1;
      if (Leds == 1024) {
         Leds = 1;
      }
   }
   if (Direction.Forward == 0) {
      Leds >>= 1;
      if (Leds == 0) {
         Leds = 1024;
      }
   }
   return;
}

void ButtonInt_Wheel(uint8_t _Buttons) {
   if (BUTTON3_PRESSED) {
      Stop = 1;
      return;
   }
   if (BUTTON1_PRESSED && (WheelSpeed < 10000) && (Direction.Forward == 1)) {
      WheelSpeed += 400;
   }
   if (BUTTON2_PRESSED && (WheelSpeed > 500) && (Direction.Forward == 1)) {
      WheelSpeed -= 400;
   }
   if (BUTTON1_PRESSED && (WheelSpeed > 500) && (Direction.Forward == 0)) {
      WheelSpeed -= 400;
   }
   if (BUTTON2_PRESSED && (WheelSpeed < 10000) && (Direction.Forward == 0)) {
      WheelSpeed += 400;
   }
   if (WheelSpeed == 10000) {
      Direction.Forward = !Direction.Forward;
   }
   initAppTimer(WheelSpeed);
   return;
}

