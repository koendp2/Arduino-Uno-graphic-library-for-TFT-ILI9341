#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library
#include "TouchScreen.h"


//I need to change the pins below for it to work. NOt sure why
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	WHITE   0xFFFF
#define	YELLOW    0xFFE0
#define	CYAN     0x07FF 
#define	GREEN   0xF81F
#define RED    0xF800
#define MAGENTA 0x07E0 
#define BLUE  0x001F 
#define BLACK   0x0000 


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
 tft.reset();
  
  tft.begin(0x9341);
  tft.setFont(TIMESNR_8);
  tft.setRotation(1);
  Serial.begin(9600);

}

void loop()
{
  int buf[318];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  tft.fillScreen(WHITE);
  tft.setColor(255,0,0);
  tft.fillRect(0, 0, 319, 13);
  tft.setColor(64, 64, 64);
  tft.fillRect(0, 226, 319, 14);
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.print("* Universal Color TFT Display Library *", CENTER, 1);
  tft.print("<http://www.RinkyDinkElectronics.com/>", CENTER, 226);

  tft.setColor(0, 0, 255);
  tft.drawRect(0, 14, 319, 211);

// Draw crosshairs
  tft.setColor(0, 0, 255);
  tft.drawLine(159, 15, 159, 224);
  tft.drawLine(1, 119, 318, 119);
  for (int i=9; i<310; i+=10)
    tft.drawLine(i, 117, i, 121);
  for (int i=19; i<220; i+=10)
    tft.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  tft.setTextColor(tft.color565(0,255,255));
  tft.print("Sin", 5, 15);
  for (int i=1; i<318; i++)
  {
    tft.drawPixel(i,(int)(119+(sin(((i*1.13)*3.14)/180)*95)));
  }
  
  tft.setTextColor(tft.color565(255,0,0));
  tft.print("Cos", 5, 27);
  for (int i=1; i<318; i++)
  {
    tft.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  tft.setTextColor(tft.color565(255,255,0));
  tft.print("Tan", 5, 39);
  for (int i=1; i<318; i++)
  {
    tft.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);

  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);
  tft.setColor(0, 0, 255);
  tft.drawLine(159, 15, 159, 224);
  tft.drawLine(1, 119, 318, 119);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(318*20); i++) 
  {
    x++;
    if (x==319)
      x=1;
    if (i>319)
    {
      if ((x==159)||(buf[x-1]==119))
        tft.setColor(0,0,255);
      else
        tft.setColor(0,0,0);
      tft.drawPixel(x,buf[x-1]);
    }
    tft.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    tft.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillRect(70+(i*20), 30+(i*20), 60, 60);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillRoundRect(190-(i*20), 30+(i*20), 60, 60, 3);
  }
  
  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        tft.setColor(255,0,255);
        break;
      case 2:
        tft.setColor(255,0,0);
        break;
      case 3:
        tft.setColor(0,255,0);
        break;
      case 4:
        tft.setColor(0,0,255);
        break;
      case 5:
        tft.setColor(255,255,0);
        break;
    }
    tft.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some lines in a pattern
   tft.setColor (255,0,0);
  for (int i=15; i<224; i+=5)
  {
    tft.drawLine(1, i, (i*1.44)-10, 224);
  }
   tft.setColor (255,0,0);
  for (int i=224; i>15; i-=5)
  {
    tft.drawLine(318, i, (i*1.44)-11, 15);
  }
  tft.setColor  (0,255,255);
  for (int i=224; i>15; i-=5)
  {
    tft.drawLine(1, i, 331-(i*1.44), 15);
  }
  tft.setColor (0,255,255);
  for (int i=15; i<224; i+=5)
  {
    tft.drawLine(318, i, 330-(i*1.44), 224);
  }
  
  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=32+random(256);
    y=45+random(146);
    r=random(30);
    tft.drawCircle(x, y, r);
  }

  delay(2000);
  
   tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316)-x;
    y2=16 +random(207)-y;
    tft.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
   tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
     tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    tft.drawRoundRect(x, y, x2-x, y2-x,2);
  }

  delay(2000);
  
   tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

  for (int i=0; i<100; i++)
  {
     tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(209);
    x2=2+random(316);
    y2=16+random(209);
    tft.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  tft.setColor(0,0,0);
  tft.fillRect(1,15,318,211);

  for (int i=0; i<10000; i++)
  {
     tft.setColor(random(255), random(255), random(255));
    tft.drawPixel(2+random(316), 16+random(209));
  }

  delay(2000);

  tft.fillScreen(tft.color565(0, 0, 255));
   tft.setColor(255, 0, 0);
  tft.fillRoundRect(80, 70, 160, 100 ,2);
  
   tft.setColor(255, 255, 255);
  tft.print("That's it!", CENTER, 93);
  tft.print("Restarting in a", CENTER, 119);
  tft.print("few seconds...", CENTER, 132);
  
   tft.setColor(0, 255, 0);
  tft.print("Runtime: (msecs)", CENTER, 210);
  tft.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}

