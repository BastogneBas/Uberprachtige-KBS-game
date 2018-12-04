#include "src/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include <stddef.h>
#include "Sprites.h"
#ifdef SD
#include <SD.h>
#endif

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//#define DEBUG

class Definitions
{
  public:
	static const int TFT_DC = 9;
	static const int TFT_CS = 10;
	static const int SD_CS = 4;

	static const uint8_t gameHeight = 11;
	static const uint8_t gameWidth = 13;
	static const uint8_t SpriteHeight=16;
	static const uint8_t SpriteWidth=16;

	static Adafruit_ILI9341 *tft;

	static void bmpDraw(char *filename, int16_t x, int16_t y){
		if(filename[0] == 'B' && filename[4] == 'K'){
			/*for(int16_t j=0,bindex=0;j<h;j++,y++){
				tft->startWrite();
				for(int16_t i=0;i<w;i++,bindex+=3){
					uint8_t r = pgm_read_byte(&blokje[bindex]);
					uint8_t g = pgm_read_byte(&blokje[bindex+1]);
					uint8_t b = pgm_read_byte(&blokje[bindex+2]);
					tft->writePixel(x+i, y, tft->color565(r,g,b));
				}
				tft->endWrite();
			}*/
			tft->drawRGBBitmap(x, y, blokje, SpriteWidth, SpriteHeight);
		}
		if(filename[0] == 'B' && filename[5] == 'L'){
			//tft->fillRect(x, y, 15, 15, ILI9341_MAROON);
			tft->drawRGBBitmap(x,y,ton,SpriteWidth,SpriteHeight);

		}if(filename[0] == 'P' && filename[4] == '1'){
			//tft->fillRect(x, y, 15, 15, ILI9341_BLUE);
			tft->drawRGBBitmap(x,y,peep1,SpriteWidth,SpriteHeight);
		}if(filename[0] == 'P' && filename[4] == '2'){
			//tft->fillRect(x, y, 15, 15, ILI9341_RED);
			tft->drawRGBBitmap(x,y,peep2,SpriteWidth,SpriteHeight);
		}
	}

#ifdef SD
#define BUFFPIXEL 20
// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.
	static void bmpDraw(char *filename, int16_t x, int16_t y)
	{

		File bmpFile;
		int bmpWidth, bmpHeight;	// W+H in pixels
		uint8_t bmpDepth;		// Bit depth (currently must be 24)
		uint32_t bmpImageoffset;	// Start of image data in file
		uint32_t rowSize;		// Not always = bmpWidth; may have padding
		uint8_t sdbuffer[3 * BUFFPIXEL];	// pixel buffer (R+G+B per pixel)
		uint8_t buffidx = sizeof(sdbuffer);	// Current position in sdbuffer
		boolean goodBmp = false;	// Set to true on valid header parse
		boolean flip = true;	// BMP is stored bottom-to-top
		int w, h, row, col, x2, y2, bx1, by1;
		uint8_t r, g, b;
		uint32_t pos = 0, startTime = millis();

		if ((x >= tft->width()) || (y >= tft->height()))
			 return;

		 Serial.println();
		 Serial.print(F("Loading image '"));
		 Serial.print(filename);
		 Serial.println('\'');

		// Open requested file on SD card
		if ((bmpFile = SD.open(filename)) == NULL)
		{
#ifdef DEBUG
			Serial.print(F("File not found"));
#endif
			return;
		}

		// Parse BMP header
		if (read16(bmpFile) == 0x4D42)
		{						// BMP signature
#ifdef DEBUG
			Serial.print(F("File size: "));
			Serial.println(read32(bmpFile));
#else
			read32(bmpFile);
#endif
			(void) read32(bmpFile);	// Read & ignore creator bytes
			bmpImageoffset = read32(bmpFile);	// Start of image data
#ifdef DEBUG
			Serial.print(F("Image Offset: "));
			Serial.println(bmpImageoffset, DEC);
			// Read DIB header
			Serial.print(F("Header size: "));
			Serial.println(read32(bmpFile));
#else
			read32(bmpFile);
#endif
			bmpWidth = read32(bmpFile);
			bmpHeight = read32(bmpFile);
			if (read16(bmpFile) == 1)
			{					// # planes -- must be '1'
				bmpDepth = read16(bmpFile);	// bits per pixel
#ifdef DEBUG
				Serial.print(F("Bit Depth: "));
				Serial.println(bmpDepth);
#endif
				if ((bmpDepth == 24) && (read32(bmpFile) == 0))
				{				// 0 = uncompressed

					goodBmp = true;	// Supported BMP format -- proceed!
#ifdef DEBUG
					Serial.print(F("Image size: "));
					Serial.print(bmpWidth);
					Serial.print('x');
					Serial.println(bmpHeight);
#endif
					// BMP rows are padded (if needed) to 4-byte boundary
					rowSize = (bmpWidth * 3 + 3) & ~3;

					// If bmpHeight is negative, image is in top-down order.
					// This is not canon but has been observed in the wild.
					if (bmpHeight < 0)
					{
						bmpHeight = -bmpHeight;
						flip = false;
					}

					// Crop area to be loaded
					x2 = x + bmpWidth - 1;	// Lower-right corner
					y2 = y + bmpHeight - 1;
					if ((x2 >= 0) && (y2 >= 0))
					{			// On screen?
						w = bmpWidth;	// Width/height of section to load/display
						h = bmpHeight;
						bx1 = by1 = 0;	// UL coordinate in BMP file
						if (x < 0)
						{		// Clip left
							bx1 = -x;
							x = 0;
							w = x2 + 1;
						}
						if (y < 0)
						{		// Clip top
							by1 = -y;
							y = 0;
							h = y2 + 1;
						}
						if (x2 >= tft->width())
							w = tft->width() - x;	// Clip right
						if (y2 >= tft->height())
							h = tft->height() - y;	// Clip bottom

						// Set TFT address window to clipped image bounds
						tft->startWrite();	// Requires start/end transaction now
						tft->setAddrWindow(x, y, w, h);

						for (row = 0; row < h; row++)
						{		// For each scanline...

							// Seek to start of scan line.  It might seem labor-
							// intensive to be doing this on every line, but this
							// method covers a lot of gritty details like cropping
							// and scanline padding.  Also, the seek only takes
							// place if the file position actually needs to change
							// (avoids a lot of cluster math in SD library).
							if (flip)	// Bitmap is stored bottom-to-top order (normal BMP)
								pos =
									bmpImageoffset + (bmpHeight - 1 -
													  (row +
													   by1)) * rowSize;
							else	// Bitmap is stored top-to-bottom
								pos =
									bmpImageoffset + (row + by1) * rowSize;
							pos += bx1 * 3;	// Factor in starting column (bx1)
							if (bmpFile.position() != pos)
							{	// Need seek?
								tft->endWrite();	// End TFT transaction
								bmpFile.seek(pos);
								buffidx = sizeof(sdbuffer);	// Force buffer reload
								tft->startWrite();	// Start new TFT transaction
							}
							for (col = 0; col < w; col++)
							{	// For each pixel...
								// Time to read more pixel data?
								if (buffidx >= sizeof(sdbuffer))
								{	// Indeed
									tft->endWrite();	// End TFT transaction
									bmpFile.read(sdbuffer,
												 sizeof(sdbuffer));
									buffidx = 0;	// Set index to beginning
									tft->startWrite();	// Start new TFT transaction
								}
								// Convert pixel from BMP to TFT format, push to display
								b = sdbuffer[buffidx++];
								g = sdbuffer[buffidx++];
								r = sdbuffer[buffidx++];
								//if(r==0x00&&g==0x00&&b==0x00)
								tft->writePixel(tft->color565(r, g, b));
							}	// end pixel
						}		// end scanline
						tft->endWrite();	// End last TFT transaction
					}			// end onscreen
#ifdef DEBUG
					Serial.print(F("Loaded in "));
					Serial.print(millis() - startTime);
					Serial.println(" ms");
#endif
				}				// end goodBmp
			}
		}

		bmpFile.close();
#ifdef DEBUG
		if (!goodBmp)
			Serial.println(F("BMP format not recognized."));
#endif
	}



// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

	static uint16_t read16(File & f)
	{
		uint16_t result;
		((uint8_t *) & result)[0] = f.read();	// LSB
		((uint8_t *) & result)[1] = f.read();	// MSB
		return result;
	}

	static uint32_t read32(File & f)
	{
		uint32_t result;
		((uint8_t *) & result)[0] = f.read();	// LSB
		((uint8_t *) & result)[1] = f.read();
		((uint8_t *) & result)[2] = f.read();
		((uint8_t *) & result)[3] = f.read();	// MSB
		return result;
	}
#endif
};

#endif
