



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

// Defining constructor
instructionScreen::instructionScreen()
{

}

// Void that gets called once when the class is being called.
void instructionScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);		// Filling the screen with darkGrey
	Definitions::tft->setCursor(55, 120);
	Definitions::tft->setTextColor(ILI9341_GREENYELLOW);
	Definitions::tft->setTextSize(4);
	Definitions::tft->println("iNsTrUcTiE pAgInA");
}

void instructionScreen::refresh()
{
	Definitions::nunchuk->update();


	// If statement that will check if the cButton is being pushed
	if (Definitions::nunchuk->cButton)
	{
		// Deleting current screen
		delete Definitions::currentScreen;

		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
	}
}

void instructionScreen::end() {

}

void instructionScreen::checkNunchuck() {

}

