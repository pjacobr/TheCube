#define PORTD_MASK 0b11111100
#define PORTB_MASK 0b00111111
#define PORTC_MASK 0b00001111

bool cube[4][4] = {0};
void draw();

void setup() {
   // put your setup code here, to run once:
   DDRD |= PORTD_MASK;
   DDRB |= PORTB_MASK;
   DDRC |= PORTC_MASK;

   PORTD &= ~(PORTD_MASK);
   PORTB &= ~(PORTB_MASK);
   PORTC &= ~(PORTC_MASK);
}

void loop() {
draw();
   while (1) {
      for (int i = 0; i < 4; ++i) {
         cube[0][i][0] ^= true;
         cube[0][i][1] ^= true;
         draw();
         delay(50);
      }
      for(int j=1; j<4; j++){
          cube[j][3][0] ^= true;
          cube[j][3][1] ^= true;
          draw();
          delay(50);
      }
      for(int k=2; k>=0; k--){
          cube[3][k][0] ^= true;
          cube[3][k][1] ^= true;
          draw();
          delay(50);
      }
      for(int i=2; i>0; i--){
          cube[i][0][0] ^= true;
          cube[3][k][1] ^= true;
          draw();
          delay(50);
      }


}

void draw() {
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

   PORTD &= ~(PORTD_MASK);
   PORTB &= ~(PORTB_MASK);
   PORTC &= ~(PORTC_MASK);

   PORTD |= portd_temp << 2;
   PORTB |= portb_temp;
   PORTC |= portc_temp;
}
 // void horizontialWall () {
// for(int z=0; z<4; z++){
   // for(int y=0;y<4;y++){
      // for(int x=0; x< 4; x++){
      // cube[y][x][z] ^= true;
      // }
   // }
   // draw();
//    delay(150);
// }

 // }

