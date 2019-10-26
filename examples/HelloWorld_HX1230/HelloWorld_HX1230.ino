// RRE font example
// (C)2019 Pawel A. Hernik

#define LCD_RST 6
#define LCD_CS  7
#define LCD_BL  8

#include "HX1230_FB.h"
#include <SPI.h>
HX1230_FB lcd(LCD_RST, LCD_CS);


#include "RREFont.h"
#include "rre_8x12.h"

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
  font.setFont(&rre_8x12);
  font.printStr(ALIGN_CENTER,0,"Hello World!");  // center
  lcd.display();
}

void loop()
{
}

