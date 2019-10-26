// RRE font example
// (C)2019 Pawel A. Hernik

#define LCD_RST 6
#define LCD_CS  7
#define LCD_BL  8

#include "HX1230_FB.h"
#include <SPI.h>
HX1230_FB lcd(LCD_RST, LCD_CS);

#include "RREFont.h"
#include "rre_term_10x16.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
  lcd.cls();

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setFont(&rre_term_10x16);
  lcd.setDither(8);
  lcd.fillRectD(8,8,SCR_WD-16,16*4-16,1);
  font.setColor(1);   font.printStr(0, 0,"ABcdeIMNmn0123");
  font.setColor(1,0); font.printStr(0,16,"ABcdeIMNmn0123");
  font.setColor(0);   font.printStr(0,32,"ABcdeIMNmn0123");
  font.setColor(0,1); font.printStr(0,48,"ABcdeIMNmn0123");
  lcd.display();
}

void loop()
{
}

