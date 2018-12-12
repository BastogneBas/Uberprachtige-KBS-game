#ifndef UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H
#define UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H

class homeScreen: public screen
{
	// Defining all the public functions and variables
	// The instructions of the functions can be found in the homeScreen.cpp file
public:
	homeScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t buttonSelect);
	void newScreen(uint8_t buttonSelect);

	// Defining variable that holds the value of the current button being selected
	uint8_t buttonSelect = 0;

	// Defining variables to fill the screen with
	uint8_t yRect = 79, yFill = 80, cursor = 90;

	// Variables for writing the sprite
	uint16_t _x = 9, _y = 5, _w = 16, _h = 16;

private:

};

#endif UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H


