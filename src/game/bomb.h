#include <stdint.h>

#ifndef BOMB_H
#define BOMB_H

class Bomb	// Define bomb class
{
  public:
	// Define functions
	Bomb(uint32_t placedTime, uint8_t x, uint8_t y, uint8_t blastRadius = 3);

	// Define variables
	uint32_t getPlacedTime();
	uint8_t getx();
	uint8_t gety();
	uint8_t getBlastRadius();
	uint8_t getPeep();

  private:
	// Define variables
	uint32_t placedTime;
	uint8_t x;
	uint8_t y;
	uint8_t blastRadius;
	uint8_t peep;
};

#endif
