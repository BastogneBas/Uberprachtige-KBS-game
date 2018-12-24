#include "bomb.h"
#include <stdint.h>

// Initialize bomb class
Bomb::Bomb(uint32_t placedTime, uint8_t x, uint8_t y, uint8_t blastRadius)
{
	placedTime = getPlacedTime();
	x = getx();
	y = gety();
	blastRadius = getBlastRadius();
}

// Get the time of when the bomb was placed
uint32_t Bomb::getPlacedTime()
{
	return placedTime;
}

// Get bomb x-position
uint8_t Bomb::getx()
{
	return x;
}

// Get bomb y-position
uint8_t Bomb::gety()
{
	return y;
}

// Get bomb range
uint8_t Bomb::getBlastRadius()
{
	return blastRadius;
}
