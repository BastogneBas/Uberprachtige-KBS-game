#include "../Adafruit_GFX_Library/Adafruit_GFX.h"	// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"	// Hardware-specific library
#include <SPI.h>
#include "../../staticDefinitions.cpp"
#include "game.h"
#include "../level/level.h"
#include "../level/levelDefs.h"
#include "../ArduinoNunchuk/ArduinoNunchuk.h"
#include "bomb.h"

#define DEBUG 1

gameScreen::gameScreen()
{
}

gameScreen::gameScreen(Level level)
{
	Serial.println("Ik ben nu bij gamescreen");
	this->level = level;
	Serial.println("Ik ben nu bij gamescreen");
}

int p2Y = 0, p2X = 0;
int p1Y = 0, p1X = 0;
//uint8_t bomb1X, bomb1Y;
//uint8_t bomb2X, bomb2Y;
uint8_t bombX, bombY;
int newX, newY;

void gameScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);
	level.begin();
	//level.printMap();
	//level.drawMap();
	/*uint8_t width = Definitions::gameWidth+1, height = Definitions::gameHeight+1;
	   for (int x = 0; x <= width; x++)
	   drawBlock(x, 0);
	   for (int y = 0; y <= height; y++)
	   drawBlock(0, y);
	   for (int x = 0; x <= width; x++)
	   drawBlock(x, height);
	   for (int y = 0; y <= height; y++)
	   drawBlock(width, y);
	   for (int y = 2; y < height; y += 2)
	   for (int x = 2; x < width; x += 2)
	   drawBlock(x, y);
	   drawPeep1(1, 1);
	   drawPeep2(Definitions::gameWidth, Definitions::gameHeight);

	   //Level level = new Level(level.getBarrels(), "Level1");
	   for (int x = 0; x < Definitions::gameHeight; x++) {
	   for (uint16_t y = 0; y < Definitions::gameWidth; y++) {
	   uint16_t mask = 1<<y;
	   if (this->level.getBarrels()[x] & (mask)) {
	   drawBarrel(y+1, x+1);
	   }
	   }
	   } */
	p2X = Definitions::gameWidth;
	p2Y = Definitions::gameHeight;
	p1X = 1;
	p1Y = 1;
}


void gameScreen::movePeep(int peep, uint16_t dirX, uint16_t dirY)
{
	// make location of peep the same is the direction from Nunchuck
	int nunX = Definitions::nunchuk->analogX;
	int nunY = Definitions::nunchuk->analogY;

	//if (peep == 2) {
		newX = p2X;
		newY = p2Y;
//	}

//	if (peep == 1) {
//		int newX = p1X;
//		int newY = p1Y;
//	}

	//Definitions::tft->fillRect(p2X*16, p2Y*16, 16, 16, ILI9341_BLACK);

	// check if nunchuk is left, if true go to left
	if (nunX <= 90 && (nunY > 100 && nunY < 150))	// && p2X > 1)
	{
		newX--;
	}

	// check if nunchuk is right, if true go to right
	if (nunX >= 150 && (nunY > 100 && nunY < 150))	// && p2X < Definitions::gameWidth)
	{
		newX++;
	}
	// check if nunchuk is up, if true go up
	if ((nunX > 100 && nunX < 150) && nunY >= 150)	// && p2Y > 1)
	{
		// Nunchuck Y is inverted.
		//p2Y--;
		newY--;
	}

	if ((nunX > 100 && nunX < 150) && nunY <= 90)	// && p2Y < Definitions::gameHeight)
	{
		//p2Y++;
		newY++;
	}

	if (newY != p2Y || newX != p2X)
	{

		if (!(level.getObjectAt(newX, newY) & mapObject::block) &&
			!(level.getObjectAt(newX, newY) & mapObject::barrel) &&
			!((level.getObjectAt(newX, newY) & mapObject::bomb) && !(level.getObjectAt(newX, newY) & mapObject::explosion)))
		{
			//if (peep == 2){
			level.unmarkObjectAt(p2X, p2Y, mapObject::peep2);
			level.markObjectAt(p2X, p2Y, mapObject::needsRedraw);
			level.markObjectAt(newX, newY, mapObject::peep2);
			level.markObjectAt(newX, newY, mapObject::needsRedraw);

			p2X = newX;
			p2Y = newY;
//			}

//			if (peep == 1){
//				level.unmarkObjectAt(p1X, p1Y, mapObject::peep1);
//				level.markObjectAt(p1X, p1Y, mapObject::needsRedraw);
//				level.markObjectAt(newX, newY, mapObject::peep1);
//				level.markObjectAt(newX, newY, mapObject::needsRedraw);
//
//				p1X = newX;
//				p1Y = newY;
//			}
		}
	}

	//draw peep on the newest location
	/*if (peep == 1)
	   {
	   drawPeep1(p2X, p2Y);

	   }

	   if (peep == 2)
	   {
	   drawPeep2(p2X, p2Y);
	   } */
	//return dirX, dirY;
}

void gameScreen::placeBomb(uint16_t x, uint16_t y)
{

	//level.setBomb(0, x, y, )
	level.markObjectAt(x, y, mapObject::bomb);
	level.markObjectAt(x, y, mapObject::needsRedraw);

}

void gameScreen::drawExplosion(uint16_t explX, uint16_t explY) {

//    int explX = p2X;
//    int explY = p2Y;

//    int X = p2X;
//    int Y = p2Y;


    for (int x = -2; x <= 2; x++) {
        if (x != 0) {
            Serial.println((!(x < 0) && !(level.getObjectAt(explX + x + 1, explY) & mapObject::block)), BIN);
            Serial.println((!(x > 0) && !(level.getObjectAt(explX + x - 1, explY) & mapObject::block)), BIN);
            if(    ((x < 0) && !((level.getObjectAt(explX + x + 1, explY)) & mapObject::block))
                || ((x > 0) && !((level.getObjectAt(explX + x - 1, explY)) & mapObject::block)))
            {
                if (    ((x < 0) && !((level.getObjectAt(explX + x + 1, explY)) & mapObject::barrel))
                    ||  ((x > 0) && !((level.getObjectAt(explX + x - 1, explY)) & mapObject::barrel)))
                {
                    if (!(level.getObjectAt(explX + x, explY) & mapObject::block)) {
                        level.markObjectAt(explX + x, explY, mapObject::explosion);
                        level.markObjectAt(explX + x, explY, mapObject::explosionH);
                        level.markObjectAt(explX + x, explY, mapObject::needsRedraw);
                        level.unmarkObjectAt(explX + x, explY, mapObject::barrel);
                    }
                }
            }
        } else {
            level.markObjectAt(explX, explY, mapObject::explosion);
            level.markObjectAt(explX, explY, mapObject::needsRedraw);
            level.unmarkObjectAt(explX, explY, mapObject::bomb);
        }
    }


    for (int y = -2; y <= 2; y++) {

        if (y != 0) {
            if (    ((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::block))
                ||  ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::block))) {

                if (    ((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::barrel))
                    ||  ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::barrel))) {

                    if (!(level.getObjectAt(explX, explY + y) & mapObject::block)) {
                        level.markObjectAt(explX, explY + y, mapObject::explosion);
                        level.markObjectAt(explX, explY + y, mapObject::explosionV);
                        level.markObjectAt(explX, explY + y, mapObject::needsRedraw);
                        level.unmarkObjectAt(explX, explY + y, mapObject::barrel);
                    }
                }
            }
        } else {
            level.markObjectAt(explX, explY, mapObject::bomb);
            level.markObjectAt(explX, explY, mapObject::explosion);
            level.markObjectAt(explX, explY, mapObject::needsRedraw);
        }
    }
}

void gameScreen::drawAir(uint16_t explX, uint16_t explY)
{
    for (int x = -2; x <= 2; x++)
    {
        level.unmarkObjectAt(explX + x, explY, mapObject::explosion);
        level.unmarkObjectAt(explX + x, explY, mapObject::bomb);

        if (!(level.getObjectAt (explX + x, explY) & mapObject::block)){
            level.markObjectAt (explX + x, explY, mapObject::air);
            level.markObjectAt (explX + x, explY, mapObject::needsRedraw);
        }
    }

    for (int y = -2; y <= 2; y++)
    {
        level.unmarkObjectAt(explX , explY + y, mapObject::explosion);
        level.unmarkObjectAt(explX, explY + y, mapObject::bomb);

        if (!(level.getObjectAt (explX, explY + y) & mapObject::block)){
            level.markObjectAt (explX, explY + y, mapObject::air);
            level.markObjectAt (explX, explY + y, mapObject::needsRedraw);
        }
    }
}

void gameScreen::end()
{
}

uint8_t RefreshCnt = 0;
//bool placed;
void gameScreen::refresh()
{
	RefreshCnt++;
	uint8_t getPlacedTime;

	//if ((RefreshCnt % 5) == 0)
	{
		Serial.println("Refresh");
		Definitions::nunchuk->update();
		if (Definitions::nunchuk->zButton && level.getBombX(0) == 0 && level.getBombTime(0) == 0 && level.getBombY(0) == 0 && level.getBombPeep(0) == 0) {

			level.setBomb(0, p2X, p2Y, RefreshCnt, 2);
			//level.setBomb(0, p2X, p2Y, RefreshCnt, 2);
			//level.setBomb(1, p1X, p1Y, RefreshCnt, 1);

			bombX = level.getBombX(0);
			bombY = level.getBombY(0);

			placeBomb(bombX, bombY);
			//getPlacedTime = level.getBombTime(0);
			//Serial.println(getPlacedTime, DEC);
			//placed = true;
		}

		if (RefreshCnt == level.getBombTime(0) + 12)
        {
		    if ((level.getObjectAt(bombX, bombY) & mapObject::bomb) && !(level.getObjectAt(bombX, bombY) & mapObject::explosion))
		    {
                drawExplosion(bombX, bombY);
                //getExTime = RefreshCnt;
            }
		}

		if (RefreshCnt == level.getBombTime(0) + 24)
		{
			if ((level.getObjectAt(bombX, bombY) & mapObject::bomb) && (level.getObjectAt(bombX, bombY) & mapObject::explosion)) {
			    drawAir(bombX, bombY);
			    level.setBomb(0, 0, 0, 0, 0);
			   // placed = false;
			}
		}

		movePeep(2, p2X, p2Y);
		//movePeep(1, p1X, p1Y);



#ifdef DEBUG
		level.printMap();
		Serial.println();
#endif
		level.drawMap();
	}
}

void gameScreen::drawPeep1(uint16_t x, uint16_t y)
{
	//Definitions::bmpDraw("PEEP1.BMP", x * 16, y * 16);
	level.drawMap();
}

void gameScreen::drawPeep2(uint16_t x, uint16_t y)
{
	//Definitions::bmpDraw("PEEP2.bmp", x * 16, y * 16);
	level.drawMap();
}

void gameScreen::drawBarrel(uint16_t x, uint16_t y)
{
	//Definitions::bmpDraw("BARREL.BMP", x * 16, y * 16);
	level.drawMap();
}

void gameScreen::drawBlock(uint16_t x, uint16_t y)
{
	//Definitions::bmpDraw("BLOCK.BMP", x * 16, y * 16);
	level.drawMap();
}

void gameScreen::drawBomb(uint16_t x, uint16_t y)
{
	if (Definitions::nunchuk->zButton)
	{
		//Definitions::bmpDraw("bomb.bmp", x * 16, y * 16);
		level.drawMap();
	}
}

void gameScreen::checkNunchuck()
{
}
