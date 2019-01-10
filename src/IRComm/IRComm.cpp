#include "../../staticDefinitions.cpp"
#include "IRComm.h"
#include <util/delay.h>
#include <Wire.h>

#ifdef DEBUG
#pragma message "Debugging!"
#endif

// *** Constructor *** //
IRComm::IRComm()
{
	/* --Initialize the send timer--
	 * Uses Timer0
	 * Toggles OC0A (Digital PIN 6) on compare match
	 * Operates in Fast PWM mode with OCRA as TOP
	 * Runs at the frequency defined in staticDefinitions.cpp
	 * Runs without prescaler
	*/
	TCCR0A = (1 << COM0A0) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << WGM02);

	// Set top based on which frequency is sending
#if PWMFREQ == 38
	OCR0A = 210;
#elif PWMFREQ == 56
	OCR0A = 143;
#else
#error Invalid PWM frequency
#endif

	// Enable timer0 compare interrupts
	TIMSK0 = (1 << OCIE0A);


	/* --Initialize the receive timer--
	 * Uses Timer2
	 * Toggles OC2A (Digital PIN 11) on compare match
	 * Operates in Fast PWM mode with OCRA as TOP
	 * Runs at the frequency defined in staticDefinitions.cpp
	 * Runs without prescaler
	*/
	TCCR2A = (1 << COM2A0) | (1 << WGM20) | (1 << WGM21);
	TCCR2B = (1 << CS20) | (1 << WGM22);

	// Set top based on which frequency is receiving
#if PWMFREQ == 38
	OCR2A = 142;
#elif PWMFREQ == 56
	OCR2A = 209;
#else
#error Invalid PWM frequency
#endif

	// Enable timer2 compare interrupts
	TIMSK2 = (1 << OCIE2A);


	// --Pin Change interrupts for receiving data--
	// Enable pin change interrupts for PCINT[15:8]
	PCICR = (1 << PCIE1);
	// Enable pin change interrupts on Analog PIN 3
	PCMSK1 = (1 << PCINT11);

	// Enable global interrupts
	sei();

	// Prepare receiving data
	startReadByte();
	startReceiveBit();
	read();
}


// *** Sending *** //
// Inherited from Stream
// Writes one byte
size_t IRComm::write(uint8_t byte){
	// We send half duplex, so wait til we're done receiving
	while(readByteHasStarted)
		PRR = PRR;
	// Disable receiving of bytes
	bitReceiveEnabled = 0;

	// Send a start-bit
	sendBit(START_BIT);
	// Send the byte
	for(uint8_t i=0;i<8;i++){
		sendBit(byte & (1 << (7-i)) ? ONE_BIT : ZERO_BIT);
	}
	// Send the stop-bit
	sendBit(STOP_BIT);

	// And go back to receiving mode
	startReadByte();
	startReceiveBit();
	return 1;
}

// Timer function
void IRComm::timer0ISR()
{
	// If a bit wants to be sent...
	if (bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ...for the specified bit to be sent...
		if (bitSendCounter >= bitSendType)
		{
			// Disable the 'let-through' pin for the IR LED, blocking the PWM signal
			PORTD |= (1 << PORTD4);
		}

		// Add one to pulsecounter
		bitSendCounter++;

		// If the bit is completely sent...
		// (a bit is sent over 100 pulses)
		if (bitSendCounter == 150)
		{
			// Signal that the bit is completely sent
			bitSendComplete = 1;
			// Reset the counter
			bitSendCounter = 0;
			// Disable the sending of a bit
			bitSendEnabled = 0;
		}
	}
}

// Sends one bit
void IRComm::sendBit(uint8_t sendType)
{
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
		PRR = PRR;
	}
}

// *** Receiving *** //
// Timer 2 Compare Interrupt
void IRComm::timer2ISR()
{
	// If receiving has been enabled
	if (bitReceiveEnabled)
	{
		// Increment the counter
		bitReceiveCounter++;

		// If no stop bit has been received after 95 ticks
		if(bitReceiveStarted && (bitReceiveCounter - bitReceiveStarted) > 95)
		{
			// Timed out...
			bitReceiveStarted = 0;
		}
	}
}

// Pin Change Interrupt
void IRComm::pcint1ISR()
{
	// If the receiving has been enabled
	if (bitReceiveEnabled)
	{
		// If the receive hasn't started yet
		if (!bitReceiveStarted)
			bitReceiveStarted = bitReceiveCounter;
		else
		{
			// The message has ended...
			// Save at which count the receive has stopped
			bitReceiveChanged = bitReceiveCounter;
			// Process the data
			uint8_t lastBitReceived = 2;

			while(lastBitReceived == 2) // If, for some reason the bit is not received yet, try again.
			{
				lastBitReceived = readByteIteration();
				if(lastBitReceived != 1)
					startReceiveBit();
			}
		}
	}
}
// Reset the receiving and indicate that it has started
void IRComm::startReceiveBit()
{
	bitReceiveChanged = 0;
	bitReceiveStarted = 0;
	bitReceiveCounter = 0;
	bitReceiveComplete = 0;
	bitReceiveEnabled = 1;
}

// Prepare for receiving the next byte
void IRComm::startReadByte()
{
	readByteIndex = 0;
	readByteCharacter = 0;
	readByteHasStarted = false;
}

// Process received bit data
int IRComm::readByteIteration()
{
	// Check if we have received a bit.
	if(bitReceiveStarted && bitReceiveChanged)
	{
		// Yes we have.
		// Stop the receival of data
		bitReceiveEnabled = 0;
		bitReceiveComplete = 1;
		// Calculate the difference between start and the end of the receival.
		uint8_t diff = bitReceiveChanged-bitReceiveStarted;
		//Serial.println(diff);
		// And do something with it
		if (((diff >= 75) && (diff <= 90)) && readByteIndex == 0 && readByteHasStarted == false)
		{
			// We have received a START_BIT, so we can start receiving.
			readByteHasStarted = true;
		}
		else if (((diff >= 75) && (diff <= 90)) && readByteIndex != 0 && readByteHasStarted == true)
		{
			// We received a START_BIT while the receiving of this byte has already started
#ifdef DEBUG
			Serial.println("\033[91mWe received a START_BIT, but the byte has already been started...\033[m");
#endif
		}
		else if (((diff >= 75) && (diff <= 90)) && readByteIndex == 0 && readByteHasStarted == false)
		{
			// We have received a valid bit, but we expected a START_BIT
#ifdef DEBUG
			Serial.println("\033[91mWe received a bit other than START_BIT, but we expected a START_BIT...\033[m");
#endif
		}
		else if ((diff >= 35) && (diff <= 50))
		{
			// We have received a valid 1 (one) bit to append to our byte
			readByteCharacter |= (1 << (7 - readByteIndex));
			readByteIndex++;
		}
		else if ((diff >= 15) && (diff <= 30))
		{
			// We have received a valid 0 (zero) bit to append to our byte
			readByteCharacter &= ~(0 << (7 - readByteIndex)); // Basically does nothing
			readByteIndex++;
		}
		else if (((diff >= 55) && (diff <= 70)) && readByteIndex == 8)
		{
			// We have received a STOP_BIT and we have a eight bits, so we have finished our byte
			// Let's check if the writebuffer hasn't already been filled
			if(writeIndex < BUFFER_SIZE)
			{
				// Nope, so we can append our byte to it.
				charbuffer[writeIndex] = readByteCharacter;
				writeIndex++;
			}
			else
			{
				// Yikes the buffer is filled. Well, let's throw away our byte and cry 😭
				uint8_t bufferlength = String(charbuffer).length();
#ifdef DEBUG
				Serial.println("\033[91m"); // We like some fancy colors in our terminal
				Serial.println("Welp! Our buffer is full... So kind of ArrayIndexOutOfBoundsException...");
				Serial.print("Index is: ");
				Serial.println(writeIndex);
				Serial.println("Please read something, or receive nothing...");
				Serial.print("Last received character:\t");
				Serial.write(readByteCharacter);
				Serial.print("\t0x");
				Serial.println(readByteCharacter, HEX);
				Serial.print("The buffer is:\t");
				Serial.println(charbuffer);
				Serial.print("The code of the first byte is:\t0x");
				Serial.println(charbuffer[0], HEX);
				Serial.print("The index of the first byte with 0x00 is: ");
				Serial.println(bufferlength);
				Serial.println("Setting writeIndex to this value");
				Serial.print("\033[m");
#endif
				writeIndex = bufferlength;
			}
			startReadByte(); // Read next byte
			startReceiveBit(); // Start receiving next bit
			return 1;
		}
		else if (((diff >= 55) && (diff <= 70)) && readByteIndex != 8)
		{
			// Yikes, the sender has sent all bits, but we haven't received all of them...
#ifdef DEBUG
			// Now cry 😭
			Serial.println("\033[91m");
			Serial.print("Hmmm, the byte is sent, but we only have ");
			Serial.print(readByteIndex);
			Serial.println(" bits");
			Serial.println("retrying");
			Serial.print("\033[m");
#endif
			// Prepair for receiving a new byte
			startReadByte();
		}
		return 0;
	}
	else
	{
		// Hey, we haven't received everything...
		return 2;
	}
}

// Shift the charbuffer one character to the left.
void IRComm::shiftbufferleft()
{
	if(writeIndex != 0)
	{
		for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		{
			charbuffer[i] = charbuffer[i+1];
			if(charbuffer[i] == 0x00)
				break;
		}
		writeIndex--;
	}
}

// *** Reading from buffer *** //
// Checks the amount of bytes waiting to be read.
int IRComm::available(){
	return String(charbuffer).length();
}

// Reads one byte, and marks it as read, so we shift it one place to the right
int IRComm::read(){
	// The receiver has received something, so there will be written to the buffer.
	// It isn't safe to read right now... Wait for it.
	while(readByteHasStarted)
		PRR=PRR;
	char ret = charbuffer[0];
	shiftbufferleft();
	return ret;
}

// Reads one byte, but doesn't do anything with it.
int IRComm::peek(){
	while(readByteHasStarted)
		PRR=PRR;
	return charbuffer[0];
}
