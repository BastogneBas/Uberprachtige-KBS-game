#include "../Adafruit_GFX_Library/Adafruit_GFX.h"	// Core graphics library
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"	// Hardware-specific library
#include "../ArduinoNunchuk/ArduinoNunchuk.h"
#include <SPI.h>
#include "../../staticDefinitions.cpp"
#include "../level/level.h"
#include "../level/levelDefs.h"
#include "game.h"
#include "bomb.h"

// Uncomment this to enable some debug code
//#define DEBUG 1

// TODO: Explain to me what this does
gameScreen::gameScreen()
{
}

// Initialize gameScreen class
gameScreen::gameScreen(Level *level)
{
    this->level = *level;
}

// Define players' positions
int p2Y = 0, p2X = 0;
int p1Y = 0, p1X = 0;
// TODO: Decide if this commented-out piece of code can be removed
/*//uint8_t bomb1X, bomb1Y;
//uint8_t bomb2X, bomb2Y;
//uint8_t bombX, bombY;*/
int newX, newY;
uint8_t lives1, lives2;

void gameScreen::begin() // Initializes the game's screen
{
	// Set background to black and draw the initial level
	Definitions::tft->fillScreen(ILI9341_BLACK);
	level.begin();

	// TODO: Decide if this commented-out piece of code can be removed
    /*level.printMap();
	level.drawMap();
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
	} */

	// Set x and y positions for both players
	p2X = Definitions::gameWidth;
	p2Y = Definitions::gameHeight;
	p1X = 1;
	p1Y = 1;

	// Draw some texts to initialize
	Definitions::tft->setTextSize(1);
	Definitions::tft->setTextColor(ILI9341_WHITE);

	// Draw texts for lives
	Definitions::tft->setCursor(250, 5);
	Definitions::tft->print("P1:");

	Definitions::tft->setCursor(250, 50);
	Definitions::tft->print("P2:");

    Definitions::tft->setCursor(250, 100);
    Definitions::tft->print("Tijd:");

    // Draw texts for scores
    Definitions::tft->setCursor(250, 110);
    Definitions::tft->print("Scores:");

    Definitions::tft->setCursor(250, 120);
    Definitions::tft->print("P1:");

    Definitions::tft->setCursor(250, 130);
    Definitions::tft->print("P2:");

	for (int i = 255; i <= 305; i += 25) // Draw the initial lives values
	{
		Definitions::tft->fillCircle(i, 30, 10, ILI9341_RED);
		Definitions::tft->fillCircle(i, 75, 10, ILI9341_RED);
	}

	/* Set the timer to begin value to count down from
	 * NOTE: is currently done because of a bug where it isn't properly set from the header sometimes
	 * Can be removed if the bug is fixed and this seems unnecessary */
	//currentTime = 180;

	// Draw initial timer and score values
	drawTimer();
	drawScore();
}

void gameScreen::end() // End the match by calculating some scores and showing the endScreen
{
    Definitions::irComm->println("P1:");
    Definitions::irComm->println(livesP1);
    Definitions::irComm->println(scoreP1);
    Definitions::irComm->println("P2: ");
    Definitions::irComm->println(livesP2);
    Definitions::irComm->println(scoreP2);
    Definitions::irComm->print("t: ");
    Definitions::irComm->println(currentTime);
    Definitions::irComm->print("dp: ");
    Definitions::irComm->println(deadPlayer);
    // Check who won the game
    checkWinner();
    // Add some extra's to the scores
    calculateEndScores();
    // Print the endScreen, showing if you won or lost and the scores
    gameScreen::drawEndScreen();

    // TODO: Find out what the heck this even is
    while (true)
    {
        if (Definitions::nunchuk->cButton)
        {
            // Jumping to the first line of the program to reset all the processes
            asm volatile ("  jmp 0");
        }
		else
		{
			PRR = PRR;
		}
    }
}

void gameScreen::checkWinner() // Check who won the game
{
    // If the game is over and the timer hasn't reached zero yet...
    // ...a player has died, so the player who's dead has lost the game
    if(!currentTime == 0)
    {
        if(deadPlayer == 1)
            winner = 2;
        else if(deadPlayer == 2)
            winner = 1;
        else
            winner = 0;
    }
    else // If the timer has reached 0 though, the winner will be based on score
    {
        if(scoreP1 > scoreP2)
            winner = 1;
        else if(scoreP1 < scoreP2)
            winner = 2;
        else if(scoreP1 == scoreP2)
            winner = 0;
    }
}

void gameScreen::calculateEndScores() // Adds or removes a bonus to the players' score based on if they won or lost
{
    if(winner == 0) // If the game is tied, give both players a bonus
    {
        scoreP1 += ((currentTime * 10) + (livesP1 * 100));
        scoreP2 += ((currentTime * 10) + (livesP2 * 100));
    }
    else if(winner == 1) // If player 1 won, give them a bonus
    {
        scoreP1 += ((currentTime * 10) + (livesP1 * 100));
        scoreP2 -= ((currentTime * 10) - (livesP2 * 100));
    }
    else if(winner == 2) // If player 2 won, give them a bonus
    {
        scoreP1 -= ((currentTime * 10) - (livesP1 * 100));
        scoreP2 += ((currentTime * 10) + (livesP2 * 100));
    }
}

void gameScreen::drawEndScreen() // Draws the screen showing the results of the match
{
    // Set title values
    Definitions::tft->setCursor(10, 10);
    Definitions::tft->setTextSize(3);
    Definitions::tft->setTextColor(ILI9341_BLACK);

    /* Show the title and some text
     * The winner gets congratulated and gets shown a green background :)
     * The loser gets a sad face and a red background :(
     * When the match is tied, both players get a neutral message with an orange background :| */
    if(winner == 0) // If the match is tied
    {
        Definitions::tft->fillScreen(ILI9341_ORANGE);
        Definitions::tft->setCursor(10, 10);
        Definitions::tft->println("Gelijkspel");

    }
    // If the current player won
    else if((winner == 1 && Definitions::currentPlayer == 1) ||
            (winner == 2 && Definitions::currentPlayer == 2))
    {
        Definitions::tft->fillScreen(ILI9341_GREEN);
        Definitions::tft->println("Gewonnen :)");

    }
    // If the current player lost
    else if((winner == 1 && Definitions::currentPlayer == 2) ||
            (winner == 2 && Definitions::currentPlayer == 1))
    {
        Definitions::tft->fillScreen(ILI9341_RED);
        Definitions::tft->println("Verloren :(");

    }

    // Show the scores...
    // Current Player
    Definitions::tft->setCursor(10, 50);
    Definitions::tft->setTextSize(2);
    Definitions::tft->println("Uw score:");

    // Opponent
    Definitions::tft->setCursor(10, 70);
    Definitions::tft->println("Tegenstander:");

    /* Makes sure the proper scores are shown for the proper player
     * Shows current player's score at current player
     * Shows opponent's score at opponent */
    if(Definitions::currentPlayer == 1)
    {
        Definitions::tft->setCursor(180, 50);
        Definitions::tft->setTextSize(2);
        Definitions::tft->print(scoreP1);

        Definitions::tft->setCursor(180, 70);
        Definitions::tft->print(scoreP2);
    }
    else
    {
        Definitions::tft->setCursor(150, 50);
        Definitions::tft->setTextSize(2);
        Definitions::tft->print(scoreP2);

        Definitions::tft->setCursor(150, 70);
        Definitions::tft->print(scoreP1);
    }

    // Prints end time
    Definitions::tft->setCursor(10, 90);
    Definitions::tft->println("Eindtijd:");
    Definitions::tft->setCursor(180, 90);
    Definitions::tft->print(currentTime);
}

uint32_t *RefreshCnt = 0;
//bool placed;
void gameScreen::refresh() // Handles refreshing the screen and updating some variables
{
    // Increment refresh counter
    *RefreshCnt++;

	if(Definitions::irComm->available)
	{
		
	}

    if((*RefreshCnt % 3) == 0) // If the refresh counter is divisible by three... (run every three refreshes)
    {
    #ifdef DEBUG
            // Shown only when debugging is enabled
            Definitions::setTextDebug();
            Definitions::println("Refresh");
    #endif
        // Get values from nunchuk controller
        Definitions::nunchuk->update();

        // If the Z button is pressed and there's no bomb on the current x and y position...
        // ...and a bomb is not currently active and [WHATEVER getBombPeep DOES]...
        if(Definitions::nunchuk->zButton && level.getBombX(0) == 0
            && level.getBombTime(0) == 0 && level.getBombY(0) == 0
            && level.getBombPeep(0) == 0)
        {
            // TODO: fix a possible bug in this code where both players place a bomb when only p1 should place one
            if(Definitions::nunchuk->cButton
                && Definitions::nunchuk->zButton) // If both C and Z are pressed...
            {
                // Place a bomb on player 2's position
                level.setBomb(1, p2X, p2Y, *RefreshCnt, 2);
                placeBomb(2, level.getBombX(1), level.getBombY(1));
            }
            if(Definitions::nunchuk->zButton) // If only the Z button is pressed...
            {
                // Place a bomb on player 1's position
                level.setBomb(0, p1X, p1Y, *RefreshCnt, 1);
                placeBomb(1, level.getBombX(0), level.getBombY(0));
            }

            // TODO: Decide if this commented-out piece of code can be removed
            /* //getPlacedTime = level.getBombTime(0);
            //Serial.println(getPlacedTime, DEC);
            //placed = true; */
        }

		// TODO: Explain this statement
        if(*RefreshCnt >= level.getBombTime(0) + 48)
        {
            if(((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb)
                && !(level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion))
                ||((level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb)
                && !(level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion)))
            {
                drawExplosion(1, level.getBombX(0), level.getBombY(0));
                drawExplosion(2, level.getBombX(1), level.getBombY(1));

            #ifdef DEBUG
                // Print some debug information on the screen
                Definitions::print("BombTime: ");
				Definitions::print(level.getBombTime(0) + 12);
				Definitions::println("  ");
				Definitions::print("ExplTime: ");
				Definitions::print(level.getBombTime(0) + 24);
				Definitions::println("      ");
            #endif
            }
        }

// TODO: Decide if this commented-out piece of code can be removed
/*
//        if (*RefreshCnt == level.getBombTime(0) + 12) {
//            if (((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb) &&
//                 !(level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion))
//                || ((level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb) &&
//                    (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion))) {
//                drawExplosion(level.getBombX(0), level.getBombY(0));
//                drawExplosion(level.getBombX(1), level.getBombY(1));
//                //getExTime = *RefreshCnt;
//            }
//        }
*/

        // TODO: Explain what this does
        if(*RefreshCnt >= level.getBombTime(0) + 92)
        {
            if((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb)
                && (level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion)
                || (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb)
                && (level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion))
            {
                drawAir(level.getBombX(0), level.getBombY(0));
                drawAir(level.getBombX(1), level.getBombY(1));
                level.setBomb(0, 0, 0, 0, 0);
                level.setBomb(1, 0, 0, 0, 0);

            }
        }

        if(Definitions::nunchuk->cButton) // If the C-Button is pressed, move player 2
        {
            movePeep(2);
        }
        else // Otherwise, move player 1
        {
            movePeep(1);
        }

    #ifdef DEBUG
        ///Print some debug information
        //level.printMap();
		//Definitions::println();
		// Print which frame we are on
		Definitions::setTextDebug();
		Definitions::tft->setCursor(0, 210);
		Definitions::tft->print("frame: ");
		Definitions::tft->println(*RefreshCnt);

		// Print the x-position of the nunchuk's joystick
		Definitions::tft->setCursor(240, 0);
		Definitions::tft->print("x:  ");
		Definitions::tft->print(Definitions::nunchuk->analogX);
		Definitions::tft->print("   ");

		// Print the y-position of the nunchuk's joystick
		Definitions::tft->setCursor(240, 8);
		Definitions::tft->print("y:  ");
		Definitions::tft->print((unsigned int) Definitions::nunchuk->analogY);
		Definitions::print("  ");
    #endif
        level.drawMap();

        drawLives();
        //gameScreen::drawLives();
    }
    timeCounter++;

    // Update the current time
    if(timeCounter >= 30) //If 30 refreshes have happened (roughly 1 second)
    {
        // Subtract one from the timer (timer counts down)
        currentTime--;
        // Draw the new timer value
        drawTimer();
        // Reset the counter for the timer
        timeCounter = 0;
    }
}

void gameScreen::drawLives() // Print the amount of lives per player on-screen
{
    // Set x-location of lives
    uint16_t x = 305;

//    Definitions::irComm->print("p1: ");
//    Definitions::irComm->println(livesP1);
//    Definitions::irComm->print("p2: ");
//    Definitions::irComm->println(livesP2);
    if(livesP1 < 3) // If player 1 has lost lives
    {
        for(int i = 3 - livesP1; i > 0; i--) // Checks for the amount of lives player 1 has left
        {
            // Draw a black circle over the amount of lives for every life player 1 has lost
            Definitions::tft->fillCircle(x, 30, 10, ILI9341_BLACK);
            // Set the x-position back by 25 pixels to draw over the next life circle
            x -= 25;
        }
    }
    // Reset the x-value for the next player
    x = 305;

    if(livesP2 < 3) // If player 2 has lost lives
    {
        for(int i = 3 - livesP2; i > 0; i--) // Checks for the amount of lives player 2 has left
        {
            // Draw a black circle over the amount of lives for every life player 2 has lost
            Definitions::tft->fillCircle(x, 75, 10, ILI9341_BLACK);
            // Set the x-position back by 25 pixels to draw over the next life circle
            x -= 25;
        }
    }

    // Checks if a player has died yet and if so, ends the game
    if(livesP1 == 0) // If player 1 has died
    {
        // Set the dead player as player 1
        deadPlayer = 1;
        // End the game
        end();
    }
    else if(livesP2 == 0) // If player 2 has died
    {
        // Set the dead player as player 2
        deadPlayer = 2;
        // End the game
        end();
    }
}

void gameScreen::drawTimer() // Draws the timer value
{
    // Sets the proper values for printing the text and prints the value of the timer
    Definitions::tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    Definitions::tft->setCursor(286, 100);
    Definitions::tft->print(currentTime);
    Definitions::tft->println(" ");

    // Ends the game if the timer has ran out
    if(currentTime == 0) // If the timer has reached zero...
    {
        // Set the winner to zero (tied)
        winner = 0;
        // End the game
        end();
    }
}

void gameScreen::drawScore() // Draws the score values
{
    // Sets values and draws score of player 1
    Definitions::tft->setCursor(274, 120);
    Definitions::tft->print(scoreP1);
    Definitions::tft->println(" ");

    // Sets values and draws score of player 2
    Definitions::tft->setCursor(274, 130);
    Definitions::tft->print(scoreP2);
    Definitions::tft->println(" ");
}

void gameScreen::movePeep(int peep) // Move a player across the level
{
	// Store the direction of nunchuk's joystick
	int nunX = Definitions::nunchuk->analogX;
	int nunY = Definitions::nunchuk->analogY;

	// Get the locations of the players and store in newX and newY
	if(peep == 1) // If the current player is player 1
	{
		// Set new positions to player 1's position
		newX = p1X;
		newY = p1Y;
	}
	if(peep == 2) // If the current player is player 2
	{
        // Set new positions to player 1's position
		newX = p2X;
		newY = p2Y;
	}

	//Definitions::tft->fillRect(p2X*16, p2Y*16, 16, 16, ILI9341_BLACK);

	// Change the new position according to the joystick position
	// TODO: decide if all commented-out code here can be removed
	if(nunX <= 70 && (nunY > 50 && nunY < 200)) // && p2X > 1)  // If the joystick is moved to the left, move player to the left
	{
		newX--;
	}
	else if(nunX >= 170 && (nunY > 50 && nunY < 200)) // && p2X < Definitions::gameWidth) // If the joystick is moved to the right, move player to the right
	{
		newX++;
	}
	else if((nunX > 50 && nunX < 200) && nunY >= 170) // && p2Y > 1) // If the joystick is moved upwards, move player upwards
	{
		// Nunchuck Y is inverted.
		//p2Y--;
		newY--;
	}
	else if((nunX > 50 && nunX < 200) && nunY <= 70) // && p2Y < Definitions::gameHeight) // If the joystick is moved downwards, move player downwards
	{
        // Nunchuck Y is inverted.
		//p2Y++;
		newY++;
	}

	// Draw player in new position
	// Checks which peep is selected and if the new position isn't the same as the selected peep's current position
	if((peep == 1 && (newY != p1Y || newX != p1X))
		|| (peep == 2 && (newY != p2Y || newX != p2X)))
	{
		// Check if the player can move to the new position (if there's a block or barrel on the new position)
		if(!(level.getObjectAt(newX, newY) & mapObject::block)
		    && !(level.getObjectAt(newX, newY) & mapObject::barrel)
		    && !((level.getObjectAt(newX, newY) & mapObject::bomb)
		    && !(level.getObjectAt(newX, newY) & mapObject::explosion)))
		{
            if(peep == 1) // If the selected player is player 1
            {
                // Draw player 1 on their new position
                level.unmarkObjectAt(p1X, p1Y, mapObject::peep1);
                level.markObjectAt(p1X, p1Y, mapObject::needsRedraw);
                level.markObjectAt(newX, newY, mapObject::peep1);
                level.markObjectAt(newX, newY, mapObject::needsRedraw);

                // Set player 1's position values
                p1X = newX;
                p1Y = newY;
				Definitions::irComm->write(0x05);
				Definitions::irComm->println("p1pos:")
				Definitions::irComm->write(p1X);
				Definitions::irComm->print(p1X);
				Definitions::irComm->print(", ");
				Definitions::irComm->write(p1Y);
				Definitions::irComm->println(p1Y);

            }
			if(peep == 2) // If the selected player is player 2
			{
				// Draw player 2 on their new position
				level.unmarkObjectAt(p2X, p2Y, mapObject::peep2);
				level.markObjectAt(p2X, p2Y, mapObject::needsRedraw);
				level.markObjectAt(newX, newY, mapObject::peep2);
				level.markObjectAt(newX, newY, mapObject::needsRedraw);

				// Set player 2's position values
				p2X = newX;
				p2Y = newY;
			}
		}
	}

    // TODO: Decide if this commented-out piece of code can be removed
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

void gameScreen::placeBomb(int peep, uint16_t x, uint16_t y) // Place a bomb
{
    // TODO: Decide if this commented-out piece of code can be removed
	//level.setBomb(0, x, y, )
	if(peep == 1) // If player 1 placed the bomb
	{
	    // Place a bomb where Player 1 placed the bomb
		level.markObjectAt(x, y, mapObject::bombPeep1);
	}
	if (peep == 2) // If player 2 placed the bomb
	{
        // Place a bomb where Player 2 placed the bomb
		level.markObjectAt(x, y, mapObject::bombPeep2);
	}
	// TODO: Wat is deez?
	// now there is a bom placed
	level.markObjectAt(x, y, mapObject::bomb);
	level.markObjectAt(x, y, mapObject::needsRedraw);
}

void gameScreen::drawExplosion(int peep, uint16_t explX, uint16_t explY) // Draws an explosion on the screen
{
// TODO: Decide if this commented-out piece of code can be removed
//    int explX = p2X;
//    int explY = p2Y;
//    int X = p2X;
//    int Y = p2Y;

    /* Checks where the explosions can be placed
     * The checks are done as follows:
     * +    +   +   +   +   +   +
     * +    +   +   2   +   +   +
     * +    +   +   1   +   +   +
     * +    -2  -1  0   1   2   +
     * +    +   +   -1  +   +   +
     * +    +   +   -2  +   +   +
     * +    +   +   +   +   +   +
     * First there will be checked if x < 0 and current space + 1 contains a block.
     * If there's a block there, the explosion should be unable the current space, so don't draw an explosion.
     * Then, there will be checked if x > 0 and current space - 1 contains a block.
     * If there's a block there, the explosion should be unable the current space, so don't draw an explosion.
     * Afterwards, the same is done, but it checks for a barrel instead of a block this time.
     * Then, there will be checked if the current space is a block. If so, don't draw an explosion.
     * Lastly, the same will be done for the y-axis.
     */
	for(int x = -2; x <= 2; x++) // Check over horizontal positions
    {
		if(x != 0) // If not on the center of the explosion
		{
		    // Check if the explosion can reach the current space for blocks
			if(((x < 0) && !(level.getObjectAt(explX + x + 1, explY) & mapObject::block))
			    || ((x > 0) && !(level.getObjectAt(explX + x - 1, explY) & mapObject::block)))
			{
                // Check if the explosion can reach the current space for barrels
				if(((x < 0) && !((level.getObjectAt(explX + x + 1, explY)) & mapObject::barrel))
				    || ((x > 0) && !((level.getObjectAt(explX + x - 1, explY)) & mapObject::barrel)))
				{
					// Check if the current space can contain an explosion
					if(!(level.getObjectAt(explX + x, explY) & mapObject::block))
					{
						if(peep == 1) // If the current player is player 1
						{
							// Draw an explosion coloured for player 1
							level.markObjectAt(explX + x, explY, mapObject::bombPeep1);
						}
						if(peep == 2) // If the current player is player 2
						{
                            // Draw an explosion coloured for player 2
							level.markObjectAt(explX + x, explY, mapObject::bombPeep2);
						}

						if((level.getObjectAt(explX + x, explY) & mapObject::peep1)) // Check if player 1 is in the blast radius
						{
						    // Remove a life from player 1
							livesP1--;
							// Add 1000 to player 2's score
							scoreP2 += 1000;
							// Draw the new value for the scores
							drawScore();
						}
						if((level.getObjectAt(explX + x, explY) & mapObject::peep2)) // Check if player 2 is in the blast radius
						{
                            // Remove a life from player 2
							livesP2--;
                            // Add 1000 to player 1's score
							scoreP1 += 1000;
                            // Draw the new value for the scores
							drawScore();
						}

						// Replace the current space's tile with a horizontal explosion
						level.markObjectAt(explX + x, explY, mapObject::explosion);
						level.markObjectAt(explX + x, explY, mapObject::explosionH);
						level.markObjectAt(explX + x, explY, mapObject::needsRedraw);
						level.unmarkObjectAt(explX + x, explY, mapObject::barrel);
					}
				}
			}
		}
		else // If on the center of the explosion
		{
			if(peep == 1) // If the current player is player 1
			{
                // Draw an explosion coloured for player 1
				level.markObjectAt(explX + x, explY, mapObject::bombPeep1);
			}
			if(peep == 2) // If the current player is player 2
			{
                // Draw an explosion coloured for player 2
				level.markObjectAt(explX + x, explY, mapObject::bombPeep2);
			}
			// Replace the current space's tile with a center explosion
			level.markObjectAt(explX, explY, mapObject::explosion);
			level.markObjectAt(explX, explY, mapObject::needsRedraw);
			level.unmarkObjectAt(explX, explY, mapObject::bomb);
		}
	}

	for(int y = -2; y <= 2; y++) // Check over vertical positions
	{
		if(y != 0) // If not on the center of the explosion
		{
            // Check if the explosion can reach the current space for blocks
			if(((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::block))
			    || ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::block)))
			{
                // Check if the explosion can reach the current space for barrels
				if(((y < 0) && !((level.getObjectAt(explX, explY + y + 1)) & mapObject::barrel))
				    || ((y > 0) && !((level.getObjectAt(explX, explY + y - 1)) & mapObject::barrel)))
				{
                    // Check if the current space can contain an explosion
					if(!(level.getObjectAt(explX, explY + y) & mapObject::block))
					{
						if(peep == 1) // If the current player is player 1
						{
                            // Draw an explosion coloured for player 1
							level.markObjectAt(explX, explY + y, mapObject::bombPeep1);
						}
						if(peep == 2) // If the current player is player 2
						{
                            // Draw an explosion coloured for player 2
							level.markObjectAt(explX, explY + y, mapObject::bombPeep2);
						}
						if((level.getObjectAt(explX, explY + y) & mapObject::peep1)) // Check if player 1 is in the blast radius
						{
                            // Remove a life from player 1
							livesP1--;
                            // Add 1000 to player 2's score
							scoreP2 += 1000;
                            // Draw the new value for the scores
							drawScore();
						}
						if((level.getObjectAt(explX, explY + y) & mapObject::peep2)) // Check if player 2 is in the blast radius
						{
                            // Remove a life from player 2
							livesP2--;
                            // Add 1000 to player 1's score
							scoreP1 += 1000;
                            // Draw the new value for the scores
							drawScore();
						}
                        // Replace the current space's tile with a vertical explosion
						level.markObjectAt(explX, explY + y, mapObject::explosion);
						level.markObjectAt(explX, explY + y, mapObject::explosionV);
						level.markObjectAt(explX, explY + y, mapObject::needsRedraw);
						level.unmarkObjectAt(explX, explY + y, mapObject::barrel);
					}
				}
			}
		}
		else // If on the center of the explosion
		{
			if((level.getObjectAt(explX, explY) & mapObject::peep1)) // Check if player 1 is in the blast radius
			{
                // Remove a life from player 1
                livesP1--;
                // Add 1000 to player 2's score
                scoreP2 += 1000;
                // Draw the new value for the scores
                drawScore();
			}
			if((level.getObjectAt(explX, explY) & mapObject::peep2)) // Check if player 2 is in the blast radius
			{
                // Remove a life from player 2
                livesP2--;
                // Add 1000 to player 1's score
                scoreP1 += 1000;
                // Draw the new value for the scores
                drawScore();
			}

			if(peep == 1) // If the current player is player 1
			{
                // Draw an explosion coloured for player 1
				level.markObjectAt(explX, explY + y, mapObject::bombPeep1);
			}
			if(peep == 2) // If the current player is player 2
			{
                // Draw an explosion coloured for player 2
				level.markObjectAt(explX, explY + y, mapObject::bombPeep2);
			}
            // Replace the current space's tile with a center explosion
			level.markObjectAt(explX, explY, mapObject::bomb);
			level.markObjectAt(explX, explY, mapObject::explosion);
			level.markObjectAt(explX, explY, mapObject::needsRedraw);
		}
	}
}

void gameScreen::drawAir(uint16_t explX, uint16_t explY) // Draws air tiles after explosions
{
	for(int x = -2; x <= 2; x++) // Scan over the horizontal positions
	{
		// Remove any explosions and bombs left on these positions
		level.unmarkObjectAt(explX + x, explY, mapObject::explosion);
		level.unmarkObjectAt(explX + x, explY, mapObject::explosionH);
		level.unmarkObjectAt(explX + x, explY, mapObject::bomb);
		level.unmarkObjectAt(explX + x, explY, mapObject::bombPeep2);
		level.unmarkObjectAt(explX + x, explY, mapObject::bombPeep1);

		// Makes sure block tiles aren't overwritten
		if(!(level.getObjectAt(explX + x, explY) & mapObject::block)) // If the positions don't contain blocks
		{
			// Replace the tiles with air
			level.markObjectAt(explX + x, explY, mapObject::air);
			level.markObjectAt(explX + x, explY, mapObject::needsRedraw);
		}
	}

	for(int y = -2; y <= 2; y++) // Scan over the vertical positions
	{
        // Remove any explosions and bombs left on these positions
		level.unmarkObjectAt(explX, explY + y, mapObject::explosion);
		level.unmarkObjectAt(explX, explY + y, mapObject::explosionV);
		level.unmarkObjectAt(explX, explY + y, mapObject::bomb);
		level.unmarkObjectAt(explX, explY + y, mapObject::bombPeep2);
		level.unmarkObjectAt(explX, explY + y, mapObject::bombPeep1);

        // Makes sure block tiles aren't overwritten
		if(!(level.getObjectAt(explX, explY + y) & mapObject::block))
		{
            // Replace the tiles with air
			level.markObjectAt(explX, explY + y, mapObject::air);
			level.markObjectAt(explX, explY + y, mapObject::needsRedraw);
		}
	}
}
