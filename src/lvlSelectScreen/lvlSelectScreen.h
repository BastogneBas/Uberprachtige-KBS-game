#ifndef UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H
#define UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H

class lvlSelectScreen:public screen
{
  public:
	lvlSelectScreen();
	void begin();
	void refresh();
	void end();
	void checkNunchuck();
	void repaint(uint8_t selectedButton);
	void startGame();

	// Defining variable that holds the value of the current button being selected
	uint8_t selectedButton = 0;

	// Defining variables to fill the screen with
	uint8_t yRect = 29, yFill = 30, cursor = 40;

  private:

};


#endif
