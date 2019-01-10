#include <Arduino.h>
#include <stdint.h>
#include "../../staticDefinitions.cpp"

#ifndef LEVEL_H
#define LEVEL_H

class mapObject
{
  public:
	// Defining all the masks. Each game property gets it's own mask.
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
	static const unsigned int bombPeep1		= 0b0000000100000000; // 0b 0000 000x 0000 0000
	static const unsigned int bombPeep2		= 0b0000001000000000; // 0b 0000 00x0 0000 0000
};

class Level
{
  public:
	// TODO: Describe all variables and what they are used for
	// Constructors
	Level(uint16_t barrels[Definitions::gameHeight], String name);
	Level(const uint16_t(*)[11], const char* name);
	Level(String name);
	Level(String name, uint16_t seed);
	Level();
	~Level();
	void begin();

	// Getters
	uint16_t *getBarrels();
	String getName();

	uint16_t getObjectAt(uint8_t x, uint8_t y);
	void setObjectAt(uint8_t x, uint8_t y, uint16_t object);
	void setObjectAt(uint8_t x, uint8_t y, uint16_t object, uint8_t drawn);
	void markObjectAt(uint8_t x, uint8_t y, uint16_t flag);
	void unmarkObjectAt(uint8_t x, uint8_t y, uint16_t flag);
	void drawMap();
//	void printMap();

	//Bomb getBomb(int index);
	//void setBomb(int index, Bomb bomb);
	void setBomb(int index, uint8_t x, uint8_t y, uint32_t time,
				 uint8_t peep);
	uint8_t getBombX(int index);
	uint8_t getBombY(int index);
	uint32_t getBombTime(int index);
	uint8_t getBombPeep(int index);


  private:
	// Variable that holds the grid of a map
	uint16_t map[Definitions::gameHeight + 2][Definitions::gameWidth + 2] = {
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

	uint8_t bombX[2] = { 0 };
	uint8_t bombY[2] = { 0 };
	uint32_t bombTime[2] = { 0 };
	uint8_t bombPeep[2] = { 0 };

	// The name of the level
	String name;
};

#endif
