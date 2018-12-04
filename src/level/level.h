#include <Arduino.h>
#include <stdint.h>
#include "../../staticDefinitions.cpp"

#ifndef LEVEL_H
#define LEVEL_H

class Level
{
  public:
	// Constructors
	Level(uint16_t barrels[Definitions::gameHeight], String name);
	Level(String name);
	Level();
	~Level();

	// Getters
	uint16_t *getBarrels();
	String getName();

	uint8_t getObjectAt(uint8_t x, uint8_t y);
  private:
	/* The barrel locations
	 * Every uint32_t is a horizontal row of barrels. So for example:
	 *  If barrels[0] = 0b0000000000010101
	 *  Shows barrels at:
	 *      ███████████████
	 *      █        B B B█
	 */
	uint16_t barrels[Definitions::gameHeight] = { 0 };

	uint8_t map[16][16] = {{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}};
	// The name of the level
	String name;
};

#endif
