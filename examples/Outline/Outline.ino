// RRE font example
// (C)2019 Pawel A. Hernik

// specific for ST7920 library, insert here your display library header and variable
#include "ST7920_SPI.h"
#include <SPI.h>
#define LCD_BACKLIGHT  9
#define LCD_CS         10
ST7920_SPI lcd(LCD_CS);

/*
// specific for Adafruit library, insert here your display library header and variable
#include <Adafruit_GFX.h>
#include <Adafruit_Soft1306.h>
#if USEHW==1
#include <Wire.h>
#endif
Adafruit_Soft1306 lcd(4);
#define SCR_WD 128
#define SCR_HT 64
*/
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

  // specific for ST7920 library
  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, HIGH);
  lcd.init();
  lcd.cls();
/*
  // specific for Adafruit
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  lcd.clearDisplay();
*/

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setFont(&rre_chicago_20x24);
  font.printStr(1,0,"Normal");
  outlineD(1,22,"Outline",16);
  outlineD(1,44,"Outline Dither",8);
  lcd.display();
}

void loop()
{
}

