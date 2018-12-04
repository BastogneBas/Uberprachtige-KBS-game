#include "../Adafruit_GFX_Library/Adafruit_GFX.h"		// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"	// Hardware-specific library
#include <SPI.h>
#include "../../staticDefinitions.cpp"
#include "game.h"
#include "../level/level.h"
#include "../level/levelDefs.h"
#include "../ArduinoNunchuk/ArduinoNunchuk.h"



gameScreen::gameScreen()
{
}

gameScreen::gameScreen(Level level)
 {
	this->level = level;
 }

int p2Y=0,p2X=0;

void gameScreen::begin()
{
	Definitions::tft->fillScreen(ILI9341_BLACK);
	uint8_t width = Definitions::gameWidth+1, height = Definitions::gameHeight+1;
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
    }
	p2X = Definitions::gameWidth;
	p2Y = Definitions::gameHeight;
}


void gameScreen::movePeep(int peep, uint16_t dirX, uint16_t dirY) {
	// make location of peep the same is the direction from Nunchuck
	int nunX = Definitions::nunchuk->analogX;
	int nunY = Definitions::nunchuk->analogY;

	int newX = p2X;
	int newY = p2Y;

	Definitions::tft->fillRect(p2X*16, p2Y*16, 16, 16, ILI9341_BLACK);

	// check if nunchuk is left, if true go to left
	if (nunX <= 90 && (nunY > 100 && nunY < 150) && p2X > 1)
	{
		newX--;
	}

	// check if nunchuk is right, if true go to right
	if (nunX >= 150 && (nunY > 100 && nunY < 150) && p2X < Definitions::gameWidth)
	{
		newX++;
	}
	// check if nunchuk is up, if true go up
	if ((nunX > 100 && nunX < 150) && nunY >= 150 && p2Y > 1)
	{
		// Nunchuck Y is inverted.
		//p2Y--;
		newY--;
	}

	if ((nunX > 100 && nunX < 150) && nunY <= 90 && p2Y < Definitions::gameHeight)
	{
		//p2Y++;
		newY++;
	}

//	if (!(level.getObjectAt(newX, newY)& mapObject::block)  &&
//		!(level.getObjectAt(newX, newY)& mapObject::barrel) &&
//		!(level.getObjectAt(newX, newY)& mapObject::bomb))
//	{
		p2X = newX;
		p2Y = newY;
		Serial.println(level.getObjectAt(newX, newY), BIN);
		Serial.println(newX);
		Serial.println(newY);
//	}

	//draw peep on the newest location
	if(peep == 1)
	{
		drawPeep1(p2X, p2Y);
	}

	if(peep == 2)
    {
	    drawPeep2(p2X, p2Y);
    }
    //return dirX, dirY;
}

void gameScreen::end()
{
}

uint8_t RefreshCnt =0;

void gameScreen::refresh()
{
	RefreshCnt++;
	Definitions::nunchuk->update();
	if((RefreshCnt%15)==0){
		movePeep(2, Definitions::gameWidth, Definitions::gameHeight);
	}
}

void gameScreen::drawPeep1(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("PEEP1.BMP", x * 16, y * 16);
}

void gameScreen::drawPeep2(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("PEEP2.bmp", x * 16, y * 16);
}

void gameScreen::drawBarrel(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("BARREL.BMP", x * 16, y * 16);
}

void gameScreen::drawBlock(uint16_t x, uint16_t y)
{
	Definitions::bmpDraw("BLOCK.BMP", x * 16, y * 16);
}

void gameScreen::drawBomb(uint16_t x, uint16_t y)
{
	if (Definitions::nunchuk->zButton)
	{
		Definitions::bmpDraw("bomb.bmp", x * 16, y * 16);
	}
}

void gameScreen::checkNunchuck()
{
}
