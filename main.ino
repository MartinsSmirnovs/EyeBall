//Pievienojot potenciometrus, to vērtības vajag mapot uz no 1 - 8 . Kad tie atradīsies pa vidu, tad arī atbilstoši acs zīlīte būs pa vidu

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

uint8_t squareLen = 6;
//SKAITĪŠANA MASĪVOS NOTIEK NO NULLES!
bool eyeArr[8][8] = {
  {0,0,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,0,0}
  };

uint8_t eyePupil[4][2] = {//seko līdzi acs zīlītes atrašanās vietai
  {3,3},{3,4},
  {4,3},{4,4}
  };
void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.display(); 

  
}

void loop() {
  drawPupil();
  drawEye();
}

void drawEye(){//uzzīmē aci atbilstoši eyeArr masīvam
  for(int i = 1; i<=8; i++){
    for(int j = 5; j<13; j++){
      if(eyeArr[i-1][j-5] == true){
        display.fillRect(j*squareLen+j, i*squareLen+i, squareLen, squareLen, WHITE);
        }
      }
    }
  display.display(); 
}

void drawPupil(){//uzzīmē acs zīlīti
  for(int i = 0; i<4; i++){
      eyeArr[eyePupil[i][0]][eyePupil[i][1]] = false;
    }
  }












