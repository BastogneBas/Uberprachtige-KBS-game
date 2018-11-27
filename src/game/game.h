#include "../../screen.h"
#include <stddef.h>

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class gameScreen:public screen
{
  public:
	gameScreen();
	//gameScreen(Level level);
	void begin();
	void end();
	void refresh();
	void operation();
	void checkNunchuck();
  private:
	void drawPeep1(uint8_t x, uint8_t y);
	void drawPeep2(uint8_t x, uint8_t y);
	void drawBarrel(uint8_t x, uint8_t y);
	void drawBlock(uint8_t x, uint8_t y);
	void drawBomb (uint8_t x, uint8_t y);
};

#endif
