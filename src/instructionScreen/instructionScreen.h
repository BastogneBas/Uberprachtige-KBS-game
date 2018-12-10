//
// Created by alber on 10-12-2018.
//

#ifndef UBERPRACHTIGE_KBS_GAME_INSTRUCTIONSCREEN_H
#define UBERPRACHTIGE_KBS_GAME_INSTRUCTIONSCREEN_H

class instructionScreen: public screen
{
public:
	instructionScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t buttonSelect, bool zButton);
	void newScreen(bool zButton, uint8_t buttonSelect);

private:
	// Defining variables

};

#endif //UBERPRACHTIGE_KBS_GAME_INSTRUCTIONSCREEN_H
