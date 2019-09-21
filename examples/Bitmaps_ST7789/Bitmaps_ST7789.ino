// RRE font example
// (C)2019 Pawel A. Hernik

#define TFT_DC    7
#define TFT_RST   8 
#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#include "RREFont.h"
#include "rre_logo.h"
#include "rre_chip.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void setup() 
{
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  /*
  lcd.fillScreen(BLACK);
  lcd.fillRect(80*0,0,64,64,RED);
  lcd.fillRect(80*1,0,64,64,GREEN);
  lcd.fillRect(80*2,0,64,64,BLUE);
  lcd.fillRect(80*0,80,64,64,CYAN);
  lcd.fillRect(80*1,80,64,64,MAGENTA);
  lcd.fillRect(80*2,80,64,64,YELLOW);
  lcd.fillRect(80*0,80*2,64,64,WHITE);*/
}

void loop()
{
  lcd.fillScreen(BLACK);
  font.setFont(&rre_logo);
  font.setScale(2,2);
  font.setSpacing(0);
  font.setColor(GREEN);
  font.printStr(17,40," !");
  delay(3000);

  lcd.fillScreen(BLACK);
  font.setFont(&rre_chip);
  font.setSpacing(0); font.setSpacingY(0);
  font.setColor(YELLOW);
  font.printStr(16,40," !\"#$%\n&'()*+\n,-./01");
  font.printStr(16+16*2,40+16*3*2,"234567");
  delay(3000);
}

