#define latchVcc 4
#define latchGnd 3
#define clockPin 11
#define dataPin 10

#define button 2
#define potX A1
#define potY A2


uint16_t potYVal;
uint16_t potXVal;
uint32_t oldTimeInt;
bool buttonPress;

uint16_t animationArr[50][3]{
    {3,3,200},
    {3,4,1000},
    {2,4,500},
    {1,5,100},
    {3,3,50},
    {3,2,50},
    {4,1,400},
    {5,1,500},
    {6,3,400},
    {6,4,500},
    {3,4,100},
    {0,2,200},
    {0,4,500},
    {0,5,100},
    {0,6,500},
    {2,7,50},
    {5,4,1000},
    {2,1,300},
    {3,3,400},
    {4,4,300},
    {5,5,200},
    {6,5,100},
    {6,4,1000},
    {6,5,800},
    {6,6,200},
    {6,2,300},
    {1,6,100},
    {0,2,400},
    {2,4,600},
    {4,7,400},
    {5,7,400},
    {6,5,150},
    {3,3,250},
    {2,3,600},
    {3,3,400},
    {3,2,1000},
    {3,1,230},
    {3,3,500},
    {5,1,100},
    {3,1,500},
    {1,2,100},
    {0,2,60},
    {2,7,30},
    {4,7,500},
    {5,7,100},
    {2,7,100},
    {3,5,100},
    {3,3,50},
    {5,3,230},
    {3,3,1000}
};

bool defaultEyeArr[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

uint8_t oldEyePupil[4][2];

uint8_t eyePupil[4][2] = {
  {3, 3}, {4, 3},
  {3, 4}, {4, 4}
};

void setup() {
  Serial.begin(9600);
  pinMode(latchVcc, OUTPUT);
  pinMode(latchGnd, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(potX, INPUT);
  pinMode(potY, INPUT);
  pinMode(button, INPUT);
  digitalWrite(latchVcc, LOW);
  digitalWrite(latchGnd, LOW);

}

bool blinkAllowed = false;
void loop() {
  mapXY();//nolasa džoistika vērtības
  driveSwitch();//uzzīmē aci
  blink();
  if(buttonPress){
      animationDrive();
  }
}

uint8_t animationCounter;
uint32_t animationMillis;
void animationDrive(){
    if(millis()-animationMillis >= animationArr[animationCounter][2]){
        animationMillis = millis();
        potXVal = animationArr[animationCounter][0];
        potYVal = animationArr[animationCounter][1];
        animationCounter ++;
        if(animationCounter >=50){
            animationCounter = 0;
        }
    }
}

uint32_t topBlinkMillis;
uint32_t botBlinkMillis;

uint16_t randomBlinkInt;
uint32_t blinkTimeInt;
uint8_t blinkTime;//mirkšķiena ilgums 
uint8_t topPart;//augšējās daļas mirkšķiena ilgums
uint8_t botPart;//apakšējās daļas mirkšķiena ilgums

uint8_t topPartCounter;
uint8_t botPartCounter;

bool blinkEyeArr[8][8];

uint8_t timesToBlink;
uint8_t oldTimesToBlink;
bool blinkDown;
void blink(){//atbilst par mirkšķināšanu
    if(millis()-blinkTimeInt>=randomBlinkInt){//tiek iestatītas miršķināšanas random vērtības'
        randomBlinkInt = random(3000, 5000);
        blinkDown = true;
        topPartCounter = 0;
        botPartCounter = 7;
        blinkAllowed = true;
        blinkTimeInt = millis();
        blinkTime = random(50, 60);
        if(oldTimesToBlink > 1){
            oldTimesToBlink = 0;
        }else {
            timesToBlink = random(1,3);
        }
        for(int i = 0; i<8; i++){
            for(int j = 0; j<8; j++){
                blinkEyeArr[i][j] = defaultEyeArr[i][j];
            }
        }
    }
    if(timesToBlink == 2 && blinkAllowed == false && blinkDown == false){
        oldTimesToBlink = timesToBlink;
        timesToBlink = 0; 
        randomBlinkInt = 0;
    }
    if(blinkAllowed == true && millis() - topBlinkMillis>=blinkTime*0.5*0.5*0.10){
        
        topBlinkMillis = millis();
        if(blinkDown && topPartCounter<=4){
            for(int i = 0; i<8; i++){
                defaultEyeArr[topPartCounter][i] = 0;
            }
            topPartCounter++;
        } else if(!(blinkDown) && topPartCounter>=0){
            for(int i = 0; i<8; i++){
                defaultEyeArr[topPartCounter][i] = blinkEyeArr[topPartCounter][i];
            }
            topPartCounter--;
        }
    }
    if (blinkAllowed == true && millis() - botBlinkMillis>=blinkTime*0.5*0.5*0.25){
        botBlinkMillis = millis();
        if(blinkDown && botPartCounter>=5){
            for(int i = 0; i<8; i++){
                defaultEyeArr[botPartCounter][i] = 0;
            }
            botPartCounter--;
        } else if(!(blinkDown) && botPartCounter<=7){
            for(int i = 0; i<8; i++){
                defaultEyeArr[botPartCounter][i] = blinkEyeArr[botPartCounter][i];
            }
            botPartCounter++;
        }
    }
    if (botPartCounter == 4 && topPartCounter == 5 && blinkAllowed == true){
        blinkDown = false;
    }
    if(blinkDown == false && botPartCounter >= 7 && topPartCounter <= 0){
        blinkAllowed = false;
    }

}


void setPupilInArr(){//iestata masīvā zīlītes vērtības

    if (defaultEyeArr[potXVal][potYVal] == true/*salīdzina ar pilno aci*/ && millis() - oldTimeInt > 75 /*debounce daļa*/ ) {
     eyePupil[0][0] = potXVal;
     eyePupil[0][1] = potYVal;
     eyePupil[1][0] = potXVal+1;
     eyePupil[1][1] = potYVal;
     eyePupil[2][0] = potXVal;
     eyePupil[2][1] = potYVal+1;
     eyePupil[3][0] = potXVal+1;
     eyePupil[3][1] = potYVal+1;
     }

     for(int i = 0; i<8; i++){
         for(int j = 0; j<8; j++){
            defaultEyeArr[i][j] = 1;
         }
     }   
     for(int i =0; i<4; i++){
         defaultEyeArr[eyePupil[i][0]][eyePupil[i][1]] = 0;
     }    

     
}




void mapXY() {//nolasa vērtības un tās pieregulē
  if (!(map(analogRead(potX), 0, 1023, 0, 7) == 0 && map(analogRead(potY), 0, 1023, 0, 7) == 0 || map(analogRead(potX), 0, 1023, 0, 7) == 6 && map(analogRead(potY), 0, 1023, 0, 7) == 0 || map(analogRead(potX), 0, 1023, 0, 7) == 0 && map(analogRead(potY), 0, 1023, 0, 7) == 6 || map(analogRead(potX), 0, 1023, 0, 7) == 6 && map(analogRead(potY), 0, 1023, 0, 7) == 6)) { //tiek pārbaudīts vai vērtību drīkst setot un vai acs zīlīte vsp neizies ārā
    if(!(buttonPress)){
        potXVal = map(analogRead(potX), 0, 1023, 0, 7);
        potYVal = map(analogRead(potY), 0, 1023, 0, 7);
    }
    if(blinkAllowed == false){
        setPupilInArr();
    }
  }
  if(digitalRead(button) == HIGH && buttonPress == true){
      while(digitalRead(button));
      buttonPress= false;
  } else if(digitalRead(button) == HIGH && buttonPress == false){
      while(digitalRead(button));
      buttonPress = true;
  }
  
}

void driveSwitch() {//tin uz priekšu abus draiverus lediem
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0 ; j < 8; j++) {
      latch(i, j);
    }
  }
}

void latch(uint8_t x, uint8_t y) {//uzzīmē aci atbilstoši defaultEyeArr
  uint8_t valueToSend = 0;
  bitWrite(valueToSend, x, defaultEyeArr[x][y]);
  shiftOut(dataPin, clockPin, LSBFIRST, valueToSend);
  digitalWrite(latchVcc, HIGH);
  digitalWrite(latchVcc, LOW);

  valueToSend = 0;
  bitWrite(valueToSend, y, defaultEyeArr[x][y]);
  shiftOut(dataPin, clockPin, LSBFIRST, (uint8_t)~valueToSend);
  digitalWrite(latchGnd, HIGH);
  digitalWrite(latchGnd, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, 255);
  digitalWrite(latchGnd, HIGH);
  digitalWrite(latchGnd, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, 0);
  digitalWrite(latchVcc, HIGH);
  digitalWrite(latchVcc, LOW);
}

