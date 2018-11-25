#include "../../scherm.h"

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

class spelScherm : public scherm {
	public:
		spelScherm();
		void begin();
		void end();
		void refresh();
	private:
		void drawPoppetje1(uint8_t x, uint8_t y);
		void drawTon(uint8_t x, uint8_t y);
		void drawBlokje(uint8_t x, uint8_t y);
};

#endif
