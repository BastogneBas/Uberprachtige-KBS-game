#include "IRComm.h"
#include <util/delay.h>
#include <Wire.h>
#include "../../staticDefinitions.cpp"

IRComm::IRComm()
{
	#if PWMFREQ == 38
	IRComm::SENDTOP = 209;
	IRComm::RECTOP = 142;
	IRComm::recTimerOverflow = 0;
	#elif PWMFREQ == 56
	IRComm::SENDTOP = 142;
	IRComm::RECTOP = 209;
	IRComm::recTimerOverflow = 0;
	#else
	#error Invalid PWM frequency
	#endif

/* --Initialize the send timer--
 * Uses Timer0
 * Toggles OC0A (Digital PIN 6) on compare match
 * Operates in Fast PWM mode with OCRA as TOP
 * Compares with SENDTOP (defined in IRComm.h)
 * Runs at the frequency defined in staticDefinitions.cpp
 * Runs without prescaler
*/
	TCCR0A = (1 << COM0A0) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << WGM02);
	OCR0A = SENDTOP;
	// Enable timer0 compare interrupts
	TIMSK0 = (1 << OCIE0A);

/* --Initialize the receive timer--
 * Uses Timer2
 * Toggles OC2A (Digital PIN 11) on compare match
 * Operates in Fast PWM mode with OCRA as TOP
 * Compares with RECTOP (defined in IRComm.h)
 * Runs at the frequency defined in staticDefinitions.cpp
 * Runs without prescaler
*/
	TCCR2A = (1 << COM2A0) | (1 << WGM20) | (1 << WGM21);
	TCCR2B = (1 << CS20) | (1 << WGM22);
	OCR2A = RECTOP;
	// Enable timer2 compare interrupts
	TIMSK2 = (1 << OCIE2A);

	// --Pin Change interrupts for receiving data--
	// Enable pin change interrupts for PCINT[15:8]
	PCICR = (1 << PCIE1);
	// Enable pin change interrupts on Analog PIN 3
	PCMSK1 = (1 << PCINT11);
	sei();
}


void IRComm::sendBit(uint8_t sendType)
{
		PORTC |= (1 << PORTC1);
		Serial.print("Send ");
		Serial.println(sendType);
/* --Sending a bit--
 * Uses timer0 as defined above
 * Timer0 will always be running and comparing...
 * ...at the frequency defined in staticDefinitions.cpp
 * If a bit will be sent/the signal will be let through to the LED...
 * ...is defined by the enabling or disabling of Digital PIN 4
 *
 * If PIN4 is high, the LED doesn't have a connection to ground...
 * ...which means that the LED won't be burning
 * If PIN4 is low however, the LED will burn at the defined frequency
 *
 * This function also handles properly initiating the sending of a bit
*/
	// Reset the sendBit counters
	bitSendComplete = 0;
	bitSendCounter = 0;
	// Set which type of bit to send
	bitSendType = sendType;

	// Allow the LED to burn
	PORTD &= ~(1 << PORTD4);

	// Indicate that a bit wants to be sent
	bitSendEnabled = 1;

	// Wait for the bit to be completely sent
	while (!bitSendComplete)
	{
#ifdef DEBUG
		// Enable digital PIN 1 to indicate that the bit is still sending
		PORTC |= (1 << PORTC1);
#endif

		// Do something useless, because the arduino can't handle empty whiles for some reason
		PORTB = PORTB;
	}

	// Disable digital PIN 1 to indicate that the sending is done
	PORTC &= ~(1 << PORTC1);
	Serial.println("Send complete");
}

// Reset the receival and indicate that it has started
void IRComm::startReceive()
{
	bitReceiveChanged = 0;
	bitReceiveStarted = 0;
	bitReceiveCounter = 0;
	bitReceiveComplete = 0;
	bitReceiveEnabled = 1;
}

void IRComm::receiveBit()
{
	startReceive();
	while(true)
	{
		if(bitReceiveStarted && bitReceiveChanged)
		{
			Serial.println(handleReceive());
			break;
		}
		else
		{
			//asm volatile ("nop");
			Serial.print("");
		}
	}
}

// Process the received data
uint8_t IRComm::handleReceive()
{
	// Stop the receival of data
	bitReceiveEnabled = 0;
	bitReceiveComplete = 1;
	uint8_t diff = bitReceiveChanged-bitReceiveStarted;
	if((diff >= 20) && (diff <= 30))
	{
		return 0;
	}
	else if ((diff >= 40) && (diff <= 50))
	{
		return 1;
	}
	else
	{
		return -1;
	}

}

size_t IRComm::write(uint8_t byte){
	return 0;
}

int IRComm::available(){
	return 0;
}

int IRComm::read(){
	return 0;
}

int IRComm::peek(){
	return 0;
}
