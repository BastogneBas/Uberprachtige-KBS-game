#ifndef UBERPRACHTIGE_KBS_GAME_ENDSCREEN_H
#define UBERPRACHTIGE_KBS_GAME_ENDSCREEN_H

class endScreen: public screen {

public:
	endScreen();
	endScreen(uint16_t scoreP1, uint16_t scoreP2);
	void begin();
	void refresh();
	void end();
	void checkNunchuck();

	uint16_t scoreP1;
	uint16_t scoreP2;

private:
// Defining variables


};
#endif //UBERPRACHTIGE_KBS_GAME_ENDSCREEN_H
