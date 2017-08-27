/*
Interfacing OV7670 Camera module with
Arduino Uno.
Work in Progress.
Author: Hardik Kalasua
*/
#include <Wire.h>

void setup()
{ 
  noInterrupts(); //Disable all interrupts
  XClk_setup(); //Setup 8MHz clock at pin 11
  twi_setup(); // Setup SCL for 100KHz
  ovPin_setup(); // Setup Data-in and interrupt pins from camera 
  Wire.begin(); // join i2c bus (address optional for master)
  delay(3000);
  
  Serial.begin(9600);
  
}

void loop(){
  Serial.print("0");
  Wire.beginTransmission(43); // device write address is 42
  Serial.print("1");
  Wire.write(byte(0x01));
  Serial.print("2");
  Wire.requestFrom(43,1);
  Serial.print("3");
  byte readByte = Wire.read();
  Serial.print("4");
  Serial.println(readByte);
  Serial.print("5");
  Wire.endTransmission();
  Serial.print("6");
  
}

void XClk_setup(void){
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

void twi_setup(void){
  //Set prescaler bits in TWI Status Register (TWSR) to 0
  TWSR &= ~3;
  //Set SCL frequency to 100KHz
  //SCLfreq = CPUfreq/(16 + 2(TWBR) - 4^(TWPS))
  //TWBR = 72, TWPS(prescaler) = 0
  TWBR = 72;
}

void ovPin_setup(void){
  //Setup Data input pins and Interrupt pins
  //DDRC bits 3:0 = 0 =>  bits configured as Data Inputs
  //DDRC 3:0 - A3,A2,A1,A0
  DDRC &= ~15;//low d0-d3 camera
  
  //~(0b11111100) = 0b00000011
  //make DDRD 7:2 = 0 => Inputs
  //d7-d4 as data inputs, d3-INT1 is VSYNC and d2-INT0 is PCLK
  DDRD &= ~252;
}

