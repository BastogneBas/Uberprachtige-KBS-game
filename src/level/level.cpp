#include "level.h"
#include <Arduino.h>
#include "../../staticDefinitions.cpp"
#include "levelDefs.h"

/* Level constructor
 * Creates a level from predefined barrel locations.
 */
Level::Level(uint16_t barrels[Definitions::gameHeight], String name)
{
	for (int x = 0; x <= Definitions::gameWidth + 1; x++)
		for (int y = 0; y <= Definitions::gameHeight + 1; y++)
			map[y][x] = 0;

	// Copy barrel locations from current level to ram
	for (uint8_t y = 0; y < Definitions::gameHeight; y++)
	{
//		Definitions::irComm->println(barrels[y], BIN);
//		this->barrels[i] = barrels[i];
//		Definitions::irComm->print(i);
//		Definitions::irComm->print(": ");
//		Definitions::irComm->println(this->barrels[i], BIN);
//		Definitions::irComm->println();


//		Definitions::irComm->print(y);
//		Definitions::irComm->print(": ");
//		Definitions::irComm->println(barrels[y], BIN);
		for (uint8_t x = 0; x < Definitions::gameWidth; x++)
		{
			uint16_t mask = (1 << (Definitions::gameWidth - 1)) >> x;
			if (barrels[y] & (mask))
			{
				setObjectAt(x + 1, y + 1, mapObject::barrel);
			}
//			markObjectAt(x + 1, y + 1, mapObject::needsRedraw);
		}
	}
	// And set the name of the current level
	this->name = name;
}

Level::Level(const uint16_t (*barrels)[Definitions::gameHeight], const char *name)
{
	// Copy barrel locations from current level to ram
	for (uint8_t i = 0; i < Definitions::gameHeight; i++)
	{
		// TODO: Fix it
//		this->barrels[i] = *barrels[i];
	}
	// And set the name of the current level
	this->name = name;
}

/* Level constructor
 * Creates a level with random barrel locations
 */
#if PEEP == 1
Level::Level(String name)
#elif PEEP == 2
Level::Level(String name, uint16_t seed)
#endif
{
#if PEEP == 1
	// Use the analog input of A0 as the seed for the random number generator
	uint16_t seed = analogRead(A0);
	Definitions::irComm->write((uint8_t)seed);
	Definitions::irComm->write((uint8_t)(seed>>8))
	randomSeed(seed);
#elif PEEP == 2
	randomSeed(seed);
#endif
	//randomSeed(0xFFFF);
	// For each line use a random uint16_t for barrel locations, and turn off the where no barrel can be placed
	for (uint8_t y = 0; y < Definitions::gameHeight; y++)
	{
//TODO: Fix it
		uint16_t barrelsy = (((uint16_t) random(0xFFFF)) &
							~(LevelDefs::YouCantPlaceBarrelsHere[y]));
		for (uint8_t x = 0; x < Definitions::gameWidth; x++)
		{
			uint16_t mask = (1 << (Definitions::gameWidth - 1)) >> x;
			if (barrelsy & mask)
			{
				setObjectAt(x + 1, y + 1, mapObject::barrel);
			}
		}
	}
	// Set the name of the level
	this->name = name;
}

/* Default constructor */
Level::Level()
{
};

// TODO: free the values
Level::~Level()
{
}

/* Initializes the screen and displays it */
void Level::begin()
{
	uint8_t width = Definitions::gameWidth + 1, height =
		Definitions::gameHeight + 1;

	//printMap();
	//Definitions::irComm->println();

//	for (int x = 0; x <= width; x++)
//		for (int y = 0; y <= height; y++)
//			map[y][x] = 0;

	//printMap();

	for (int x = 0; x <= width; x++)
		setObjectAt(x, 0, mapObject::block);
	for (int y = 0; y <= height; y++)
		setObjectAt(0, y, mapObject::block);
	for (int x = 0; x <= width; x++)
		setObjectAt(x, height, mapObject::block);
	for (int y = 0; y <= height; y++)
		setObjectAt(width, y, mapObject::block);

	for (uint8_t y = 2; y <= height; y += 2)
		for (uint8_t x = 2; x <= width; x += 2)
			setObjectAt(x, y, mapObject::block);


	setObjectAt(1, 1, mapObject::peep1);
	setObjectAt(Definitions::gameWidth, Definitions::gameHeight,
				mapObject::peep2);

	for (uint8_t y = 0; y < Definitions::gameHeight; y++)
	{
//		Definitions::irComm->print(y);
//		Definitions::irComm->print(": ");
//		Definitions::irComm->println(this->barrels[y], BIN);
		for (uint8_t x = 0; x < Definitions::gameWidth; x++)
		{
//			uint16_t mask = (1 << (Definitions::gameWidth - 1)) >> x;
//			if (this->barrels[y] & (mask))
//			{
//				setObjectAt(x + 1, y + 1, mapObject::barrel);
//			}
			markObjectAt(x + 1, y + 1, mapObject::needsRedraw);
		}
	}

	Definitions::irComm->println();
	printMap();
}

// TODO: Fix it
//// Returns the barrel locations
//uint16_t *Level::getBarrels()
//{
//	return this->barrels;
//}

// Returns the level name
String Level::getName()
{
	return this->name;
}

/* Returns the X value of the bomb specified */
uint8_t Level::getBombX(int index)
{
	return this->bombX[index];
}

/* Returns the Y value of the bomb specified */
uint8_t Level::getBombY(int index)
{
	return this->bombY[index];
}

/* Returns the time, the specified bomb was placed */
uint32_t Level::getBombTime(int index)
{
	return this->bombTime[index];
}

/* Returns by which peep the specified bomb was placed */
uint8_t Level::getBombPeep(int index)
{
	return this->bombPeep[index];
}

/* Sets the value of a bomb at the specified index */
void Level::setBomb(int index, uint8_t x, uint8_t y, uint32_t time,
					uint8_t peep)
{
	bombX[index] = x;
	bombY[index] = y;
	bombTime[index] = time;
	bombPeep[index] = peep;
}

void Level::printMap()
{
#ifdef DEBUG
	for (uint8_t y = 0; y <= Definitions::gameHeight + 1; y++)
	{
		for (uint8_t x = 0; x <= Definitions::gameWidth + 1; x++)
		{
			if (map[y][x] <= 0xFFF)
				Definitions::irComm->print("0");
			if (map[y][x] <= 0xFF)
				Definitions::irComm->print("0");
			if (map[y][x] <= 0xF)
				Definitions::irComm->print("0");
			Definitions::irComm->print(map[y][x], HEX);
			Definitions::irComm->print("\t");
		}
		Definitions::irComm->println();
	}
#endif
}

uint16_t Level::getObjectAt(uint8_t x, uint8_t y)
{
	return map[y][x];
}

void Level::setObjectAt(uint8_t x, uint8_t y, uint16_t object)
{
	this->setObjectAt(x, y, object, false);
}

void Level::setObjectAt(uint8_t x, uint8_t y, uint16_t object,
						uint8_t drawn)
{
	if (!drawn)
		map[y][x] = (object | mapObject::needsRedraw);
	else
		map[y][x] = (object & ~(mapObject::needsRedraw));
}

void Level::markObjectAt(uint8_t x, uint8_t y, uint16_t flag)
{
	map[y][x] |= flag;
}

void Level::unmarkObjectAt(uint8_t x, uint8_t y, uint16_t flag)
{
	map[y][x] &= ~flag;
}

void Level::drawMap()
{
	for (uint8_t y = 0; y <= Definitions::gameHeight + 1; y++)
	{
		Definitions::tft->startWrite();
		for (uint8_t x = 0; x <= Definitions::gameWidth + 1; x++)
		{
			uint16_t currentObject =
				getObjectAt(x, y) & ~(mapObject::needsRedraw);
			uint16_t Redraw =
				(getObjectAt(x, y) & mapObject::needsRedraw) ==
				mapObject::needsRedraw;
			setObjectAt(x, y, currentObject, true);
			if (Redraw)
			{
				if (!(currentObject & mapObject::air))
				{
					uint16_t _x = x * 16, _y = y * 16;
#ifdef DEBUG
					Definitions::println("Draw air");
#endif
					Definitions::tft->writeFillRect(_x, _y, 16, 16,
													ILI9341_BLACK);
				}
				if (currentObject & mapObject::block)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw block");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&blokje
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::barrel)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw barrel");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&ton
														  [j * _w + i]));
						}
					}
				}

				if (currentObject & mapObject::bomb
					&& !(currentObject & mapObject::explosion)
					&& currentObject & mapObject::bombPeep2)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw bomb");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&bomb2Idle
														  //(&explosion2Center
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::explosion
					&& !(currentObject & mapObject::explosionV)
					&& currentObject & mapObject::bombPeep2)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion H");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion2H
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::explosion
					&& (currentObject & mapObject::explosionV)
					&& currentObject & mapObject::bombPeep2)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion V");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion2V
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::bomb
					&& currentObject & mapObject::explosion
					&& currentObject & mapObject::bombPeep2)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion2Center
														  [j * _w + i]));
						}
					}
				}

				if (currentObject & mapObject::bomb
					&& !(currentObject & mapObject::explosion)
					&& currentObject & mapObject::bombPeep1)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw bomb");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&bomb1Idle
														  //(&explosion2Center
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::explosion
					&& !(currentObject & mapObject::explosionV)
					&& currentObject & mapObject::bombPeep1)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion H");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion1H
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::explosion
					&& (currentObject & mapObject::explosionV)
					&& currentObject & mapObject::bombPeep1)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion V");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion1V
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::bomb
					&& currentObject & mapObject::explosion
					&& currentObject & mapObject::bombPeep1)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw explosion");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&explosion1Center
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::peep1)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw peep1");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&peep1
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::peep2)
				{
					uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
#ifdef DEBUG
					Definitions::println("Draw peep2");
#endif
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 (&peep2
														  [j * _w + i]));
						}
					}
				}
			}
		}
		Definitions::tft->endWrite();
	}
}
