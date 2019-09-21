// RRE font example
// (C)2019 Pawel A. Hernik

#define TFT_DC    7
#define TFT_RST   8 
#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#include "RREFont.h"
#include "rre_chicago_20x24.h"

RREFont font;

// needed for RREFont library initialization
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

int outline(int x, int y, char *str, int c1, int c2)
{
  int d=2;
  font.setColor(c1);
  font.printStr(x-d,y-d,str);
  font.printStr(x-0,y-d,str);
  font.printStr(x+d,y-d,str);
  font.printStr(x-d,y+d,str);
  font.printStr(x-0,y+d,str);
  font.printStr(x+d,y+d,str);
  font.printStr(x-d,y,str);
  font.printStr(x+d,y,str);
  font.setColor(c2);
  font.printStr(x,y,str);
}

void setup() 
{
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setColor(WHITE);
  font.setFont(&rre_chicago_20x24);
  font.setScale(2,2);
  font.setSpacing(2);
  font.printStr(2,0,"Normal");
  outline(2,60,"Outline",WHITE,BLACK);
  outline(2,120,"Outline Col",YELLOW,RED);
  font.setScale(1,1);
  outline(2,180,"Outline Color",CYAN,RGBto565(0,120,0));
}

void loop()
{
}

