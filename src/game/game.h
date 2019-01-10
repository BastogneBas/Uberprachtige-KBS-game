#include "../../screen.h"
#include <stddef.h>
#include "../level/level.h"

#ifndef SPELSCHERM_H
#define SPELSCHERM_H

// Defining all the datatypes
#define START_BYTE 		0x01
#define LEVEL_BYTE 		0x02
#define LIVES_BYTE 		0x03
#define SCORE_BYTE 		0x04
#define PEEP_LOCATION	0x05
#define BOMB_LOCATION 	0x06
#define STOP_BYTE 		0x07
#define TIME_BYTE		0x08
#define SEND_AGAIN 		0xFF

class gameScreen:public screen // Defines the gameScreen class
{
  public:
	// Define Constructors
	gameScreen();
	gameScreen(Level *level);
	gameScreen(uint8_t levelnr);
	gameScreen(String levelname, uint16_t seed = 0);
	// Define functions
	void begin();				// Start the game
	void end();					// End the game
	void refresh();				// Refresh the screen
	void drawEndScreen();		// Show the endscreen
	void drawLives();			// Draw the lives onscreen
	void drawTimer();			// Draw the timer value
	void drawScore();			// Draw the score values
	void checkWinner();			// Check who won the game
	void calculateEndScores();	// Calculate the bonus scores

  private:
	// Define functions
	void movePeep(int peep);										// Set which player to be moved
	void movePeep(int peep, int, int);								// Move the player with given positions
	void placeBomb(int peep, uint16_t x, uint16_t y);				// Place a bomb
	void drawExplosion(int peep, uint16_t explX, uint16_t explY);	// Draw the explosion from the bombs
	void drawAir(uint16_t explX, uint16_t explY);					// Draw air after explosions
	// Define Variables
	Level level;
	uint8_t livesP1 = 3;		// Player 1 & 2's lives
	uint8_t livesP2 = 3;
	int16_t scoreP1 = 4000;		// Player 1 & 2's scores
	int16_t scoreP2 = 4000;
	uint8_t timeCounter = 0;	// Time counter
	uint8_t deadPlayer = 0;		// Indicate if a player has died
	uint8_t winner = 0;			// Indicate the winner
};
#endif
