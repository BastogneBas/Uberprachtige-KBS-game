#include <Adafruit_GFX.h>		// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "../../staticDefinitions.cpp"
#include "spel.h"

void printDirectory(File dir, int numTabs);
//Adafruit_ILI9341 *Definitions::tft;

spelScherm::spelScherm()
{
	if (!SD.begin(Definitions::SD_CS))
	{
	}

	//File root = SD.open("/");
	//printDirectory(root, 0);
}

void spelScherm::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);
	uint8_t width = 14, height = 16;
	for (int x = 0; x <= height; x++)
		drawBlokje(x, 0);
	for (int y = 0; y <= width; y++)
		drawBlokje(0, y);
	for (int x = 0; x <= height; x++)
		drawBlokje(x, width);
	for (int y = 0; y <= width; y++)
		drawBlokje(height, y);
	for (int y = 2; y < width; y += 2)
		for (int x = 2; x < height; x += 2)
			drawBlokje(x, y);
	drawPoppetje1(1, 1);
	drawTon(1, 2);
	drawTon(2, 1);
}

void spelScherm::end()
{
}

void spelScherm::refresh()
{
}

void spelScherm::drawPoppetje1(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("sp1.bmp", x * 16, y * 16);
}

void spelScherm::drawTon(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("Ton.bmp", x * 16, y * 16);
}

void spelScherm::drawBlokje(uint8_t x, uint8_t y)
{
	Definitions::bmpDraw("blokje.bmp", x * 16, y * 16);
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
