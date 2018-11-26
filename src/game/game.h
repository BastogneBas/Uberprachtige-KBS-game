#include "../../screen.h"
#include <stddef.h>

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class gameScreen:public screen
{
  public:
	gameScreen();
	void begin();
	void end();
	void refresh();
  private:
	void drawPeep1(uint8_t x, uint8_t y);
	void drawBarrel(uint8_t x, uint8_t y);
	void drawBlock(uint8_t x, uint8_t y);
};

#endif
