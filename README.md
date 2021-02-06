# RREFont
Fast RRE Font rendering library

TL;DR:
RRE fonts are rendered using optimized set of rectangles instead of pixels like in typical bitmap approach.

YT Videos:

https://youtu.be/BJRCe_FB0vI

https://youtu.be/OOvzmHcou4E 

https://youtu.be/C6NsHxKbiVg

## Configuration

In RREFont.h use following flags:
```
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
```

If you are low with memory consider disabling all BG modes (support for non-flickering background) and other unused modes/types

To avoid flickering draw chars to the buffer in RAM then render it to LCD using lcd.drawImage()

## Main advantages:
- very fast rendering even on slow MCUs
- usually such fonts save a lot of memory especially for large fonts (for example 97x104 Times font takes only about 18k of flash instead of 120k bitmap font)
- RRE fonts can be easily scaled in both directions
- this library can be used also for rendering bitmaps (which in RRE format use much less memory)
- special vertical or horizontal lines mode supported for special applications
- separate pixels mode supported too
- the most efficient for large color LCDs
- there are over 40 various size fonts included
- antialiased fonts are possible https://github.com/cbm80amiga/ST7735_antialiased_rre_fonts

## Requirements:
- any Arduino graphics library supporting "fillRect()" function

## Tested with:
- own ST7920 gfx library
- SSD1306 OLED + Adafruit GFX Library
- own ST7735/ST7789/ILI9341 SPI + Adafruit GFX Library
- own ILI9327, ST7781 TFT libs

## Most used methods
```
void init(void (*rectFun)(int x, int y, int w, int h, int c), int swd, int sht);
   Inits the library with rectFun callback and screen dimensions
void setFillRectFun(void (*fun)(int x, int y, int w, int h, int c))
   Changes default fill rect callback
void setFont(RRE_Font *f)
   Sets current font
void setCR(uint8_t _cr)
   Switches 'carriage return' mode (text wrapping)
void setFg(int _fg)
void setBg(int _bg)
void setColor(int c)
void setColor(int c, int _bg)
   setFg/setBg are obsolete, use setColor to change background/foreground colors
   If background and foreground are the same only foreground color is used to render (transparent background)
void setBold(uint8_t _bold)
  Sets bold mode
void setSpacing(uint8_t sp)
void setSpacingY(uint8_t sp)
  Sets distances between characters and lines
void setScale(uint8_t _sx, uint8_t _sy)
void setScale(uint8_t s)
  Sets scale in X/Y axes
void setFontMinWd(uint8_t wd)
void setCharMinWd(uint8_t wd)
  Sets min char width for entire font to ensure fixed width rendering
void setDigitMinWd(uint8_t wd)
  Sets min char width to ensure fixed width rendering for digits, + and space
  Callback for isNumber() can be changed by setIsNumberFun()

int getWidth()
int getHeight()
  Returns fong global width/height
int charWidth(uint8_t c, int *_xmin=0)
  Returns the width for the given character
int strWidth(char *str)
  Returns the width for entire string
int drawChar(int x, int y, unsigned char c)
  Draws single character
int printStr(int xpos, int ypos, char *str)
  Draws char* string
```

## Support

If you find it useful and want to buy me a coffee or a beer:

https://www.paypal.me/cbm80amiga
