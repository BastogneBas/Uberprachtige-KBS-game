// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "lvlSelectScreen.h"
#include "../homeScreen/homeScreen.h"
#include "../../screen.h"
#include "../level/levelDefs.h"
#include "../game/game.h"


lvlSelectScreen::lvlSelectScreen()
{
	Serial.begin(9600);
}

// Void that fills the screen
void lvlSelectScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_DARKGREY);		// Filling the screen with darkGrey

	uint8_t yRect = 29, yFill = 30, cursor = 40;

	for (int i = 1; i <= 4; i++)
	{
		Definitions::tft->drawRect(19, yRect, 242, 42, ILI9341_BLACK);
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

uint8_t selectedButton = 0;

void lvlSelectScreen::refresh()
{
	Definitions::nunchuk->update();

	uint8_t nunY = Definitions::nunchuk->analogY;
	bool zButton = Definitions::nunchuk->zButton;
	bool cButton = Definitions::nunchuk->cButton;

	Serial.print("Waarde y: ");
	Serial.println(nunY);

	Serial.print("Waarde z: ");
	Serial.println(zButton);

	Serial.print("Waarde buttonselect: ");
	Serial.println(selectedButton);

	if (nunY < 50)
	{
		if (!(selectedButton >= 4))
		{
			selectedButton++;
			lvlSelectScreen::repaint(selectedButton);
		}
		else
		{
			lvlSelectScreen::repaint(selectedButton);
		}
	}
	else if (nunY > 200)
	{
		if ((!(selectedButton <= 0)) || (!(selectedButton == 1)))
		{
			selectedButton--;
			lvlSelectScreen::repaint(selectedButton);
		}
		else
		{
			lvlSelectScreen::repaint(selectedButton);
		}
	}

	if ((zButton) && (!selectedButton == 0))
	{
		lvlSelectScreen::startGame(selectedButton);
	}

	if (cButton)
	{
		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
		selectedButton = 0;
	}

}

void lvlSelectScreen::repaint(uint8_t selectedButton)
{
	if (selectedButton == 1)
	{
		Definitions::tft->drawRect(19, 29, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_BLACK);

	}
	else if (selectedButton == 2)
	{
		Definitions::tft->drawRect(19, 29, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_BLACK);

	}
	else if (selectedButton == 3)
	{
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(19, 179, 242, 42, ILI9341_BLACK);
	}
	else if (selectedButton == 4)
	{
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(19, 179, 242, 42, ILI9341_WHITE);
	}
}

void lvlSelectScreen::startGame(uint8_t selectedButton)
{

	if (selectedButton == 1)
	{
		gameScreen gamescreen = gameScreen(LevelDefs::getLevel(0));
		gamescreen.begin();

		Definitions::currentScreen =&gamescreen;
	}
	else if (selectedButton == 2)
	{
		gameScreen gamescreen = gameScreen(LevelDefs::getLevel(1));

		gamescreen.begin();

		Definitions::currentScreen =&gamescreen;
	}
	else if (selectedButton == 3)
	{
		Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_RED);
		gameScreen gamescreen = gameScreen(LevelDefs::getLevel(2));
		gamescreen.begin();

		Definitions::currentScreen =&gamescreen;

	}
	else if (selectedButton == 4)
	{
		gameScreen gamescreen = gameScreen(LevelDefs::getLevel("Random"));
		gamescreen.begin();

		Definitions::currentScreen =&gamescreen;
	}

}


void lvlSelectScreen::end()
{

}

void lvlSelectScreen::checkNunchuck()
{

}

