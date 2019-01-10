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

// gameScreen default constructor
gameScreen::gameScreen()
{
}

// Initialize gameScreen class with level pointer
gameScreen::gameScreen(Level *level)
{
    this->level = *level;
}

// Initialize gameScreen class with level number
gameScreen::gameScreen(uint8_t levelnr)
{
	this->level = Level(LevelDefs::levelBarrels[levelnr], LevelDefs::levelNames[levelnr]);
}

// Initialize gameScreen class with level name and seed
gameScreen::gameScreen(String levelName, uint16_t seed)
{
#if PEEP == 1
	this->level = Level(levelName);
#elif PEEP == 2
	this->level = Level(levelName, seed);
#endif
}

// Initialize players' positions
int p2Y = 0, p2X = 0;
int p1Y = 0, p1X = 0;
int newX, newY;
// Initialize players' lives
uint8_t lives1, lives2;

// Keeps proper timer value towards the end of the game
int endTimer;

void gameScreen::begin() // Initializes the game's screen
{
	// Set background to black and draw the initial level
	Definitions::tft->fillScreen(ILI9341_BLACK);
	level.begin();

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

	// Set the countdown timer to start value
	Definitions::currentTime = 180;

	// Draw initial timer and score values
	drawTimer();
	drawScore();

	level.drawMap();

	// Set the timer again to make up for lost time during level setup
	Definitions::currentTime = 180;
}

void gameScreen::end() // End the match by calculating some scores and showing the endScreen
{
	// Set the endTimer
	endTimer = Definitions::currentTime;
    // Check who won the game
    checkWinner();
    // Add some extra's to the scores
    calculateEndScores();

    // Send and receive data based on which player the current arduino has
#if PEEP==1
	// Send the stop byte
	Definitions::irComm->write(0x07);

	// Send player 1's score
	Definitions::irComm->write(0x04);
	Definitions::irComm->write(scoreP1);

	// Send the timer value
	Definitions::irComm->write(0x08);
	Definitions::irComm->write(Definitions::currentTime);

	// Wait on incoming data
	while(!Definitions::irComm->available())
		PRR=PRR;

	// Store the incoming command
	uint8_t endreccmd = Definitions::irComm->read();

	// If the score indication byte is received
	if(endreccmd == 0x04)
	{
		// Wait on incoming data
		while(!Definitions::irComm->available())
			PRR=PRR;
		// Set player 2's score
		scoreP2 = Definitions::irComm->read();
	}
#elif PEEP==2
	// Send player 2's score
	Definitions::irComm->write(0x04);
	Definitions::irComm->write(scoreP2);

	// Wait on incoming data
	while(!Definitions::irComm->available())
		PRR=PRR;

	// Store the incoming command
	uint8_t endreccmd = Definitions::irComm->read();

	// If the score indication byte is received
	if(endreccmd == 0x04)
	{
		// Wait on incoming data
		while(!Definitions::irComm->available())
			PRR=PRR;
		// Set player 1's score
		scoreP1 = Definitions::irComm->read();
	}
	// If the timer indication byte is received
	else if(endreccmd == 0x08)
	{
		// Wait on incoming data
		while(!Definitions::irComm->read())
			PRR=PRR;
		// Set the timer
		currentTime = Definitions::irComm->read();
	}
#endif

    // Print the endScreen, showing if you won or lost and the scores
    gameScreen::drawEndScreen();

    // Reset the game when the C-Button is pressed
    while (true)
    {
        if (Definitions::nunchuk->cButton)
		{
			// Jumping to the first line of the program to reset all the processes
			asm volatile ("  jmp 0");
		}
		else
			PRR = PRR;
    }
}

void gameScreen::checkWinner() // Check who won the game
{
    // If the game is over and the timer hasn't reached zero yet...
    // ...a player has died, so the player who's dead has lost the game
    if(!Definitions::currentTime == 0)
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
        scoreP1 += ((endTimer / 2) + (livesP1 * 20));
        scoreP2 += ((endTimer / 2) + (livesP2 * 20));
    }
    else if(winner == 1) // If player 1 won, give them a bonus
    {
        scoreP1 += ((endTimer/ 2) + (livesP1 * 20));
        scoreP2 -= ((endTimer / 2) - (livesP2 * 20));
    }
    else if(winner == 2) // If player 2 won, give them a bonus
    {
        scoreP1 -= ((endTimer / 10) - (livesP1 * 20));
        scoreP2 += ((endTimer / 10) + (livesP2 * 20));
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
    Definitions::tft->print(endTimer);
}

// Store how many refreshes have been done
uint32_t *RefreshCnt = 0;
void gameScreen::refresh() // Handles refreshing the screen and updating some variables
{
    // Increment refresh counter
    *RefreshCnt++;

    // Handle receiving of data
	if(Definitions::irComm->available())
	{
		// Store the incoming command
		uint8_t recCmd = Definitions::irComm->read();

		// If the lives indication byte is received
		if(recCmd == 0x03)
		{
			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received lives
			uint8_t recLives = Definitions::irComm->read();

			/* If the current arduino is PEEP 1, they received the lives of PEEP 2...
			 * So set the lives for PEEP 2
			 * If the current arduino is PEEP 2 it's the other way around */
		#if PEEP==1
			livesP2 = recLives;
		#elif PEEP==2
			livesP1 = recLives;
		#endif
		}
		// If the score indication byte is received
		else if(recCmd == 0x04)
		{
			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received score
			uint8_t recScore = Definitions::irComm->read();

			/* If the current arduino is PEEP 1, they received the score of PEEP 2...
			 * So set the score for PEEP 2
			 * If the current arduino is PEEP 2 it's the other way around */
			#if PEEP==1
				scoreP2 = recScore;
			#elif PEEP==2
				scoreP1 = recScore;
			#endif
		}
		// If the player location indication byte is received
		else if(recCmd == 0x05)
		{
			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received x position data
			uint8_t recPx = Definitions::irComm->read();

			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received y position data
			uint8_t recPy = Definitions::irComm->read();

			/* If the current arduino is PEEP 1, they received the location data of PEEP 2...
			 * So set the location for PEEP 2
			 * If the current arduino is PEEP 2 it's the other way around */
		#if PEEP==1
			movePeep(2, recPx, recPy);
		#elif PEEP==2
			movePeep(1, recPx, recPy);
		#endif
		}
		// If the bomb location indication byte is received
		else if(recCmd == 0x06)
		{
			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received x position data
			uint8_t recBx = Definitions::irComm->read();

			// Wait for incoming data
			while(!Definitions::irComm->available())
				PRR=PRR;
			// Store the received y position data
			uint8_t recBy = Definitions::irComm->read();

			/* If the current arduino is PEEP 1, they received the bomb location data for PEEP 2...
			 * So place a bomb of PEEP 2
			 * If the current arduino is PEEP 2 it's the other way around */
		#if PEEP==1
			level.setBomb(1, recBx, recBy, *RefreshCnt, 2);
			placeBomb(2, level.getBombX(1), level.getBombY(1));
		#elif PEEP==2
			level.setBomb(0, recBx, recBy, *RefreshCnt, 1);
			placeBomb(1, level.getBombX(0), level.getBombY(0));
		#endif
		}
	}

	// Draw the timer value
	drawTimer();

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
#if PEEP == 1
        if(Definitions::nunchuk->zButton && level.getBombX(0) == 0
            && level.getBombTime(0) == 0 && level.getBombY(0) == 0
            && level.getBombPeep(0) == 0)
        {
			// If the current arduino has player 1, place a bomb for player 1
			#if PEEP==1
				level.setBomb(0, p1X, p1Y, *RefreshCnt, 1);
				placeBomb(1, level.getBombX(0), level.getBombY(0));
			// If the current arduino has player 2, place a bomb for player 2
			#elif PEEP==2
				level.setBomb(1, p2X, p2Y, *RefreshCnt, 2);
                placeBomb(2, level.getBombX(1), level.getBombY(1));
			#endif
        }
#endif
		// TODO: Explain this statement
#if PEEP == 1
        if(*RefreshCnt >= level.getBombTime(0) + 48)
        {
            if((level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::bomb)
                && !(level.getObjectAt(level.getBombX(0), level.getBombY(0)) & mapObject::explosion))
            {
                drawExplosion(1, level.getBombX(0), level.getBombY(0));

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
#elif PEEP == 2
        if(*RefreshCnt >= level.getBombTime(1) + 48)
            {
                if((level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::bomb)
                    && !(level.getObjectAt(level.getBombX(1), level.getBombY(1)) & mapObject::explosion))
                {
                    drawExplosion(2, level.getBombX(1), level.getBombY(1));

                #ifdef DEBUG
                    // Print some debug information on the screen
                    Definitions::print("BombTime: ");
                    Definitions::print(level.getBombTime(1) + 12);
                    Definitions::println("  ");
                    Definitions::print("ExplTime: ");
                    Definitions::print(level.getBombTime(1) + 24);
                    Definitions::println("      ");
                #endif
                }
            }
#endif

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
#if PEEP==1
        movePeep(1);
#elif PEEP==2
		movePeep(2);
#endif

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
    }
	
	if((*RefreshCnt % 100) == 0)
	{
		#if PEEP==1
			Definitions::irComm->write(0x05);
			Definitions::irComm->write(p1X);
			Definitions::irComm->write(p1Y);
		#elif PEEP==2
			Definitions::irComm->write(0x05);
			Definitions::irComm->write(p2X);
			Definitions::irComm->write(p2Y);
		#endif
	}
}

void gameScreen::drawLives() // Print the amount of lives per player on-screen
{
#if PEEP==1
	Definitions::irComm->write(0x03);
	Definitions::irComm->write(scoreP1);
#elif PEEP==2
	Definitions::irComm->write(0x03);
	Definitions::irComm->write(scoreP2);
#endif
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
    Definitions::tft->print(Definitions::currentTime);
    Definitions::tft->println(" ");

    // Ends the game if the timer has ran out
    if(Definitions::currentTime == 0) // If the timer has reached zero...
    {
        // Set the winner to zero (tied)
        winner = 0;
        // End the game
        end();
    }
}

void gameScreen::drawScore() // Draws the score values
{
#if PEEP==1
	Definitions::irComm->write(0x04);
	Definitions::irComm->write(scoreP1);
#elif PEEP==2
	Definitions::irComm->write(0x04);
	Definitions::irComm->write(scoreP2);
#endif
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

	movePeep(peep, newX, newY);
}

void gameScreen::movePeep(int peep, int newX, int newY)
{
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
//				Definitions::irComm->write(0x05);
//				Definitions::irComm->println("p1pos:");
//				Definitions::irComm->write(p1X);
//				Definitions::irComm->print(p1X);
//				Definitions::irComm->print(", ");
//				Definitions::irComm->write(p1Y);
//				Definitions::irComm->println(p1Y);

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
			#if PEEP==1
				Definitions::irComm->write(0x05);
				Definitions::irComm->write(p1X);
				Definitions::irComm->write(p1Y);
			#elif PEEP==2
				Definitions::irComm->write(0x05);
				Definitions::irComm->write(p2X);
				Definitions::irComm->write(p2Y);
			#endif

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

	Definitions::irComm->write(0x06);
	Definitions::irComm->write(x);
	Definitions::irComm->write(y);
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
							drawLives();
							// Add 1000 to player 2's score
							scoreP2 += 10;
							// Draw the new value for the scores
							drawScore();
						}
						if((level.getObjectAt(explX + x, explY) & mapObject::peep2)) // Check if player 2 is in the blast radius
						{
                            // Remove a life from player 2
							livesP2--;
							drawLives();
                            // Add 1000 to player 1's score
							scoreP1 += 10;
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
							drawLives();
                            // Add 1000 to player 2's score
							scoreP2 += 10;
                            // Draw the new value for the scores
							drawScore();
						}
						if((level.getObjectAt(explX, explY + y) & mapObject::peep2)) // Check if player 2 is in the blast radius
						{
                            // Remove a life from player 2
							livesP2--;
							drawLives();
                            // Add 1000 to player 1's score
							scoreP1 += 10;
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
				drawLives();
                // Add 1000 to player 2's score
                scoreP2 += 10;
                // Draw the new value for the scores
                drawScore();
			}
			if((level.getObjectAt(explX, explY) & mapObject::peep2)) // Check if player 2 is in the blast radius
			{
                // Remove a life from player 2
                livesP2--;
				drawLives();
                // Add 1000 to player 1's score
                scoreP1 += 10;
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
