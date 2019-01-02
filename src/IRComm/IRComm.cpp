#include "IRComm.h"
#include <util/delay.h>
#include <Wire.h>
#include "../../staticDefinitions.cpp"

IRComm::IRComm()
{
	#if PWMFREQ == 38
	IRComm::SENDTOP = 210;
	IRComm::RECTOP = 142;
	IRComm::recTimerOverflow = 0;
	#elif PWMFREQ == 56
	IRComm::SENDTOP = 143;
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
		// Do something useless, because the arduino can't handle empty whiles for some reason
		PORTB = PORTB;
	}

	// Disable digital PIN 1 to indicate that the sending is done
	PORTC &= ~(1 << PORTC1);
}

// Reset the receival and indicate that it has started
void IRComm::startReceiveBit()
{
	bitReceiveChanged = 0;
	bitReceiveStarted = 0;
	bitReceiveCounter = 0;
	bitReceiveComplete = 0;
	bitReceiveEnabled = 1;
}

// Process the received data
uint8_t IRComm::handleReceiveBit()
{
	// Stop the receival of data
	bitReceiveEnabled = 0;
	bitReceiveComplete = 1;
	uint8_t diff = bitReceiveChanged-bitReceiveStarted;
	if((diff >= 15) && (diff <= 30))
	{
		return ZERO_BIT;
	}
	else if ((diff >= 35) && (diff <= 50))
	{
		return ONE_BIT;
	}
	else if ((diff >= 55) && (diff <= 70))
	{
		return STOP_BIT;
	}
	else if ((diff >= 75) && (diff <= 90))
	{
		return START_BIT;
	}
	else
	{
		return -1;
	}

}

size_t IRComm::write(uint8_t byte){
	// We send half duplex, so wait til we're done receiving
	while(readByteHasStarted)
		PRR = PRR;
	bitReceiveEnabled = 0;

	sendBit(START_BIT);
	for(uint8_t i=0;i<8;i++){
		sendBit(byte & (1 << (7-i)) ? ONE_BIT : ZERO_BIT);
	}
	sendBit(STOP_BIT);

	// And go back to receiving mode
	readByteStart();
	startReceiveBit();
	return 0;
}

int IRComm::available(){
	return String(charbuffer).length();
}

void IRComm::readByteStart()
{
	readByteIndex = 0;
	readByteCharacter = 0;
	readByteHasStarted = false;
}

int IRComm::readByteIteration()
{
	if(bitReceiveStarted && bitReceiveChanged)
	{
	uint8_t bit = handleReceiveBit();
	if (bit == 255)
	{
		return 0; // We have something invalid. Probably noise...
	}
	else if (bit == 80 && readByteIndex == 0 && readByteHasStarted == false)
	{
		readByteHasStarted = true;
	}
	else if (bit == 80 && readByteIndex != 0 && readByteHasStarted == false)
	{
		//Serial.print("Randomly received START_BIT at ");
		//Serial.println(index);
	}
	else if (bit != 80 && readByteIndex == 0 && readByteHasStarted == false)
	{
		//Serial.println("Wait, this isn't a START_BIT");
	}
	else if (bit == 40)
	{
		readByteCharacter |= (1 << (7-readByteIndex));
		readByteIndex++;
	}
	else if (bit == 20)
	{
		readByteCharacter |= 0; // Basically does nothing
		readByteIndex++;
	}
	else if (bit == 60 && readByteIndex == 8)
	{
		if(writeIndex < BUFFER_SIZE)
		{
			charbuffer[writeIndex] = readByteCharacter;
			writeIndex++;
		}
		else
		{
			Serial.println("\033[91m"); // We like some fancy colors in our terminal
			Serial.println("Welp! Our buffer is full... So kind of ArrayIndexOutOfBoundsException...");
			Serial.print("Index is: ");
			Serial.println(writeIndex);
			Serial.println("Please read something, or receive nothing...");
			Serial.print("\033[m");
		}
		readByteStart(); // Read next byte
		startReceiveBit(); // Start receiving next bit
		return 1;
	}
	else if (bit == 60 && readByteIndex != 8)
	{
		Serial.println("\033[91m");
		Serial.print("Hmmm, the byte is sent, but we only have ");
		Serial.print(readByteIndex);
		Serial.println(" bits");
		Serial.println("retrying");
		Serial.print("\033[m");
		readByteStart();
	}
	return 0;
	}
	else
	{
		return 2;
	}
}

void IRComm::shiftbufferleft()
{
	if(writeIndex != 0)
	{
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		charbuffer[i] = charbuffer[i+1];
		writeIndex--;
		if(charbuffer[i+1] == 0x00)
			break;
	}
	}
}

int IRComm::read(){
	// The receiver has received something, so there will be written to the buffer.
	// It isn't safe to read right now... Wait for it.
	while(readByteHasStarted)
		PRR=PRR;
	char ret = charbuffer[0];
	shiftbufferleft();
	return ret;
}

int IRComm::peek(){
	while(readByteHasStarted)
		PRR=PRR;
	return charbuffer[0];
}
