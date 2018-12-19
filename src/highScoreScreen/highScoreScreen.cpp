// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "highScoreScreen.h"
//#include "../endScreen/endScreen.h"
#include "../homeScreen/homeScreen.h"
#include "../../screen.h"

highScoreScreen::highScoreScreen()
{

}

void highScoreScreen::begin()
{
	// Making the background black
	Definitions::tft->fillScreen(ILI9341_BLACK);
	Definitions::tft->setTextColor(ILI9341_WHITE);

	// Writing title
	Definitions::tft->setCursor(20, 10);
	Definitions::tft->setTextSize(3);
	Definitions::tft->println("Highscores");

	// Drawing lines
	Definitions::tft->drawLine(10, 40, 310, 40, ILI9341_WHITE);
	Definitions::tft->drawLine(160, 40, 160, 240, ILI9341_WHITE);
	Definitions::tft->drawLine(10, 65, 310, 65, ILI9341_WHITE);
	Definitions::tft->drawLine(40, 65, 40, 240, ILI9341_WHITE);
	Definitions::tft->drawLine(190, 65, 190, 240, ILI9341_WHITE);

	// Draw rectangle around the lines
	Definitions::tft->drawRect(10,0,300,240,ILI9341_WHITE);

	// Drawing players
	Definitions::tft->setCursor(20, 45);
	Definitions::tft->setTextSize(2);
	Definitions::tft->println("Player 1");

	Definitions::tft->setCursor(170, 45);
	Definitions::tft->println("Player 2");

	// For loop to print the numbers 1 - 5 and the highscore values for p1 and p2
	for (uint8_t i = 1; i <= 5; i++)
	{
		// Printing values 1 - 5 on the left side
		Definitions::tft->setCursor(20, highScoreScreen::cursorValue);
		Definitions::tft->print(i);

		// Printing values 1 - 5 on the right side
		Definitions::tft->setCursor(170, highScoreScreen::cursorValue);
		Definitions::tft->print(i);

		// TODO get values from the highScores class instead of highScoreScreen.h file
		// Sorting the arrays for players 1 and 2
		for (uint8_t a = 0; a <= 4; a++)
		{
			// Looping until all the values have been checked (j <= 4 - a)
			for (uint8_t j = 0; j <= 4 - a; j++)
			{
				// If statement that checks if the current index > the index +1
				if (highScoreScreen::highScoresP1[j] > highScoreScreen::highScoresP1[j + 1])
				{
					// If so, the variable score will be set to the index value.
					highScoreScreen::scorep1 = highScoreScreen::highScoresP1[j];

					// The index value will be set to index value + 1
					highScoreScreen::highScoresP1[j] = highScoreScreen::highScoresP1[j + 1];

					// And the index value + 1 will be set to the index value
					highScoreScreen::highScoresP1[j + 1] = highScoreScreen::scorep1;
				}

				// The same things happen with the highscores for p2
				if (highScoreScreen::highScoresP2[j] > highScoreScreen::highScoresP2[j + 1])
				{
					highScoreScreen::scorep2 = highScoreScreen::highScoresP2[j];
					highScoreScreen::highScoresP2[j] = highScoreScreen::highScoresP2[j + 1];
					highScoreScreen::highScoresP2[j + 1] = highScoreScreen::scorep2;
				}
			}
		}

		// Printing the scores for player 1
		Definitions::tft->setCursor(50, highScoreScreen::cursorValue);
		Definitions::tft->print(highScoreScreen::highScoresP1[5 - i]);

		// Printing the scores for player 2
		Definitions::tft->setCursor(200, highScoreScreen::cursorValue);
		Definitions::tft->print(highScoreScreen::highScoresP2[5 - i]);


		// the y value for the cursor += 33, so the values will be underneath each other
		highScoreScreen::cursorValue += 33;
	}
}

void highScoreScreen::refresh()
{
	Definitions::nunchuk->update();

	// Checking if the cButton is being pushed
	if (Definitions::nunchuk->cButton)
	{
		// Deleting the currentScreen
		delete Definitions::currentScreen;

		// Jumping to the first line of the program to reset all the processes
		asm volatile ("  jmp 0");

	}
}

void highScoreScreen::end() {


}

void highScoreScreen::checkNunchuck() {

}