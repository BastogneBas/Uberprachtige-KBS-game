#include "src/game/game.h"
#include "screen.h"
#include "staticDefinitions.cpp"
#include "src/level/levelDefs.h"
#include "src/homeScreen/homeScreen.h"
#include <avr/io.h>

// If we are debugging, uncomment this. Then there will be Serial communication.
#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;

// The nunchuk needs to be redefined here
ArduinoNunchuk *Definitions::nunchuk;

// Pointer to the current visible screen.
screen *Definitions::currentScreen;

int startRefresh = 0, refreshDone = 1;

ISR(TIMER0_COMPA_vect)
{
	if (startRefresh)
		PORTD |= (1 << PORTD5);
	else
		PORTD &= ~(1 << PORTD5);

	if (refreshDone)
		PORTD |= (1 << PORTD6);
	else
		PORTD &= ~(1 << PORTD6);
}

ISR(TIMER1_COMPA_vect)
{
	if(refreshDone){
		//PORTD |= (1 << PORTD5);
		startRefresh=1;
	}
	PORTD ^= (1 << PORTD2);
	
	//Serial.println("refresh");
	//Definitions::currentScreen->refresh();
}

//ISR(_VECTOR(0)){
//	DDRD |= (1 << DDD4);
//	PORTD |= (1 << PORTD4);
//}

int main()
{
	// TODO: replace with own initialisation.
	// Default Arduino initialisation.
//#warning Needs to be replaced
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6);
	//PORTD = 0xFF;
	//_delay_ms(10);
	//PORTD = 0x00;
	if(MCUSR & (1 << WDRF))
		PORTD |= (1 << PORTD2);
	if(MCUSR & (1 << EXTRF))
		PORTD |= (1 << PORTD3);
	if(MCUSR & (1 << PORF))
		PORTD |= (1 << PORTD4);
	if(MCUSR)
		PORTD |= (1 << PORTD5);

	init();

	// Turn on Serial communication if we are debugging
#ifdef DEBUG
	Serial.begin(500000);
	Serial.println("Welkom!");
#endif

	//cli();
	TCCR0A = (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B = (0<<WGM02)  | (0<<CS02) | (1<<CS01) | (0<<CS00);
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	
	OCR1A = (uint16_t)6250;
	TIMSK1 = (1<<OCF1A);

	// Initialize the tft
	Definitions::tft =
		new Adafruit_ILI9341(Definitions::TFT_CS, Definitions::TFT_DC);
	Definitions::tft->begin();
	yield();
	Definitions::tft->setRotation(1);
	Definitions::tft->fillScreen(ILI9341_BLACK);

	// Initialize the Nunchuk for player 1
	Definitions::nunchuk = new ArduinoNunchuk();
	Definitions::nunchuk->init();

	// Opening the homeScreen
	Definitions::currentScreen = new homeScreen();
	Definitions::currentScreen->begin();

	//sei();

	
	

	for(;;)
	{
		// Refresh screen
		if(startRefresh){
			refreshDone = 0;
			PORTD |= (1 << PORTD6);
#ifdef DEBUG
			Serial.println("refresh");
#endif
			Definitions::currentScreen->refresh();
			startRefresh = 0;
			refreshDone = 1;
		}else{
			PORTB=PORTB;
		}
	/*if (startRefresh)
		PORTD |= (1 << PORTD5);
	else
		PORTD &= ~(1 << PORTD5);

	if (refreshDone)
		PORTD |= (1 << PORTD6);
	else
		PORTD &= ~(1 << PORTD6);
		}*/
		//delay(100);
	}
}
