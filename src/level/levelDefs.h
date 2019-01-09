#include "level.h"
#include "Arduino.h"

#ifndef LEVELDEFS_H
#define LEVELDEFS_H

class LevelDefs
{
  public:
	/* Return a predefined level.
	 * levelId = 0 --> Level 1
	 * levelId = 1 --> Level 2
	 * Etc.
	 */
	static Level *getLevel(uint8_t levelId);

	// A mask for the locations where no barrels can be placed
	static const uint16_t YouCantPlaceBarrelsHere[11];
	/* The static barrel locations we will be using.
	 * For explanation see: level.h
	 */
	static const uint16_t levelBarrels[][11];

	// The static level names that will be used.
	static const char *levelNames[];
  private:
};
#endif
