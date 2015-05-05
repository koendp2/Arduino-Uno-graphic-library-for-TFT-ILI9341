/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "Adafruit_GFX.h"
#include "glcdfont.c"
#include "fonts.h"

 #include <avr/pgmspace.h>

Adafruit_GFX::Adafruit_GFX(int16_t w, int16_t h):
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textSize  = 1;
  textcolor = 0xFFFF;
  wrap      = true;
    // Default to GLCDFONT to be compatible with existing code
  setFont(GLCDFONT);		// May also be set to TIMESNR_8, CENTURY_8, COMICS_8 or TEST (for testing candidate fonts)
}

void Adafruit_GFX::setFont(uint8_t f) {
  font = f;
  switch(font) {
    case TIMESNR_8:
      fontData = timesNewRoman_8ptBitmaps;
	  fontDesc = timesNewRoman_8ptDescriptors;
      fontKern = 1;
      break;
    case CENTURY_8:
      fontData = centuryGothic_8ptBitmaps;
	  fontDesc = centuryGothic_8ptDescriptors;
      fontKern = 1;
      break;
    case ARIAL_8:
      fontData = arial_8ptBitmaps;
	  fontDesc = arial_8ptDescriptors;
      fontKern = 1;
      break;
    case COMICS_8:
      fontData = comicSansMS_8ptBitmaps;
	  fontDesc = comicSansMS_8ptDescriptors;
      fontKern = 1;
      break;
    case GLCDFONT:
      fontData = glcdfontBitmaps;
	  fontDesc = glcdfontDescriptors;
      fontKern = 1;
      break;
    case TEST:
      fontData = testBitmaps;
	  fontDesc = testDescriptors;
      fontKern = 1;
      break;
	default:
      font = ARIAL_8;
      fontData = arial_8ptBitmaps;
	  fontDesc = arial_8ptDescriptors;
      fontKern = 1;
      break;
  }

  fontStart = pgm_read_byte(fontData+FONT_START);
  fontEnd = pgm_read_byte(fontData+FONT_END);
}

// Draw a circle outline
void Adafruit_GFX::drawCircle(int16_t x0, int16_t y0, int16_t r) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r);
  drawPixel(x0  , y0-r);
  drawPixel(x0+r, y0 );
  drawPixel(x0-r, y0 );

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y);
    drawPixel(x0 - x, y0 + y);
    drawPixel(x0 + x, y0 - y);
    drawPixel(x0 - x, y0 - y);
    drawPixel(x0 + y, y0 + x);
    drawPixel(x0 - y, y0 + x);
    drawPixel(x0 + y, y0 - x);
    drawPixel(x0 - y, y0 - x);
  }
}

void Adafruit_GFX::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y);
      drawPixel(x0 + y, y0 + x);
    } 
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y);
      drawPixel(x0 + y, y0 - x);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x);
      drawPixel(x0 - x, y0 + y);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x);
      drawPixel(x0 - x, y0 - y);
    }
  }
}

void Adafruit_GFX::fillCircle(int16_t x0, int16_t y0, int16_t r) {
  drawFastVLine(x0, y0-r, 2*r+1);
  fillCircleHelper(x0, y0, r, 3, 0);
}

// Used to do circles and roundrects
void Adafruit_GFX::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void Adafruit_GFX::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0);
    } else {
      drawPixel(x0, y0);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void Adafruit_GFX::drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h) {
  drawFastHLine(x, y, w);
  drawFastHLine(x, y+h-1, w);
  drawFastVLine(x, y, h);
  drawFastVLine(x+w-1, y, h);
}

void Adafruit_GFX::drawFastVLine(int16_t x, int16_t y,
				 int16_t h) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1);
}

void Adafruit_GFX::drawFastHLine(int16_t x, int16_t y,
				 int16_t w) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y);
}

void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h);
  }
}

void Adafruit_GFX::fillScreen(uint16_t color) {
  drawcolor = color;
  fillRect(0, 0, _width, _height);
}

// Draw a rounded rectangle
void Adafruit_GFX::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r); // Bottom
  drawFastVLine(x    , y+r  , h-2*r); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1);
  drawCircleHelper(x+w-r-1, y+r    , r, 2);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4);
  drawCircleHelper(x+r    , y+h-r-1, r, 8);
}

// Fill a rounded rectangle
void Adafruit_GFX::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r) {
  // smarter version
  fillRect(x+r, y, w-2*r, h);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1);
}

// Draw a triangle
void Adafruit_GFX::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2) {
  drawLine(x0, y0, x1, y1);
  drawLine(x1, y1, x2, y2);
  drawLine(x2, y2, x0, y0);
}

// Fill a triangle
void Adafruit_GFX::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1);
  }
}

void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
			      const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color) {
drawcolor = color;
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        drawPixel(x+i, y+j);
      }
    }
  }
}

// Draw a 1-bit color bitmap at the specified x, y position from the
// provided bitmap buffer (must be PROGMEM memory) using color as the
// foreground color and bg as the background color.
void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            uint16_t color, uint16_t bg) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
		drawcolor = color;
        drawPixel(x+i, y+j);
      }
      else {
		drawcolor = bg;
      	drawPixel(x+i, y+j);
      }
    }
  }
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void Adafruit_GFX::drawXBitmap(int16_t x, int16_t y,
                              const uint8_t *bitmap, int16_t w, int16_t h,
                              uint16_t color) {
  
  int16_t i, j, byteWidth = (w + 7) / 8;
  drawcolor = color;
  
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8))) {
        drawPixel(x+i, y+j);
      }
    }
  }
}

size_t Adafruit_GFX::writeString(uint8_t *str) {

	while(*str)
	{
		write(*str++);
	}
	return 1;
}

size_t Adafruit_GFX::write(uint8_t c) {

FontDescriptor desc;

  if (c == '\n') {
   	memcpy_P(&desc, (const void *)(fontDesc), 4);
    cursor_y += textSize*desc.height;	//all chars are same height so use height of space char
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawFastChar(cursor_x, cursor_y, c);
    getFontDescriptor(&desc, c);
 	uint16_t w = desc.width;
	uint16_t h = desc.height;
    /*if (fontKern > 0 && textcolor != textbgcolor) {
      drawcolor = textbgcolor;
      fillRect(cursor_x+w*textSize,cursor_y,fontKern*textSize,h*textSize);
    }*/
    cursor_x += textSize*(w+fontKern);
    if (wrap && (cursor_x > (_width - textSize*w))) {
      cursor_y += textSize*h;
      cursor_x = 0;
    }
  }
  return 1;
}

void Adafruit_GFX::drawFastChar(int16_t x, int16_t y, unsigned char c) {
  // Update in subclasses if desired!
  drawChar(x,y,c);
}

void Adafruit_GFX::getFontDescriptor(FontDescriptor* desc, unsigned char c)
{
  if (c < fontStart || c > fontEnd) {
    c = 0;
  }
  else {
    c -= fontStart;
  }

   memcpy_P(desc, (const void *)(fontDesc + c), 4);
}

// Draw a character
void Adafruit_GFX::drawChar(int16_t x, int16_t y, unsigned char c) {

	FontDescriptor desc;
	getFontDescriptor(&desc, c);
   	//Serial.print((char)c); Serial.print("\n");

   	//Serial.print(desc.width); Serial.print("\n");
   	//Serial.print(desc.height); Serial.print("\n");
   	//Serial.print(desc.offset); Serial.print("\n"); Serial.print("\n");
   	

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + (desc.width * textSize) - 1) < 0) || // Clip left
     ((y + (desc.height * textSize) - 1) < 0))   // Clip top
    return;

	uint8_t bitCount=0;
  	uint16_t fontIndex = desc.offset + 2; 
  
  for (int8_t i=0; i<desc.height; i++ ) {	
    uint8_t line;
    for (int8_t j = 0; j<desc.width; j++) {	
      if (bitCount++%8 == 0) {
        line = pgm_read_byte(fontData+fontIndex++);
      }
      if (line & 0x80) {
        drawcolor = textcolor;
        if (textSize == 1) {// default sizeFast
          drawPixel(x+j, y+i);
          }
        else {  // big size
          fillRect(x+(j*textSize), y+(i*textSize), textSize, textSize);
        } 
      } /*else if (drawcolor != textbgcolor) {
      	drawcolor = textbgcolor;
        if (textSize == 1) // default size
          drawPixel(x+j, y+i);
        else {  // big size
          fillRect(x+j*textSize, y+i*textSize, textSize, textSize);
        }
      }*/
      line <<= 1;
    }
    bitCount = 0;
  }
}

void Adafruit_GFX::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void Adafruit_GFX::setTextSize(uint8_t s) {
  textSize = (s > 0) ? s : 1;
}

void Adafruit_GFX::setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = c;
}

void Adafruit_GFX::setColor(uint16_t c) {
  drawcolor = c;
}


void Adafruit_GFX::setTextWrap(boolean w) {
  wrap = w;
}

uint8_t Adafruit_GFX::getRotation(void) const {
  return rotation;
}

void Adafruit_GFX::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}



void Adafruit_GFX::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void Adafruit_GFX::print(char *st, int x, int y)
{
  
	if (x==RIGHT)
		{		
		x=(_width+1)-(stringWidth(st));
		}
	if (x==CENTER){
		x=((_width+1)-(stringWidth(st)))/2;
		}
		
	setCursor(x,y);
	writeString((unsigned char *)st);
}


uint16_t Adafruit_GFX::stringWidth(char *str)
{
	Serial.println(str);
	FontDescriptor desc;
	int width = 0;
	while(*str)
	{
		getFontDescriptor(&desc, *str++);
		width += desc.width*textSize;	
	}
	Serial.println(width);
	return width;
}

void Adafruit_GFX::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	dtostrf(num,10,10,st);
	//_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st,x,y);
}


// Return the size of the display (per current rotation)
int16_t Adafruit_GFX::width(void) const {
  return _width;
}
 
int16_t Adafruit_GFX::height(void) const {
  return _height;
}

void Adafruit_GFX::invertDisplay(boolean i) {
  // Do nothing, must be subclassed if supported
}

void Adafruit_GFX::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	drawcolor = color565(r,g,b);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_GFX::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}



