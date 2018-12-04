#include "level.h"
#include <Arduino.h>
#include "../../staticDefinitions.cpp"
#include "levelDefs.h"

/* Level constructor
 * Creates a level from predefined barrel locations.
 */
Level::Level(uint16_t barrels[Definitions::gameHeight], String name)
{
	// Copy barrel locations from current level to ram
	for (uint8_t i = 0; i < Definitions::gameHeight; i++)
	{
		this->barrels[i] = barrels[i];
	}
	// And set the name of the current level
	this->name = name;
}

/* Level constructor
 * Creates a level with random barrel locations
 */
Level::Level(String name)
{
	// Use the analog input of A0 as the seed for the random number generator
	randomSeed(analogRead(A0));
	// For each line use a random uint16_t for barrel locations, and turn off the where no barrel can be placed
	for (uint8_t i = 0; i < Definitions::gameHeight; i++)
	{
		this->barrels[i] =
			(((uint16_t) random(0xFFFF)) &
			 ~(LevelDefs::YouCantPlaceBarrelsHere[i]));
	}
	// Set the name of the level
	this->name = name;
}

// Dofault constructor
Level::Level()
{
};

// TODO: free the values
Level::~Level()
{
}

// Returns the barrel locations
uint16_t *Level::getBarrels()
{
	return this->barrels;
}

// Returns the level name
String Level::getName()
{
	return this->name;
}

uint8_t Level::getObjectAt(uint8_t x, uint8_t y){
	return map[x][y];
}

void Level::setObjectAt(uint8_t x, uint8_t y, uint8_t object){
	map[x][y] = object;
}

void Level::drawMap();
