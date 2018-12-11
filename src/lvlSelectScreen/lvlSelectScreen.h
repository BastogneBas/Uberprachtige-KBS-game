#ifndef UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H
#define UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H

class lvlSelectScreen: public screen
{
public:
lvlSelectScreen();
void begin();
void refresh();
void end();
void checkNunchuck();
void repaint(uint8_t selectedButton);
void startGame(uint8_t selectedButton);

private:

};


#endif //UBERPRACHTIGE_KBS_GAME_LVLSELECTSCREEN_H
