/*===============================================================================

FILE:              xxx.ino

DESCRIPTION:        Program to echo serial input characters from
                    Putty keyboard back to Putty display.
                    This version uses proper pointers.

COMPILER:          Arduino IDE
NOTES:             Put other information here ...

MODIFICATION HISTORY:

Author                  Date               Version
================================================================================
D. Egbert,              04/03/2017         Version 1.5,
Shawn Ray               2018-10-19         Version 1.6

================================================================================*///

 #define RDA 0x80
 #define TBE 0x20
 #define ADA 0xC0
   
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;  // USART Control and Status Register # A, This line enables u2x0 which doubles transmission speed
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;  // USART Control and Status Register # B, Sets the number of Data bits and enables tx and rx
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;  // USART Control and Status Register # C, enabling parity and extra information.
 volatile unsigned int  *myUBRR0  = (unsigned int  *)0x00C4;  // USART Bit Rate Register
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;  // USART Data register


 volatile unsigned char *myTCCR1B = (unsigned char*) 0x81;
 volatile unsigned int  *myTCNT1  = (unsigned int *) 0x84;    // no prescaler 
 volatile unsigned char *myTIFR1  = (unsigned char*) 0x36;    // no interupts


volatile unsigned int   *myAPIN   = (unsigned int*)  0x52;    // address for analog 15 pin in the arduino,

volatile unsigned char  *myADCSRA = (unsigned char*) 0x7A;    // ADC control and status register A
volatile unsigned char  *myADCSRB = (unsigned char*) 0x7B;    // ADC control and status register B
volatile unsigned int   *myADLAR  = (unsigned int *) 0x78;    // the ADC data register, converstion result found here   
volatile unsigned char  *myADMUX  = (unsigned char*) 0x7C;    // determines what type of voltage to sample

//
// main()
//


void setup()
{
 // initialize the serial port on USART0:
 // Serial.begin(9600) sets the baud rate
  U0init(9600);
  adc_init();
  
  *myTCCR1B   = 0;
  *myTCNT1    = 0; 
  *myTIFR1    = 0;

 
}


void loop(){
  
  //unsigned char shift = 0 ;
  //shift = shift >> adc_read(15);
  //U0putchar((unsigned char)(adc_read(15) >> 8));
  //U0putchar('\n');
  
  //adc_read(0x52);
  
 
  int number;
  int first;
  int second;
  int third;
  int fourth;

  
  number = adc_read(0);
  fourth = (number % 10) + 48;
  number /= 10;
  third = (number % 10) + 48;
  number /= 10;
  second = (number % 10) + 48;
  number /= 10;
  first = (number % 10) + 48; 

  U0putchar(first);
  U0putchar(second);
  U0putchar(third);
  U0putchar(fourth);
  U0putchar('\n');
  
}



void adc_init(){
  
  *myADMUX = 0x40; 
  *myADCSRA = 0x80;
  *myADCSRB &= 0xF8;
  
    
}

unsigned int adc_read(unsigned char adc_channel){
 
 
 if(adc_channel > 7)
  {
    *myADCSRB |= 0b00001000;
  }
  else
  {
    *myADCSRB &= 0b11110111;
  }

  adc_channel = adc_channel % 8;
  *myADMUX &= 0b11100000;
  *myADMUX += adc_channel;
  
  *myADCSRA |= 0x40;
  while(*myADCSRA & 0x40){}
  return *myADLAR; 
}



//
// function to initialize USART0 to "int" Baud, 8 data bits,
// no parity, and one stop bit. Assume FCPU = 16MHz.
//
void U0init(unsigned long U0baud)
{
//  Students are responsible for understanding
//  this initialization code for the ATmega2560 USART0
//  and will be expected to be able to intialize
//  the USART in differrent modes.
//
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}

//
// Read USART0 RDA status bit and return non-zero true if set
//

 unsigned char U0kbhit()
{
  //return true if RDA is set or false if not
  return (RDA & *myUCSR0A);
  
}


//
// Read input character from USART0 input buffer
//
unsigned char U0getchar()
{
  // return what is in the data register
  return *myUDR0;
}


//
// Wait for USART0 TBE to be set then write character to
// transmit buffer
//
void U0putchar(unsigned char U0pdata)
{
 while (!(TBE & *myUCSR0A));
    *myUDR0 = U0pdata;
    
}

void U0putInt(unsigned char input){

  int quotient  = 0;
  int remainder = 0;
  
}


void myDelay(unsigned int ticks){

  // stop the timer (set prescaler to 000)

  *myTCCR1B &= 0xF8;                 // 0b1111 1000

  // set the counts (16 bits -> 2 addresses!)

  *myTCNT1 |= (unsigned int) (65536 - ticks);

  // start the timer (set prescalar to 001)
  
  *myTCCR1B |= 0x01;                // 0b0000 0001

  // wait for overflow

  while((*myTIFR1 & 0x01) == 0);

  // stop the timer (set prescaler to 000)

  *myTCCR1B &= 0xF8;                // 0b1111 1000

  // reset TOV (write a 1 to reset to 0)

  *myTIFR1 |= 0x01;                 // 0b0000 0001

  
}


