// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

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
#define	GREEN   0x07E0 
#define RED    0xF800
#define MAGENTA  0xF81F
#define BLUE  0x001F 
#define BLACK   0x0000 


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int16_t counter = 0;
 unsigned char naam1 [] = "\n";
 unsigned char naam2 [] = "Myriam De Wulf\n";
 unsigned char naam3 [] = "Musti\n";


void setup(void) {
  Serial.begin(9600);
  Serial.println("Paint!");
  
  tft.reset();
  
  tft.begin(0x9341);

  tft.fillScreen(WHITE);

  tft.setRotation(1);
  for(int16_t x = 0; x < 255; x++)
  {
    //Serial.println(tft.color565(0, 0, x));
    //Serial.println(tft.color565(0, x, 0));
    Serial.println(tft.color565(x, 0, 0));

    tft.drawFastVLine(x,0,80, tft.color565(0, 0, x));
    tft.drawFastVLine(x,80,80, tft.color565(0, x, 0));
    tft.drawFastVLine(x,160,80, tft.color565(x, 0, 0));
    /*for (int16_t y = 0; y < 240; y++)
      tft.drawPixel(x,y, RED);  */
  }
  pinMode(13, OUTPUT);
}

void loop()
{

  
 // Serial.println(naam);
  if(counter == 0)
  {
  tft.setFont(CENTURY_8);
  //tft.setCursor(20,20 );
  for(int k=1; k < 3; k++)
   { 
  tft.setTextSize(k);
  tft.setTextColor(YELLOW);
  for(int i = 0; i < 5 ; i++)
  {
    tft.setFont(i);
    for(int j = 'a'; j <= 'z'; j++)
      tft.write(j);
     tft.write('\n');
  }
  }
  }
  counter = 1;
 
  
}
