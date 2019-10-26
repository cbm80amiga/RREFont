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
#include "rre_arialdig47b.h"
#include "rre_arialdig47n.h"
#include "rre_bold13x20.h"
#include "rre_arialb_16.h"
//#include "rre_arialn_16.h"
//#include "rre_ariali_16.h"

RREFont font;

// needed for RREFont library initialization, define your fillRect
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

char buf[100];

void setup() 
{
  Serial.begin(9600);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
  lcd.cls();

  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
}

void showFullFont()
{
  lcd.cls();
  for(int i=0;i<96; i++) buf[i]=i+' ';
  buf[96]=0;
  font.setCR(1);
  font.printStr(0,0,buf);
  lcd.display();
}

void loop()
{
  int i;
  unsigned long cnt;

   // 8x12 vga full font, fixed width test
  font.setFont(&rre_8x12); font.rFont->ht=11;
  font.setCharMinWd(8); font.setSpacing(0);
  showFullFont();
  delay(4000);

  lcd.cls();
  font.setCR(0);
  font.setFillRectFun(customRect);
  font.setFont(&rre_8x12);
  font.printStr(1,4,"Regular text:\nABCDabcd0123");
  outlineD(1,36,"Outline text:\nABCDabcd0123",16);
  lcd.display();
  delay(4000);

   // Arial bold full font
  //font.setFont(&rre_arialb_16);
  //showFullFont();
  //delay(4000);

  // 4 big digits 
  for(i=cnt=0;i<200;i++) {
    snprintf(buf,5,"%04u",cnt);
    //font.setFont(&rre_ArialDig47b);
    font.setFont(&rre_ArialDig47n);
    lcd.cls();
    font.setCharMinWd(31);
    font.setFillRectFun(customRect);
    font.printStr(0,8,buf);
    cnt+=1234;
    lcd.display();
  }

  // 4 big digits dithered
  for(i=cnt=0;i<200;i++) {
    snprintf(buf,5,"%04u",cnt);
    font.setFont(&rre_ArialDig47b);
    //font.setFont(&rre_ArialDig47n);
    lcd.cls();
    font.setCharMinWd(31);
    font.setFillRectFun(customRectDither);
    lcd.setDither(12);
    font.printStr(0,8,buf);
    cnt+=1234;
    lcd.display();
  }

  // 4 big digits, outline with dithered inside
  for(i=cnt=0;i<100;i++) {
    snprintf(buf,5,"%03u",cnt);
    //font.setFont(&rre_ArialDig47b);
    font.setFont(&rre_ArialDig47n);
    lcd.cls();
    font.setCharMinWd(31);
    font.setFillRectFun(customRectDither);
    outlineD(1,8,buf,14);
    cnt+=1234;
    lcd.display();
  }


  // 13x20 digits test
  for(i=cnt=0;i<300;i++) {
    font.setFont(&rre_Bold13x20);
    lcd.cls();
    font.setCharMinWd(13);
    snprintf(buf,10,"%09lu",cnt);
    cnt+=12345;
    font.setFillRectFun(customRectDither);
    lcd.setDither(12);
    font.printStr(0,0,buf);
    lcd.setDither(8);
    snprintf(buf,10,"12:34-9+%d",(cnt/100000)%10);
    font.printStr(0,22,buf);
    font.setCharMinWd(0);
    lcd.setDither(16);
    snprintf(buf,20,"8.22+9-1.%02d",(cnt/100000)%100);
    font.printStr(0,44,buf);
    lcd.display();
  }
  font.setFillRectFun(customRect);


  // 13x20 digits test + scale Y and X
  for(i=cnt=0;i<300;i++) {
    font.setFont(&rre_Bold13x20);
    lcd.cls();
    font.setCharMinWd(13);
    snprintf(buf,10,"%09lu",cnt);
    cnt+=12345;
    font.setScale(1,2); font.printStr(0,0,buf);
    font.setScale(2,1); font.printStr(0,44,buf);
    lcd.display();
  }
  font.setScale(1,1);


  // different background modes
  lcd.cls();
  font.setFont(&rre_arialb_16);
  lcd.setDither(8); lcd.fillRectD(8,8,SCR_WD-16,16*4-16,1);
  font.setColor(1);   font.printStr(0, 0,"ABcdeIMNmn0123");
  font.setColor(1,0); font.printStr(0,16,"ABcdeIMNmn0123");
  font.setColor(0);   font.printStr(0,32,"ABcdeIMNmn0123");
  font.setColor(0,1); font.printStr(0,48,"ABcdeIMNmn0123");
  lcd.display();
  font.setColor(1);
  delay(3000);

}

