#include "level.h"
#include "Arduino.h"

#ifndef LEVELDEFS_H
#define LEVELDEFS_H

class LevelDefs{
	public:
		//	"Level 1"
		//};

		static Level getLevel(uint8_t levelId);//{
		//	return Level(levelBarrels[levelId], levelNames[levelId]);
		//}
		//static Level level1 = Level(level1_barrels, (const char*)"Level 1");

		/* The static levels we will be using.
		 * For explanation see: level.h
		 */
		//Level Levels[] = {level1};

		// A mask for the locations where no barrels can be placed
		static const uint16_t YouCantPlaceBarrelsHere[11]={
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
		//Level getLevel(uint8_t levelId){
		//	return Level(levelBarrels[levelId], levelNames[levelId]);
		//}
		static const uint16_t levelBarrels[][11];/* =
		{
			{
				0b0000001010101010,
				0b0000000000000000,
				0b0000001010101010,
				0b0000000000000000,
				0b0000001010101010,
				0b0000000000000000,
				0b0000001010101010,
				0b0000000000000000,
				0b0000001010101010,
				0b0000000000000000,
				0b0000001010101010
			}
		};*/
	private:
		static const char* levelNames[];
};
#endif
