/*
Init the cycle timer with a speed value 1-255.
The LEDs are cycled clockwise to avoid power drain.
A value of 0 stops cycling and enables all 8 LEDs permanently.
The LedCycle variable holds the current cycled LED.

The activateLeds function activates the given LEDs.

The Stop variable can be used as a helper to stop the application and return to the launcher.

Init the general purpose app timer with a speed value 1-65535.

Define a main function, a apptimer ISR and a button ISR for your application:
void App_<Appname>(void)
void TimerInt_<Appname>(void);
void ButtonInt_<Appname>(uint8_t _Buttons);
The _Buttons variable holds the button conditions:
BUTTON1_PRESSED
BUTTON2_PRESSED
BUTTON3_PRESSED

Application number 0 is reserved for the Launcher application.
*/

#ifndef TKS_BADGE_H
#define TKS_BADGE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2

#define BUTTON1_PRESSED (_Buttons & (1 << BUTTON1))
#define BUTTON2_PRESSED (_Buttons & (1 << BUTTON2))
#define BUTTON3_PRESSED (_Buttons & (1 << BUTTON3))

typedef void (*FuncPntr) (void);
typedef void (*FuncPntrBtn) (uint8_t);

//Application inclusions
#include "launcher.h"
#include "all_on.h"
#include "wheel.h"

//Application startfunction
#define APPLICATIONS\
   App_Launcher,\
   App_AllOn,\
   App_Wheel

//Application timer ISR
#define APPTIMERS\
   TimerInt_Launcher,\
   TimerInt_AllOn,\
   TimerInt_Wheel
   
//Application button ISR
#define APPBUTTONS\
   ButtonInt_Launcher,\
   ButtonInt_AllOn,\
   ButtonInt_Wheel

extern volatile uint8_t AppNumber;
extern volatile uint8_t Stop;
extern volatile uint8_t LedCycle;
extern volatile uint8_t Buttons;

void initPorts(void);
void initPowersave(void);
void initButtons(void);
void initCycleTimer(uint8_t _Speed);
void initAppTimer(uint16_t _Speed);
void activateLeds(uint8_t _Leds);
void debounceButtons(volatile uint8_t *_port, uint8_t _mask);

#endif
