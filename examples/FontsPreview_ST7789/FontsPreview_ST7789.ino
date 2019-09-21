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
  lcd.init(SCR_WD, SCR_HT);
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
}

void showFont(char *name)
{
  lcd.fillScreen(BLACK);
  font.setCR(0);
  font.setColor(WHITE);
  font.printStr(0,font.getHeight()*0,name);
  font.printStr(0,font.getHeight()*1,"0123456789:;.-+*()!?/");
  font.printStr(0,font.getHeight()*2,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  font.printStr(0,font.getHeight()*3,"abcdefghijklmnopqrstuvwxyz");
  delay(5000);
}

void loop()
{
  // use REDUCE_MEM in RREFont.h to free some flash memory
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
  //font.setFont(&rre_ocr_16x24); showFont("[ocr_16x24]");  
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

  //font.setFont(&rre_ubuntu_32); showFont("[ubuntu_32]");
  //font.setFont(&rre_ubuntub_32); showFont("[ubuntub_32]");
}

