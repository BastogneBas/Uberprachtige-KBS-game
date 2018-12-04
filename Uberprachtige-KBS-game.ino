#include "src/game/game.h"
#include "screen.h"
#include "staticDefinitions.cpp"
#include "src/level/levelDefs.h"

// If we are debugging, uncomment this. Then there will be Serial communication.
#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;
// Pointer to the current visible screen.
screen *currentScreen;

int main()
{
	// TODO: replace with own initialisation.
	// Default Arduino initialisation.
#warning Needs to be replaced
	init();
	// Turn on Serial communication if we are debugging
#ifdef DEBUG
	Serial.begin(9600);
#endif

	// Initialize the tft
	Definitions::tft =
		new Adafruit_ILI9341(Definitions::TFT_CS, Definitions::TFT_DC);
	Definitions::tft->begin();
	yield();
	Definitions::tft->setRotation(1);
	Definitions::tft->fillScreen(ILI9341_BLACK);

	// TODO: replace with level selection screen.
#warning Needs te be replaced
	// Immediately start game screen.
	//gameScreen gamescreen = gameScreen(LevelDefs::getLevel(0));
	gameScreen gamescreen = gameScreen(Level("Random"));
	gamescreen.begin();

	// Sets the current screen to the current loaded screen
	currentScreen = &gamescreen;
	while (1)
	{
		// Refresh screen
		currentScreen->refresh();
	}
}
