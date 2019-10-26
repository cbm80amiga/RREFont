// RRE font example
// (C)2019 Pawel A. Hernik

// specific for ST7920 library, insert here your display library header and variable
#include "ST7920_SPI.h"
#include <SPI.h>
#define LCD_BACKLIGHT  9
#define LCD_CS         10
ST7920_SPI lcd(LCD_CS);

#include "RREFont.h"
#include "rre_term_10x16.h"

RREFont font;


// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, HIGH);
  lcd.init();
  lcd.cls();

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values

  font.setFont(&rre_term_10x16);
  lcd.setDither(8);
  lcd.fillRectD(8,8,128-16,64-16,1);
  font.setColor(1);   font.printStr(0, 0,"ABcdeIMNmn0123");
  font.setColor(1,0); font.printStr(0,16,"ABcdeIMNmn0123");
  font.setColor(0);   font.printStr(0,32,"ABcdeIMNmn0123");
  font.setColor(0,1); font.printStr(0,48,"ABcdeIMNmn0123");
  lcd.display(); 
}

void loop()
{
}

