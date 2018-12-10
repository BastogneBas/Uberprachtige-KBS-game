#include "levelDefs.h"

const char *LevelDefs::levelNames[] = {
	"Level 1"
};

const uint16_t LevelDefs::levelBarrels[][11] = {
        {
                0b0000001000000001,
                0b0000000000000001,
                0b0000000000100001,
                0b0000000000000001,
                0b0000000000000001,
                0b0000000000000001,
                0b0000000000000001,
                0b0000000000000001,
                0b0000000000000001,
                0b0000000010000001,
                0b0001000000000000
        }
};

const uint16_t LevelDefs::YouCantPlaceBarrelsHere[11] = {
	0b1110000000000111,
	0b1110101010101011,
	0b1110000000000001,
	0b1110101010101010,
	0b1110000000000000,
	0b1110101010101010,
	0b1110000000000000,
	0b1110101010101010,
	0b1111000000000000,
	0b1111101010101010,
	0b1111110000000000
};

Level LevelDefs::getLevel(uint8_t levelId)
{
	return Level(LevelDefs::levelBarrels[levelId],
				 LevelDefs::levelNames[levelId]);
}
