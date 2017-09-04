#include <SD.h>

int CS_Pin = 10;

void setup() {

 
  // SD setup
  Serial.begin(9600);
  pinMode(CS_Pin, OUTPUT);
  if (SD.begin(CS_Pin)) {
    Serial.println("Card Initialization Successful!");
  }else{
    Serial.println("Card Initialization Failed!");
    }

    File dataFile = SD.open("test.bmp", FILE_WRITE);
                
  for(int i=0;i<240;i++){
    for(int j=0;j<320;j++){
      dataFile.write(0x77);
      }
    }           

  dataFile.close();

  }


void loop(){
  
  }
