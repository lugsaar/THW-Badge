#include "thw-badge.h"

volatile uint8_t AppNumber;
volatile uint8_t Stop;
volatile uint8_t LedCycle;
volatile uint8_t Buttons;

FuncPntr Application[] = {
    APPLICATIONS};

FuncPntr AppTimerInterrupt[] = {
    APPTIMERS};

FuncPntrBtn AppButtonInterrupt[] = {
    APPBUTTONS};

// volatile unsigned long sleep_time = 0x3FFu;

int main(void)
{

  cli(); // stop all interrupts

  initPorts();
  initPowersave();
  initButtons();

  // //START TIMER SETUP
  // //TIMER SETUP for highly preceise timed measurements
  // // turn on CTC mode
  // TCCR1A = 0;// set entire TCCR1A register to 0
  // TCCR1B = 0;// same for TCCR1B
  // TCCR1B |= (1 << WGM12);
  // // Set prescaler to 8 --> CLK / 8 = 1 MHz
  // TCCR1B |= (1 << CS11); // | (1 << CS10);

  // //initialize counter value to 0;
  // TCNT1  = 0;

  // // set timer count for 1KHz increments
  // OCR1A = 1000 - 1;

  // // enable timer compare interrupt
  // TIMSK1 |= (1 << OCIE1A);

  sei();

  // Start Launcher in sleepmode
  Stop = 0;
  // Application[0]();
  App_Wheel();

  return 0;
}

void initPorts(void)
{

  // Set Pin 0 - 4 of PORTC and PORTD as output.
  DDRC |= 0x1Fu;
  DDRD |= 0x1Fu;

  // Enable pull-ups on button input pins
  PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
}

void initPowersave(void) {
  //Disable Universal-Serial-Interface and AD-Converter
	PRR |= (1 << PRUSART0) | (1 << PRADC);
  //Select sleepmode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  return;
}




void initButtons(void)
{
  // Set Pin Change Interrupt for button input pins
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT1) | (1 << PCINT2);
}

void initCycleTimer(uint8_t _Speed)
{
  //_Speed 0 means all LEDs permanent
  if (_Speed == 0)
  {
    LedCycle = 0xff;
    return;
  }
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  // CTC mode
  TCCR0A |= (1 << WGM01);
  // CLK / 64
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // Decrement counter to avoid missing interrupt
  if (TCNT0 > OCR0A)
  {
    TCNT0 = OCR0A - 1;
  }
  // Timer0 Output Compare A Match Interrupt
  OCR0A = _Speed;
  TIMSK0 |= (1 << OCIE0A);
  return;
}

void initAppTimer(uint16_t _Speed)
{
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  // CTC mode, CLK / 64
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Decrement counter to avoid missing interrupt
  if (TCNT1 > OCR1A)
  {
    TCNT1 = OCR1A - 1;
  }
  // Timer1 Output Compare A Match Interrupt
  OCR1A = _Speed;
  TIMSK1 |= (1 << OCIE1A);
  return;
}

void activateLeds(uint8_t _Leds)
{
  PORTC = (0x1F & _Leds);
  // PORTC = ~(LedCycle & _Leds);
  return;
}

void debounceButtons(volatile uint8_t *_port, uint8_t _mask)
{
  uint8_t port_buffer;
  uint8_t debounce_buffer;
  for (debounce_buffer = 0; debounce_buffer != 0xff;)
  {
    debounce_buffer <<= 1;
    port_buffer = *_port;
    _delay_us(15);
    if ((*_port & _mask) == (port_buffer & _mask))
    {
      debounce_buffer |= 0x01;
    }
  }
  return;
}

// ISR(TIMER1_COMPA_vect) {
//   Interrupt every ms

//   static unsigned int sys_tick_ms = 0;

//   cli();
//   //write your timer code here

//   if( (sys_tick_ms & sleep_time) == 0u ){

//     PORTB ^= (1 << PB5);

//   }

//   sys_tick_ms += 1;

//   sei();
//   return;

// }

ISR(TIMER0_COMPA_vect)
{
  cli();
  LedCycle <<= 1;
  if (LedCycle == 0)
  {
    LedCycle = 1;
  }
  sei();
  return;
}

ISR(TIMER1_COMPA_vect)
{
  cli();
  // AppTimerInterrupt[AppNumber]();
  TimerInt_Wheel();
  sei();
  return;
}

ISR(PCINT0_vect)
{
  // Wake up
  cli();
  debounceButtons(&PINB, (1 << PB1) | (1 << PB2));
  if (!(PINB & (1 << PB1)))
  {
    Buttons |= (1 << BUTTON1);
  }
  if (!(PINB & (1 << PB2)))
  {
    Buttons |= (1 << BUTTON2);
  }
  // if (!(PINB & (1 << PB0)))
  // {
  //   Buttons |= (1 << BUTTON3);
  // }
  // AppButtonInterrupt[AppNumber](Buttons);
  ButtonInt_Wheel(Buttons);
  Buttons = 0;

  // debounceButtons(&PINB, (1 << PB2) | (1 << PB1));
  // if (!(PINB & (1 << PB1)))
  // {
  //   if (sleep_time >= 0x3u)
  //   {
  //     sleep_time = (sleep_time >> 1);
  //   }
  // }
  // if (!(PINB & (1 << PB2)))
  // {
  //   if (sleep_time <= 0x3FFFu)
  //   {
  //     sleep_time = (sleep_time << 1) | 0x1u;
  //   }
  // }

  sei();
  return;
}
