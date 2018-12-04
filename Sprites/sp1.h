/*  GIMP header image file format (RGB): /home/bas/Dokumentoj/Uberprachtige-KBS-game/Sprites/sp1.h  */

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
	"!!!!!!!!!!!!!!!!\"1MB*U;-2(<-48`6/7@$2(<-*U;-\"1MB!!!!!!!!!!!!!!!!"
	"!!!!!!!!!!!!%B5G-F7=;;4R@,H^:*\\N:*\\N@,H^;;4R-67=#!UC!!!!!!!!!!!!"
	"!!!!!!!!!!!!'#645)<?A]$`B-(`B-(`B-(`B-(`A]$`5)<?'#64!!!!!!!!!!!!"
	"!!!!!!!!!!!!'3655)@@A]$`@\\T_@,H^@,H^@\\T_A]$`5)@@'365!!!!!!!!!!!!"
	"!!!!!!!!!!!!&2QZ0GK]:[4X;+8X;+8X;+8X;+8X:[4X0GK]&2QZ!!!!!!!!!!!!"
	"!!!!&R=8&R=8%R-4,%W;6Z,M4)(6/WP$/WP$4)(66Z,M,%W;%R-4!!!!(2U>!!!!"
	"$AY/?VN(HHFC1D5M*4&C.E2`-42903YS03YS-429.E2`*4&C1D5MH8>A@&V)$Q]0"
	"*S=HLY.EW[;&H86<345LSK71+S%<Z=CZZ=CZ+S%<SK71345LH86<W[;&LY.E*S=H"
	"!!!!1$%IJHJ=7EF;*TF`:UZ!NZ2_Q:6ZQ:6ZNZ2_:UZ!*TF`7EF;JHJ=0CYE!!!!"
	"!!!!!!!!2$)H*S1M(C-^*$*H1TR=4U.?4U.?1TR=*$*H(C-^*S1M2$)H!!!!!!!!"
	"!!!!\"Q=(%R-4(C:'8%QL;&E]*4:Q*U#**U#**4:Q;&E]8%QL(C:'%R-4#!A)!!!!"
	"!!!!&\"15(\"]O,%W@96JGM:%-)S%;'RM<'RM<)S%;M:%-96JG,%W@(\"]O$!Q-!!!!"
	"!!!!&\"15(SF28:4E::TF.'+U)D*H'RQB'RQB)D*H.'+U::TF8:4E(SF2&\"15!!!!"
	"!!!!*S=H'RM<+%6Z0W+2*$BI'RQ>'RM<'RM<'RQ>*$BI0W+2+%6Z'RM<*#1E!!!!"
	"!!!!'BI;&256'RM<'RM<'RM<'RM<'RM<'RM<'RM<'RM<'RM<'RM<%R-4%R-4!!!!"
	"!!!!!!!!%R-4*#1E%2%2&R=8'RM<'RM<'RM<'RM<&R=8%2%2*#1E%B)3!!!!!!!!"
	"";
