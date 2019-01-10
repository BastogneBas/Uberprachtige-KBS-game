// Including libs for enabling the screen
#include "../Adafruit_GFX_Library/Adafruit_GFX.h"	// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"	// Hardware-specific library
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
	Definitions::tft->fillScreen(ILI9341_BLACK);

#if PEEP==1
	// For loop that makes the four buttons in the levelSelectScreen
	for (uint8_t i = 1; i <= 4; i++)
	{
		// Setting the values for the fist button
		Definitions::tft->drawRect(19, lvlSelectScreen::yRect, 242, 42,
								   ILI9341_DARKGREY);
		Definitions::tft->fillRect(20, lvlSelectScreen::yFill, 240, 40,
								   ILI9341_YELLOW);

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
	lvlSelectScreen::selectedButton = 1;
	lvlSelectScreen::repaint(lvlSelectScreen::selectedButton);
#elif PEEP==2
	Definitions::tft->setTextSize(2);
	Definitions::tft->setTextColor(ILI9341_WHITE);
	Definitions::tft->setCursor(0, 0);
	Definitions::tft->println("Wachten op speler 1...");
	Definitions::tft->setTextSize(1);
	waitForStart();
#endif
}

// Checking if player 2 is playing, if so, he has to wait for the level that is going to be played.
#if PEEP==2

// Function that waits until the level has been received
void lvlSelectScreen::waitForStart()
{
	// Defining variable that holds the seed
	uint16_t seed = 0;

	// While true
	while(1)
	{
		while(!Definitions::irComm->available())
			// C++ doesn't like empty while-loops, so we do nothing with PRR = PRR
			PRR = PRR;

		// 		
		int receivedcmd = Definitions::irComm->read();

		if(receivedcmd == LEVEL_BYTE)
		{
			Definitions::tft->print("Starting level ");

			while(!Definitions::irComm->available())
				PRR = PRR;

			int receivedlvl = Definitions::irComm->read();
			if(receivedlvl == 0x04)
			{
				Definitions::tft->println("random");
				selectedButton = 4;
				Definitions::tft->println("Waiting for seed...");

				while(!(Definitions::irComm->available()>1))
					PRR=PRR;

				seed = Definitions::irComm->read();
				seed = (Definitions::irComm->read() << 8);
			}
			else if(receivedlvl > 0x04)
			{
				Definitions::tft->println("error, lvl# too high");
			}
			else
			{
				Definitions::tft->println(receivedlvl);
				selectedButton = receivedlvl;
			}
		}
		else if(receivedcmd == START_BYTE)
		{
			startGame(seed);
			break;
		}
		else
		{
		}
	}
}
#endif

// Defining refreshcount variable
uint32_t *refreshCount = 0;

// Void for refreshing the levelSelectScreen
void lvlSelectScreen::refresh()
{
	*refreshCount++;

	if ((*refreshCount % 5)== 0) {
		// Updating the nunchuk values
		Definitions::nunchuk->update();

		// If statement that checks if the nunchuk joystick is pushed down
		if (Definitions::nunchuk->analogY < 50) {
			// Checking if buttonSelect (variable that holds the current buttonValue)
			// isn't >= 4. This will check if the button is at or past the last button
			if (!(lvlSelectScreen::selectedButton >= 4)) {
				// If it isn't, it means that the last button hasn't been reached yet, so we can increment it
				lvlSelectScreen::selectedButton++;
			}
			// Else, we only have to repaint the button, which must be done every time
			lvlSelectScreen::repaint(lvlSelectScreen::selectedButton);
		}

			// If statement that checks if the nunchuk joystick is pushed down
		else if (Definitions::nunchuk->analogY > 200) {
			// The same thing happens if the joystick is being pushed up
			// If buttonSelect isn't smaller or equal to 0 OR buttonSelect isn't 1
			if (!(lvlSelectScreen::selectedButton <= 1)) {
				// Then it is possible to decrement the value (button hasn't reached the bottom yet)
				lvlSelectScreen::selectedButton--;
			}
			// Else, we only have to repaint the button, which must be done every time
			lvlSelectScreen::repaint(lvlSelectScreen::selectedButton);
		}


		// If statement that will check if the zButton is being pushed
		// and if buttonSelect != 0. With the zButton we can select button in the menu
		if ((Definitions::nunchuk->zButton)) {

			// If true, the newScreen function will be called. This function
			// Will close the current screen, and call the new screen
			lvlSelectScreen::startGame();

		}

		// If statement that will check if the cButton is being pushed
		if (Definitions::nunchuk->cButton) {
			// First, delete pointer to the current screen
			delete Definitions::currentScreen;

			// If true, we will return to the previous screen (= homeScreen)
			// Will close the current screen, and call the new screen
			// selectedButton will also be set to 0
			delete Definitions::currentScreen;
			asm volatile ("  jmp 0");


		}
	}
}

// The repaint function will repaint the buttons, so it looks like you
// are moving through the menu. Selected == white and unselected == black
void lvlSelectScreen::repaint(uint8_t selectedButton)
{
	Definitions::tft->setTextSize(3);

	// If buttonSelect is 1, it means that the first button (Level 1, in this example) is selected
	if (selectedButton == 1)
	{
		// This will rewrite the border of the button being selected white
		Definitions::tft->drawRect(19, 29, 242, 42, ILI9341_WHITE);

		// This will rewrite the border of the button underneath black
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_DARKGREY);

		// Overwrite text
		Definitions::tft->setTextColor(ILI9341_RED);
		Definitions::tft->setCursor(35, 40);
		Definitions::tft->print("Level 1");

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 90);
		Definitions::tft->print("Level 2");


	}

	// The same things happen with levels 2, 3 and the random level
	else if (selectedButton == 2)
	{
		Definitions::tft->drawRect(19, 29, 242, 42, ILI9341_DARKGREY);
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_DARKGREY);

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 40);
		Definitions::tft->print("Level 1");

		Definitions::tft->setTextColor(ILI9341_RED);
		Definitions::tft->setCursor(35, 90);
		Definitions::tft->print("Level 2");

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 140);
		Definitions::tft->print("Level 3");
	}
	else if (selectedButton == 3)
	{
		Definitions::tft->drawRect(19, 79, 242, 42, ILI9341_DARKGREY);
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_WHITE);
		Definitions::tft->drawRect(19, 179, 242, 42, ILI9341_DARKGREY);

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 90);
		Definitions::tft->print("Level 2");

		Definitions::tft->setTextColor(ILI9341_RED);
		Definitions::tft->setCursor(35, 140);
		Definitions::tft->print("Level 3");

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 190);
		Definitions::tft->print("Random level");
	}
	else if (selectedButton == 4)
	{
		Definitions::tft->drawRect(19, 129, 242, 42, ILI9341_DARKGREY);
		Definitions::tft->drawRect(19, 179, 242, 42, ILI9341_WHITE);

		Definitions::tft->setTextColor(ILI9341_BLACK);
		Definitions::tft->setCursor(35, 140);
		Definitions::tft->print("Level 3");

		Definitions::tft->setTextColor(ILI9341_RED);
		Definitions::tft->setCursor(35, 190);
		Definitions::tft->print("Random level");
	}
}

// Function that will be called if the user wants to go to start a level
void lvlSelectScreen::startGame(uint16_t seed)
{
	// Send the level
	Definitions::irComm->write(LEVEL_BYTE);
	Definitions::irComm->write(selectedButton);
	// Send the startbyte
	Definitions::irComm->write(START_BYTE);

	// Checking if buttonSelect is > 0 && <= 4 for general functions on all buttons
	if ((selectedButton > 0) && (selectedButton <= 4))
	{
		// Start the random level
	    if(selectedButton == 4)
        {
            delete Definitions::currentScreen;
            // PEEP 2 requires a seed for the random level while PEEP 1 doesn't
		#if PEEP == 1
			Definitions::currentScreen = new gameScreen("Random");
		#elif PEEP == 2
            Definitions::currentScreen = new gameScreen("Random", seed);
		#endif
        }
	    // Start the selected level
	    else
        {
            delete Definitions::currentScreen;
            Definitions::currentScreen = new gameScreen(selectedButton-1);
        }

	    // Start the game
		Definitions::currentScreen->begin();
	}
}

void lvlSelectScreen::end()
{
}

void lvlSelectScreen::checkNunchuck()
{
}
