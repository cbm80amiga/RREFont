# RREFont
Fast RRE Font rendering library

TL;DR:
RRE fonts are rendered using optimized set of rectangles instead of pixels like in typical bitmap approach.

YT Videos:

https://youtu.be/BJRCe_FB0vI

https://youtu.be/OOvzmHcou4E 

## Configuration

In RREFont.h use following flags:
```
// -------  CONFIG ---------
// saves 400 bytes
#define CONVERT_PL_CHARS 0
// saves 720 bytes
#define ENABLE_NOSORT    0
// disable unused types to save memory (up to 4100 bytes)
#define ENABLE_RRE_16B   1
#define ENABLE_RRE_24B   1
#define ENABLE_RRE_32B   1
#define ENABLE_RRE_V16B  1
#define ENABLE_RRE_H16B  1
#define ENABLE_RRE_V24B  1
#define ENABLE_RRE_H24B  1
// --------------------------
```

## Main advantages:
- very fast rendering even on slow MCUs
- usually such fonts save a lot of memory especially for large fonts (for example 97x104 Times font takes only about 18k of flash instead of 120k bitmap font)
- RRE fonts can be easily scaled in both directions
- this library can be used also for rendering bitmaps (which in RRE format use much less memory)
- special vertical or horizontal lines mode supported for special applications
- the most efficient for large color LCDs
- there are over 40 various size fonts included

## Requirements:
- any Arduino graphics library supporting "fillRect()" function

## Tested with:
- own ST7920 gfx library
- SSD1306 OLED + Adafruit GFX Library
- own ST7789 SPI + Adafruit GFX Library
- own ILI9327, ST7781 TFT libs

If you find it useful and want to buy me a coffee or a beer:

https://www.paypal.me/cbm80amiga
