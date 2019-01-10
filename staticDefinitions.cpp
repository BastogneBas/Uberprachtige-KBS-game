#include "src/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include "src/ArduinoNunchuk/ArduinoNunchuk.h"
#include <stddef.h>
#include <string.h>
#include "sprites.h"
#include "screen.h"
#include "src/IRComm/IRComm.h"

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* Defines based on if infrared comms are used
 * Uncomment IR line to enable IR comms
 * Uncomment TFT line to enable the tft display
 * Uncomment RECEIVER line to set current arduino as a receiver
 * Uncomment SENDER line to set current arduino as a sender
 * Note: For the final project, enable IR and TFT.
 * SENDER and RECEIVER are given at compilation time. SEE: README.md */
#define IR 1
#define TFT 1
//#define RECEIVER 1
//#define SENDER 1

/* Defines based on which of the two players the current arduino is
 * If PEEP is defined as 1, the current arduino will be player 1
 * If PEEP is defined as 2, the current arudino will be player 2
 * The frequecny of IR comms will be dependant on which player you are */

// First two are used for compilation with -DP1 or -DP2
#ifdef P1
	#pragma message "Compiling for PEEP 1"
	#define PEEP 1
#else
	#ifdef P2
		#pragma message "Compiling for PEEP 2"
		#define PEEP 2
	#else
		// Define own peep
		#pragma message "Compiling for source specified PEEP"
		#define PEEP 1
	#endif
#endif
//#define PEEP = 2
#if PEEP == 1
#define PWMFREQ 38
#elif PEEP == 2
#define PWMFREQ 56
#endif

/// Uncomment this to enable debug code
//#define DEBUG

class Definitions // Define the constants
{
  public:
	/* Define which PINs the TFT display is on
	 * DC defines the Data/Control PIN
	 * CS defines the Chip Select PIN */
	static const int TFT_DC = 9;
	static const int TFT_CS = 10;

	// Define constant dimensions for the game and sprites
	static const uint8_t gameHeight = 11;
	static const uint8_t gameWidth = 13;
	static const uint8_t SpriteHeight = 16;
	static const uint8_t SpriteWidth = 16;

// Define which player the current arduino has
#if PEEP == 1
	static const uint8_t currentPlayer = 1;
#elif PEEP == 2
	static const uint8_t currentPlayer = 2;
#endif
	// Defining some static definitions, so they can be called from all the classes.
	static Adafruit_ILI9341 *tft;
	static ArduinoNunchuk *nunchuk;
	static screen *currentScreen;
	static Stream *irComm;

	static uint8_t timerCounter;
	static uint8_t currentTime;

	/* Set the text position at 0,0 and color at white on black
	 * FOR DEBUGGING */
	static void setTextDebug()
	{
		tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		tft->setCursor(0, 0);
		tft->setTextSize(1);
	}
};

#endif
