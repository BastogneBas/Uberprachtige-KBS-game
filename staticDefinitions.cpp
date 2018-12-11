#include "src/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include <stddef.h>
#include "Sprites.h"
#include "src/ArduinoNunchuk/ArduinoNunchuk.h"
#include "screen.h"

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//#define DEBUG

class Definitions
{
  public:
	static const int TFT_DC = 9;
	static const int TFT_CS = 10;
	static const int SD_CS = 4;

	static const uint8_t gameHeight = 11;
	static const uint8_t gameWidth = 13;
	static const uint8_t SpriteHeight = 16;
	static const uint8_t SpriteWidth = 16;

	static Adafruit_ILI9341 *tft;
	static ArduinoNunchuk *nunchuk;
	static screen *currentScreen;

	static void bmpDraw(char *filename, int16_t x, int16_t y)
	{
		if (filename[0] == 'B' && filename[4] == 'K')
		{
			/*for(int16_t j=0,bindex=0;j<h;j++,y++){
			   tft->startWrite();
			   for(int16_t i=0;i<w;i++,bindex+=3){
			   uint8_t r = pgm_read_byte(&blokje[bindex]);
			   uint8_t g = pgm_read_byte(&blokje[bindex+1]);
			   uint8_t b = pgm_read_byte(&blokje[bindex+2]);
			   tft->writePixel(x+i, y, tft->color565(r,g,b));
			   }
			   tft->endWrite();
			   } */
			tft->drawRGBBitmap(x, y, blokje, SpriteWidth, SpriteHeight);
		}
		if (filename[0] == 'B' && filename[5] == 'L')
		{
			//tft->fillRect(x, y, 15, 15, ILI9341_MAROON);
			tft->drawRGBBitmap(x, y, ton, SpriteWidth, SpriteHeight);

		}
		if (filename[0] == 'P' && filename[4] == '1')
		{
			//tft->fillRect(x, y, 15, 15, ILI9341_BLUE);
			tft->drawRGBBitmap(x, y, peep1, SpriteWidth, SpriteHeight);
		}
		if (filename[0] == 'P' && filename[4] == '2')
		{
			//tft->fillRect(x, y, 15, 15, ILI9341_RED);
			tft->drawRGBBitmap(x, y, peep2, SpriteWidth, SpriteHeight);
		}
	}
};

#endif
