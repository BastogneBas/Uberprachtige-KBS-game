#include "../../screen.h"
#include <stddef.h>
#include "../level/level.h"

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class gameScreen:public screen
{
  public:
	gameScreen();
	gameScreen(Level level);
	void begin();
	void end();
	void refresh();
	void checkNunchuck();
  private:
	void drawPeep1(uint16_t x, uint16_t y);
	void drawPeep2(uint16_t x, uint16_t y);
	void drawBarrel(uint16_t x, uint16_t y);
	void drawBlock(uint16_t x, uint16_t y);
	void drawBomb (uint16_t x, uint16_t y);
	void movePeep (int peep, uint16_t x, uint16_t y);
    void placeBomb ();
	Level level;
};

#endif
