// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "endScreen.h"
#include "../homeScreen/homeScreen.h"
#include "../../screen.h"

endScreen::endScreen() {

}

endScreen::endScreen(uint16_t scoreP1, uint16_t scoreP2)
{
	this->scoreP1 = scoreP1;
	this->scoreP2 = scoreP2;
}

void endScreen::begin()
{
	// Setting title
	Definitions::tft->setCursor(10, 10);
	Definitions::tft->setTextSize(3);
	Definitions::tft->setTextColor(ILI9341_BLACK);

	/*
	 * If statements for setting the title and some text
	 * The winner gets congratulated
	 */

	// First we check if the currentPlayer did win the game
	if ((endScreen::scoreP1 > endScreen::scoreP2 && Definitions::currentPlayer == 1) ||
			(endScreen::scoreP2 > endScreen::scoreP1 && Definitions::currentPlayer == 2))
	{
		// Making the background green
		Definitions::tft->fillScreen(ILI9341_GREEN);

		// If so, it will be shown on the screen
		Definitions::tft->println("Gewonnen :)");
	}
	// Else, it means that the player did lose the game
	else
	{
		Definitions::tft->fillScreen(ILI9341_RED);
		Definitions::tft->println("Verloren :(");
	}

	// Setting title for when the scores are equal
	if (endScreen::scoreP1 == endScreen::scoreP2)
	{
		// Making the background orange
		Definitions::tft->setCursor(10, 10);
		Definitions::tft->fillScreen(ILI9341_ORANGE);

		Definitions::tft->println("Gelijkspel");
	}

	// Showing the scores
	Definitions::tft->setCursor(10, 50);
	Definitions::tft->setTextSize(2);
	Definitions::tft->println("Uw score:");

	Definitions::tft->setCursor(10, 70);
	Definitions::tft->println("Score p2:");

	if (Definitions::currentPlayer == 1)
	{
		Definitions::tft->setCursor(150, 50);
		Definitions::tft->setTextSize(2);
		Definitions::tft->print(endScreen::scoreP1);

		Definitions::tft->setCursor(150, 70);
		Definitions::tft->print(endScreen::scoreP2);
	}
	else
	{
		Definitions::tft->setCursor(150, 50);
		Definitions::tft->setTextSize(2);
		Definitions::tft->print(endScreen::scoreP2);

		Definitions::tft->setCursor(150, 70);
		Definitions::tft->print(endScreen::scoreP1);
	}

}

void endScreen::refresh()
{
	Definitions::nunchuk->update();

	// Checking if the cButton is being pushed
	if (Definitions::nunchuk->cButton)
	{
		// Opening the homeScreen
		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
	}
}

void endScreen::end()
{
	// Deleting current screen
	delete Definitions::currentScreen;
	asm volatile ("  jmp 0");
}

void endScreen::checkNunchuck ()
{

}
