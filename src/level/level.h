#include <Arduino.h>
#include <stdint.h>
#include "../../staticDefinitions.cpp"
#include "../game/bomb.h"

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
	// Masks
	static const unsigned int air			= 0b0000000000000000; // 0b 0000 0000 0000 0000

	static const unsigned int block			= 0b0000000000000001; // 0b 0000 0000 0000 000x
	static const unsigned int barrel		= 0b0000000000000010; // 0b 0000 0000 0000 00x0
	static const unsigned int peep1			= 0b0000000000000100; // 0b 0000 0000 0000 0x00
	static const unsigned int peep2			= 0b0000000000001000; // 0b 0000 0000 0000 x000

	static const unsigned int bomb 			= 0b0000000000010000; // 0b 0000 0000 000x 0000
	static const unsigned int explosion 	= 0b0000000000100000; // 0b 0000 0000 00x0 0000
	static const unsigned int explosionH	= 0b0000000000000000; // 0b 0000 0000 0x00 0000
	static const unsigned int explosionV	= 0b0000000001000000; // 0b 0000 0000 0x00 0000
	static const unsigned int needsRedraw	= 0b0000000010000000; // 0b 0000 0000 x000 0000

	static const unsigned int blockIdMsk	= 0b1111111100000000; // 0b xxxx xxxx 0000 0000
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
	void begin();

	// Getters
	uint16_t *getBarrels();
	String getName();

	uint16_t getObjectAt(uint8_t x, uint8_t y);
	void setObjectAt(uint8_t x, uint8_t y, uint16_t object, uint8_t drawn =
					 false);
	void markObjectAt(uint8_t x, uint8_t y, uint16_t flag);
	void unmarkObjectAt(uint8_t x, uint8_t y, uint16_t flag);
	void drawMap();
	void printMap();

	//Bomb getBomb(int index);
	//void setBomb(int index, Bomb bomb);
	void setBomb(int index, uint8_t x, uint8_t y, uint8_t time, uint8_t peep);
	uint8_t getBombX();
	uint8_t getBombY();
	uint8_t getBombTime();
	uint8_t getBombPeep();


  private:
	/* The barrel locations
	 * Every uint32_t is a horizontal row of barrels. So for example:
	 *  If barrels[0] = 0b0000000000010101
	 *  Shows barrels at:
	 *      ███████████████
	 *      █        B B B█
	 */
	 uint16_t barrels[Definitions::gameHeight] = { 0 };

	uint16_t map[Definitions::gameHeight+2][Definitions::gameWidth+2] = {
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

	uint8_t *bombX[2];
	uint8_t *bombY[2];
	uint8_t *bombTime[2];
	uint8_t *bombPeep[2];

	// The name of the level
	String name;
};

#endif
