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
//#define DEBUG

// The tft needs to be redefined here, for some reason.
Adafruit_ILI9341 *Definitions::tft;

// The nunchuk needs to be redefined here
ArduinoNunchuk *Definitions::nunchuk;

// Pointer to the current visible screen.
screen *Definitions::currentScreen;

// IRcomm needs to be redefined here
#ifdef IR
IRComm *Definitions::irComm;
#else
Stream *Definitions::irComm;
#endif


static int startRefresh = 0, refreshDone = 1;


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
*/
ISR(TIMER0_COMPA_vect)
{
#ifdef IR
	// If a bit wants to be sent...
	if (Definitions::irComm->bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ...for the specified bit to be sent...
		if (Definitions::irComm->bitSendCounter >= Definitions::irComm->bitSendType)
		{
			// Disable the 'let-through' pin for the IR LED, blocking the PWM signal
			PORTD |= (1 << PORTD4);
		}

		// Add one to pulsecounter
		Definitions::irComm->bitSendCounter++;

		// If the bit is completely sent...
		// (a bit is sent over 100 pulses)
		if (Definitions::irComm->bitSendCounter == 200)
		{
			// Signal that the bit is completely sent
			Definitions::irComm->bitSendComplete = 1;
			// Reset the counter
			Definitions::irComm->bitSendCounter = 0;
			// Disable the sending of a bit
			Definitions::irComm->bitSendEnabled = 0;
		}
	}
#endif
}

ISR(TIMER1_COMPA_vect)
{
	if (refreshDone)
	{
		startRefresh = 1;
	}
}

/* --Compare interrupt for receive timer--
 * When a bit is being received, a pin change interrupt will happen
 * This will enable the counter for this timer interrupt
 * This timer will count 'pulses' at the frequency of the sender
 * For more info, see the comment at the pin change interrupt
*/
ISR(TIMER2_COMPA_vect)
{
#ifdef IR
	if (Definitions::irComm->bitReceiveEnabled)
	{
		Definitions::irComm->bitReceiveCounter++;
		if(Definitions::irComm->bitReceiveStarted && 
			(Definitions::irComm->bitReceiveCounter - Definitions::irComm->bitReceiveStarted) > 95)
		{
			// Timed out...
			Definitions::irComm->bitReceiveStarted = 0;
		}
	}
#endif
}

// Receival Pin Change Interrupt
ISR(PCINT1_vect)
{
#ifdef IR
	// If the receiving has been enabled
	if (Definitions::irComm->bitReceiveEnabled)
	{
		// If the receive hasn't started yet
		if (!Definitions::irComm->bitReceiveStarted)
		{
			Definitions::irComm->bitReceiveStarted = Definitions::irComm->bitReceiveCounter;
		}
		else
		{
			// The message has ended...
			// Save at which count the receive has stopped
			Definitions::irComm->bitReceiveChanged = Definitions::irComm->bitReceiveCounter;
			// Process the data
			uint8_t lastBitReceived = 2;
			while(lastBitReceived == 2) // If, for some reason the bit is not received jet, try again.
			{
				lastBitReceived = Definitions::irComm->readByteIteration();
				if(lastBitReceived != 1)
				{
					Definitions::irComm->startReceiveBit();
				}
			}
		}
		if(PINC & (1 << PINC3))
			PORTB &= ~(1 << PORTB4);
		else
			PORTB |= (1 << PORTB4);
	}
#warning IR
#endif
}

void own_init(){
	DDRB |=
		(1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4)
		| (1 << DDB5);
	DDRD |=
		(1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5)
		| (1 << DDD6) | (1 << DDD7);
	DDRC |= (1 << DDC1);

	PORTC |= (1 << PORTC3) | (1 << PORTC2);
	

	TCCR1A =
		(0 << COM1A1) |
		(0 << COM1A0) |
		(0 << COM1B1) |
		(0 << COM1B0) |
		(0 << WGM11)  |
		(0 << WGM10)  ;
	TCCR1B =
		(0 << ICNC1) |
		(0 << ICES1) |
		(0 << WGM13) |
		(1 << WGM12) |
		(1 << CS12)  |
		(0 << CS11)  |
		(0 << CS10)  ;

	OCR1A = (uint16_t) 1562;
	TIMSK1 = (1 << OCIE1A);

	sei();
	
}

int main()
{
	PORTB = 0;

	// TODO: replace with own initialisation.
	// SEE: https://github.com/arduino/ArduinoCore-avr/blob/b084848f2eaf9ccb3ac9a64ac5492d91df4706bf/cores/arduino/wiring.c#L241
	// Default Arduino initialisation.
#warning Needs to be replaced
	own_init();

#ifndef IR
	// Initialize the tft
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

#warning not IR
	// Initialize the Nunchuk for player 1
	Definitions::nunchuk = new ArduinoNunchuk();
	Definitions::nunchuk->init();

	// Opening the homeScreen
	Definitions::currentScreen = new homeScreen();
	Definitions::currentScreen->begin();
#endif

#ifdef IR
	Serial.begin(500000);
	// Construct the irComm class
	Definitions::irComm = new IRComm();

	Serial.println("Morning!");
	Serial.print("This is ");
	Serial.print(PEEP);
	Serial.print(" speaking at ");
	Serial.print(PWMFREQ);
	Serial.println(" kHz");
	
	Definitions::irComm->startReadByte();
	Definitions::irComm->startReceiveBit();
	Serial.println(Definitions::irComm->read());
#else
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

	for (;;)
	{
		// Refresh screen
		if (startRefresh)
		{
			refreshDone = 0;
#ifdef IR
		if(Serial.available()){
			Definitions::irComm->write(Serial.read());
		}
			if(Definitions::irComm->available())
			{
				Serial.print(Definitions::irComm->available());
				Serial.print("\t");
				Serial.print(Definitions::irComm->peek(), HEX);
				Serial.print("\t");
				Serial.write(Definitions::irComm->read());
				Serial.print("\t");
				Serial.println(Definitions::irComm->available());
			}
#endif
#ifndef IR
			Definitions::currentScreen->refresh();
#endif
			startRefresh = 0;
			refreshDone = 1;
		}
		else
		{
			/* We need to do something in our loop for some reason, so we set
			 * the Power Reduction Register to the value of itself... */
			PRR = PRR;		
		}
	}
	PORTB = 0xFF;
	PORTD = 0xFF;
}
