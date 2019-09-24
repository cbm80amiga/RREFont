# RREFont
Fast RRE Font rendering library

TL;DR:
RRE fonts are rendered using optimized set of rectangles instead of pixels like in typical bitmap approach.

Main advantages:
- very fast rendering even on slow MCUs
- usually such fonts save a lot of memory especially for large fonts (for example 97x104 Times font takes only about 18k of flash instead of 120k bitmap font)
- RRE fonts can be easily scaled in both directions
- this library can be used also for rendering bitmaps (which in RRE format use much less memory)
- special vertical or horizontal lines mode supported for special applications
- the most efficient for large color LCDs

Requirements:
- any Arduino graphics library supporting "fillRect()" function

Tested with:
- own ST7920 gfx library
- SSD1306 OLED + Adafruit GFX Library
- own ST7789 SPI + Adafruit GFX Library
- own ILI9327, ST7781 TFT libs

Video:
https://youtu.be/BJRCe_FB0vI
