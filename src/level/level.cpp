#include "level.h"
#include <Arduino.h>
//#include "../../staticDefinitions.cpp"

/* Level constructor
 * Creates a level from predefined barrel locations.
 */
Level::Level(uint16_t barrels[Definitions::gameHeight], String name)
{
	for(uint8_t i=0;i<Definitions::gameHeight;i++){
		
	}
}

/* Level constructor
 * Creates a level with random barrel locations
 */
Level::Level(){}

// Returns the barrel locations
uint16_t* Level::getBarrels()
{
	return this->barrels;
}

// Returns the level name
String Level::getName(){
	return this->name;
}
