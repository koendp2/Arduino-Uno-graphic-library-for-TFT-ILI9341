// UTFT_ViewFont 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the included fonts.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library


// Declare which fonts we will be using

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup()
{
   tft.reset();
  
  tft.begin(0x9341);
}

void loop()
{
  tft.fillScreen(tft.color565(0, 0, 0));
  tft.setTextColor(tft.color565(0, 255, 0));

  tft.setFont(TIMESNR_8);
  tft.print(" !\"#$%&'()*+,-./", CENTER, 0);
  tft.print("0123456789:;<=>?", CENTER, 16);
  tft.print("@ABCDEFGHIJKLMNO", CENTER, 32);
  tft.print("PQRSTUVWXYZ[\\]^_", CENTER, 48);
  tft.print("`abcdefghijklmno", CENTER, 64);
  tft.print("pqrstuvwxyz{|}~ ", CENTER, 80);

  tft.setFont(CENTURY_8);
  tft.print(" !\"#$%&'()*+,-./0123456789:;<=>?", CENTER, 120);
  tft.print("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", CENTER, 132);
  tft.print("`abcdefghijklmnopqrstuvwxyz{|}~ ", CENTER, 144);

  tft.setFont(ARIAL_8);
  tft.print("0123456789", CENTER, 190);

  while(1) {};
}

