//
// Created by alber on 10-12-2018.
//

#ifndef UBERPRACHTIGE_KBS_GAME_HIGHSCORESCREEN_H
#define UBERPRACHTIGE_KBS_GAME_HIGHSCORESCREEN_H

class highScoreScreen: public screen
{
public:
	homeScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t buttonSelect, bool zButton);
	void newScreen(bool zButton, uint8_t buttonSelect);

private:
	// Defining variables

};

#endif //UBERPRACHTIGE_KBS_GAME_HIGHSCORESCREEN_H
