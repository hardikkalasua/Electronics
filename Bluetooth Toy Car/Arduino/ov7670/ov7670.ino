/*
Interfacing OV7670 Camera module with
Arduino Uno.
Work in Progress.
Current Status: Successful SCCB interfacing. Can Read/Write OV7670 register values. 
Author: Hardik Kalasua (hardik.kalasua@gmail.com)

NOTE: OV7670 SCCB interface does't work if XCLK not provided.
*/


#include <Wire.h>

void setup()
{ 
  noInterrupts(); //Disable all interrupts
  XCLK_SETUP();   //Setup 8MHz clock at pin 11
//TWI_SETUP();    // Setup SCL for 100KHz
//OV7670_PINS();  // Setup Data-in and interrupt pins from camera
  delay(1000);
  interrupts();
  Wire.begin();
  Serial.begin(9600);

  delay(100);

  // Slave 7-bit address is 0x21.
  // R/W bit set automatically by Wire functions
  // dont write 0x42 or 0x43 for slave address
  Wire.beginTransmission(0x21);
  // Reset all register values
  Wire.write(0x12);  
  Wire.write(0x80);
  Wire.endTransmission();
  delay(500); // wait for reset to complete

  
}

void loop(){
  // Reading from a register is done in two steps
  // Step 1: Write register address to the slave 
  // from which data is to be read. 
  Wire.beginTransmission(0x21); // 7-bit Slave address
  Wire.write(0x11);  // reading from register 0x11
  Wire.endTransmission();

  // Step 2: Read 1 byte from Slave
  Wire.requestFrom(0x21, 1);
  Serial.println(Wire.available());
  Serial.println(Wire.read());
  delay(5000);
  
  
}

void XCLK_SETUP(void){
  pinMode(11, OUTPUT); //Set pin 11 to output
  
  //Initialize timer 2
  //Clear the EXCLK and AS2 bits in ASSR
  ASSR &= ~ (_BV(EXCLK) | (_BV(AS2)));
  
  //WGM22, WGM21 & WGM20 bits SET- Fast PWM mode
  //COM2A0 SET- Toggle OC2A on compare match
  TCCR2A = (1 << COM2A0) | (1 << WGM21) | (WGM20);
  //SET CS20 bit for clock select with no prescaling
  TCCR2B = (1 << WGM22) | (1 << CS20); 
  //Output Compare Register 2A(OCR2A) = 0
  //This will lead to a match on every clock cycle
  //Toggle OC2A output pin on every match instance
  //Therefore, the generated waveform will have half
  //the frequency of the driving clock i.e. 8Mhz
  //OC2A pin- PB3 (alternate functn) pin i.e. Arduino pin 11
  OCR2A = 0;

}

// Two Wire Interface Setup
// Sets the frequency of the SCL line
// Default is 100KHz so we won't use this function
void TWI_SETUP(void){
  //Set prescaler bits in TWI Status Register (TWSR) to 0
  TWSR &= ~3;
  //Set SCL frequency to 100KHz
  //SCLfreq = CPUfreq/(16 + 2(TWBR) - 4^(TWPS))
  //TWBR = 72, TWPS(prescaler) = 0
  TWBR = 72;
}

void OV7670_PINS(void){
  //Setup Data input pins and Interrupt pins
  //DDRC bits 3:0 = 0 =>  bits configured as Data Inputs
  //DDRC 3:0 - A3,A2,A1,A0
  DDRC &= ~15;//low d0-d3 camera
  
  //~(0b11111100) = 0b00000011
  //make DDRD 7:2 = 0 => Inputs
  //d7-d4 as data inputs, d3-INT1 is VSYNC and d2-INT0 is PCLK
  DDRD &= ~252;
}

