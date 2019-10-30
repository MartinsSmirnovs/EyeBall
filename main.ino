/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define button 2
#define potX A0
#define potY A1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

uint32_t oldTimeInt;
bool buttonPress;
uint16_t potYVal;
uint16_t potXVal;
uint8_t squareLen = 6;
//SKAITĪŠANA MASĪVOS NOTIEK NO NULLES!
bool defaultEyeArr[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};


uint8_t eyePupil[4][2] = {{3,3}, {4,3}, {3,4}, {4,4}};

void setup() {
  pinMode(potX, INPUT);
  pinMode(potY, INPUT);
  pinMode(button, INPUT);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(1000);
  display.clearDisplay();
  display.display();

}

void loop() {
  //mapXY();//nolasa ievadītās vērtības

  potXVal = 3; //pagaidu testa variants, jo noplīsa džoistika vads
  potYVal = 3;
  
  
  drawPupil();//Iestata acs zīlītes vērtības masīvā
  drawEye();//uzzīmē acs baltumu
  blinkEye();
}

void blinkEye(){
  //kods priekš nomirkšķināšanas
}



void drawPupil(){
  if (defaultEyeArr[potXVal][potYVal] == true/*salīdzina ar pilno aci*/ && millis() - oldTimeInt > 100 /*debounce daļa*/ ) {
     oldTimeInt = millis();
     eyePupil[0][0] = potXVal;
     eyePupil[0][1] = potYVal;
     eyePupil[1][0] = potXVal+1;
     eyePupil[1][1] = potYVal;
     eyePupil[2][0] = potXVal;
     eyePupil[2][1] = potYVal+1;
     eyePupil[3][0] = potXVal+1;
     eyePupil[3][1] = potYVal+1;           
    }
}

void mapXY() {//nolasa vērtības un tās pieregulē     JĀIESTATA, LAI ACS PUNKTS KUSTĒTOS VISUR IZŅEMOT KAD X == 0 UN Y == 0 VAI X == 6 UN Y == 0 VAI X == 0 UN Y == 6 UN X == 6 UN Y == 6
  if(!(map(analogRead(potX), 0, 1023, 0, 7) == 0 && map(analogRead(potY), 0, 1023, 0, 7) == 0 || map(analogRead(potX), 0, 1023, 0, 7) == 6 && map(analogRead(potY), 0, 1023, 0, 7) == 0 || map(analogRead(potX), 0, 1023, 0, 7) == 0 && map(analogRead(potY), 0, 1023, 0, 7) == 6 || map(analogRead(potX), 0, 1023, 0, 7) == 6 && map(analogRead(potY), 0, 1023, 0, 7) == 6)){//tiek pārbaudīts vai vērtību drīkst setot un vai acs zīlīte vsp neizies ārā
  potXVal = map(analogRead(potX), 0, 1023, 0, 7);
  potYVal = map(analogRead(potY), 0, 1023, 0, 7);
  buttonPress = digitalRead(button);
}
}

void drawEye() { //uzzīmē aci atbilstoši eyeArr masīvam
  for (int i = 1; i <= 8; i++) {
    for (int j = 5; j < 13; j++) {
      if (defaultEyeArr[i - 1][j - 5] == true && !(eyePupil[0][0] == i-1 && eyePupil[0][1]  == j-5|| eyePupil[1][0] == i-1 && eyePupil[1][1]  == j-5|| eyePupil[2][0] == i-1 && eyePupil[2][1]  == j-5|| eyePupil[3][0] == i-1 && eyePupil[3][1] == j-5)) {
        display.fillRect(j * squareLen + j, i * squareLen + i, squareLen, squareLen, WHITE);
      } else {
        display.fillRect(j * squareLen + j, i * squareLen + i, squareLen, squareLen, BLACK);
      }
    }
  }
  display.display();
}