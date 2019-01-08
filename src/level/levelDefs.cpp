#include "levelDefs.h"

const char *LevelDefs::levelNames[] = {
	"Level 1",
	"Level 2",
	"Level 3"
};

const uint16_t LevelDefs::levelBarrels[][11] = {
	{
	 0b0000000101001001,
	 0b0000000100010001,
	 0b0000000110011011,
	 0b0001000101010101,
	 0b0001011000100111,
	 0b0001000101010101,
	 0b0001100110011111,
	 0b0001010101010101,
	 0b0001001100110000,
	 0b0001000101000000,
	 0b0001010010110000,
	 },
	{
	 0b0000000111111111,		//
	 0b0000000101010101,
	 0b0000000111111111,		//
	 0b0001010101010101,
	 0b0001111111111111,		//
	 0b0001010101010101,
	 0b0001111111111111,		//
	 0b0001010101010101,
	 0b0001111111110000,		//
	 0b0001010101010000,
	 0b0001111111110000,		//
	 },
	{
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000,
	 0b0000000000000000}

};

/*
 *  When making a level, this is a level full of barrels
 *  Copy it and just make some 1's, 0's.
 *
	0b0000000111111111,
	0b0000000101010101,
	0b0000000111111111,
	0b0001010101010101,
	0b0001111111111111,
	0b0001010101010101,
	0b0001111111111111,
	0b0001010101010101,
	0b0001111111110000,
	0b0001010101010000,
	0b0001111111110000,
*/

const uint16_t LevelDefs::YouCantPlaceBarrelsHere[11] = {
	0b1111110000000000,
	0b1111101010101010,
	0b1111000000000000,
	0b1110101010101010,
	0b1110000000000000,
	0b1110101010101010,
	0b1110000000000000,
	0b1110101010101010,
	0b1110000000000001,
	0b1110101010101011,
	0b1110000000000111
};

Level* LevelDefs::getLevel(uint8_t levelId)
{
	Definitions::irComm->println("lvldef:84");
    const uint16_t (*pBarrels)[11] = &LevelDefs::levelBarrels[levelId];
	Definitions::printScreenSize((uint16_t)pBarrels);
    Definitions::irComm->println("lvldef:86");
	Level ret;
	//Level* ret = new Level(pBarrels, LevelDefs::levelNames[levelId]);
	ret = Level();
	Level* pret = &ret;
	Definitions::irComm->println("lvldef:88");
	Definitions::irComm->println((uint32_t) pret, HEX);
	Definitions::irComm->println(sizeof(*pret), HEX);

    Level** rat = &pret;
    Definitions::irComm->println("lvldef:93");
    Definitions::irComm->println((uint32_t) rat, HEX);
    Definitions::irComm->println(sizeof(*rat), HEX);

	return pret;
}
