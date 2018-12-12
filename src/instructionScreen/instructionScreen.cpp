



// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "../homeScreen/homeScreen.h"
#include "instructionScreen.h"
#include "../../screen.h"

instructionScreen::instructionScreen()
{

}

void instructionScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_DARKGREY);		// Filling the screen with darkGrey
	Definitions::tft->setCursor(55, 120);
	Definitions::tft->setTextColor(ILI9341_BLACK);
	Definitions::tft->setTextSize(4);
	Definitions::tft->println("Instructie pagina");
}

void instructionScreen::refresh()
{
	Definitions::nunchuk->update();

	uint8_t nunY = Definitions::nunchuk->analogY;
	bool zButton = Definitions::nunchuk->zButton;
	bool cButton = Definitions::nunchuk->cButton;

	if (cButton)
	{
		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
	}
}

void instructionScreen::end() {

}

void instructionScreen::checkNunchuck() {

}

