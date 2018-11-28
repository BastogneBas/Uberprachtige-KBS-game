#include "level.h"
#include <Arduino.h>
#include "../../staticDefinitions.cpp"
#include "levelDefs.h"

/* Level constructor
 * Creates a level from predefined barrel locations.
 */
Level::Level(uint16_t barrels[Definitions::gameHeight], String name)
{
	for(uint8_t i=0;i<Definitions::gameHeight;i++){
		this->barrels[i]=barrels[i];
	}
	this->name = name;
}

/* Level constructor
 * Creates a level with random barrel locations
 */
Level::Level(String name){
	randomSeed((uint8_t)this);
	for(uint8_t i=0;i<Definitions::gameHeight;i++){
		this->barrels[i] = ((uint16_t)random(0xFFFFFFFF) & ~(LevelDefs::YouCantPlaceBarrelsHere[i]));
	}
	this->name = "Generated level";
}

Level::Level(){};

Level::~Level(){}

// Returns the barrel locations
uint16_t* Level::getBarrels()
{
	return this->barrels;
}

// Returns the level name
String Level::getName(){
	return this->name;
}
