#include "../../screen.h"
#include <stddef.h>
#include "../level/level.h"

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class gameScreen:public screen // Defines the gameScreen class
{
  public:
	// Define functions
	gameScreen();
	gameScreen(Level *level);
	gameScreen(uint8_t levelnr);
	void begin();
	void end();
	void refresh();
	void drawEndScreen();
	void drawLives();
	void drawTimer();
	void drawScore();
	void checkWinner();
	void calculateEndScores();

  private:
	// Define functions
	void movePeep(int peep);
	void placeBomb(int peep, uint16_t x, uint16_t y);
	void drawExplosion(int peep, uint16_t explX, uint16_t explY);
	void drawAir(uint16_t explX, uint16_t explY);
	// Define Variables
	Level level;
	uint8_t livesP1 = 3;
	uint8_t livesP2 = 3;
	int16_t scoreP1 = 4000;
	int16_t scoreP2 = 4000;
	uint8_t timeCounter = 0;
	uint8_t currentTime = 180;
	uint8_t deadPlayer = 0;
	uint8_t winner = 0;
};
#endif
