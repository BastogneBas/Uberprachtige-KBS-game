// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "homeScreen.h"
#include "src/lvlSelectScreen/lvlSelectScreen.h"
#include "../../screen.h"

homeScreen::homeScreen()
{
}

// Void that fills the screen
void homeScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_DARKGREY);		// Filling the screen with darkGrey

	// Defining variables to fill the screen
	uint8_t yRect = 79, yFill = 80, cursor = 90;

	for (int i = 1; i <= 3; i++)
	{
		Definitions::tft->drawRect(39, yRect, 242, 42, ILI9341_BLACK);
		Definitions::tft->fillRect(40, yFill, 240, 40, ILI9341_YELLOW);

		Definitions::tft->setCursor(55, cursor);
		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setTextSize(3);
		if (i == 1) {
			Definitions::tft->print("Levels");
		}
		else if (i == 2)
		{
			Definitions::tft->print("High-scores");
		} else {
			Definitions::tft->print("Instructions");
		}
		yRect += 50;
		yFill += 50;
		cursor += 50;
	}
}

uint8_t buttonSelect = 0;

void homeScreen::refresh() {
	Definitions::nunchuk->update();

	uint8_t nunY = Definitions::nunchuk->analogY;
	bool zButton = Definitions::nunchuk->zButton;

	if (nunY < 50) {
		if (!(buttonSelect == 3)) {
			buttonSelect++;
			homeScreen::repaint(buttonSelect, zButton);
		}
	}

	else if (nunY > 200)
	{
			if ((!buttonSelect == 0) || (!buttonSelect == 1))
			{
				buttonSelect--;
				homeScreen::repaint(buttonSelect);
			}
	}
}

void homeScreen::repaint(uint8_t buttonSelect)
{
	if (buttonSelect == 1)
	{
		Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(39, 79, 242, 42, ILI9341_WHITE);

		homeScreen::newScreen(zButton, buttonSelect);
	}
	else if (buttonSelect == 2)
	{
		Definitions::tft->drawRect(39, 79, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_BLACK);

		homeScreen::newScreen(zButton, buttonSelect);
	}
	else if (buttonSelect == 3)
	{
		Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_WHITE);
	}
}

void homeScreen::newScreen(bool zButton, uint8_t buttonSelect)
{
	if (zButton)
	{
		if (buttonSelect == 1)
		{
			Definitions::tft->drawRect(39, 79, 242, 42, ILI9341_RED);
			Definitions::currentScreen = new lvlSelectScreen();
			Definitions::currentScreen->begin();
		}
		else if (buttonSelect == 2)
		{
			Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_RED);
			Definitions::currentScreen = new highScoreScreen();
			Definitions::currentScreen->begin();
		}
		else if (buttonSelect == 3)
		{
			Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_RED);
			Definitions::currentScreen = new highScoreScreen();
			Definitions::currentScreen->begin();
		}
	}

}


void homeScreen::end() {

}

void homeScreen::checkNunchuck() {

}

