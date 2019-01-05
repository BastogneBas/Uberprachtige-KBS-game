#include "src/homeScreen/homeScreen.h"
#include "src/game/game.h"
#include "src/level/levelDefs.h"
#include "src/IRComm/IRComm.h"
#include "staticDefinitions.cpp"
#include "screen.h"
#include <avr/io.h>
#include <HardwareSerial.h>
#include <Stream.h>
#include <HardwareSerial_private.h>

/* Defines if debugging for IR is enabled
 * Uncomment this to enable debugging
 * WARNING: May break other screen drawing stuff
 * Please, only enable when debugging and disable when pushing to git */
//#define IRDEBUG

// If we are debugging, uncomment this. Then there will be Serial communication.
//#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;

// The nunchuk needs to be redefined here
ArduinoNunchuk *Definitions::nunchuk;

// Pointer to the current visible screen.
screen *Definitions::currentScreen;

// IRcomm needs to be redefined here
Stream *Definitions::irComm;

// Reset refreshing
static int startRefresh = 0, refreshDone = 1;

#ifdef IR
/* --Compare interrupt for send timer--
 * Makes sure that pulses are sent over the proper frequency
 * Keeps a counter of amount of overflows
 * First checks if sending of a bit is actually enabled
 * Then checks if the counter equals the amount of pulses for the bit to be sent
 * If it sent the bit type that wanted to be sent...
 * Disable the LED
 * 
 * A bit will always be sent in under 100 pulses
 * The amount of pulses for every bit type is defined in IRComm.h
 * If the counter reached 100, the bit is done sending
 * This will end the sending of the bit
 *
 * (is only enabled when IR is enabled) */
ISR(TIMER0_COMPA_vect)
{
	// If a bit wants to be sent...
	if (irComm->bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ...for the specified bit to be sent...
		if (irComm->bitSendCounter >= irComm->bitSendType)
		{
			// Disable the 'let-through' pin for the IR LED, blocking the PWM signal
			PORTD |= (1 << PORTD4);
		}

		// Add one to pulsecounter
		irComm->bitSendCounter++;

		// If the bit is completely sent...
		// (a bit is sent over 100 pulses)
		if (irComm->bitSendCounter == 100)
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
#endif

ISR(TIMER1_COMPA_vect) // Timer1 output compare interrupt
{
	if(refreshDone)
	{
		//PORTD |= (1 << PORTD5);
		startRefresh = 1;
	}
	//Serial.println("refresh");
	//#ifndef IRDEBUG
	//Definitions::currentScreen->refresh();
	//#endif
}

#ifdef IR
/* --Compare interrupt for receive timer--
 * When a bit is being received, a pin change interrupt will happen
 * This will enable the counter for this timer interrupt
 * This timer will count 'pulses' at the frequency of the sender
 * For more info, see the comment at the pin change interrupt
 *
 * (is only enabled when IR is enabled) */
ISR(TIMER2_COMPA_vect)
{
	if (irComm->bitReceiveEnabled)
	{
		irComm->bitReceiveCounter++;
	}
}
#endif

#ifdef IR
/* --Pin Change Interrupt for receive timer--
 * If a PIN Change Interrupt is found on Analog PIN A3...
 * ...the receiving of a bit has begun
 * This is because the receiver will change from a high to a low signal...
 * ...when it receives something.
 * 
 * First, we will check if receiving has been enabled
 * Then, we will check if the receiving has started yet
 * If this isn't the case, start the receiving and start a counter
 * If the receiving has already started however and we got an interrupt...
 * ...the receiving has ended
 * This is because the signal will change from low to high after receiving
 * After receiving, the counted pulses will be processed and we will know what we received
 *
 * (is only enabled when IR is enabled) */
ISR(PCINT1_vect)
{
#ifdef IRDEBUG
	PORTD ^= (1 << PORTD2);
#endif
	// If the receiving has been enabled
	if (irComm->bitReceiveEnabled)
	{
		// If the receive hasn't started yet
		if (!irComm->bitReceiveStarted)
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
}
#endif

int main() // Main function
{
	// Set PINs to output
	DDRB |=
		(1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4)
		| (1 << DDB5);
	DDRD |=
		(1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5)
		| (1 << DDD6) | (1 << DDD7);
	DDRC |= (1 << DDC1);

	// Set Analog PIN A3 to HIGH
	PORTC |= (1 << PORTC3);
	// Set Digital PINs 8 to 13 to LOW
	PORTB = 0;

	// TODO: replace with own initialisation.
	// SEE: https://github.com/arduino/ArduinoCore-avr/blob/b084848f2eaf9ccb3ac9a64ac5492d91df4706bf/cores/arduino/wiring.c#L241
	// Default Arduino initialisation.
	#warning Needs to be replaced
	//init();

	// Initialize Timer 0
	TCCR0A =
		(0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0)
		| (1 << WGM01) | (1 << WGM00);
	TCCR0B = (0 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00);

	// Initialize Timer 1
	TCCR1A =
		(0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0)
		| (0 << WGM11) | (0 << WGM10);
	TCCR1B =
		(0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12)
		| (1 << CS12)	| (0 << CS11) | (0 << CS10);

	// Set timer 1 to compare after 1562 ticks
	//OCR1A = (uint16_t)6250;
	OCR1A = (uint16_t) 1562;
	// Enable timer compare interrupts for timer 1
	TIMSK1 = (1 << OCF1A);

	// Enable global interupts
	sei();


///Only works when not debugging IR
#ifndef IRDEBUG
	// Initialize the TFT screen
	Definitions::tft =
		new Adafruit_ILI9341(Definitions::TFT_CS, Definitions::TFT_DC);
	Definitions::tft->begin();
	yield();
	Definitions::tft->setRotation(1);
	Definitions::tft->fillScreen(ILI9341_BLACK);

#ifdef DEBUG
	// Turn on Serial communication if we are debugging	
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
///I don't know what this was supposed to do, so I left it --Niels
#else
// TODO: Decide if this commented-out piece of code can be removed
//	Definitions::irComm =
//		new HardwareSerial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C,
//						   &UDR0);
//	((HardwareSerial *) (Definitions::irComm))->begin(9600);

//#if PEEP == 1
//	char mystr[] = "1hello";
//	Definitions::irComm->print(mystr);
//	//delay (1000);
//
//#else
//	char mystr[10];
//	Definitions::irComm->readBytes(mystr, 5);
//	Definitions::irComm->println(mystr);
//	//delay(1000);
//#endif
#endif

	for(;;) // Loop forever
	{
		if(startRefresh) // Refresh screen when a refresh has been indicated
		{
			refreshDone = 0;
		#ifndef IRDEBUG
			///Only refresh the screen when not debugging IR
			Definitions::currentScreen->refresh();
		#endif
			startRefresh = 0;
			refreshDone = 1;
		}
		else
		{
			// Does nothing, but the arduino can't handle empty whiles for some reason, so we put this in instead
			PORTB = PORTB;
		}
		// TODO: Decide if this commented-out piece of code can be removed
		/*if (startRefresh)
		   PORTD |= (1 << PORTD5);
		   else
		   PORTD &= ~(1 << PORTD5);

		   if (refreshDone)
		   PORTD |= (1 << PORTD6);
		   else
		   PORTD &= ~(1 << PORTD6);
		   } */
	}
	PORTB = 0xFF;
	PORTD = 0xFF;
}
