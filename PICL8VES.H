
//#define PICLOWTEST // Csak hibat jelez. Bar o vizsgalja csak kiirast, de
					 // csak pic15.cpp-n keresztul lehet hivni, aminek kell
					 // kiirast vizsgalnia.

// res: 320x200 -> 1, 320x240 - > 2, 640x400 -> 3, 640x480 -> 4

// O allitja be Pscr15 x es ysize mereteit:
void setmode8_l( int res );
void settextmode( void );

void putline8_l( int x, int y, unsigned char* tomb, int xsize );
void getline8_l( int x, int y, unsigned char* tomb, int xsize );

void putpixel8_l( int x, int y, unsigned char szin );

// Fizikai kepernyore mutat:
extern pic8* Pscr8;

// Csak teszt celokra:
//void probalow15( void );

