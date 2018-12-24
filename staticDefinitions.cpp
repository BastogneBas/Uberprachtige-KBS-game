#include "src/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include <stddef.h>
#include <string.h>
#include "Sprites.h"
#include "src/ArduinoNunchuk/ArduinoNunchuk.h"
#include "screen.h"
#include "src/IRComm/IRComm.h"

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define IR 1

//#define RECEIVER 1
//#define SENDER 1

#define PEEP 2
//#define PEEP = 2
#if PEEP == 1
#define PWMFREQ 38
#elif PEEP == 2
#define PWMFREQ 56
#endif

//#define DEBUG

class Definitions
{
  public:
	static const int TFT_DC = 9;
	static const int TFT_CS = 10;
	static const int SD_CS = 4;

	static const uint8_t gameHeight = 11;
	static const uint8_t gameWidth = 13;
	static const uint8_t SpriteHeight = 16;
	static const uint8_t SpriteWidth = 16;


#if PEEP == 1
	static const uint8_t currentPlayer = 1;
#elif PEEP == 2
	static const uint8_t currentPlayer = 2;
#endif

	static Adafruit_ILI9341 *tft;
	static ArduinoNunchuk *nunchuk;
	static screen *currentScreen;

#ifdef IR
	static IRComm *irComm;
#else
	static Stream *irComm;
#endif





	/* Set the text position at 0,0 and color at white on black
	 * FOR DEBUGGING */
	static void setTextDebug()
	{
		tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		tft->setCursor(0, 0);
		tft->setTextSize(1);
	}

	/* BEGIN PRINTs */
	/* SEE: https://github.com/arduino/ArduinoCore-avr/blob/all-master/cores/arduino/Print.h */
	static void print(const String s)
	{
		tft->print(s);
	}

	static void print(const char str[])
	{
		tft->print(str);
	}

	static void print(char c)
	{
		tft->print(c);
	}

	static void print(unsigned char b, int base)
	{
		tft->print(b, base);
	}

	static void print(int n, int base)
	{
		tft->print(n, base);
	}

	static void print(unsigned int n, int base)
	{
		tft->print(n, base);
	}

	static void print(long n, int base)
	{
		tft->print(n, base);
	}

	static void print(unsigned long n, int base)
	{
		tft->print(n, base);
	}

	static void print(double n, int digits)
	{
		tft->print(n, digits);
	}

	static void print(bool b, int base)
	{
		tft->print(b, base);
	}
	//static auto& print = static_cast<void>(tft->print);

	static void println(void)
	{
		tft->println();
	}

	static void println(const String & s)
	{
		tft->println(s);
	}

	static void println(const char c[])
	{
		tft->println(c);
	}

	static void println(char c)
	{
		tft->println(c);
	}

	static void println(unsigned char b, int base)
	{
		tft->println(b, base);
	}

	static void println(int num, int base)
	{
		tft->println(num, base);
	}

	static void println(unsigned int num, int base)
	{
		tft->println(num, base);
	}

	static void println(long num, int base)
	{
		tft->println(num, base);
	}

	static void println(unsigned long num, int base)
	{
		tft->println(num, base);
	}

	static void println(double num, int digits)
	{
		tft->println(num, digits);
	}

	static const uint8_t timer = 180;

	/* END PRINTs */
};

#endif
