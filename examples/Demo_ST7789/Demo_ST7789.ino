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
#include "rre_8x12.h"
#include "rre_arialdig47b.h"
#include "rre_arialdig47n.h"
#include "rre_arialdig47n-v.h"
#include "rre_arialdig47n-h.h"
#include "rre_bold13x20.h"
#include "rre_arialb_16.h"
#include "rre_ubuntu_32.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }


int outline(int x, int y, char *str, int c1, int c2)
{
  font.setColor(c1);
  font.printStr(x-1,y-1,str);
  font.printStr(x-0,y-1,str);
  font.printStr(x+1,y-1,str);
  font.printStr(x-1,y+1,str);
  font.printStr(x-0,y+1,str);
  font.printStr(x+1,y+1,str);
  font.printStr(x-1,y,str);
  font.printStr(x+1,y,str);
  font.setColor(c2);
  font.printStr(x,y,str);
}
char buf[100];

void setup() 
{
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
}

void showFullFont()
{
  for(int i=0;i<96; i++) buf[i]=i+' ';
  buf[96]=0;
  font.setCR(1);
  font.printStr(0,0,buf);
}

void loop()
{
  int i;
  unsigned long cnt;

   // 8x12 vga full font, fixed width test
  font.setColor(WHITE);
  font.setFont(&rre_8x12);
  font.setScale(2,2);
  font.setCharMinWd(8); font.setSpacing(0);
  lcd.fillScreen(BLACK);
  showFullFont();
  delay(4000);

   // Arial bold full font
  font.setScale(1,1);
  font.setFont(&rre_arialb_16);
  lcd.fillScreen(WHITE);
  font.setColor(BLACK);
  showFullFont();
  delay(4000);

  // outline
  lcd.fillScreen(BLACK);
  font.setFont(&rre_arialb_16);
  font.setCR(0);
  font.setScale(2,2);
  font.setColor(WHITE);
  font.printStr(16,4,"Regular text:\nABCDabcd0123");
  outline(16,84,"Outline text:\nABCDabcd0123",WHITE,BLACK);
  outline(16,164,"Outline text:\nABCDabcd0123",CYAN,BLUE);
  delay(4000);

  // 4 big digits 
  lcd.fillScreen(BLACK);
  font.setScale(1,2);
  for(i=cnt=0;i<100;i++) {
    snprintf(buf,5,"%04u",cnt);
    font.setFont(&rre_ArialDig47n);
    font.setCharMinWd(31+5);
    font.setColor(WHITE,BLACK);
    font.printStr(0,0,buf);
    font.setFont(&rre_ArialDig47n);
    font.setCharMinWd(31+5);
    font.setColor(YELLOW,RED); 
    font.printStr(0,120,buf);
    cnt+=1234;
  }

/*
  // 4 big digits, outline
  lcd.fillScreen(BLACK);
  font.setScale(1,2);
  for(i=cnt=0;i<50;i++) {
    lcd.fillRect(0,7,4*35,2*48,BLACK);
    snprintf(buf,5,"%04u",cnt);
    font.setFont(&rre_ArialDig47b);
    font.setCharMinWd(31);
    outline(1,8,buf,YELLOW,RED);
    delay(1000);
    cnt+=1234;
  }
*/

  // 13x20 digits test
  lcd.fillScreen(BLACK);
  font.setScale(1,2);
  for(i=cnt=0;i<200;i++) {
    font.setFont(&rre_Bold13x20);
    font.setCharMinWd(15);
    snprintf(buf,10,"%09lu",cnt);
    cnt+=12345;
    font.setColor(WHITE,BLACK);
    font.printStr(0,0,buf);
    font.setColor(YELLOW,BLACK);
    snprintf(buf,10,"12:34-9+%d",(cnt/100000)%10);
    font.printStr(0,50,buf);
    font.setCharMinWd(0); font.setSpacing(2);
    font.setColor(GREEN,BLACK);
    snprintf(buf,20,"8.22+9-1.%03d ",(cnt/100000)%100);
    font.printStr(0,100,buf);
  }

  // 13x20 digits test + scale Y and X
  lcd.fillScreen(BLACK);
  for(i=cnt=0;i<200;i++) {
    font.setFont(&rre_Bold13x20);
    font.setCharMinWd(13);
    snprintf(buf,10,"%09lu",cnt);
    cnt+=12345;
    font.setColor(GREEN,BLACK);
    font.setScale(1,1); font.printStr(0,0,buf);
    font.setColor(YELLOW,BLACK);
    font.setScale(1,2); font.printStr(0,34,buf);
    font.setColor(CYAN,BLACK);
    font.setScale(2,1); font.printStr(0,90,buf);
  }
  font.setScale(1,1);

  // different background modes
  lcd.fillScreen(BLACK);
  font.setFont(&rre_ubuntu_32);
  lcd.fillRect(16,16,240-16*2,40*4-16*2,RED);
  font.setColor(YELLOW);       font.printStr(0,40*0,"ABcdeIMNmn012345");
  font.setColor(YELLOW, BLUE); font.printStr(0,40*1,"ABcdeIMNmn012345");
  font.setColor(BLACK);        font.printStr(0,40*2,"ABcdeIMNmn012345");
  font.setColor(BLACK,GREEN);  font.printStr(0,40*3,"ABcdeIMNmn012345");
  delay(4000);
}

