/*
 * Rafael Hernandez
 * Lab 2 Part C, using INT0 external interrupt.
 * Overview: use external interrupt , INT0 or INT 1, to control a 7 segment led output.
 */
double potValue; 

// Read analog function
int my_analogRead(uint8_t pin)
{
  uint8_t low, high;
  
  ADMUX |= (1<<REFS0);   // default Vref
  ADMUX |= (1<<MUX0)|(1<<MUX1);

  // start the conversion
  ADCSRA |= (1<<ADSC);
  

  // ADSC is cleared when the conversion finishes
  while (bit_is_set(ADCSRA, ADSC));

  // we have to read ADCL first; doing so locks both ADCL
  // and ADCH until ADCH is read.  Reading ADCL second would
  // cause the results of each conversion to be discarded,
  // as ADCL and ADCH would be locked when it completed.
  low  = ADCL;
  high = ADCH;

  // combine the two bytes
  return (high << 8) | low;
}



void loop()
{
         show_numbers(); 
}

#include "avr/io.h"
volatile int count; // will be used in case statement to get correct output.
void turnoff(); // fucntion to turn off all led's on 7seg
void turnon(); // function holds case switch statement
void setup() {
 //activating the correct pins to be used
 DDRB = DDRB | B00111111; // set pins PB(5 -> 0) as outputs
 DDRD = DDRD | 11000000; // set pins PD7 and PD6 as outputs
 // also sets PD2 as input (INT0)

 //interrupt setup
 sei(); // set global interrupt in sreg bit 7
 EIMSK |= (1 << INT0); // enables interrupt of SREG bit is also high.
 EICRA |= (1 << ISC01);// decides if activation is on rising or falling edge, or level sensed?
 // in this case it is rising edge activated.
 EICRA |= (0 << ISC00); // if ISC01 and ISC00, or bit1 and bit0 of EICRA register are 10, then
 // we get the interrupt activation on a falling edge. It happens when we
 // let go of the push button and the button itself rises up, this cuts the signal
 // and this appears as a falling edge.

 Serial.begin(9600); // to show us the value of count we'll use a serial output.
 //initial number output
 PORTB = B00111001; //9
 PORTD = B10000000; //9
 count = 9;

 pinMode (A3, INPUT);
  //Serial.begin(9600);
  ADCSRA |= (1<<ADEN);
  sei();

}

// Interrupt vector activated on the button push. 
ISR(INT0_vect) // now points to INT0_vect address
{
  potValue = my_analogRead(A3);
  Serial.print("\nRaw ADC Conversion: "); //used to check what current output should be.
  Serial.println (potValue);          // This prints the raw ADC conversion, a number form 0 - 1023.
  Serial.print("current number (Volt):"); //used to check what current output should be.
  Serial.println(potValue/204.6); //used to check what current output should be.
}

// This Function just acts to turn off the leds in the seven segment display.
void turnoff(){
 PORTB = B00000000; // set all PORTB to LOW
 PORTD = B00000000; // set all PORTD to LOW
 delay(100); // delay 0.1 seconds.
}

// Turns on seven segment display using a case statement.
void turnon(){
 switch (count) {
 case 0:
 PORTB = B00111111; //0
 PORTD = B00000000;
 break;
 case 1:
 PORTB = B00011000; //1
 PORTD = B00000000;
 break;
 case 2:
 PORTB = B00110110; //2
 PORTD = B10000000;
 break;
 case 3:
 PORTB = B00111100; //3
 PORTD = B10000000;
 break;
 case 4:
 PORTB = B00011001; //4
 PORTD = B10000000;
 break;
 case 5:
 PORTB = B00101101; //5
 PORTD = B10000000;
 break;
 case 6:
 PORTB = B00101111; //6
 PORTD = B10000000;
 break;
 case 7:
 PORTB = B00111000; //7
 PORTD = B00000000; 
 break;
 case 8:
 PORTB = B00111111; //8
 PORTD = B10000000;
 break;
 case 9:
 PORTB = B00111001; //9
 PORTD = B10000000; //9
 break;
 case 10:
 PORTB = B00000000; // This case is for the decimal point on the seven segment display.
 PORTD = B01000000;
 break;
 }
}

// This function takes the raw ADC input and makes the current stored value display on the seven segment.
int show_numbers(){
  count = potValue/204.6; //This shows the ones digits value.
  turnon();
  delay(1000);
  turnoff();

  count = 10;             // This displays the decimal point
  turnon();
  delay(1000);
  turnoff;
  
  
  count = ((potValue/204.6)*10);  // This displays the tenths place.
  count = count%10;
  turnon();
  delay(1000);
  turnoff();

  count = ((potValue/204.6)*100); // This displays the hundreths place.
  count = count%10;
  turnon();
  delay(1000);
  turnoff();
  delay(1000);
  
}
