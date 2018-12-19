#include "../../screen.h"
#include <stddef.h>
#include "../level/level.h"

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class gameScreen: public screen
{
  public:
	gameScreen();
	gameScreen(Level level);
	void begin();
	void end();
	void refresh();
	void writeEndScreen();
  private:
	void movePeep (int peep, uint16_t x, uint16_t y);
    void placeBomb (int peep, uint16_t x, uint16_t y);
    void drawExplosion (int peep, uint16_t explX, uint16_t explY);
    void drawAir (uint16_t explX, uint16_t explY);
	Level level;
	uint16_t scoreP1;
	uint16_t scoreP2;
};

#endif
