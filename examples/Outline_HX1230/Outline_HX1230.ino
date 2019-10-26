// RRE font example
// (C)2019 Pawel A. Hernik

#define LCD_RST 6
#define LCD_CS  7
#define LCD_BL  8

#include "HX1230_FB.h"
#include <SPI.h>
HX1230_FB lcd(LCD_RST, LCD_CS);

#include "RREFont.h"
#include "rre_chicago_20x24.h"

RREFont font;

// needed for RREFont library initialization
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }
void customRectDither(int x, int y, int w, int h, int c) { return lcd.fillRectD(x, y, w, h, c); }

int outlineD(int x, int y, char *str, int dither)
{
  font.setColor(1);
  font.setFillRectFun(customRect);
  font.printStr(x-1,y-1,str);
  font.printStr(x-0,y-1,str);
  font.printStr(x+1,y-1,str);
  font.printStr(x-1,y+1,str);
  font.printStr(x-0,y+1,str);
  font.printStr(x+1,y+1,str);
  font.printStr(x-1,y,str);
  font.printStr(x+1,y,str);
  font.setColor(0);
  font.setFillRectFun(customRectDither);
  lcd.setDither(dither);
  font.printStr(x,y,str);
  font.setColor(1);
  font.setFillRectFun(customRect);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
  lcd.cls();

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setFont(&rre_chicago_20x24);
  font.printStr(1,0,"Normal");
  outlineD(1,22,"Outline",16);
  outlineD(1,44,"OutlineD",8);
  lcd.display();
}

void loop()
{
}

