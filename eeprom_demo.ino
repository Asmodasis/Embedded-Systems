
// get a pointer to the EEPROM ADDRESS REGISTER (16-bit)
volatile unsigned int* EEPROM_ADDR_REG = (unsigned int*) 0x41;
// get a pointer to the EEPROM Data Register (8-bit)
volatile unsigned char* EEPROM_DATA_REG = (unsigned char*) 0x40;
// get a pointer to the EEPROM Control Register (8-bit)
volatile unsigned char* EEPROM_CNTRL_REG = (unsigned char*) 0x3F;

void setup() {
  // Start Serial
  Serial.begin(9600);
  unsigned char data_in = 0x41;     // Make up some data to save
  unsigned char data_out = 0x00;    // Create a variable to read it back fom
  unsigned int address = 0x0025;    // Pick a random address to save it
  eeprom_write(address, data_in);   // Save the data input to the address we picked
  data_out = eeprom_read(address);  // Read the data back from the address we picked
  if(data_in != data_out)           // Check to make sure what we wrote is what we read
  {
    Serial.println("EEPROM Error");
  }
  Serial.print("Data input : ");    // Print the data back to the serial terminal
  Serial.print(data_in, HEX);
  Serial.print(", Data output : " );
  Serial.println(data_out, HEX);
}
void loop() 
{
  
}
void eeprom_write(unsigned int address, unsigned char data_in)
{
  /* Wait for completion of previous write */
  while(*EEPROM_CNTRL_REG & 0x02);
  /* Set up address and Data Registers */
  *EEPROM_ADDR_REG = address;
  *EEPROM_DATA_REG = data_in;
  /* Write logical one to EEMPE */
  *EEPROM_CNTRL_REG |= 0x04;
  /* Write logical zero to EEPE */
  *EEPROM_CNTRL_REG &= ~(0x02);
  /* Write logical one to EEPE */
  *EEPROM_CNTRL_REG |= 0x02;
}
unsigned char eeprom_read(unsigned int address)
{
  /* Wait for completion of previous write */
  while(*EEPROM_CNTRL_REG & 0x02);
  /* Set up address register */
  *EEPROM_ADDR_REG = address;
  /* Start eeprom read by writing EERE */
  *EEPROM_CNTRL_REG |= 0x01;
  /* Return data from Data Register */
  return *EEPROM_DATA_REG;
}
