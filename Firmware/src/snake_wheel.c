#include "snake_wheel.h"

uint16_t SnakeWheelSpeed;

uint16_t temp_leds_left = 1;
uint16_t temp_leds_right = 1024;

void App_SnakeWheel(void) {
   SnakeWheelSpeed = 1600;

   initCycleTimer(16);
   initAppTimer(SnakeWheelSpeed);

   while (Stop == 0) {
      activateLeds(Leds);
   }
   return;
}

void TimerInt_SnakeWheel(void) {
   
   
   if (temp_leds_left == 1024) {
      temp_leds_left = 1;
   } else {
      temp_leds_left <<= 1;
   }
   
   if (temp_leds_right == 1) {
      temp_leds_right = 1024;
   } else {
      temp_leds_right >>= 1;
   }

   Leds = temp_leds_left | temp_leds_right;
   return;
}

void ButtonInt_SnakeWheel(uint8_t _Buttons) {
   if (BUTTON3_PRESSED) {
      Stop = 1;
      return;
   }
   if (BUTTON1_PRESSED && (SnakeWheelSpeed < 10000)) {
      SnakeWheelSpeed += 400;
   }
   if (BUTTON2_PRESSED && (SnakeWheelSpeed > 500)) {
      SnakeWheelSpeed -= 400;
   }
   
   initAppTimer(SnakeWheelSpeed);
   return;
}
