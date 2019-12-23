// Piano Lab project


//measurements made in Hertz

#define freqA        440
#define freqASharp   466
#define freqB        494
#define freqMidC     523
#define freqCSharp   554
#define freqD        587
#define freqDSharp   624
#define freqE        659
#define freqF        698
#define freqFSharp   740
#define freqG        784
#define freqGSharp   831

#define myHIGH       1
#define myLOW        0

// Define Port B Register Pointers
volatile unsigned char* port_b     = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b      = (unsigned char*) 0x24; 
volatile unsigned char* pin_b      = (unsigned char*) 0x23;

volatile unsigned char* myTCCR1B   = (unsigned char*) 0x81;
volatile unsigned char* myTCNT1    = (unsigned char*) 0x84; // no prescaler 
volatile unsigned char* myTIFR1    = (unsigned char*) 0x36; // no interupts
 
uint8_t pianoPin   =  25; // the pin the piano is playing on


void setup() 
{
  // setup the UART
  Serial.begin(9600); 
  pinMode(pianoPin,OUTPUT);

  *myTCCR1B   = 0;
  *myTCNT1    = 0; 
  *myTIFR1    = 0;

  *ddr_b |= 0x40;
  

  
}
void loop() 
{
  unsigned char in_char;
  uint16_t noteDuration = 500;        // 500 millisecond delay
    
  // if the UART received a character
  if(Serial.available() > 0)
  {
    // read in the character
    //Serial.println("Thank you for running the Piano Program lowercase for regular notes, capitols for sharps.  ");

    in_char = Serial.read();
    
    // write the character back to the Serial Port
    
    if((int)in_char != 10){
    
      Serial.print("The Key you pressed is : ");
      Serial.write(in_char);
      Serial.write('\n');

      switch(in_char){
        
        case 'a':
          myTone(pianoPin , freqA, noteDuration);
        break;
        
        case 'A':
          myTone(pianoPin , freqASharp, noteDuration);
        break;
        
        case 'b':
          myTone(pianoPin , freqB, noteDuration);
        break;
        
        case 'c':
          myTone(pianoPin , freqMidC, noteDuration);
        break;
        
        case 'C':
          myTone(pianoPin , freqCSharp, noteDuration);
        break;
        
        case 'd':
          myTone(pianoPin , freqD, noteDuration);
        break;
        
        case 'D':
          myTone(pianoPin , freqDSharp, noteDuration);
        break;
        
        case 'e':
          myTone(pianoPin , freqE, noteDuration);
        break;
        
        case 'f':
          myTone(pianoPin , freqF, noteDuration);
        break;
        
        case 'F':
          myTone(pianoPin , freqFSharp, noteDuration);
        break;
        
        case 'g':
          myTone(pianoPin , freqG, noteDuration);
        break;
        
        case 'G':
          myTone(pianoPin , freqGSharp, noteDuration);
        break;

        case 'q': case 'Q': 
          myTone(pianoPin , 1, noteDuration);
         break;
        default:
        Serial.println("You did not select a valid input, please try again.");
        break;
      }
    }
    
   
  }
  
}

void myTone(uint8_t pin, uint16_t noteFreq, uint16_t duration){

  unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long halfPeriod = (1000000L / noteFreq / 2);

 //pinMode(pin, OUTPUT);            // set mode for output
  
  *port_b |= 0x40;
  
 while(currentTime - startTime < duration){

      //myDigitalWrite(pin, myHIGH);
      digitalWrite(pin, HIGH);
      myDelay(halfPeriod);
      //myDigitalWrite(pin, myLOW);
      digitalWrite(pin, LOW);
      myDelay(halfPeriod);

      currentTime = millis();
 }
  *port_b &= 0xBF;
  //pinMode(pin, INPUT);              // reset mode for input
  
}

void myDigitalRead(){
  


}


// pin is where you write it to, value is what you write to it
void myDigitalWrite(uint8_t pin, uint8_t value){



}


void myDelay(unsigned int ticks){

  // stop the timer (set prescaler to 000)

  *myTCCR1B &= 0xF8;                 // 0b1111 1000

  // set the counts (16 bits -> 2 addresses!)

  *myTCNT1 = (unsigned int) (65536 - ticks);

  // start the timer (set prescalar to 001)
  
  *myTCCR1B |= 0x01;                // 0b0000 0001

  // wait for overflow

  while((*myTIFR1 & 0x01) == 0);

  // stop the timer (set prescaler to 000)

  *myTCCR1B &= 0xF8;                // 0b1111 1000

  // reset TOV (write a 1 to reset to 0)

  *myTIFR1 |= 0x01;                 // 0b0000 0001

  
}


