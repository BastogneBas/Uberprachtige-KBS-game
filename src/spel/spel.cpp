//#include <Arduino.h>
#include <Adafruit_GFX.h>		// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library
#include <SPI.h>
#include <SD.h>
//#include "../../bitmap.cpp"
#include "../../staticDefinitions.cpp"
#include "spel.h"
// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.

//#define TFT_DC 9
//#define TFT_CS 10
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//#define SD_CS 4

//#define DEBUG

//void bmpDraw(char *filename, int16_t x, int16_t y);
//uint16_t read16(File & f);
//uint32_t read32(File & f);
void printDirectory(File dir, int numTabs);
Adafruit_ILI9341 *Definitions::tft;

spelScherm::spelScherm()
{

	init();
#ifdef DEBUG
	Serial.begin(9600);
#endif
	Definitions::tft->begin();

	yield();

	if (!SD.begin(Definitions::SD_CS))
	{
	}

	//File root = SD.open("/");
	//printDirectory(root, 0);

//if(0){
	Definitions::tft->setRotation(1);
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
//}
	while (1) ;
}

void spelScherm::begin(){}
void spelScherm::end(){}
void spelScherm::refresh(){}

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
