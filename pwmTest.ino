#define LED1_MASK 0b1                        // port 8 on board
#define LED1_PORT PORTB
#define LED2_MASK 0b10                       // port 9 on board
#define LED2_PORT PORTB
#define PORTB_MASK 0b11                      // what ports are being used

#define LED_FLICKER_RATE 60
#define BRIGHTNESS_DEGREES 64

#define TIMER1_PRESCALER 64
#define CLOCK 16000000

// =============================== GLOBAL VARIABLES ============================== //
uint8_t leds[2] = {0};




// ================================ BEGIN PROGRAM ================================ //

void setup(void) {
   // ============================ INITIALIZE TIMER 1 ============================ //
   cli();                                    // disable all interrupts
   TCCR1A = 0;                               // set entire TCCR1A register to 0
   TCCR1B = 0;                               // same for TCCR1B
   TCNT1  = 0;                               // initialize counter value to 0

   // desired_interrupt_frequency = led_flicker_rate * brightness_degrees
   // using equation compare_match_register = 16MHz / (prescaler * desired_interrupt_frequency) - 1, must be less than 65536 (timer 1 max counter)
   OCR1A = CLOCK / TIMER1_PRESCALER / LED_FLICKER_RATE / BRIGHTNESS_DEGREES - 1;
   TCCR1B |= (1 << WGM12);                   // CTC mode
   TCCR1B |= (1 << CS11) | (1 << CS10);      // Set CS11 and CS12 bits for 64 prescaler 
   TIMSK1 |= (1 << OCIE1A);                  // enable timer compare interrupt
   // ============================ END INITIALIZATION ============================ //

   PORTB &= ~(PORTB_MASK);                   // turn off all LEDs at beginning
   DDRB |= PORTB_MASK;                       // set ports 8 and 9 as outputs
  // LED1_PORT |= LED1_MASK;                   // stagger which LED is on

   interrupts();                             // enable all interrupts
}

void loop(void) {
   for (uint8_t counter = 0;  counter < 245; ++counter) {
      leds[0] = (sin(counter / 10.0) + 1) * BRIGHTNESS_DEGREES / 2;
      leds[1] = (-sin(counter / 10.0) + 1) * BRIGHTNESS_DEGREES / 2;
      delay(42);
   }
}

inline void draw(uint8_t counter) {
   if (counter >= (BRIGHTNESS_DEGREES - leds[0])) {
      if (!(LED1_PORT & LED1_MASK)) LED1_PORT |= LED1_MASK;
   }
   else {
      if (LED1_PORT & LED1_MASK) LED1_PORT &= ~LED1_MASK;
   }

   if (counter >= (BRIGHTNESS_DEGREES - leds[1])) {
      if (!(LED2_PORT & LED2_MASK)) LED2_PORT |= LED2_MASK;
   }
   else {
      if (LED2_PORT & LED2_MASK) LED2_PORT &= ~LED2_MASK;
   }
}

ISR(TIMER1_COMPA_vect) {
   static uint8_t pwmCounter = 0;
   draw(pwmCounter);
  if (++pwmCounter == BRIGHTNESS_DEGREES) pwmCounter = 0;
}
