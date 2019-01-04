#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <Stream.h>
#include <WString.h>

//#include "../../staticDefinitions.cpp"

#ifndef IRCOMM_H
#define IRCOMM_H

// Bit type definitions based on amount of pulses
#define START_BIT      80
#define STOP_BIT       60
#define ONE_BIT        40
#define ZERO_BIT       20

// Bit type identifier definitions
#define ZERO_TYPE		0
#define ONE_TYPE		1
#define START_TYPE		2
#define STOP_TYPE		3

// Size of the buffer.
#define BUFFER_SIZE	   50

// Indicates that debugging on a single Arduino is enabled
//#define SOLODEBUG

// PWMFREQ is defined in Static Definations, based on which player is playing

class IRComm: public Stream
{
  public:
	IRComm();

	// Variables defined based on what frequencies we are using
//	uint8_t SENDTOP;
//	uint8_t RECTOP;
//	uint8_t recTimerOverflow;

	// Variable definitions
	uint8_t bitSendEnabled = 0;
	uint16_t bitSendCounter = 0;
	uint8_t bitSendType = 0;
	uint8_t bitSendComplete = 0;

	uint16_t bitReceiveEnabled = 0;
	uint16_t bitReceiveCounter = 0;
	uint16_t bitReceiveStarted = 0;
	uint16_t bitReceiveChanged = 0;
	uint16_t bitReceiveComplete = 0;

	uint8_t typeReceived;
	uint8_t readByteCharacter = 0;
	uint8_t readByteIndex = 0;
	uint8_t readByteHasStarted = false;

	// Function definitions
	void sendBit(uint8_t sendType);
	void startReceiveBit();
	uint8_t ahandleReceiveBit();
	void startReadByte();
	int readByteIteration();
	char charbuffer[BUFFER_SIZE] = "";
	uint8_t writeIndex = 0;

	// Inherited from Stream
	size_t write(uint8_t);
	int available();
	int read();
	int peek();


//	AFTER REWITE OF CODE
	

  protected:

  private:
	void shiftbufferleft();
};

#endif
