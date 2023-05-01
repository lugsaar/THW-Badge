#include <Arduino.h>

const byte interruptPin_01 = 2;
const byte interruptPin_02 = 3;
volatile unsigned long sleep_time = 0x3FFu;

static void button_interrupt_handler_01() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 50ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 50)
  {
    if(sleep_time >= 0x3u){
      sleep_time = (sleep_time >> 1) ;
    }
  }
  last_interrupt_time = interrupt_time;
}

static void button_interrupt_handler_02() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 50ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 50)
  {
    if(sleep_time <= 0x3FFFu){
      sleep_time = (sleep_time << 1) | 0x1u ;
    }
  }
  last_interrupt_time = interrupt_time;
}

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(interruptPin_01, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_01), button_interrupt_handler_01, FALLING);

  pinMode(interruptPin_02, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_02), button_interrupt_handler_02, FALLING);


  //START TIMER SETUP
  //TIMER SETUP for highly preceise timed measurements 
  cli();//stop all interrupts
  // turn on CTC mode
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCCR1B |= (1 << WGM12);
  // Set prescaler to 64 --> CLK / 64 = 250000 Hz
  TCCR1B |= (1 << CS11) | (1 << CS10); 
  
  //initialize counter value to 0;
  TCNT1  = 0;
  
  // set timer count for 1KHz increments
  OCR1A = 250 - 1;
  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
}

// the loop function runs over and over again forever
void loop() {
  
}


ISR(TIMER1_COMPA_vect) {
  //Interrupt every ms 

  static unsigned int sys_tick_ms = 0;
  
  
  cli();
  //write your timer code here

  if( (sys_tick_ms & sleep_time) == 0u ){
    int state = digitalRead(LED_BUILTIN);
    if( state == LOW)
    {
      digitalWrite(LED_BUILTIN,HIGH);
    }else if (state == HIGH)
    {
      digitalWrite(LED_BUILTIN,LOW);
    }
  }

  sys_tick_ms += 1;

  sei();
  return;

}
