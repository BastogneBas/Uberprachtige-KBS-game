// Defining includes
//#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include "../../staticDefinitions.cpp"

#ifndef IRCOMM_H
#define IRCOMM_H

// Defining used pins
#define LED_PIN         PORTB6
#define PCI_PIN         PORTD4

// Defining macros for the ledpin
#define ENABLE_LEDPIN   PORTB |= (1 << LED_PIN)
#define DISABLE_LEDPIN  PORTB &= ~(1 << LED_PIN)
#define TOGGLE_LEDPIN   PORTB ^= (1 << LED_PIN)

// Defining macros for the Pin Change Interrupt pin
#define RCPIN_HIGH     !(PORTD & (1 << PCI_PIN))

// Defining pulse spaces between bits
#define START_BIT      80
#define STOP_BIT       60
#define ONE_BIT        40
#define ZERO_BIT       20

// Defining types of bits
#define ZERO_TYPE		0
#define ONE_TYPE		1
#define START_TYPE		2
#define STOP_TYPE		3

// PWMFREQ is defined in Static Definations, based on which player is playing

class IRComm
{
  public:
	IRComm();
	// Defining variables
#if PWMFREQ == 38
	uint8_t SENDTOP = 209;
	uint8_t RECTOP = 142;
	//uint8_t RECTOP = 209;
	uint8_t recTimerOverflow = 0;

#elif PWMFREQ == 56
	uint8_t SENDTOP = 142;
	uint8_t RECTOP = 209;
	uint8_t recTimerOverflow = 0;

#else
	#error Invalid PWM Frequency
#endif

	/* Deprecated
	 * uint8_t step;
	 * uint8_t pulseCounter;
	 * uint16_t counter;
	 * uint8_t pulseTimerOn = false;
	 * uint8_t bitType;
	 * uint8_t receivedData;
	 * uint8_t measuredTime;
	 * uint8_t bitTimerRunning = 0;
	 * uint8_t bitTimerCounter;
	 * uint8_t bitTimerOverflow;
	 * uint8_t sendTimerInited = 0;
	 * uint8_t bitToSend;
	 * uint8_t typeToSend;
	 * uint8_t sendComplete;
	 */

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

	// Defining functions
	void sendBit(uint8_t sendType);
	void startReceive();
	void handleReceive();
  protected:

  private:

};

#endif
