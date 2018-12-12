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
	// Making the background black
	Definitions::tft->fillScreen(ILI9341_BLACK);
	Definitions::tft->setTextColor(ILI9341_WHITE);

	// Writing title
	Definitions::tft->setCursor(20, 20);
	Definitions::tft->setTextSize(3);
	Definitions::tft->println("Instructiescherm");

	// Z-button explanation
	Definitions::tft->setCursor(20, 60);
	Definitions::tft->setTextSize(1);
	Definitions::tft->println("Z-knop     Bom neerleggen / knop aandrukken");

	// c-button explanation
	Definitions::tft->setCursor(20, 80);
	Definitions::tft->println("C-knop     Terug naar het vorige scherm");

	// joystick explanation
	Definitions::tft->setCursor(20, 100);
	Definitions::tft->println("Joystick   Bewegen door het spel/ de menu's");
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

void instructionScreen::end()
{

}

void instructionScreen::checkNunchuck()
{

}