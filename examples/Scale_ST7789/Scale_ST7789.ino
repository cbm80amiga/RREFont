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
#include "rre_ubuntu_32.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void setup() 
{
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setColor(WHITE);
  font.setFont(&rre_ubuntu_32);
  font.setScale(1,1);
  font.printStr(0,0,"Normal");
  font.setBold(1);
  font.printStr(0,40,"Normal Bold");
  font.setBold(0);
  font.setScale(2,1);
  font.printStr(0,40*2,"Double Wd");
  font.setScale(1,2);
  font.printStr(0,40*3,"Double Ht");
}

void loop()
{
}

