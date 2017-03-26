#define PORTD_OFF_MASK 0b11111100
#define PORTB_OFF_MASK 0b00111111
#define PORTC_OFF_MASK 0b00001111

#define LAYER1_ON (PORTC |= 0b00100000)
#define LAYER1_OFF (PORTC &= ~(0b00100000))

#define LAYER2_ON (PORTC |= 0b00010000)
#define LAYER2_OFF (PORTC &= ~(0b00010000))

bool cube[2][4][4] = {0};

void draw(bool cube[4][4]);

void setup() {
   // initialize timer1 
   noInterrupts();           // disable all interrupts
   TCCR1A = 0;
   TCCR1B = 0;
   TCNT1 = 0;

   DDRD |= 0b11111100;
   DDRB |= PORTB_OFF_MASK;
   DDRC |= 0b00111111;

   PORTD &= ~(PORTD_OFF_MASK);
   PORTB &= ~(PORTB_OFF_MASK);
   PORTC &= ~(PORTC_OFF_MASK);   

   OCR1A = 4;                // compare match register 16MHz/65536/60Hz
   TCCR1B |= (1 << WGM12);   // CTC mode
   TCCR1B |= (1 << CS12);    // 256 prescaler 
   TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
   interrupts();             // enable all interrupts

   //draw();
}

void loop() {
   cube[0][rand() % 4] [rand() %4] ^= true;
   cube[1][rand() % 4] [rand() %4] ^= true;
   delay(250);
}

ISR(TIMER1_COMPA_vect) {
   LAYER1_ON;
   draw(cube[0]);
   LAYER1_OFF;

   LAYER2_ON;
   draw(cube[1]);
   LAYER2_OFF;
}

void draw(bool cube[4][4]) {
   char portd_temp = 0;
   char portb_temp = 0;
   char portc_temp = 0;

   for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
         if (cube[i][j]) {
            if ((i * 4 + j) < 6)
               portd_temp |= 1 << (i * 4 + j);
            else if ((i * 4 + j) < 12)
               portb_temp |= 1 << (i * 4 + j - 6);
            else
               portc_temp |= 1 << (i * 4 + j - 12);
         }
      }
   }

   PORTD &= ~(PORTD_OFF_MASK);
   PORTB &= ~(PORTB_OFF_MASK);
   PORTC &= ~(PORTC_OFF_MASK);

   PORTD |= portd_temp << 2;
   PORTB |= portb_temp;
   PORTC |= portc_temp;
}



