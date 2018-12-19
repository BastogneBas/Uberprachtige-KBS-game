
#ifndef UBERPRACHTIGE_KBS_GAME_HIGHSCORESCREEN_H
#define UBERPRACHTIGE_KBS_GAME_HIGHSCORESCREEN_H

class highScoreScreen:public screen
{
  public:
	highScoreScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();

	// Defining value for printing the numbers / highscores
	uint8_t cursorValue = 78;

	// Defining variables fot holding the score temporarily
	uint16_t scorep1;
	uint16_t scorep2;

	// Defining arrays for the highScores
	uint16_t highScoresP1[5] = { 1065, 5848, 854, 12254, 5651 };
	uint16_t highScoresP2[5] = { 2654, 8795, 12546, 4846, 8515 };


  private:
	// Defining variables

};

#endif
