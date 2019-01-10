#include "../../screen.h"

#ifndef UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H
#define UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H

// Defining all the datatypes
#define START_BYTE 		0x01
#define LEVEL_BYTE 		0x02
#define LIVES_BYTE 		0x03
#define SCORE_BYTE 		0x04
#define PEEP_LOCATION	0x05
#define BOMB_LOCATION 	0x06
#define STOP_BYTE 		0x07
#define TIME_BYTE		0x08
#define SEND_AGAIN 		0xFF

class lvlSelectScreen:public screen
{
  public:
	lvlSelectScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t selectedButton);
	inline void startGame() { startGame(0); }
	void startGame(uint16_t seed);
	void waitForStart();

	// Defining variable that holds the value of the current button being selected
	uint8_t selectedButton = 1;

	// Defining variables to fill the screen with
	uint8_t yRect = 29, yFill = 30, cursor = 40;

  private:

};


#endif
