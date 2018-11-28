#include "levelDefs.h"

const char* LevelDefs::levelNames[] = {
	"Level 1"};

const uint16_t LevelDefs::levelBarrels[][11]=
{
	{
				0b0001001000100011,
				0b0000000000000000,
				0b0000000000100000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0001000000000001
	}
};

Level LevelDefs::getLevel(uint8_t levelId){
	return Level(LevelDefs::levelBarrels[levelId], LevelDefs::levelNames[levelId]);
}
