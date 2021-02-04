// RRE Font support library
// (c) 2019-21 by Pawel A. Hernik

#ifndef _RRE_FONT_H
#define _RRE_FONT_H 

#include <Arduino.h>

// ---------  CONFIG ----------
// adds 400 bytes
#define CONVERT_PL_CHARS    1
// adds 720 bytes
#define ENABLE_NOSORT       0
// enable only unused types to save memory (up to 4-5 kbytes)
#define ENABLE_RRE_16B      1
#define ENABLE_RRE_24B      1
#define ENABLE_RRE_32B      1
#define ENABLE_RRE_V16B     1
#define ENABLE_RRE_V16B_BG  1  // support for non-flickering background clear
#define ENABLE_RRE_H16B     1
#define ENABLE_RRE_H16B_BG  1
#define ENABLE_RRE_V24B     1
#define ENABLE_RRE_V24B_BG  1
#define ENABLE_RRE_H24B     1
#define ENABLE_RRE_H24B_BG  0
#define ENABLE_RRE_P8B      0
#define ENABLE_RRE_P16B     1
// ----------------------------

#define RRE_16B     0    // 16x16, rects (X4Y4W4H4)
#define RRE_V16B    1    // 64x32, lines (X6Y5W0H5)
#define RRE_H16B    2    // 32x64, lines (X5Y6W5H0)
#define RRE_24B     3    // 64x64, rects (X6Y6W6H6)
#define RRE_V24B    4    // 256x256, vertical   lines (X8Y8W0H8)
#define RRE_H24B    5    // 256x256, horizontal lines (X8Y8W8H0)
#define RRE_32B     6    // 256x256, rects (X8Y8W8H8)
#define RRE_P8B     7    // 16x16,   pixels (X4Y4W0H0)
#define RRE_P16B    8    // 256x256, pixels (X8Y8W0H0)
#define RRE_NO_SORT 0x80 // old fonts not optimized for fast width calculation

#define ALIGN_LEFT    0
#define ALIGN_RIGHT  -1 
#define ALIGN_CENTER -2

struct RRE_Font {
  uint8_t type;
  uint8_t wd;
  uint8_t ht;
  uint8_t firstCh;
  uint8_t lastCh;
  const uint8_t *rects;
  const uint16_t *offs;
};
// ---------------------------------
class RREFont {
public:
  RREFont();
  void init(void (*rectFun)(int x, int y, int w, int h, int c), int swd, int sht);
  void setFont(RRE_Font *f);
  void setCR(uint8_t _cr) { cr = _cr; }
  void setFg(int _fg) { fg = _fg; }
  void setBg(int _bg) { bg = _bg; }
  void setColor(int c) { fg = bg = c; }
  void setColor(int c, int _bg) { fg = c; bg = _bg; }
  void setBold(uint8_t _bold) { bold = _bold; }
  void setSpacing(uint8_t sp) { spacing = sp; }
  void setSpacingY(uint8_t sp) { spacingY = sp; }
  void setScale(uint8_t _sx, uint8_t _sy) { sx = _sx; sy = _sy; }
  void setScale(uint8_t s) { sx = sy = s; }
  void setFontMinWd(uint8_t wd) { minCharWd = wd; }
  void setCharMinWd(uint8_t wd) { minCharWd = wd; }
  void setDigitMinWd(uint8_t wd) { minDigitWd = wd; }
  unsigned char convertPolish(unsigned char _c);
  int getWidth() { return rFont ? rFont->wd : 0; }
  int getHeight() { return rFont ? rFont->ht : 0; }
  int charWidthNoSort(uint8_t c, int *_xmin);
  int charWidthOptim(uint8_t c, int *_xmin=0);
  int charWidth(uint8_t c, int *_xmin=0);
  int drawChar(int x, int y, unsigned char c);
  int strWidth(char *str);
  int printStr(int xpos, int ypos, char *str);
  void setIsNumberFun(bool (*fun)(uint8_t)) { isNumberFun=fun; }
  void setFillRectFun(void (*fun)(int x, int y, int w, int h, int c)) { fillRectFun=fun; }

  static bool isNumber(uint8_t ch);
  static bool isNumberExt(uint8_t ch);

public:
  bool (*isNumberFun)(uint8_t ch);
  void (*fillRectFun)(int x, int y, int w, int h, int c);
  RRE_Font *rFont;
  int xf,yf,wf,hf;
  int scrWd, scrHt;

  int spacing = 1;
  int spacingY = 0;
  int bold = 0;
  int sx = 1;
  int sy = 1;
  int cr = 0;
  int fg = 1;
  int bg = 1;
  uint8_t minCharWd;
  uint8_t minDigitWd;
  uint8_t dualChar;
};
#endif

