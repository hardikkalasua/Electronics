/*
Bluetooth Controlled 
DC motor code
*/

// Define constants at the top
int m1A = 6;
int m1B = 5;
int m2A = 9;
int m2B = 10;
int levelLeft = 0;
int levelRight = 0;
const byte numChars = 9;
char receivedChars[numChars];
boolean newData = false;

/* All arduino programs have to have two
methods in them- void setup() and void loop()
*/

void setup(){
  //initialize pins as outputs
  Serial.begin(9600);  
  Serial.println("<Arduino is ready!>");
  pinMode(m1A, OUTPUT);
  pinMode(m1B, OUTPUT);
  pinMode(m2A, OUTPUT);
  pinMode(m2B, OUTPUT);
}

void loop(){
  recvWithStartEndMarkers();
  showNewData();
  levelLeft = calculateLevelLeft();
  levelRight = calculateLevelRight();
  motion(receivedChars[0], levelLeft, m1A, m1B);
  motion(receivedChars[4], levelRight, m2A, m2B);
  
}

void recvWithStartEndMarkers(){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while(Serial.available() > 0 && newData == false){
    rc = Serial.read();

    if(recvInProgress == true){
      if(rc != endMarker){
        receivedChars[ndx] = rc;
        ndx++;

        if(ndx >= numChars){
          ndx = numChars - 1;
          }
        }else{
          receivedChars[ndx] = '\0'; // Terminate the string
          recvInProgress = false;
          ndx = 0;
          newData = true;
          }
      }else if(rc == startMarker){
        recvInProgress = true;
        }
    }
}

void showNewData(){
 if(newData == true){
    Serial.print("This just in ..");
    Serial.println(receivedChars);
    
    newData = false;
  }   
}

int calculateLevelLeft(){
  int huns = receivedChars[1] - '0';
  int tens = receivedChars[2] - '0';
  int ones = receivedChars[3] - '0';

  int level = 100*huns + 10*tens + ones;
  return level;
}

int calculateLevelRight(){
  int huns = receivedChars[5] - '0';
  int tens = receivedChars[6] - '0';
  int ones = receivedChars[7] - '0';

  int level = 100*huns + 10*tens + ones;
  return level;
}


void motion(char dir, int level, int pinA, int pinB){
  if(dir == '+'){
    analogWrite(pinA, level);
    analogWrite(pinB, LOW);
  }else if(dir == '-'){
    analogWrite(pinB, level);
    analogWrite(pinA, LOW);
  }else{
    analogWrite(pinB, LOW);
    analogWrite(pinA, LOW);
    }
}

