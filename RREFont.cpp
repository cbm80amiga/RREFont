// RRE Font support library
// (c) 2019-21 by Pawel A. Hernik

#include "RREFont.h"

// ----------------------------------------------------------------
RREFont::RREFont()
{
}
// ----------------------------------------------------------------
// fillRect function callback and screen width and height is necessary to initialize the library
void RREFont::init(void (*rectFun)(int x, int y, int w, int h, int c), int swd, int sht)
{
  fillRectFun = rectFun;
  scrWd = swd;
  scrHt = sht;
  cr = bold = dualChar = 0;
  isNumberFun = &isNumber;
  sx = sy = 1;
  fg = bg = 1;
}

// ---------------------------------
void RREFont::setFont(RRE_Font *f)
{
  rFont = f;
  minCharWd = minDigitWd = 0;
  spacing = 1;
  spacingY = 0;
}

// ---------------------------------
int RREFont::charWidthNoSort(uint8_t c, int *_xmin) 
{
  if(c<rFont->firstCh || c>rFont->lastCh) return c==' ' ? rFont->wd/2 : 0;
  uint16_t recIdx = pgm_read_word(&(rFont->offs[c-rFont->firstCh]));
  uint16_t recNum = pgm_read_word(&(rFont->offs[c-rFont->firstCh+1]))-recIdx;
  if(!recNum) return rFont->wd/2;
  int i, xmin = rFont->wd, xmax = 0;
  switch(rFont->type & 0xf) {
    case RRE_16B:
      for(i=0; i<recNum; i++) {
        uint16_t *rects = (uint16_t*)rFont->rects;
        int v = pgm_read_word(&rects[i+recIdx]);
        xf = v & 0x000f;
        wf = 1+((v & 0x0f00)>>8);
        if(xf<xmin) xmin = xf;
        if(xf+wf>xmax) xmax = xf+wf;
      }
      break;
    case RRE_24B:
      for(i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0]) & 0x3f;
        wf = 1+(pgm_read_byte(&rects[2]) & 0x3f);
        if(xf<xmin) xmin = xf;
        if(xf+wf>xmax) xmax = xf+wf;
      }
      break;
    case RRE_32B:
      for(i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*4;
        xf = pgm_read_byte(&rects[0]);
        wf = pgm_read_byte(&rects[2])+1;
        if(xf<xmin) xmin = xf;
        if(xf+wf>xmax) xmax = xf+wf;
      }
      break;
    case RRE_V24B:
      for(i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0]);
        wf = 1;
        if(xf<xmin) xmin = xf;
        if(xf+wf>xmax) xmax = xf+wf;
      }
      break;
    case RRE_H24B:
      for(i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0]);
        wf = pgm_read_byte(&rects[2])+1;
        if(xf<xmin) xmin = xf;
        if(xf+wf>xmax) xmax = xf+wf;
      }
      break;
  }
  //Serial.println(String((char)c)+" xmin="+String(xmin)+" xmax="+xmax);
  if(!xmax) { xmax=rFont->wd; xmin=0; }
  if(_xmin) *_xmin=xmin;
  return xmax-xmin;
}

// ----------------------------------------------------------------
int RREFont::charWidthOptim(uint8_t c, int *_xmin)
{
  if(c<rFont->firstCh || c>rFont->lastCh) return c==' '? rFont->wd/2 : 0;
  uint16_t recIdx = pgm_read_word(&(rFont->offs[c-rFont->firstCh]));
  uint16_t recNum = pgm_read_word(&(rFont->offs[c-rFont->firstCh+1]))-recIdx;
  if(!recNum) return rFont->wd/2;
  if(_xmin) *_xmin=0;
  int chWd = rFont->wd;
  int lastIdx = recNum-1+recIdx;
  switch(rFont->type & 0xf) {
#if ENABLE_RRE_16B==1
    case RRE_16B: {
      uint16_t *rects = (uint16_t*)rFont->rects;
      uint16_t v = pgm_read_word(&rects[lastIdx]);
      chWd = (v & 0x000f)+1+((v & 0x0f00)>>8);
      }
      break;
#endif
#if ENABLE_RRE_24B==1
    case RRE_24B: {
      uint8_t *rects = (uint8_t*)rFont->rects + lastIdx*3;
      chWd = (pgm_read_byte(&rects[0]) & 0x3f)+(pgm_read_byte(&rects[2]) & 0x3f)+1;
      }
      break;
#endif
#if ENABLE_RRE_32B==1
    case RRE_32B: {
      uint8_t *rects = (uint8_t*)rFont->rects + lastIdx*4;
      chWd = pgm_read_byte(&rects[0])+pgm_read_byte(&rects[2])+1;
      }
      break;
#endif
#if ENABLE_RRE_V16B==1
    case RRE_V16B: {
      uint16_t *rects = (uint16_t*)rFont->rects;
      uint16_t v = pgm_read_word(&rects[lastIdx]);
      chWd = (v & 0x3f)+1;
      }
      break;
#endif
#if ENABLE_RRE_H16B==1
    case RRE_H16B: {
      uint16_t *rects = (uint16_t*)rFont->rects;
      uint16_t v = pgm_read_word(&rects[lastIdx]);
      chWd = (v & 0x1f)+((v>>11) & 0x1f)+1;
      }
      break;
#endif
#if ENABLE_RRE_V24B==1
    case RRE_V24B: {
      uint8_t *rects = (uint8_t*)rFont->rects + lastIdx*3;
      chWd = pgm_read_byte(&rects[0])+1;
      }
      break;
#endif
#if ENABLE_RRE_H24B==1
    case RRE_H24B: {
      uint8_t *rects = (uint8_t*)rFont->rects + lastIdx*3;
      chWd = pgm_read_byte(&rects[0])+pgm_read_byte(&rects[2])+1;
      }
      break;
#endif
#if ENABLE_RRE_P164B==1
    case RRE_P16B:
      chWd = pgm_read_byte(rFont->rects+lastIdx*2)+1;
      break;
#endif
#if ENABLE_RRE_P8B==1
    case RRE_P8B:
      chWd = (pgm_read_byte(rFont->rects+lastIdx)&0xf)+1;
      break;
#endif
  }
  return chWd;
}

// ----------------------------------------------------------------
int RREFont::charWidth(uint8_t c, int *_xmin)
{
#if CONVERT_PL_CHARS==1
  c = convertPolish(c);
#endif
#if ENABLE_NOSORT==1
  int wd = (rFont->type & RRE_NO_SORT) ? charWidthNoSort(c,_xmin) : charWidthOptim(c,_xmin);
#else
  int wd = charWidthOptim(c,_xmin);
#endif
  int wdL = 0, wdR = spacing; // default spacing before and behind char
  if((*isNumberFun)(c) && minDigitWd>0) {
    if(minDigitWd>wd) {
      wdL = (minDigitWd-wd)/2;
      wdR += (minDigitWd-wd-wdL);
    }
  } else if(minCharWd>wd) {
    wdL = (minCharWd-wd)/2;
    wdR += (minCharWd-wd-wdL);
  }
  return (wd+wdL+wdR+bold)*sx;
}

// ----------------------------------------------------------------
int RREFont::drawChar(int x, int y, unsigned char c) 
{
#if CONVERT_PL_CHARS==1
  c = convertPolish(c);
#endif
  if(x>=scrWd || y>=scrHt || x+rFont->wd*sx-1<0 || y+rFont->ht*sy-1<0) return 0;
  if(c<rFont->firstCh || c>rFont->lastCh) return charWidth(c);
  uint16_t recIdx = pgm_read_word(&(rFont->offs[c-rFont->firstCh]));
  uint16_t recNum = pgm_read_word(&(rFont->offs[c-rFont->firstCh+1]))-recIdx;
#if ENABLE_NOSORT==1
  int xmin, chWd = (rFont->type & RRE_NO_SORT) ? charWidthNoSort(c,&xmin) : charWidthOptim(c,&xmin);
#else
  int xmin, chWd = charWidthOptim(c,&xmin);
#endif
  int wd=chWd, wdL = 0, wdR = spacing;
  if((*isNumberFun)(c) && minDigitWd>0) {
    if(minDigitWd>wd) {
      wdL = (minDigitWd-wd)/2;
      wdR += (minDigitWd-wd-wdL);
    }
  } else if(minCharWd>wd) {
    wdL = (minCharWd-wd)/2;
    wdR += (minCharWd-wd-wdL);
  }
  if(x+wd+wdL+wdR>scrWd) wdR = max(scrWd-x-wdL-wd, 0);
  if(x+wd+wdL+wdR>scrWd) wd  = max(scrWd-x-wdL, 0);
  if(x+wd+wdL+wdR>scrWd) wdL = max(scrWd-x, 0);
  wd+=wdR+wdL;
  int type = rFont->type & 0xf;
#if ENABLE_RRE_V16B_BG==1 || ENABLE_RRE_H16B_BG==1 || ENABLE_RRE_V24B_BG==1 || ENABLE_RRE_H24B_BG==1
  if(bg!=fg && (!recNum || type==RRE_16B ||type==RRE_24B ||type==RRE_32B))
#else
  if(bg!=fg)
#endif
    (*fillRectFun)(x, y, (wd+bold)*sx, rFont->ht*sy, bg);
  x+=wdL*sx;
  if(!recNum) return (wd+bold)*sx;
  switch(type) {
#if ENABLE_RRE_16B==1
    case RRE_16B:
      for(int i=0; i<recNum; i++) {
        uint16_t *rects = (uint16_t*)rFont->rects;
        uint16_t v = pgm_read_word(&rects[i+recIdx]);
        xf = (v & 0x000f)-xmin;
        yf = (v & 0x00f0)>>4;
        wf = ((v & 0x0f00)>>8)+1;
        hf = ((v & 0xf000)>>12)+1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
        //Serial.println(String(i)+" "+xf+" "+yf+" "+wf+" "+hf);
      }
      break;
#endif
#if ENABLE_RRE_24B==1
    case RRE_24B:
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = (pgm_read_byte(&rects[0]) & 0x3f)-xmin;
        yf = (pgm_read_byte(&rects[1]) & 0x3f);
        wf = (pgm_read_byte(&rects[2]) & 0x3f)+1;
        hf = 1+(((pgm_read_byte(&rects[0]) & 0xc0)>>6) | ((pgm_read_byte(&rects[1]) & 0xc0)>>4) | ((pgm_read_byte(&rects[2]) & 0xc0)>>2));
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_32B==1
    case RRE_32B:
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*4;
        xf = pgm_read_byte(&rects[0])-xmin;
        yf = pgm_read_byte(&rects[1]);
        wf = pgm_read_byte(&rects[2])+1;
        hf = pgm_read_byte(&rects[3])+1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
        //Serial.println(String(i)+" "+xf+" "+yf+" "+wf+" "+hf);
      }
      break;
#endif
#if ENABLE_RRE_V16B==1
    case RRE_V16B:
#if ENABLE_RRE_V16B_BG==1
      if(bg!=fg) {
        if(wdL>0) (*fillRectFun)(x-wdL*sx, y, wdL*sx, rFont->ht*sy, bg);
        uint16_t *rects = (uint16_t*)rFont->rects+recIdx;
        int idx=0;
        while(idx<recNum) {
          uint16_t v = pgm_read_word(rects+idx);
          xf = (v & 0x3f)-xmin;
          int ybg=0,xfCur=xf;
          while(xf==xfCur && idx<recNum) {
            v = pgm_read_word(rects+idx);
            yf = (v>>6) & 0x1f;
            hf = ((v>>11) & 0x1f)+1;
            if(yf>ybg) (*fillRectFun)(x+xfCur*sx, y+ybg*sy, 1*sx, (yf-ybg)*sy, bg);
            ybg = yf+hf;
            (*fillRectFun)(x+xfCur*sx, y+yf*sy, bold+1*sx, hf*sy, fg);
            idx++;
            v = pgm_read_word(rects+idx);
            xf = (v & 0x3f)-xmin;
          }
          // last bg line
          if(ybg<rFont->ht) (*fillRectFun)(x+xfCur*sx, y+ybg*sy, bold+1*sx, (rFont->ht-ybg)*sy, bg);
        }
        if(wdR>0) (*fillRectFun)(x+chWd*sx, y, wdR*sx, rFont->ht*sy, bg);
      } else
#endif
      for(int i=0; i<recNum; i++) {
        uint16_t *rects = (uint16_t*)rFont->rects;
        uint16_t v = pgm_read_word(&rects[i+recIdx]);
        xf = (v & 0x3f)-xmin;
        yf = (v>>6) & 0x1f;
        hf = ((v>>11) & 0x1f)+1;
        wf = 1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_H16B==1
    case RRE_H16B:
#if ENABLE_RRE_H16B_BG==1
      if(bg!=fg) {
        if(wdL>0) (*fillRectFun)(x-wdL*sx, y, wdL*sx, rFont->ht*sy, bg);
        uint16_t *rects = (uint16_t*)rFont->rects;
        int idx=0,yfCur=0;
        while(idx<recNum) {
          uint16_t v = pgm_read_word(rects+idx+recIdx);
          yf = (v>>5) & 0x3f;
          if(yf-yfCur>1 /*|| (yf-yfCur>0 && yfCur==0)*/) (*fillRectFun)(x+0*sx, y+yfCur*sy, chWd*sx, (yf-yfCur)*sy, bg);
          //Serial.print(idx); Serial.print("  "); Serial.print(yf); Serial.print("  "); Serial.println(yfCur);
          yfCur=yf;
          int xbg=0;
          while(yf==yfCur && idx<recNum) {
            xf = v & 0x1f;
            wf = ((v>>11) & 0x1f)+1;
            if(xf>xbg) (*fillRectFun)(x+xbg*sx, y+yf*sy, (xf-xbg)*sx, 1*sy, bg);
            xbg = xf+wf;
            //if(idx==recNum-1) fg=0x07E0; else fg=0xffff;
            (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, 1*sy, fg);
            idx++;
            v = pgm_read_word(rects+idx+recIdx);
            yf = (v>>5) & 0x3f;
          }
          // last bg line
          if(xbg<chWd) (*fillRectFun)(x+xbg*sx, y+yfCur*sy, (chWd-xbg)*sx, 1*sy, bg);
          //Serial.print(idx); Serial.print(" ++ "); Serial.print(yf); Serial.print("  "); Serial.println(yfCur);
          if(idx==recNum-1 && yfCur<rFont->ht) (*fillRectFun)(x+0*sx, y+yfCur*sy, chWd*sx, (yfCur<rFont->ht-yfCur)*sy, bg);
        }
        if(wdR>0) (*fillRectFun)(x+chWd*sx, y, wdR*sx, rFont->ht*sy, bg);
      } else
#endif
      for(int i=0; i<recNum; i++) {
        uint16_t *rects = (uint16_t*)rFont->rects;
        uint16_t v = pgm_read_word(&rects[i+recIdx]);
        xf = (v & 0x1f)-xmin;
        yf = (v>>5) & 0x3f;
        wf = ((v>>11) & 0x1f)+1;
        hf = 1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_V24B==1
    case RRE_V24B:
#if ENABLE_RRE_V24B_BG==1
      if(bg!=fg) {
        if(wdL>0) (*fillRectFun)(x-wdL*sx, y, wdL*sx, rFont->ht*sy, bg);
        uint8_t *rects = (uint8_t*)rFont->rects + recIdx*3;
        int idx=0;
        while(idx<recNum*3) {
          xf = pgm_read_byte(rects+idx+0);
          int ybg=0,xfCur=xf;
          while(xf==xfCur && idx<recNum*3) {
            yf = pgm_read_byte(rects+idx+1);
            hf = pgm_read_byte(rects+idx+2)+1;
            if(yf>ybg) (*fillRectFun)(x+xfCur*sx, y+ybg*sy, 1*sx, (yf-ybg)*sy, bg);
            ybg = yf+hf;
            (*fillRectFun)(x+xfCur*sx, y+yf*sy, bold+1*sx, hf*sy, fg);
            idx += 3;
            xf = pgm_read_byte(rects+idx+0);
          }
          // last bg line
          if(ybg<rFont->ht) (*fillRectFun)(x+xfCur*sx, y+ybg*sy, bold+1*sx, (rFont->ht-ybg)*sy, bg);
        }
        if(wdR>0) (*fillRectFun)(x+chWd*sx, y, wdR*sx, rFont->ht*sy, bg);
      } else
#endif
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0])-xmin;
        yf = pgm_read_byte(&rects[1]);
        hf = pgm_read_byte(&rects[2])+1;
        wf = 1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_H24B==1
    case RRE_H24B:
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0])-xmin;
        yf = pgm_read_byte(&rects[1]);
        wf = pgm_read_byte(&rects[2])+1;
        hf = 1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_P16B==1
    case RRE_P16B:
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*2;
        xf = pgm_read_byte(&rects[0])-xmin;
        yf = pgm_read_byte(&rects[1]);
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+sx, sy, fg);
      }
      break;
#endif
#if ENABLE_RRE_P8B==1
    case RRE_P8B:
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx);
        xf = (pgm_read_byte(&rects[0])&0xf)-xmin;
        yf = pgm_read_byte(&rects[1])>>4;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+sx, sy, fg);
      }
      break;
#endif
  }
  return (wd+bold)*sx;
}

// ----------------------------------------------------------------
int RREFont::strWidth(char *str)
{
  int wd = 0;
  while (*str) wd += charWidth(*str++);
  return wd;
}

// ----------------------------------------------------------------
int RREFont::printStr(int xpos, int ypos, char *str)
{
  unsigned char ch;
  int stl, row;
  int x = xpos;
  int y = ypos;
  int wd = strWidth(str);

  if(x==ALIGN_RIGHT) x = scrWd - wd; // right = -1
  else if(x<0) x = (scrWd - wd) / 2; // center = -2
  if(x<0) x = 0; // left

  while(*str) {
    char ch = *str++;
    int wd = charWidth(ch);
    if(ch==10 || cr && x+wd>=scrWd) { 
      x = xpos; 
      y += rFont->ht * sy + spacingY;
      if(ch==10) continue;
    }
    drawChar(x,y,ch);
    x+=wd;
  }
  return x;
}
// ----------------------------------------------------------------
// callbacks
bool RREFont::isNumber(uint8_t ch)
{
  return isdigit(ch) || ch==' ' || ch=='-';
}

bool RREFont::isNumberExt(uint8_t ch)
{
  return isdigit(ch) || ch=='-' || ch=='+' || ch=='.' || ch==' ';
}
// ----------------------------------------------------------------
unsigned char RREFont::convertPolish(unsigned char _c)
{
  unsigned char pl, c = _c;
  if(c==196 || c==197 || c==195) {
    dualChar = c;
    return 0;
  }
  if(dualChar) { // UTF8 coding
    switch(_c) {
      case 133: pl = 1+9; break; // 'π'
      case 135: pl = 2+9; break; // 'Ê'
      case 153: pl = 3+9; break; // 'Í'
      case 130: pl = 4+9; break; // '≥'
      case 132: pl = dualChar==197 ? 5+9 : 1; break; // 'Ò' and '•'
      case 179: pl = 6+9; break; // 'Û'
      case 155: pl = 7+9; break; // 'ú'
      case 186: pl = 8+9; break; // 'ü'
      case 188: pl = 9+9; break; // 'ø'
      //case 132: pl = 1; break; // '•'
      case 134: pl = 2; break; // '∆'
      case 152: pl = 3; break; // ' '
      case 129: pl = 4; break; // '£'
      case 131: pl = 5; break; // '—'
      case 147: pl = 6; break; // '”'
      case 154: pl = 7; break; // 'å'
      case 185: pl = 8; break; // 'è'
      case 187: pl = 9; break; // 'Ø'
      default:  return c; break;
    }
    dualChar = 0;
  } else   
  switch(_c) {  // Windows coding
    case 165: pl = 1; break; // •
    case 198: pl = 2; break; // ∆
    case 202: pl = 3; break; //  
    case 163: pl = 4; break; // £
    case 209: pl = 5; break; // —
    case 211: pl = 6; break; // ”
    case 140: pl = 7; break; // å
    case 143: pl = 8; break; // è
    case 175: pl = 9; break; // Ø
    case 185: pl = 10; break; // π
    case 230: pl = 11; break; // Ê
    case 234: pl = 12; break; // Í
    case 179: pl = 13; break; // ≥
    case 241: pl = 14; break; // Ò
    case 243: pl = 15; break; // Û
    case 156: pl = 16; break; // ú
    case 159: pl = 17; break; // ü
    case 191: pl = 18; break; // ø
    default:  return c; break;
  }
  return pl+'~'+1;
}
// ---------------------------------
