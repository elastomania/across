
#define PICLOWTEST

// Ezeket kell hasznalni
#define SCREENXSIZE (320)
#define SCREENYSIZE (200)

void initmode_l( int soreltolas, int color  );
void closegraph_l( void );

void putbytes_l( int x, int y, unsigned char* tomb, int xsize );
void getbytes_l( int x, int y, unsigned char* tomb, int xsize );
void ppixel_l( int x, int y, int szin );
int gpixel_l( int x, int y );
