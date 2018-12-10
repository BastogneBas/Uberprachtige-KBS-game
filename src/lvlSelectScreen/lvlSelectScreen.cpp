//
// Created by alber on 10-12-2018.
//



// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "lvlSelectScreen.h"
#include "../../screen.h"

lvlSelectScreen::lvlSelectScreen()
{

}

// Void that fills the screen
void lvlSelectScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_DARKGREY);		// Filling the screen with darkGrey

	uint8_t yRect = 29, yFill = 30, cursor = 40;

	for (int i = 1; i <= 4; i++)
	{
		Definitions::tft->drawRect(1, yRect, 242, 42, ILI9341_BLACK);
		Definitions::tft->fillRect(20, yFill, 240, 40, ILI9341_YELLOW);

		Definitions::tft->setCursor(35, cursor);
		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setTextSize(3);
		if (i == 4) {
			Definitions::tft->print("Random level");
		}
		else
		{
			Definitions::tft->print("Level ");
			Definitions::tft->println(i);
		}
		yRect += 50;
		yFill += 50;
		cursor += 50;
	}
}


void lvlSelectScreen::refresh()
{
}

void lvlSelectScreen::end()
{

}

void lvlSelectScreen::checkNunchuck()
{

}

