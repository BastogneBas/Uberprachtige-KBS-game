//
// Created by alber on 7-12-2018.
//

#ifndef UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H
#define UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H

// Defining touchscreen variables


class homeScreen: public screen
{

public:
	homeScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t buttonSelect);
	void newScreen(uint8_t buttonSelect);

private:
	// Defining variables


};

#endif UBERPRACHTIGE_KBS_GAME_HOMESCREEN_H


