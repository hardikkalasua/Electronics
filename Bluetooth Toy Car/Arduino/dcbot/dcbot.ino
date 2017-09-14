/*
Bluetooth Controlled 
DC motor code
*/

// Define constants at the top
int mleftA = 5;
int mleftB = 6;
int mrightA = 10;
int mrightB = 11;
int levelLeft = 0;
int levelRight = 0;
int levelLin = 0;
int levelDir = 0;
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
  pinMode(mleftA, OUTPUT);
  pinMode(mleftB, OUTPUT);
  pinMode(mrightA, OUTPUT);
  pinMode(mrightB, OUTPUT);
}

void loop(){
  recvWithStartEndMarkers();
  showNewData();
  levelLin = calculateLevelLin();
  levelDir = calculateLevelDir();
  levelLeft = calculatelevelLeft(receivedChars[4],levelDir,levelLin);
  levelRight = calculatelevelRight(receivedChars[4],levelDir,levelLin);
  if(levelLeft>255){levelLeft = 255;}
  if(levelLeft<0){levelLeft = 0;}
  if(levelRight>255){levelRight = 255;}
  if(levelRight<0){levelRight = 0;}

  motion(receivedChars[0], levelLeft, levelRight);
  
}


int calculatelevelRight(char dir, int levelDir, int levelLin){
    if(dir == 'l'){
      return (levelLin + levelDir/2);
    }else{
      return (levelLin - levelDir/2);
    }

}

int calculatelevelLeft(char dir, int levelDir, int levelLin){
    if(dir == 'l'){
      return (levelLin - levelDir/2);
    }else{
      return (levelLin + levelDir/2);
    }
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

int calculateLevelLin(){
  int huns = receivedChars[1] - '0';
  int tens = receivedChars[2] - '0';
  int ones = receivedChars[3] - '0';

  int level = 100*huns + 10*tens + ones;
  return level;
}

int calculateLevelDir(){
  int huns = receivedChars[5] - '0';
  int tens = receivedChars[6] - '0';
  int ones = receivedChars[7] - '0';

  int level = 100*huns + 10*tens + ones;
  return level;
}


void motion(char dir, int left, int right){
  if(dir == '+'){
    analogWrite(mleftA, left);
    analogWrite(mleftB, LOW);
    analogWrite(mrightA, right);
    analogWrite(mrightB, LOW);
  }else if(dir == '-'){
    analogWrite(mleftB, left);
    analogWrite(mleftA, LOW);
    analogWrite(mrightB, right);
    analogWrite(mrightA, LOW);
  }else{
    analogWrite(mleftB, LOW);
    analogWrite(mleftA, LOW);
    analogWrite(mrightB, LOW);
    analogWrite(mrightA, LOW);
    }
}
