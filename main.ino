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
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};
bool eyeArr[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

uint8_t oldPupil[4][2];//iepriekšējā zīlītes vērtība
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
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  mapXY();//nolasa ievadītās vērtības

  if (defaultEyeArr[potXVal][potYVal] == true/*salīdzina ar pilno aci*/ && millis() - oldTimeInt > 100 /*debounce daļa*/ ) {
    oldTimeInt = millis();
    for(int i = 0; i<4; i++){
      for(int j = 0; j<2; j++){
        oldPupil[i][j] = eyePupil[i][j];
        }
    } 
     eyePupil[0][0] = potXVal;
     eyePupil[0][1] = potYVal;
     eyePupil[1][0] = potXVal+1;
     eyePupil[1][1] = potYVal;
     eyePupil[2][0] = potXVal;
     eyePupil[2][1] = potYVal+1;
     eyePupil[3][0] = potXVal+1;
     eyePupil[3][1] = potYVal+1;           
    }

    Serial.println();
    Serial.println();
    for(int i = 0; i< 4; i++){
      if(i == 2){
        Serial.println();
        }
      for(int j = 0; j<2; j++){
        Serial.print(eyePupil[i][j]);
        Serial.print(" ");
        }
      }

  drawPupil(oldPupil, true);//iestata iepriekšējās zīlītes vērtības acs baltumam
  drawPupil(eyePupil, false);//iestat esošās zīlītes vērtības
  drawEye();//uzzīmē acs baltumu
  /*
  Serial.println("Getting Val:");
  Serial.print("X: ");
  Serial.println(potXVal);
  Serial.print("Y: ");
  Serial.println(potYVal);*/
  /*
  Serial.println();
    for(int i = 0; i<8; i++){
    Serial.print("{");
    for(int j = 0; j<8; j++){
      Serial.print(eyeArr[i][j]);
      Serial.print(",");
      }
    Serial.println("}");
    }*/
}

void mapXY() {//nolasa vērtības un tās pieregulē
  potXVal = map(analogRead(potX), 0, 1023, 0, 7);
  potYVal = map(analogRead(potY), 0, 1023, 0, 7);
  buttonPress = digitalRead(button);
}

void drawEye() { //uzzīmē aci atbilstoši eyeArr masīvam
  for (int i = 1; i <= 8; i++) {
    for (int j = 5; j < 13; j++) {
      if (eyeArr[i - 1][j - 5] == true) {
        display.fillRect(j * squareLen + j, i * squareLen + i, squareLen, squareLen, WHITE);
      } else {
        display.fillRect(j * squareLen + j, i * squareLen + i, squareLen, squareLen, BLACK);
      }
    }
  }
  display.display();
}


void drawPupil(uint8_t pupilArr[4][2], bool state) { //uzzīmē acs zīlīti
    for(int i = 0; i<4; i++){
      for(int j = 0; j<2; j++){
        eyeArr[pupilArr[i][j]][pupilArr[i][1]] = state;
        }
      }
}