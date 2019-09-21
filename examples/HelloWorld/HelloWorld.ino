// RRE font example
// (C)2019 Pawel A. Hernik

// specific for ST7920 library, insert here your display library header and variable
#include "ST7920_SPI.h"
#include <SPI.h>
#define LCD_BACKLIGHT  9
#define LCD_CS         10
ST7920_SPI lcd(LCD_CS);

#include "RREFont.h"
#include "rre_8x12.h"

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
  font.setFont(&rre_8x12);
  font.printStr(ALIGN_CENTER,0,"Hello World!");  // center
  lcd.display(0);
}

void loop()
{
}

