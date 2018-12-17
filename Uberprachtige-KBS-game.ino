#include "src/game/game.h"
#include "screen.h"
#include "staticDefinitions.cpp"
#include "src/level/levelDefs.h"
#include "src/homeScreen/homeScreen.h"
#include <avr/io.h>
#include "src/IRComm/IRComm.h"

// initialize the IR Communications class
IRComm *irComm;

// If we are debugging, uncomment this. Then there will be Serial communication.
#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;

// The nunchuk needs to be redefined here
ArduinoNunchuk *Definitions::nunchuk;

// Pointer to the current visible screen.
screen *Definitions::currentScreen;

static int startRefresh = 0, refreshDone = 1;

// Send Timer compare interrupt
ISR(TIMER0_COMPA_vect)
{
	// If a bit wants to be sent...
	if(irComm->bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ...for the specified bit to be sent...
		if(irComm->bitSendCounter >= irComm->bitSendType)
		{
			// Disable the 'let-through' pin for the IR LED
			PORTD |= (1 << PORTD4);
		}

		// Add one to pulsecounter
		irComm->bitSendCounter++;

		// If the bit is completely sent...
		// (a bit is sent over 100 pulses)
		if(irComm->bitSendCounter == 100)
		{
			// Signal that the bit is completely sent
			irComm->bitSendComplete = 1;
			// Reset the counter
			irComm->bitSendCounter = 0;
			// Disable the sending of a bit
			irComm->bitSendEnabled = 0;
		}
	}
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

// Receive timer compare interrupt
ISR(TIMER2_COMPA_vect)
{
	if(irComm->bitReceiveEnabled)
	{
		irComm->bitReceiveCounter++;
	}
}

//ISR(_VECTOR(0)){
//	DDRD |= (1 << DDD4);
//	PORTD |= (1 << PORTD4);
//}

// Receival Pin Change Interrupt
ISR(PCINT1_vect)
{
	// If Analog PIN A3 is high
	if(PORTC & (1 << PORTC3))
	{
		// Enable digital pin 2
		PORTD |= (1 << PORTD2);
	}
	else
	{
		// Disable digital pin 2
		PORTD &= ~(1 << PORTD2);
	}

	// If the receiving of a bit is enabled
	if(irComm->bitReceiveEnabled)
	{
		// If the receiving of a bit has started
		if(!irComm->bitReceiveStarted)
		{
			// Enable Digital PIN 7
			PORTD |= (1 << PORTD7);
			// Reset the receive counter
			irComm->bitReceiveCounter = 0;
			// Indicate that the receiving has started
			irComm->bitReceiveStarted = 1;
		}
		// If the state of the bit hasn't changed yet
		else if(!irComm->bitReceiveChanged)
		{
			// Disable digital PIN 7
			PORTD &= ~(1 << PORTD7);
			// Indicate that the state of the bit has changed
			irComm->bitReceiveChanged = irComm->bitReceiveCounter;
			// Indicate that the receiving is complete
			irComm->bitReceiveComplete = true;
			// Disable the receiving of a bit
			irComm->bitReceiveEnabled = false;
		}
	}
}

int main()
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);

	//PORTD = 0xFF;
	//_delay_ms(10);
	//PORTD = 0x00;

	PORTB = 0;
	PORTD |= (1 << PORTD4);

	// TODO: replace with own initialisation.
	// Default Arduino initialisation.
	//#warning Needs to be replaced
	init();

	// Turn on Serial communication if we are debugging
#ifdef DEBUG
	Serial.begin(500000);
	Serial.println("Welkom!");
#endif

	//cli();
	TCCR0A = (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (0<<WGM02)  | (0<<CS02) | (1<<CS01) | (0<<CS00);
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	
	//OCR1A = (uint16_t)6250;
	OCR1A = (uint16_t)1562;
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
	
	// Construct the irComm class
	//irComm = new IRComm();
	for(;;)
	{
		// Refresh screen
		if(startRefresh){
			refreshDone = 0;
			//PORTD |= (1 << PORTD6);
#ifdef DEBUG
			//Serial.println("refresh");
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
