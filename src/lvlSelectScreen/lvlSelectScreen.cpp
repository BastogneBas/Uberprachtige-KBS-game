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

// Defining constructor
lvlSelectScreen::lvlSelectScreen()
{

}

// Void that gets called once when the class is being called.
void lvlSelectScreen::begin()
{
	// Filling the screen with darkGrey
	Definitions::tft->fillScreen(ILI9341_DARKGREY);

	// For loop that makes the four buttons in the levelSelectScreen
	for (int i = 1; i <= 4; i++)
	{
		// Setting the values for the fist button
		Definitions::tft->drawRect(19, lvlSelectScreen::yRect, 242, 42, ILI9341_BLACK);
		Definitions::tft->fillRect(20, lvlSelectScreen::yFill, 240, 40, ILI9341_YELLOW);

		// Setting the colors
		Definitions::tft->setCursor(35, lvlSelectScreen::cursor);
		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setTextSize(3);

		// If i == 4, it means that the last button is being made, so the
		// button should say "Random level"
		if (i == 4)
		{
			Definitions::tft->print("Random level");
		}
		// Else, the hardcoded levels will be showed (buttons 1-3) with levels 1,2 and 3
		else
		{
			Definitions::tft->print("Level ");
			Definitions::tft->println(i);
		}

		// Adding 50 to the variables, so the buttons will be underneath each other
		lvlSelectScreen::yRect += 50;
		lvlSelectScreen::yFill += 50;
		lvlSelectScreen::cursor += 50;
	}
}

// Void for refreshing the levelSelectScreen
void lvlSelectScreen::refresh()
{
	// Updating the nunchuk values
	Definitions::nunchuk->update();

	// If statement that checks if the nunchuk joystick is pushed down
	if (Definitions::nunchuk->analogY < 50)
	{
		// Checking if buttonSelect (variable that holds the current buttonValue)
		// isn't >= 4. This will check if the button is at or past the last button
		if (!lvlSelectScreen::selectedButton >= 4)
		{
			// If it isn't, it means that the last button hasn't been reached yet, so we can increment it
			lvlSelectScreen::selectedButton++;
		}
		// Else, we only have to repaint the button, which must be done every time
		lvlSelectScreen::repaint(lvlSelectScreen::selectedButton);
	}

	// If statement that checks if the nunchuk joystick is pushed down
	else if (Definitions::nunchuk->analogY > 200)
	{
		// The same thing happens if the joystick is being pushed up
		// If buttonSelect isn't smaller or equal to 0 OR buttonSelect isn't 1
		if ((!lvlSelectScreen::selectedButton <= 0) || (!lvlSelectScreen::selectedButton == 1))
		{
			// Then it is possible to decrement the value (button hasn't reached the bottom yet)
			lvlSelectScreen::selectedButton--;
		}
		// Else, we only have to repaint the button, which must be done every time
		lvlSelectScreen::repaint(lvlSelectScreen::selectedButton);
	}

	// If statement that will check if the zButton is being pushed
	// and if buttonSelect != 0. With the zButton we can select button in the menu
	if ((Definitions::nunchuk->zButton) && (!lvlSelectScreen::selectedButton == 0))
	{
		// If true, the newScreen function will be called. This function
		// Will close the current screen, and call the new screen
		lvlSelectScreen::startGame(lvlSelectScreen::selectedButton);
	}

	// If statement that will check if the cButton is being pushed
	if (Definitions::nunchuk->cButton)
	{
		// If true, we will return to the previous screen (= homeScreen)
		// Will close the current screen, and call the new screen
		// selectedButton will also be set to 0
		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
		lvlSelectScreen::selectedButton = 0;
	}
}

// The repaint function will repaint the buttons, so it looks like you
// are moving through the menu. Selected == white and unselected == black
void lvlSelectScreen::repaint(uint8_t selectedButton)
{
	// If buttonSelect is 1, it means that the first button (Level 1, in this example) is selected
	if (selectedButton == 1)
	{
		// This will rewrite the border of the button being selected white
		Definitions::tft->drawRect(19, 29, 242, 42, ILI9341_WHITE);

		// This will rewrite the border of the button underneath black
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_BLACK);
	}

	// The same things happen with levels 2, 3 and the random level
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

// Function that will be called if the user wants to go to start a level
void lvlSelectScreen::startGame(uint8_t selectedButton)
{
	// Checking if buttonSelect is > 0 && <= 4 for general functions on all buttons
	if (selectedButton > 0 && selectedButton <= 4)
	{
		// If buttonSelect == 1, the lvlSelect screen will be called and started.
		// The same things happen with the other buttons
		if (selectedButton == 1)
		{
			Definitions::currentScreen = new gameScreen(LevelDefs::getLevel(0));
		}
		else if (selectedButton == 2)
		{
			Definitions::currentScreen = new gameScreen(LevelDefs::getLevel(1));
		}
		else if (selectedButton == 3)
		{
			Definitions::currentScreen = new gameScreen(LevelDefs::getLevel(2));
		}
		else if (selectedButton == 4)
		{
			Definitions::currentScreen = new gameScreen(LevelDefs::getLevel("Random"));

		}
		Definitions::currentScreen->begin();

		// TODO close the currentscreen
	}

}


void lvlSelectScreen::end()
{

}

void lvlSelectScreen::checkNunchuck()
{

}

