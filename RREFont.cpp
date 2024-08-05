// RRE Font library
// (c) 2019-24 by Pawel A. Hernik

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
// used only for very old fonts where last rect was not placed at max X
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
// char width taken from last rect (with max X)
int RREFont::charWidth(uint8_t c)
{
  if(c<rFont->firstCh || c>rFont->lastCh) return c==' '? rFont->wd/2 : 0;
  uint16_t recIdx = pgm_read_word(&(rFont->offs[c-rFont->firstCh]));
  uint16_t recNum = pgm_read_word(&(rFont->offs[c-rFont->firstCh+1]))-recIdx;
  if(!recNum) return c==' ' ? rFont->wd/2 : 0;
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
    case RRE_V16B:
      chWd = (pgm_read_word(rFont->rects+lastIdx*2) & 0x3f)+1;
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
    case RRE_V24B:
      chWd = pgm_read_byte(rFont->rects + lastIdx*3)+1;
      break;
#endif
#if ENABLE_RRE_H24B==1
    case RRE_H24B: {
      uint8_t *rects = (uint8_t*)rFont->rects + lastIdx*3;
      chWd = pgm_read_byte(&rects[0])+pgm_read_byte(&rects[2])+1;
      }
      break;
#endif
#if ENABLE_RRE_P8B==1
    case RRE_P8B:
      chWd = (pgm_read_byte(rFont->rects+lastIdx)&0xf)+1;
      break;
#endif
#if ENABLE_RRE_P16B==1
    case RRE_P16B:
      chWd = pgm_read_byte(rFont->rects+lastIdx*2)+1;
      break;
#endif
  }
  return chWd;
}

// ----------------------------------------------------------------
// charWidth + spacing, multiplied by scale
int RREFont::charWidthFull(uint8_t c)
{
#if CONVERT_PL_CHARS==1
  c = convertPolish(c);
#endif
#if ENABLE_NOSORT==1
  int wd = (rFont->type & RRE_NO_SORT) ? charWidthNoSort(c,_xmin) : charWidth(c);
#else
  int wd = charWidth(c);
#endif
  if(wd<=0) return 0;
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
  if((c<rFont->firstCh || c>rFont->lastCh) && c!=' ') return 0;

  int xmin=0; // left for compatibility with old NoSort fonts
#if ENABLE_NOSORT==1
  int chWd = (rFont->type & RRE_NO_SORT) ? charWidthNoSort(c,&xmin) : charWidth(c);
#else
  int chWd = charWidth(c);
#endif
  int wd = chWd, wdL = 0, wdR = spacing;
  if(minDigitWd>0 && (*isNumberFun)(c)) {
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
  if(bg!=fg && (c==' ' || type==RRE_16B || type==RRE_24B || type==RRE_32B || type==RRE_P8B || type==RRE_P16B))
#else
  if(bg!=fg)
#endif 
    (*fillRectFun)(x, y, (wd+bold)*sx, rFont->ht*sy, bg);

  if(c==' ') return (wd+bold)*sx;
  uint16_t recIdx = pgm_read_word(&(rFont->offs[c-rFont->firstCh]));
  uint16_t recNum = pgm_read_word(&(rFont->offs[c-rFont->firstCh+1]))-recIdx;
  if(!recNum) return 0;
  x+=wdL*sx;
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
        yf=hf=0;
        uint16_t v;
        int yb,hb,xfo=0;
        for(int i=0; i<recNum; i++) {
          v = pgm_read_word(rects+i);
          xf = (v & 0x3f)-xmin;
          if(xf!=xfo) {
            if(xf-xfo>1) (*fillRectFun)(x+(xfo+1)*sx, y, (xf-xfo-1)*sx, rFont->ht*sy, bg);
            (*fillRectFun)(x+xfo*sx, y+(yf+hf)*sy, 1*sx, (rFont->ht-yf-hf)*sy, bg);
            yf=hf=0;
          }
          yb = yf+hf;
          yf = (v>>6) & 0x1f;
          hf = ((v>>11) & 0x1f)+1;
          hb = yf-yb;
          (*fillRectFun)(x+xf*sx, y+yb*sy,      1*sx, hb*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yf*sy, bold+1*sx, hf*sy, fg);
          xfo=xf;
        }
        (*fillRectFun)(x+xfo*sx, y+(yf+hf)*sy, 1*sx, (rFont->ht-yf-hf)*sy, bg);
        
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
        /*
        // fast but very complex
        uint16_t *rects = (uint16_t*)rFont->rects+recIdx;
        xf=wf=0;
        int xfend,yfend,wfend;
        uint16_t v;
        v = pgm_read_word(rects+recNum-1);
        yfend = (v>>5) & 0x3f;
        xfend = v & 0x1f;
        wfend = ((v>>11) & 0x1f)+1;
        int xb,wb,yfo=0;
        for(int i=0; i<recNum-1; i++) {
          v = pgm_read_word(rects+i);
          yf = (v>>5) & 0x3f;
          if(yf!=yfo) { // yf = next column
            if(yfend==yfo) { // if current column = yfend rect finish with yfrect
              xb = xf+wf;
              xf = xfend;
              wf = wfend;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
            }
            if(yfend>yfo && yfend<yf) { // if empty columns check yfend rect
              xb = 0;
              xf = xfend;
              wf = wfend;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
            }
            if(yf-yfo>1) { // if empty columns - fill with bg color
              if(yfend<yf && yfend>yfo) {
                (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yfend-yfo-1)*sy, bg);
                (*fillRectFun)(x, y+(yfend+1)*sy, chWd*sx, (yf-yfend-1)*sy, bg);
              } else
                (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yf-yfo-1)*sy, bg);
            }
            (*fillRectFun)(x+(xf+wf)*sx, y+yfo*sy, (chWd-xf-wf)*sx, 1*sy, bg);
            xf=wf=0;
          }
          xb = xf+wf;
          xf = v & 0x1f;
          wf = ((v>>11) & 0x1f)+1;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yf*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, 1*sy, fg);
          yfo=yf;
        }

        yf = rFont->ht;
        if(yfend==yfo) {
          xb = xf+wf;
          xf = xfend;
          wf = wfend;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
        }
        if(yfend>yfo) {
          xb = 0;
          xf = xfend;
          wf = wfend;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
        }
        if(yf-yfo>1) {
          if(yfend<rFont->ht && yfend>yfo) {
            (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yfend-yfo-1)*sy, bg);
            (*fillRectFun)(x, y+(yfend+1)*sy, chWd*sx, (yf-yfend-1)*sy, bg);
          } else
            (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yf-yfo-1)*sy, bg);
        }
        (*fillRectFun)(x+(xf+wf)*sx, y+yfo*sy, (chWd-xf-wf)*sx, 1*sy, bg);
        */
        // simple but slower
        for(int yy=0; yy<rFont->ht; yy++) {
          int xb,wb;
          xf=wf=0;
          uint16_t v,*rects = (uint16_t*)rFont->rects+recIdx;
          for(int i=0; i<recNum; i++) {
            v = pgm_read_word(rects+i);
            yf = (v>>5) & 0x3f;
            if(yf==yy) {
              xb = xf+wf;
              xf = v & 0x1f;
              wf = ((v>>11) & 0x1f)+1;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yf*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, 1*sy, fg);
            }
          }
          (*fillRectFun)(x+(xf+wf)*sx, y+yy*sy, (chWd-xf-wf)*sx, 1*sy, bg);
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
        /*
        // old version
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
          if(ybg<rFont->ht) (*fillRectFun)(x+xfCur*sx, y+ybg*sy, bold+1*sx, (rFont->ht-ybg)*sy, bg);
        }
        */
        // assumed that last rect used for width calculation will be always at the bottom (last y)
        uint8_t *rects = (uint8_t*)rFont->rects + recIdx*3;
        yf=hf=0;
        uint16_t v;
        int yb,hb,xfo=0;
        for(int i=0; i<recNum*3; i+=3) {
          xf = pgm_read_byte(rects+i+0);
          if(xf!=xfo) {
            if(xf-xfo>1) (*fillRectFun)(x+(xfo+1)*sx, y, (xf-xfo-1)*sx, rFont->ht*sy, bg);
            (*fillRectFun)(x+xfo*sx, y+(yf+hf)*sy, 1*sx, (rFont->ht-yf-hf)*sy, bg);
            yf=hf=0;
          }
          yb = yf+hf;
          yf = pgm_read_byte(rects+i+1);
          hf = pgm_read_byte(rects+i+2)+1;
          hb = yf-yb;
          (*fillRectFun)(x+xf*sx, y+yb*sy,      1*sx, hb*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yf*sy, bold+1*sx, hf*sy, fg);
          xfo=xf;
        }
        (*fillRectFun)(x+xfo*sx, y+(yf+hf)*sy, 1*sx, (rFont->ht-yf-hf)*sy, bg);
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
#if ENABLE_RRE_H24B_BG==1
      if(bg!=fg) {
        if(wdL>0) (*fillRectFun)(x-wdL*sx, y, wdL*sx, rFont->ht*sy, bg);
        /*
        // fast but very complex, to solve issue with ':' and similar
        uint8_t *rects = (uint8_t*)rFont->rects+(recIdx+recNum-1)*3;
        xf=wf=0;
        int xfend,yfend,wfend;
        yfend = pgm_read_byte(&rects[1]);
        xfend = pgm_read_byte(&rects[0]);
        wfend = pgm_read_byte(&rects[2])+1;
        int xb,wb,yfo=0;
        for(int i=0; i<recNum-1; i++) {
          rects = (uint8_t*)rFont->rects+(recIdx+i)*3;
          yf = pgm_read_byte(&rects[1]);
          if(yf!=yfo) { // yf = next column
            if(yfend==yfo) { // if current column = yfend rect finish with yfrect
              xb = xf+wf;
              xf = xfend;
              wf = wfend;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
            }
            if(yfend>yfo && yfend<yf) { // if empty columns check yfend rect
              xb = 0;
              xf = xfend;
              wf = wfend;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
            }
            if(yf-yfo>1) { // if empty columns - fill with bg color
              if(yfend<yf && yfend>yfo) {
                (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yfend-yfo-1)*sy, bg);
                (*fillRectFun)(x, y+(yfend+1)*sy, chWd*sx, (yf-yfend-1)*sy, bg);
              } else
                (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yf-yfo-1)*sy, bg);
            }
            (*fillRectFun)(x+(xf+wf)*sx, y+yfo*sy, (chWd-xf-wf)*sx, 1*sy, bg);
            xf=wf=0;
          }
          xb = xf+wf;
          xf = pgm_read_byte(&rects[0]);
          wf = pgm_read_byte(&rects[2])+1;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yf*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, 1*sy, fg);
          yfo=yf;
        }

        yf = rFont->ht;
        if(yfend==yfo) {
          xb = xf+wf;
          xf = xfend;
          wf = wfend;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
        }
        if(yfend>yfo) {
          xb = 0;
          xf = xfend;
          wf = wfend;
          wb = xf-xb;
          (*fillRectFun)(x+xb*sx, y+yfend*sy,      wb*sx, 1*sy, bg);
          (*fillRectFun)(x+xf*sx, y+yfend*sy, bold+wf*sx, 1*sy, fg);
        }
        if(yf-yfo>1) {
          if(yfend<rFont->ht && yfend>yfo) {
            (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yfend-yfo-1)*sy, bg);
            (*fillRectFun)(x, y+(yfend+1)*sy, chWd*sx, (yf-yfend-1)*sy, bg);
          } else
            (*fillRectFun)(x, y+(yfo+1)*sy, chWd*sx, (yf-yfo-1)*sy, bg);
        }
        (*fillRectFun)(x+(xf+wf)*sx, y+yfo*sy, (chWd-xf-wf)*sx, 1*sy, bg);
        */
        
        // simple but slower
        for(int yf=0; yf<rFont->ht; yf++) {
          int xb,wb;
          xf=wf=0;
          uint8_t *rects = (uint8_t*)rFont->rects+recIdx*3;
          for(int i=0; i<recNum; i++) {
            if(pgm_read_byte(rects+1)==yf) {
              xb = xf+wf;
              xf = pgm_read_byte(rects);
              wf = pgm_read_byte(rects+2)+1;
              wb = xf-xb;
              (*fillRectFun)(x+xb*sx, y+yf*sy,      wb*sx, 1*sy, bg);
              (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, 1*sy, fg);
            }
            rects+=3;
          }
          (*fillRectFun)(x+(xf+wf)*sx, y+yf*sy, (chWd-xf-wf)*sx, 1*sy, bg);
        }
        
        if(wdR>0) (*fillRectFun)(x+chWd*sx, y, wdR*sx, rFont->ht*sy, bg);
      } else
#endif // ENABLE_RRE_H24B_BG
      for(int i=0; i<recNum; i++) {
        uint8_t *rects = (uint8_t*)rFont->rects + (i+recIdx)*3;
        xf = pgm_read_byte(&rects[0])-xmin;
        yf = pgm_read_byte(&rects[1]);
        wf = pgm_read_byte(&rects[2])+1;
        hf = 1;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, fg);
      }
      break;
#endif // ENABLE_RRE_H24B

#if ENABLE_RRE_P8B==1
    case RRE_P8B: {
      uint8_t *rects = (uint8_t*)rFont->rects + recIdx;
      for(int i=0; i<recNum; i++) {
        yf = pgm_read_byte(&rects[0]);
        xf = yf&0xf;
        yf = yf>>4;
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+sx, sy, fg);
        rects++;
      }
      }
      break;
#endif // ENABLE_RRE_P8B

#if ENABLE_RRE_P16B==1
    case RRE_P16B: {
      uint8_t *rects = (uint8_t*)rFont->rects + recIdx*2;
      for(int i=0; i<recNum; i++) {
        xf = pgm_read_byte(&rects[0]);
        yf = pgm_read_byte(&rects[1]);
        (*fillRectFun)(x+xf*sx, y+yf*sy, bold+sx, sy, fg);
        rects+=2;
      }
      } 
      break;
#endif // ENABLE_RRE_P16B

  }
  return (wd+bold)*sx;
}

// ----------------------------------------------------------------
int RREFont::strWidth(char *str)
{
  int wd = 0;
  while (*str) wd += charWidthFull(*str++);
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
    int wd = charWidthFull(ch);
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
      case 133: pl = 1+9; break; // '�'
      case 135: pl = 2+9; break; // '�'
      case 153: pl = 3+9; break; // '�'
      case 130: pl = 4+9; break; // '�'
      case 132: pl = dualChar==197 ? 5+9 : 1; break; // '�' and '�'
      case 179: pl = 6+9; break; // '�'
      case 155: pl = 7+9; break; // '�'
      case 186: pl = 8+9; break; // '�'
      case 188: pl = 9+9; break; // '�'
      //case 132: pl = 1; break; // '�'
      case 134: pl = 2; break; // '�'
      case 152: pl = 3; break; // '�'
      case 129: pl = 4; break; // '�'
      case 131: pl = 5; break; // '�'
      case 147: pl = 6; break; // '�'
      case 154: pl = 7; break; // '�'
      case 185: pl = 8; break; // '�'
      case 187: pl = 9; break; // '�'
      default:  return c; break;
    }
    dualChar = 0;
  } else   
  switch(_c) {  // Windows coding
    case 165: pl = 1; break; // �
    case 198: pl = 2; break; // �
    case 202: pl = 3; break; // �
    case 163: pl = 4; break; // �
    case 209: pl = 5; break; // �
    case 211: pl = 6; break; // �
    case 140: pl = 7; break; // �
    case 143: pl = 8; break; // �
    case 175: pl = 9; break; // �
    case 185: pl = 10; break; // �
    case 230: pl = 11; break; // �
    case 234: pl = 12; break; // �
    case 179: pl = 13; break; // �
    case 241: pl = 14; break; // �
    case 243: pl = 15; break; // �
    case 156: pl = 16; break; // �
    case 159: pl = 17; break; // �
    case 191: pl = 18; break; // �
    default:  return c; break;
  }
  return pl+'~'+1;
}
// ---------------------------------
