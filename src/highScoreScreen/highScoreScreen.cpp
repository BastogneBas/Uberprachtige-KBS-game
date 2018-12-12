
// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "highScoreScreen.h"
#include "../homeScreen/homeScreen.h"
#include "../../screen.h"

highScoreScreen::highScoreScreen()
{

}

void highScoreScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);		// Filling the screen with darkGrey
	Definitions::tft->setCursor(55, 120);
	Definitions::tft->setTextColor(ILI9341_GREENYELLOW);
	Definitions::tft->setTextSize(4);
	Definitions::tft->println("Bonnie 100. Guus INFINITY");
}

void highScoreScreen::refresh()
{
	Definitions::nunchuk->update();

	if (Definitions::nunchuk->cButton)
	{
		// Deleting current screen
		delete Definitions::currentScreen;

		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
	}
}

void highScoreScreen::end() {

}

void highScoreScreen::checkNunchuck() {

}

