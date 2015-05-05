#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

 #include "Arduino.h"
 #include "Print.h"
 #include "fonts.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1


class Adafruit_GFX : public Print {

 public:

  Adafruit_GFX(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  virtual void drawPixel(int16_t x, int16_t y) = 0;

  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1),
    drawFastVLine(int16_t x, int16_t y, int16_t h),
    drawFastHLine(int16_t x, int16_t y, int16_t w),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h),
    fillScreen(uint16_t color),
    invertDisplay(boolean i),
    drawFastChar(int16_t x, int16_t y, unsigned char c);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void
    drawCircle(int16_t x0, int16_t y0, int16_t r),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername),
    fillCircle(int16_t x0, int16_t y0, int16_t r),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta),
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius),
    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, 
      int16_t w, int16_t h, uint16_t color),
    drawChar(int16_t x, int16_t y, unsigned char c),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c),
    setColor(uint16_t c),
 	setColor(uint8_t r, uint8_t g, uint8_t b),
    setTextSize(uint8_t s),
    setTextWrap(boolean w),
    setRotation(uint8_t r),
	setFont(uint8_t f);
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

    
	uint16_t stringWidth(char *str);
	void 	print(char *st, int x, int y);
	void	printNumI(long num, int x, int y, int length=0, char filler=' ');
	void	printNumF(double num, byte dec, int x, int y, char divider='.', int length=0, char filler=' ');
   
    

  virtual size_t write(uint8_t);
  virtual size_t writeString(uint8_t *);

  int16_t height(void) const;
  int16_t width(void) const;

  uint8_t getRotation(void) const;

 protected:
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y;
  uint16_t
    textcolor, drawcolor;
  uint8_t
    textSize,
    rotation,
    font,
    fontStart,
    fontEnd;
  int8_t
    fontKern;
  const uint8_t* fontData;
  const FontDescriptor* fontDesc;
  void getFontDescriptor(FontDescriptor*, unsigned char c);
  boolean
    wrap; // If set, 'wrap' text at right edge of display
};

#endif // _ADAFRUIT_GFX_H
