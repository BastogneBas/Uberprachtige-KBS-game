/*  GIMP header image file format (RGB): /home/bas/Dokumentoj/Uberprachtige-KBS-game/Sprites/Ton.h  */

static unsigned int width = 16;
static unsigned int height = 16;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}
static char *header_data =
	"7D5W>52'@%N.B667BV:9BV:9BV:9BV:9BV:9BV:9BV:9BV:9B667@%N.>52'74-U"
	"7D%S?5.%@%:(B%R.CV*4D&.5D&.5D&*5D&*5D&*5D&*5CV*4B%R/@%:(?5.%7D%S"
	")R56?E.%@%6'A%B*F&F;M(*ULH\"SGVV@GVV@GVV@GVV@DV26A5F+@%6'?E.%*297"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	")BM<<U6'=5:(>%F+A626FG6HF'2GBV>:BV>:BV>:BV>:@F&3>5F+=5:(<U6')BM<"
	"*2U>AV&3BF*4C667GG2FN(F\\MHB[I7>JI7>JI7>JI7>JFF^ACF:8BF*4AV&3*2U>"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	"*2U>AV&3BF*4C667GG2FN(F\\MHB[I7>JI7>JI7>JI7>JFF^ACF:8BF*4AV&3*2U>"
	")BM<<U6'=5:(>%F+A626FG6HF'2GBV>:BV>:BV>:BV>:@F&3>5J+=5:(<U6')BM<"
	"*\"M<?E2&@56'A5F+FFN=N(6XMH.VHF^BHF^BHF^BHF^BE6:8AUJ,@56'?E2&*\"M<"
	",3!A?E.%@%6'A%B*F&F;M(*ULH\"SGVV@GVV@GVV@GVV@DV26A5F+@%6'?E.%,C!B"
	";$U_C%^1D667F&R>FFR?FVV@FVV@FFV?FFV?FFV?FFV?F6R?F&R>D667C%^1;$U_"
	"43IL9T=Y;$M]=U6'@%N-@%N-@%N-@%N-@%N-@%N-@%N-@%N->%6';4M]9T=Y3SAJ"
	"";
