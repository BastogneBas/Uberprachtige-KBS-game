#include <Arduino.h>
#include <stdint.h>
#include "../../staticDefinitions.cpp"

#ifndef LEVEL_H
#define LEVEL_H


// *INDENT-OFF*
/*enum class mapObject:unsigned char
{
	air			= 0b00000000,
	block		= 0b00000001,
	barrel		= 0b00000010,
	peep1		= 0b00000100,
	peep2		= 0b00001000,
	bomb 		= 0b00010000,
	explosion 	= 0b00100000,

	peep1Air	= peep1|air
};*/

class mapObject
{
  public:
	static const unsigned char air			= 0b00000000;
	static const unsigned char block		= 0b00000001;
	static const unsigned char barrel		= 0b00000010;
	static const unsigned char peep1		= 0b00000100;
	static const unsigned char peep2		= 0b00001000;
	static const unsigned char bomb 		= 0b00010000;
	static const unsigned char explosion 	= 0b00100000;
};

// *INDENT-ON*

/*mapObject& operator|(mapObject& m1, mapObject& m2){
	switch(m1){
		case m1:
			return m1;
			break;
	}
};*/


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
	void setObjectAt(uint8_t x, uint8_t y, uint8_t object);
	void drawMap();

  private:
	/* The barrel locations
	 * Every uint32_t is a horizontal row of barrels. So for example:
	 *  If barrels[0] = 0b0000000000010101
	 *  Shows barrels at:
	 *      ███████████████
	 *      █        B B B█
	 */
	 uint16_t barrels[Definitions::gameHeight] = { 0 };

uint8_t map[16][16] =
		{ 
			{0},
		   	{0},
			{0},
		{0}, 
		{0}, 
		{0},
		{0}, 
		{0}, 
		{0},
		{0}, 
		{0}, 
		{0},
		{0}, 
		{0}, 
		{0},
		{0} 
		};

	// The name of the level
	String name;
};

#endif
