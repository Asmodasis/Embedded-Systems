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
   
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;  // USART Control and Status Register # A, This line enables u2x0 which doubles transmission speed
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;  // USART Control and Status Register # B, Sets the number of Data bits and enables tx and rx
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;  // USART Control and Status Register # C, enabling parity and extra information.
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;  // USART Bit Rate Register
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;  // USART Data register


 volatile unsigned char* myTCCR1B   = (unsigned char*) 0x81;
 volatile unsigned int* myTCNT1    = (unsigned int*) 0x84; // no prescaler 
 volatile unsigned char* myTIFR1    = (unsigned char*) 0x36; // no interupts


//
// main()
//


void setup()
{
 // initialize the serial port on USART0:
 // Serial.begin(9600) sets the baud rate
  U0init(9600);
  *myTCCR1B   = 0;
  *myTCNT1    = 0; 
  *myTIFR1    = 0;

 
}


void loop()
{
  unsigned int cs1;
  unsigned int quotient;
  unsigned int remainder;

  // Serial.available()
  // gets the number of bytes available from the Serial port
  while (U0kbhit()==0){}; // wait for RDA = true

  // Serial.read()
  cs1 = U0getchar();    // read character

  quotient = cs1 / 16;
  remainder = cs1 % 16;

  if(quotient < 9)
    quotient += 48;      // shift to 0 on ascii table, pivot at 48
  else
    quotient += 55;     // pivot from A which is 65 taking the 10 digits away, 65 - 10 = 55

  if(remainder < 9)
    remainder += 48;
  else
    remainder += 55;
    
  // Serial.print()
  U0putchar('0');
  U0putchar('x');
  U0putchar(quotient);
  U0putchar(remainder);
  U0putchar('\n');
  

  //U0putInt(cs1);     // echo character
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


/*
unsigned char convertToHex(unsigned int decimal){
  
  if(decimal <= 9 || decimal >= 0)
    return decimal;
  else{
    switch(decimal){
      case 10:
       return 'A';
       break;
       case 11:
       return 'B';
       break;
       case 12:
       return 'C';
       break;
       case 13:
       return 'D';
       break;
       case 14:
       return 'E';
       break;
       case 15:
       return 'F';
       break;
    }
  }
  
}
*/
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


