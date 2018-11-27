#include "level.h"

class LevelDefs{
	public:
		/* The static levels we will be using.
		 * For explanation see: level.h
		 */
		static const Level[] = {
			Level({
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
					}, "Level 1")
		};
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
}
