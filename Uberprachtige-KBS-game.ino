#include "src/game/game.h"
#include "screen.h"
#include "staticDefinitions.cpp"
#include "src/level/levelDefs.h"
#include "src/homeScreen/homeScreen.h"
#include <avr/io.h>
#include "src/IRComm/IRComm.h"
#include <HardwareSerial.h>
#include <Stream.h>
#include <HardwareSerial_private.h>

// Used for when debugging Infrared Comms
// Used to not destroy work of others while working on IR :)
//#define IRDEBUG

// initialize the IR Communications class
// If we are debugging, uncomment this. Then there will be Serial communication.
#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;

// The nunchuk needs to be redefined here
ArduinoNunchuk *Definitions::nunchuk;

// Pointer to the current visible screen.
screen *Definitions::currentScreen;

// IRcomm needs to be redefined here
Stream *Definitions::irComm;


static int startRefresh = 0, refreshDone = 1;


// Send Timer compare interrupt
ISR(TIMER0_COMPA_vect)
{
#ifdef IR
	// If a bit wants to be sent...
	if(irComm->bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ...for the specified bit to be sent...
		if(irComm->bitSendCounter >= irComm->bitSendType)
		{
			// Disable the 'let-through' pin for the IR LED, blovking the PWM signal
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
#endif
}


ISR(TIMER1_COMPA_vect)
{
	if(refreshDone){
		//PORTD |= (1 << PORTD5);
		startRefresh=1;
	}
	
	//Serial.println("refresh");
	//#ifndef IRDEBUG
		//Definitions::currentScreen->refresh();
	//#endif
}

// Receive timer compare interrupt
ISR(TIMER2_COMPA_vect)
{
#ifdef IR
	if(irComm->bitReceiveEnabled)
	{
		irComm->bitReceiveCounter++;
	}
#endif
}

//ISR(_VECTOR(0)){
//	DDRD |= (1 << DDD4);
//	PORTD |= (1 << PORTD4);
//}

// Receival Pin Change Interrupt
ISR(PCINT1_vect)
{
	#ifdef IRDEBUG
		PORTD ^= (1 << PORTD2);
    #endif
#ifdef IR
	// If the receiving has been enabled
	if(irComm->bitReceiveEnabled)
	{
		// If the receive hasn't started yet
		if(!irComm->bitReceiveStarted)
		{
			#ifdef IRDEBUG
				PORTD |= (1 << PORTD3);
			#endif
			// Indicate that the receive has started
			irComm->startReceive();
		}
		else
		{
			#ifdef IRDEBUG
				PORTD ^= (1 << PORTD5);
			#endif
			// The message has ended...
			// Save at which count the receive has stopped
			irComm->bitReceiveChanged = irComm->bitReceiveCounter;
			// Process the data
			irComm->handleReceive();
		}
	}
#endif
}

int main()
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	DDRD |= (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRC |= (1 << DDC1);

	PORTC |= (1 << PORTC3);
	//PORTD = 0xFF;
	//_delay_ms(10);
	//PORTD = 0x00;

	PORTB = 0;
	//PORTD |= (1 << PORTD4);

	// TODO: replace with own initialisation.
	// SEE: https://github.com/arduino/ArduinoCore-avr/blob/b084848f2eaf9ccb3ac9a64ac5492d91df4706bf/cores/arduino/wiring.c#L241
	// Default Arduino initialisation.
#warning Needs to be replaced
	init();

	//cli();
	TCCR0A = (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (0<<WGM02)  | (0<<CS02) | (1<<CS01) | (0<<CS00);
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	
	//OCR1A = (uint16_t)6250;
	OCR1A = (uint16_t)1562;
	TIMSK1 = (1<<OCF1A);

	// Initialize the tft
	#ifndef IRDEBUG
		Definitions::tft =
			new Adafruit_ILI9341(Definitions::TFT_CS, Definitions::TFT_DC);
		Definitions::tft->begin();
		yield();
		Definitions::tft->setRotation(1);
		Definitions::tft->fillScreen(ILI9341_BLACK);

	// Turn on Serial communication if we are debugging
#ifdef DEBUG
	Definitions::println("Welkom!");
#endif

	// Initialize the Nunchuk for player 1
	Definitions::nunchuk = new ArduinoNunchuk();
	Definitions::nunchuk->init();

		// Opening the homeScreen
		Definitions::currentScreen = new homeScreen();
		Definitions::currentScreen->begin();
	#endif	
#ifdef IR
	// Construct the irComm class
	irComm = new IRComm();

	#ifdef IRDEBUG
		irComm->bitReceiveEnabled = 1;
		irComm->sendBit(ONE_BIT);
    #endif
#else
		Definitions::irComm = new HardwareSerial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
	((HardwareSerial*)(Definitions::irComm))->begin(500000);

	#if PEEP == 1
		char mystr[] = "1hello";
		Definitions::irComm->write(mystr, 5);
		delay (1000);

	#else
		char mystr[10];
		Definitions::irComm->readBytes(mystr, 5);
		Definitions::irComm->println(mystr);
		delay(1000);
    #endif

#endif

	for(;;)
	{
	#ifdef IR
		irComm->sendBit(ONE_BIT);
	#endif
		//irComm->sendBit(ONE_BIT);
		// Refresh screen
		if(startRefresh){
			refreshDone = 0;
			//PORTD |= (1 << PORTD6);
#ifdef DEBUG
			//Serial.println("refresh");
#endif
			#ifndef IRDEBUG
				Definitions::currentScreen->refresh();
			#endif
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
