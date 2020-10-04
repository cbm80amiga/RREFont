// RRE font example
// (C)2019-20 Pawel A. Hernik

/*
 128x64 ST7567 connections in SPI mode (only 5-6 wires between LCD and MCU):

 #01 LED  -> D6, GND or any pin via resistor
 #02 RST  -> D9 or any pin
 #03 CS   -> D10 or any pin
 #04 DC   -> D8 or any pin
 #05 SCK  -> D13/SCK
 #06 SDI  -> D11/MOSI
 #07 3V3  -> VCC (3.3V)
 #08 GND  -> GND
*/

#define LCD_BL  6
#define LCD_CS  10
#define LCD_DC  8
#define LCD_RST 9

#include "ST7567_FB.h"
#include <SPI.h>
ST7567_FB lcd(LCD_DC, LCD_RST, LCD_CS);

#include "RREFont.h"

#include "rre_5x8.h"
#include "rre_6x8.h"
//#include "rre_7x8_nosort.h"
#include "rre_bold_6x8.h"
#include "rre_7x12.h"
#include "rre_8x12.h"
#include "rre_vga_8x16.h"
#include "rre_c64_7x7.h"
#include "rre_c64_7x8.h"
#include "rre_term_10x16.h"
#include "rre_nadine_16x16.h"
#include "rre_arialb_16.h"
#include "rre_arialn_16.h"
#include "rre_ariali_16.h"
#include "rre_ocr_16x24.h"
#include "rre_12x16.h"
#include "rre_term_10x16.h"

#include "rre_bold13x20.h"
#include "rre_chicago_20x24.h"
#include "rre_ubuntu_32.h"
#include "rre_ubuntub_32.h"
#include "rre_digi_19x15.h"

#include "rre_fjg_8x16.h"
#include "rre_c_8x16.h"
#include "rre_x_8x16.h"
#include "rre_comp_8x16.h"
#include "rre_ncw_8x16.h"
#include "rre_kappa_8x16.h"
#include "rre_script_8x16.h"
#include "rre_fixed_8x16.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void setup() 
{
  Serial.begin(9600);

  // specific for ST7920 library
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, LOW);
  lcd.init();
  lcd.cls();

/*
  // specific for Adafruit
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  lcd.clearDisplay();
*/
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
}

void showFont(char *name)
{
  lcd.cls();
  font.setCR(0);
  font.printStr(0,font.getHeight()*0,name);
  font.printStr(0,font.getHeight()*1,"0123456789:;.-+*()!?/");
  font.printStr(0,font.getHeight()*2,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  font.printStr(0,font.getHeight()*3,"abcdefghijklmnopqrstuvwxyz");
  lcd.display();
  delay(5000);
}

void loop()
{
  // use REDUCE_MEM for more fonts
/*
  // part1
  font.setFont(&rre_c64_7x8); showFont("[c64_7x8]");
  font.setFont(&rre_c64_7x7); showFont("[c64_7x7]");
  font.setFont(&rre_5x8); showFont("[5x8]");
  font.setFont(&rre_6x8); showFont("[6x8]");
  font.setFont(&rre_bold_6x8); showFont("[bold_6x8]");
  font.setFont(&rre_7x12); showFont("[7x12]");
  font.setFont(&rre_8x12); showFont("[8x12]");
  font.setFont(&rre_vga_8x16); showFont("[vga_8x16]");

  font.setFont(&rre_arialn_16); showFont("[arialn_16]");
  font.setFont(&rre_arialb_16); showFont("[arialb_16]");
  font.setFont(&rre_ariali_16); showFont("[ariali_16]");

  font.setFont(&rre_12x16); showFont("[12x16]");
  font.setFont(&rre_term_10x16); showFont("[term_10x16]");
  //font.setFont(&rre_nadine_16x16); showFont("[nadine_16x16]");
  font.setFont(&rre_ocr_16x24); showFont("[ocr_16x24]");  
*/

  // part2
  font.setFont(&rre_fjg_8x16); showFont("[fjg_8x16]");
  font.setFont(&rre_kappa_8x16); showFont("[kappa_8x16]");
  font.setFont(&rre_ncw_8x16); showFont("[ncw_8x16]");
  font.setFont(&rre_c_8x16); showFont("[c_8x16]");
  font.setFont(&rre_fixed_8x16); showFont("[fixed_8x16]");
  font.setFont(&rre_x_8x16); showFont("[x_8x16]");
  font.setFont(&rre_comp_8x16); showFont("[comp_8x16]");
  font.setFont(&rre_script_8x16); showFont("[script_8x16]");

  font.setFont(&rre_Bold13x20); showFont("[13:20]");
  font.setFont(&rre_chicago_20x24); showFont("[chicago_20x24]");
  font.setFont(&rre_digi_19x15); showFont("[digi_19x15]");

  font.setFont(&rre_ubuntu_32); showFont("[ubuntu_32]");
  //font.setFont(&rre_ubuntub_32); showFont("[ubuntub_32]");

}

