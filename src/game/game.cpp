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
	uint8_t width = 14, height = 12;
	for (int x = 0; x <= height; x++)
		drawBlock(x, 0);
	for (int y = 0; y <= width; y++)
		drawBlock(0, y);
	for (int x = 0; x <= height; x++)
		drawBlock(x, width);
	for (int y = 0; y <= width; y++)
		drawBlock(height, y);
	for (int y = 2; y < width; y += 2)
		for (int x = 2; x < height; x += 2)
			drawBlock(x, y);
	drawPeep1(1, 1);

	Level level = new Level(level.getBarrels(), "Level1");
	for (int x = 1; i <= level; i++) {
        for (int y = 1; j < 16; j++) {

            if (level.getBarrels() & (1 << level.getBarrels[x])) {
                drawBarrel(x, y);
            }

        }
    }
	};




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

void gameScreen::drawPeep1(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("peep1.bmp", x * 16, y * 16);
}

void gameScreen::drawPeep2(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("peep2.bmp", x * 16, y * 16);
}

void gameScreen::drawBarrel(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("barrel.bmp", x * 16, y * 16);
}

void gameScreen::drawBlock(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("block.bmp", x * 16, y * 16);
}

void gameScreen::drawBomb(uint8_t x, uint8_t y)
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
