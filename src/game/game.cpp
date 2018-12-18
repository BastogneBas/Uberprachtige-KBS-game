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
	this->level = level;
}

int p2Y = 0, p2X = 0;
int p1Y = 0, p1X = 0;
//uint8_t bomb1X, bomb1Y;
//uint8_t bomb2X, bomb2Y;
//uint8_t bombX, bombY;
int newX, newY;
uint8_t lives1, lives2;

void gameScreen::begin()
{
    lives1 == 3;
    lives2 == 3;
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

	// if player is 1
	if (peep == 1)
	{
	    // newX is equal to peep location X
        newX = p1X;

        // newY is equal to peep location Y
        newY = p1Y;
    }

    // if player is 2
	if (peep == 2) {

        // newX is equal to peep location X
		newX = p2X;

        // newY is equal to peep location Y
		newY = p2Y;
	}

	//Definitions::tft->fillRect(p2X*16, p2Y*16, 16, 16, ILI9341_BLACK);

	// check if nunchuk is left, if true go to left
	if (nunX <= 90)	// && p2X > 1)
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

	// check which peep and new location is not equal to peep location
	if ((peep == 1 && (newY != p1Y || newX != p1X)) || (peep == 2 && (newY != p2Y || newX != p2X)))
	{
        // if true check if there is no block and no barrel and no bomb
		if (!(level.getObjectAt(newX, newY) & mapObject::block) &&
			!(level.getObjectAt(newX, newY) & mapObject::barrel) &&
			!((level.getObjectAt(newX, newY) & mapObject::bomb) && !(level.getObjectAt(newX, newY) & mapObject::explosion)))
		{
		    // if player is 2
			if (peep == 2){
			    // player 2 is moving to new location
			level.unmarkObjectAt(p2X, p2Y, mapObject::peep2);
			level.markObjectAt(p2X, p2Y, mapObject::needsRedraw);
			level.markObjectAt(newX, newY, mapObject::peep2);
			level.markObjectAt(newX, newY, mapObject::needsRedraw);

			p2X = newX;
			p2Y = newY;
			}

			// if player is 1
			if (peep == 1){
			    // player 1 is moving to new location
				level.unmarkObjectAt(p1X, p1Y, mapObject::peep1);
				level.markObjectAt(p1X, p1Y, mapObject::needsRedraw);
				level.markObjectAt(newX, newY, mapObject::peep1);
				level.markObjectAt(newX, newY, mapObject::needsRedraw);

				p1X = newX;
				p1Y = newY;
			}
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

void gameScreen::placeBomb(int peep, uint16_t x, uint16_t y)
{

	//level.setBomb(0, x, y, )
	// check which peep places the bomb
	if (peep == 1) {
        level.markObjectAt(x, y, mapObject::bombPeep1);
    }
    if (peep == 2) {
        level.markObjectAt(x, y, mapObject::bombPeep2);
    }
    // now there is a bom placed
	level.markObjectAt(x, y, mapObject::bomb);
	level.markObjectAt(x, y, mapObject::needsRedraw);


}

void gameScreen::drawExplosion(int peep, uint16_t explX, uint16_t explY) {

//    int explX = p2X;
//    int explY = p2Y;

//    int X = p2X;
//    int Y = p2Y;


    for (int x = -2; x <= 2; x++) {
        // if x is not equal to 0
        if (x != 0) {
#ifdef DEBUG
			Definitions::setTextDebug();
			Definitions::print("x: ");
			Definitions::print(x);
			Definitions::print("\t");
			Definitions::print( ((x < 0) && !(level.getObjectAt(explX + x + 1, explY) & mapObject::block)) , BIN);
			Definitions::print("\t");
           	Definitions::println( ((x > 0) && !(level.getObjectAt(explX + x - 1, explY) & mapObject::block)), BIN);
#endif
            // if place before explosion is no block
            if(    ((x < 0) && !(level.getObjectAt(explX + x + 1, explY) & mapObject::block))
                || ((x > 0) && !(level.getObjectAt(explX + x - 1, explY) & mapObject::block)))
            {
                // and place before is no barrel
                if (    ((x < 0) && !((level.getObjectAt(explX + x + 1, explY)) & mapObject::barrel))
                    ||  ((x > 0) && !((level.getObjectAt(explX + x - 1, explY)) & mapObject::barrel)))
                {
                    // and the exact place is no block
                    if (!(level.getObjectAt(explX + x, explY) & mapObject::block)) {
                        // if player is 1
                        if (peep == 1){
                            // it is an explosion of peep1 bomb
                            level.markObjectAt(explX + x, explY, mapObject::bombPeep1);
                        }

                        // if player is 2
                        if (peep == 2)
                        {
                            // it is an explosion of peep2 bomb
                            level.markObjectAt(explX + x, explY, mapObject::bombPeep2);
                        }

                        // the location is now an explosion horizontal and location is no barrel anymore
                        level.markObjectAt(explX + x, explY, mapObject::explosion);
                        level.markObjectAt(explX + x, explY, mapObject::explosionH);
                        level.markObjectAt(explX + x, explY, mapObject::needsRedraw);
                        level.unmarkObjectAt(explX + x, explY, mapObject::barrel);
                    }
                }
            }
            // when x is equal to 0
        } else {
            // if player is 1
            if (peep == 1)
            {
                // it is a bomb of player 1
                level.markObjectAt(explX + x, explY, mapObject::bombPeep1);
            }

            // if player is 2
            if (peep == 2)
            {
                // it is a bomb of player 2
                level. markObjectAt(explX + x, explY, mapObject::bombPeep2);
            }
            // it is an explosion center and on the location is not a bomb anymore --> bom has exploded!
            level.markObjectAt(explX, explY, mapObject::explosion);
            level.markObjectAt(explX, explY, mapObject::needsRedraw);
            level.unmarkObjectAt(explX, explY, mapObject::bomb);
        }
    }


    for (int y = -2; y <= 2; y++) {
    // if y is not equal to 0
        if (y != 0) {
            // check if place before is no block
            if (    ((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::block))
                ||  ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::block))) {

                // check if place before is no barrel
                if (    ((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::barrel))
                    ||  ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::barrel))) {

                    // check if the exact location is no block
                    if (!(level.getObjectAt(explX, explY + y) & mapObject::block)) {

                        // if player is 1
                        if (peep == 1)
                        {
                            // explosion is of player 1 bomb
                            level.markObjectAt(explX, explY + y, mapObject::bombPeep1);
                        }

                        // if player is 2
                        if (peep == 2)
                        {
                            // explosion is of bomb peep 2
                            level.markObjectAt(explX, explY + y, mapObject::bombPeep2);
                        }
                        // all the location are a explosion and no barrel anymore
                        level.markObjectAt(explX, explY + y, mapObject::explosion);
                        level.markObjectAt(explX, explY + y, mapObject::explosionV);
                        level.markObjectAt(explX, explY + y, mapObject::needsRedraw);
                        level.unmarkObjectAt(explX, explY + y, mapObject::barrel);
                    }
                }
            }
            // x is equal to 0
        } else {

            // if player is 1
            if (peep == 1)
            {
                // explosion is of player 1 bomb
                level.markObjectAt(explX, explY + y, mapObject::bombPeep1);
            }

            // if player is 2
            if (peep == 2)
            {
                // explosion is of bomb player 2
                level.markObjectAt(explX, explY + y, mapObject::bombPeep2);
            }
            // all the locations are an explosion and a bomb --> center of explosion
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
        // remove the explosion
        level.unmarkObjectAt(explX + x, explY, mapObject::explosion);
		level.unmarkObjectAt(explX + x, explY, mapObject::explosionH);
        level.unmarkObjectAt(explX + x, explY, mapObject::bomb);
        level.unmarkObjectAt(explX + x, explY, mapObject::bombPeep2);
        level.unmarkObjectAt(explX + x, explY, mapObject::bombPeep1);

        // if there is no block
        if (!(level.getObjectAt (explX + x, explY) & mapObject::block)){

            // drawair instead
            level.markObjectAt (explX + x, explY, mapObject::air);
            level.markObjectAt (explX + x, explY, mapObject::needsRedraw);
        }
    }

    for (int y = -2; y <= 2; y++)
    {
        // remove explosion
        level.unmarkObjectAt(explX , explY + y, mapObject::explosion);
		level.unmarkObjectAt(explX, explY + y, mapObject::explosionV);
        level.unmarkObjectAt(explX, explY + y, mapObject::bomb);
        level.unmarkObjectAt(explX, explY + y, mapObject::bombPeep2);
        level.unmarkObjectAt(explX, explY + y, mapObject::bombPeep1);

        // when it is no block
        if (!(level.getObjectAt (explX, explY + y) & mapObject::block)){
            // location change into air
            level.markObjectAt (explX, explY + y, mapObject::air);
            level.markObjectAt (explX, explY + y, mapObject::needsRedraw);
        }
    }
}

void gameScreen::end()
{
}

uint32_t RefreshCnt = 0;
//bool placed;
void gameScreen::refresh() {

    RefreshCnt++;
    uint8_t getPlacedTime;

    //if ((RefreshCnt % 5) == 0)
    {
#ifdef DEBUG
		Definitions::setTextDebug();
		Definitions::println("Refresh");
#endif
        Definitions::nunchuk->update();
        if (Definitions::nunchuk->zButton && level.getBombX(0) == 0 && level.getBombTime(0) == 0 &&
            level.getBombY(0) == 0 && level.getBombPeep(0) == 0) {

            level.setBomb(1, p2X, p2Y, RefreshCnt, 2);
            level.setBomb(0, p1X, p1Y, RefreshCnt, 1);

            placeBomb(1, level.getBombX(0), level.getBombY(0));
            placeBomb(2, level.getBombX(1), level.getBombY(1));

            //getPlacedTime = level.getBombTime(0);
            //Serial.println(getPlacedTime, DEC);
            //placed = true;
        }

        if (RefreshCnt >= level.getBombTime(0) + 12) {
            if (((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb) &&
                 !(level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion))
                || ((level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb) &&
                    !(level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion))) {
                drawExplosion(1, level.getBombX(0), level.getBombY(0));
                drawExplosion(2, level.getBombX(1), level.getBombY(1));

                Definitions::print("BombTime: ");
                Definitions::print(level.getBombTime(0) + 12);
                Definitions::println("  ");
                Definitions::print("ExplTime: ");
                Definitions::print(level.getBombTime(0) + 24);
                Definitions::println("      ");

            }
        }

//        if (RefreshCnt == level.getBombTime(0) + 12) {
//            if (((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb) &&
//                 !(level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion))
//                || ((level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb) &&
//                    (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion))) {
//                drawExplosion(level.getBombX(0), level.getBombY(0));
//                drawExplosion(level.getBombX(1), level.getBombY(1));
//                //getExTime = RefreshCnt;
//            }
//        }


        if (RefreshCnt >= level.getBombTime(0) + 24) {
            if ((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb) &&
                (level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion)
                || (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb) &&
                   (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion)) {
                drawAir(level.getBombX(0), level.getBombY(0));
                drawAir(level.getBombX(1), level.getBombY(1));
                level.setBomb(0, 0, 0, 0, 0);
                level.setBomb(1, 0, 0, 0, 0);

            }
        }
    }

    if (Definitions::nunchuk->cButton)
    {
        movePeep(2, p2X, p2Y);
    } else {
        movePeep(1, p1X, p1Y);
    }
    
#ifdef DEBUG
        //level.printMap();
		//Definitions::println();
#endif
	Definitions::setTextDebug();
	Definitions::print("frame: ");
	Definitions::println(RefreshCnt);
	level.drawMap();
}
