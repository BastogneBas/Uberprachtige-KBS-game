#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include "../../staticDefinitions.cpp"

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

// Indicates that debugging on a single Arduino is enabled
#define SOLODEBUG

// PWMFREQ is defined in Static Definations, based on which player is playing

class IRComm
{
  public:
	IRComm();
	
	// Variables defined based on what frequencies we are using
#if PWMFREQ == 38
	uint8_t SENDTOP = 209;

#ifdef SOLODEBUG
	uint8_t RECTOP = 209;
#else
	uint8_t RECTOP = 142;
#endif

	uint8_t recTimerOverflow = 0;

#elif PWMFREQ == 56
	uint8_t SENDTOP = 142;
	uint8_t RECTOP = 209;
	uint8_t recTimerOverflow = 0;

#else
	#error Invalid PWM Frequency
#endif

	// Variable definitions
	uint8_t bitSendEnabled = 0;
	uint8_t bitSendCounter = 0;
	uint8_t bitSendType = 0;
	uint8_t bitSendComplete = 0;
	uint8_t bitReceiveEnabled = 0;
	uint8_t bitReceiveCounter = 0;
	uint8_t bitReceiveStarted = 0;
	uint8_t bitReceiveChanged = 0;
	uint8_t bitReceiveComplete = 0;
	uint8_t typeReceived;

	// Function definitions
	void sendBit(uint8_t sendType);
	void startReceive();
	void handleReceive();
  
  protected:

  private:

};

#endif
