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
//#ifdef IR
//IRComm *Definitions::irComm;
//#else
Stream *Definitions::irComm;
//#endif

uint8_t Definitions::timerCounter;
uint8_t Definitions::currentTime;

// Reset refreshing
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
 *
 * (is only enabled when IR is enabled) */
ISR(TIMER0_COMPA_vect)
{
	((IRComm*)(Definitions::irComm))->timer0ISR();
}

ISR(TIMER1_COMPA_vect) // Timer1 output compare interrupt
{
	if(refreshDone)
	{
		startRefresh = 1;
	}
	Definitions::timerCounter++;
	if(Definitions::timerCounter >= 39)
	{
		Definitions::currentTime--;
		Definitions::timerCounter = 0;
	}
}

/* --Compare interrupt for receive timer--
 * When a bit is being received, a pin change interrupt will happen
 * This will enable the counter for this timer interrupt
 * This timer will count 'pulses' at the frequency of the sender
 * For more info, see the comment at the pin change interrupt
 *
 * (is only enabled when IR is enabled) */
ISR(TIMER2_COMPA_vect)
{
	((IRComm*)(Definitions::irComm))->timer2ISR();
}

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
	((IRComm*)(Definitions::irComm))->pcint1ISR();
}

void own_init(){
	// Set PINs to output
	DDRB |=
		(1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4)
		| (1 << DDB5);
	DDRD |=
		(1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5)
		| (1 << DDD6) | (1 << DDD7);
	DDRC |= (1 << DDC1);

	PORTC |= (1 << PORTC3) | (1 << PORTC2);

	/* --Interrupt for timer1--
	 * The timer works in CTC mode
	 * With a prescaler of 256
	 * The top (OCR1A) will be set to
	 * 1562 so the timer will be ticking
	 * 100 times/second.
	 */

	TCCR1A =
		(0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) |
		(0 << COM1B0) | (0 << WGM11)  | (0 << WGM10);
	TCCR1B =
		(0 << ICNC1) | (0 << ICES1) | (0 << WGM13) |
		(1 << WGM12) | (1 << CS12)  | (0 << CS11)  |
		(0 << CS10);

	OCR1A = (uint16_t) 1562;
	TIMSK1 = (1 << OCIE1A);

	// Enable global interupts
	sei();
}

int main()
{
	// TODO: replace with own initialisation.
	// SEE: https://github.com/arduino/ArduinoCore-avr/blob/b084848f2eaf9ccb3ac9a64ac5492d91df4706bf/cores/arduino/wiring.c#L241
	// Default Arduino initialisation.
#warning Needs to be replaced
	own_init();
	//Serial.begin(500000);
	Definitions::irComm = new IRComm();
	Definitions::irComm->println('\n');

//#ifndef IR
	// Initialize the tft
	Definitions::tft =
		new Adafruit_ILI9341(Definitions::TFT_CS, Definitions::TFT_DC);
#ifdef TFT
#warning TFT
	Definitions::tft->begin();
	yield();
	Definitions::tft->setRotation(1);
	Definitions::tft->fillScreen(ILI9341_BLACK);
#endif

#ifdef DEBUG
	// Turn on Serial communication if we are debugging	
	Definitions::println("Welkom!");
#endif

	// Initialize the Nunchuk for player 1
	//Definitions::printScreenSize((uint16_t)Definitions::nunchuk);
	Definitions::nunchuk = new ArduinoNunchuk();
	Definitions::nunchuk->init();

#ifdef TFT
	// Opening the homeScreen
	//Definitions::printScreenSize((uint16_t)Definitions::currentScreen);
	Definitions::currentScreen = new homeScreen();
	Definitions::currentScreen->begin();
	Definitions::printScreenSize((uint16_t)Definitions::currentScreen);

#endif

#ifdef IR
//	Serial.setTimeout(12);
	// Construct the irComm class

	Definitions::irComm->println("Morning!");
//	Serial.print("This is ");
//	Serial.print(PEEP);
//	Serial.print(" speaking at ");
//	Serial.print(PWMFREQ);
//	Serial.println(" kHz");
	
	//Definitions::irComm->startReadByte();
	//Definitions::irComm->startReceiveBit();
	//Definitions::irComm->println(Definitions::irComm->read());
#else
	Definitions::irComm =
		new HardwareSerial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C,
						   &UDR0);
	((HardwareSerial *) (Definitions::irComm))->begin(9600);

#if PEEP == 1
	char mystr[] = "hello";
	Definitions::irComm->print(mystr);
	//delay (1000);

#else
	char mystr[10];
	Definitions::irComm->readBytes(mystr, 5);
	Definitions::irComm->println(mystr);
	//delay(1000);
#endif

#endif

	while (true)
	{
		//Serial.println("Refresh");
		// Refresh screen
		if (startRefresh)
		{
			refreshDone = 0;
			//if(Definitions::irComm->available())
			//{
			//	Definitions::irComm->write(Definitions::irComm->read());
			//}
//			if (Serial.available()){
//				char buffer[64] = {0};
//				Serial.readBytes(buffer, Serial.available());
//				Serial.println(buffer);
//				Definitions::irComm->print(buffer);
//				Serial.println("Done");
//			}
//
//			if(Definitions::irComm->available())
//			{
//				Serial.print(Definitions::irComm->available());
//				Serial.print("\t");
//				Serial.print(Definitions::irComm->peek(), HEX);
//				Serial.print("\t");
//				Serial.write(Definitions::irComm->read());
//				Serial.println();
//				//Serial.print("\t");
//				//Definitions::irComm->println(Definitions::irComm->available());
//			}
        #ifdef TFT
//			Definitions::irComm->print("\t0x");
//			Definitions::irComm->print((uint16_t)Definitions::currentScreen, HEX);
//			Definitions::irComm->print("\t0x");
//			Definitions::irComm->println(sizeof(((Definitions::currentScreen))), HEX);
			Definitions::currentScreen->refresh();
		#endif
			startRefresh = 0;
			refreshDone = 1;
		}
		else
		{
			/* We need to do something in our loop for some reason, so we set
			 * the Power Reduction Register to the value of itself... */
			//asm volatile ("nop");
			PRR = PRR;
		}
	}
}
