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

void Level::begin()
{
	Serial.println("begin!");
	uint8_t width = Definitions::gameWidth + 1, height =
		Definitions::gameHeight + 1;

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
		for (uint8_t x = 0; x < Definitions::gameWidth; x++)
		{
			uint16_t mask = 1 << y;
			if (getBarrels()[x] & (mask))
			{
				setObjectAt(x + 1, y + 1, mapObject::barrel);
			}
			else
			{
				//setObjectAt(y, x,mapObject::air);
			}
			markObjectAt(x + 1, y + 1, mapObject::needsRedraw);
		}
	}
	//while(1);
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

void Level::printMap()
{
	for (uint8_t y = 0; y <= Definitions::gameHeight + 1; y++)
	{
		for (uint8_t x = 0; x <= Definitions::gameWidth + 1; x++)
		{
			Serial.print(map[y][x], HEX);
			Serial.print(" ");
		}
		Serial.println();
	}
}

uint16_t Level::getObjectAt(uint8_t x, uint8_t y)
{
	//printMap();
	return map[y][x];
}

void Level::setObjectAt(uint8_t x, uint8_t y, uint16_t object,
						uint8_t drawn = false)
{
	if (!drawn)
		map[y][x] = (object | mapObject::needsRedraw);
	else
		map[y][x] = (object & ~(mapObject::needsRedraw));
	//printMap();
	//Serial.println();
	//delay(15);
}

void Level::markObjectAt(uint8_t x, uint8_t y, uint16_t flag)
{
	map[y][x] |= flag;
	//printMap();
	//Serial.println();
	//delay(15);
}

void Level::unmarkObjectAt(uint8_t x, uint8_t y, uint16_t flag)
{
	Serial.println(map[y][x], BIN);
	map[y][x] &= ~flag;
	Serial.println(map[y][x], BIN);
}

void Level::drawMap()
{
	Serial.println("draw!");
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
			//Serial.println(Redraw);
			setObjectAt(x, y, currentObject, true);
			if (Redraw)
			{
				if (!(currentObject & mapObject::air))
				{
			uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
					Serial.println("Draw air");
					Definitions::tft->writeFillRect(_x, _y, 16, 16,
													ILI9341_BLACK);
				}
				if (currentObject & mapObject::block)
				{
			uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
					Serial.println("Draw block");
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
					Serial.println("Draw barrel");
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
				if (currentObject & mapObject::bomb)
				{
			uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
					Serial.println("Draw bomb");
					for (uint16_t j = 0; j < _h; j++, _y++)
					{
						for (uint16_t i = 0; i < _w; i++)
						{
							Definitions::tft->writePixel(_x + i, _y,
														 pgm_read_word
														 //(&bomb1Idle
														 (&explosion2Center
														  [j * _w + i]));
						}
					}
				}
				if (currentObject & mapObject::peep1)
				{
			uint16_t _x = x * 16, _y = y * 16, _w = 16, _h = 16;
					Serial.println("Draw peep1");
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
					Serial.println("Draw peep2");
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
