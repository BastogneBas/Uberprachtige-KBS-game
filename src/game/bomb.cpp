#include "bomb.h"
#include <stdint.h>

Bomb::Bomb(uint32_t placedTime, uint8_t x, uint8_t y, uint8_t blastRadius)	// Initialize bomb class
{
	placedTime = getPlacedTime();
	x = getx();
	y = gety();
	blastRadius = getBlastRadius();
}

uint32_t Bomb::getPlacedTime()	// Return the time of when the bomb was placed
{
	return placedTime;
}

uint8_t Bomb::getx()	// Return bomb x-position
{
	return x;
}

uint8_t Bomb::gety()	// Return bomb y-position
{
	return y;
}

uint8_t Bomb::getBlastRadius()	// Return bomb range
{
	return blastRadius;
}
