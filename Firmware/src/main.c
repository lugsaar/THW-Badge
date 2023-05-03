#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

volatile unsigned long sleep_time = 0x3FFu;



// the setup function runs once when you press reset or power the board
int main(void) {
  
  cli();//stop all interrupts

  // initialize digital pin LED_BUILTIN as an output.
  DDRB  |= (1 << PB5);
  
  //Enable pull-ups on button input pins
  PORTB |= (1 << PORTB1) | (1 << PORTB2) ;

  // Insert nop for synchronization


  //Set Pin Change Interrupt for button input pins
  PCICR  |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT1) | (1 << PCINT2) ;


  //START TIMER SETUP
  //TIMER SETUP for highly preceise timed measurements 
  // turn on CTC mode
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCCR1B |= (1 << WGM12);
  // Set prescaler to 8 --> CLK / 8 = 1 MHz
  TCCR1B |= (1 << CS11); // | (1 << CS10); 
  
  //initialize counter value to 0;
  TCNT1  = 0;
  
  // set timer count for 1KHz increments
  OCR1A = 1000 - 1;
  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();

  while(1){

  }

  return 0;
}


void debounceButtons(volatile uint8_t *_port, uint8_t _mask) {
	uint8_t port_buffer;
	uint8_t debounce_buffer;
	for (debounce_buffer = 0; debounce_buffer != 0xff;) {
		debounce_buffer <<= 1;
		port_buffer = *_port;
		_delay_us(15);
		if ((*_port & _mask) == (port_buffer & _mask)) {
			debounce_buffer |= 0x01;
		}
	}
   return;
}


ISR(TIMER1_COMPA_vect) {
  //Interrupt every ms 

  static unsigned int sys_tick_ms = 0;
  
  
  cli();
  //write your timer code here

  if( (sys_tick_ms & sleep_time) == 0u ){

    PORTB ^= (1 << PB5); 
   
  }

  sys_tick_ms += 1;

  sei();
  return;

}

ISR(PCINT0_vect)
{
  // Wake up
  cli();
  debounceButtons(&PINB, (1 << PB2) | (1 << PB1));
  if (!(PINB & (1 << PB1)))
  {
    if (sleep_time >= 0x3u)
    {
      sleep_time = (sleep_time >> 1);
    }
  }
  if (!(PINB & (1 << PB2)))
  {
    if (sleep_time <= 0x3FFFu)
    {
      sleep_time = (sleep_time << 1) | 0x1u;
    }
  }

  sei();
  return;
}
