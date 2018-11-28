#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"	// Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "../../staticDefinitions.cpp"
#include "game.h"
#include "../level/level.h"
#include "../level/levelDefs.h"

void printDirectory(File dir, int numTabs);
//Adafruit_ILI9341 *Definitions::tft;

gameScreen::gameScreen()
{
	if (!SD.begin(Definitions::SD_CS))
	{
		Serial.println("SD not started!");
	}

	//File root = SD.open("/");
	//printDirectory(root, 0);
}

gameScreen::gameScreen(Level level)
 {
	this->level = level;
 }

void gameScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);
	uint8_t width = Definitions::gameWidth+1, height = Definitions::gameHeight+1;
	for (int x = 0; x <= width; x++)
		drawBlock(x, 0);
	for (int y = 0; y <= height; y++)
		drawBlock(0, y);
	for (int x = 0; x <= width; x++)
		drawBlock(x, height);
	for (int y = 0; y <= height; y++)
		drawBlock(width, y);
	for (int y = 2; y < height; y += 2)
		for (int x = 2; x < width; x += 2)
			drawBlock(x, y);
	drawPeep1(1, 1);

	//Level level = new Level(level.getBarrels(), "Level1");
	for (int x = 0; x < Definitions::gameHeight; x++) {
        for (uint16_t y = 0; y < Definitions::gameWidth; y++) {
			uint16_t mask = 1<<y;
            if (this->level.getBarrels()[x] & (mask)) {
                drawBarrel(y+1, x+1);
            }
        }
    }
	/*drawBarrel(1, 2);
	 *drawBarrel(2, 1);
	 */
}

void gameScreen::end()
{
}

void gameScreen::refresh()
{
}

void gameScreen::drawPeep1(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("PEEP1.BMP", x * 16, y * 16);
}

void gameScreen::drawPeep2(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("peep2.bmp", x * 16, y * 16);
}

void gameScreen::drawBarrel(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("BARREL.BMP", x * 16, y * 16);
}

void gameScreen::drawBlock(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("BLOCK.BMP", x * 16, y * 16);
}

void gameScreen::drawBomb(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("bomb.bmp", x * 16, y * 16);
}

void gameScreen::checkNunchuck()
{
}

void printDirectory(File dir, int numTabs)
{
	while (true)
	{

		File entry = dir.openNextFile();
		if (!entry)
		{
			// no more files
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++)
		{
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory())
		{
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
		}
		else
		{
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}
