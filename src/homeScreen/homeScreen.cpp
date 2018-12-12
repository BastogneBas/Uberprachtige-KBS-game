// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"		// Hardware-specific library
#include <SPI.h>

// Including nunchuck libs
#include "../ArduinoNunchuk/ArduinoNunchuk.h"

// Other includes
#include "../../staticDefinitions.cpp"
#include "homeScreen.h"
#include "../lvlSelectScreen/lvlSelectScreen.h"
#include "../instructionScreen/instructionScreen.h"
#include "../highScoreScreen/highScoreScreen.h"
#include "../../screen.h"

// Defining constructor
homeScreen::homeScreen()
{

}

// Void that gets called once when the class is being called.
void homeScreen::begin()
{
	// Filling the screen with darkGrey
	Definitions::tft->fillScreen(ILI9341_DARKGREY);

	// For loop that makes the three buttons in the homeScreen
	for (int i = 1; i <= 3; i++)
	{
		// Setting the values for the fist button
		Definitions::tft->drawRect(39, homeScreen::yRect, 242, 42, ILI9341_BLACK);
		Definitions::tft->fillRect(40, homeScreen::yFill, 240, 40, ILI9341_YELLOW);

		// Setting the colors
		Definitions::tft->setCursor(55, homeScreen::cursor);
		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setTextSize(3);

		// If i == 1 (means that the first button is being made), write Levels
		// Idem button 2 and 3.
		if (i == 1)
		{
			Definitions::tft->print("Levels");
		}
		else if (i == 2)
		{
			Definitions::tft->print("High-scores");
		} else
		{
			Definitions::tft->print("Instructions");
		}

		// Adding 50 to the variables, so the buttons will be underneath each other
		homeScreen::yRect += 50;
		homeScreen::yFill += 50;
		homeScreen::cursor += 50;
	}
}

// Void for refreshing the homepage
void homeScreen::refresh()
{
	// Updating the nunchuk values
	Definitions::nunchuk->update();

	// If statement that checks if the nunchuk joystick is pushed down
	if (Definitions::nunchuk->analogY < 50)
	{
		// Checking if buttonSelect (variable that holds the current buttonValue)
		// isn't >= 3. This will check if the button is at or past the last button
		if (!(homeScreen::buttonSelect >= 3))
		{
			// If it isn't, it means that the last button hasn't been reached yet, so we can increment it
			homeScreen::buttonSelect++;
		}
		// Else, we only have to repaint the button, which must be done every time
		homeScreen::repaint(homeScreen::buttonSelect);
	}

	// If statement that checks if the nunchuk joystick is pushed down
	else if (Definitions::nunchuk->analogY > 200)
	{
		// The same thing happens if the joystick is being pushed up
		// If buttonSelect isn't smaller or equal to 0 OR buttonSelect isn't 1
		if ((!homeScreen::buttonSelect <= 0) || (!homeScreen::buttonSelect == 1))
		{
			// Then it is possible to decrement the value (button hasn't reached the bottom yet)
			homeScreen::buttonSelect--;
		}
		// Else, we only have to repaint the button, which must be done every time
		homeScreen::repaint(homeScreen::buttonSelect);
	}

	// If statement that will check if the zButton is being pushed
	// and if buttonSelect != 0. With the zButton we can select button in the menu
	if ((Definitions::nunchuk->zButton) && (!homeScreen::buttonSelect == 0))
	{
		// If true, the newScreen function will be called. This function
		// Will close the current screen, and call the new screen
		newScreen(homeScreen::buttonSelect);
	}
}

// The repaint function will repaint the buttons, so it looks like you
// are moving through the menu. Selected == white and unselected == black
void homeScreen::repaint(uint8_t buttonSelect)
{
	// If buttonSelect is 1, it means that the first button (Levels, in this example) is selected
	if (buttonSelect == 1)
	{
		// This will rewrite the border of the button being selected white
		Definitions::tft->drawRect(39, 79, 242, 42, ILI9341_WHITE);

		// This will rewrite the border of the button underneath black
		Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_BLACK);
	}

	// The same things happens with buttons 2 and 3
	else if (buttonSelect == 2)
	{
		Definitions::tft->drawRect(39, 79, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_BLACK);
	}
	else if (buttonSelect == 3)
	{
		Definitions::tft->drawRect(39, 129, 242, 42, ILI9341_BLACK);
		Definitions::tft->drawRect(39, 179, 242, 42, ILI9341_WHITE);
	}
}

// Function that will be called if the user wants to go to a new window (= zButton being pressed)
void homeScreen::newScreen(uint8_t buttonSelect)
{
	// Checking if buttonSelect is > 0 && <= 3 for general functions on all buttons
	if (buttonSelect > 0 && buttonSelect <= 3)
	{
		// If buttonSelect == 1, the lvlSelect screen will be called and started.
		// Idem buttons 2 and 3
		if (buttonSelect == 1)
		{
			Definitions::currentScreen = new lvlSelectScreen();
		}
		else if (buttonSelect == 2)
		{
			Definitions::currentScreen = new highScoreScreen();
		}
		else if (buttonSelect == 3)
		{
			Definitions::currentScreen = new instructionScreen();
		}
		// Opening the new window
		Definitions::currentScreen->begin();

		// TODO close the currentscreen
		Definitions::tft->endWrite();
	}

	// At the end, buttonSelect will be rosetted, so it will be at 0 again
	homeScreen::buttonSelect = 0;
}

void homeScreen::end() {

}

void homeScreen::checkNunchuck() {

}

