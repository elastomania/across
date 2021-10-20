// Ez egy olyan kep, aminek nincsenek hatarai, de mindig csak egy reszletet
// tudja megjegyezni, aminek maximalis meretei bemeno size parameterek!:

// Koordinatak mindig nagy koordinatak!

class pic8;

class pic8f {
// torlendo
public:
	pic8* ppic;
	int x0, y0; // x0  0 es xsize-1 kozott van.
	int xsize, ysize; // Kis meretek.
public:
		pic8f( int xsize, int ysize );
		~pic8f( void );
	int getxsize( void );
	int getysize( void );

	unsigned char gpixel( int x, int y );
	void ppixel( int x, int y, unsigned char szin );

	void vizvonalcpy( int x, int y, unsigned char* tomb );
	void fuggvonalcpy( int x, int y, unsigned char* tomb );

	void place( int x, int y );
	// A kovetkezoek place alapjan dolgoznak:
	void vizvonalset( int x, int y, int xsize, unsigned char szin );
	void csakegyblt( pic8* pp, int x, int y );
	void csakegyblt( pic8* pp, int x, int y, int x1, int y1, int x2, int y2 );
	void freshrectangle( int x1, int y1, int x2, int y2 );
	unsigned char* getcim( int x, int y );

	void fuggszegmens( int x, int y, int size, unsigned char szin );
	void vizszegmens( int x, int y, int size, unsigned char szin );
};

class magassag;
class texture;

struct st320;

// Bal also sarok koordinatajanak a valtozasa:
// Ha x1 vagy y1 kisebb -10000, egeszet ujra csinalja:
void tolpic8f( int x1, int y1, int x2, int y2, pic8f* pf,
			magassag* pmviz, magassag* pmfugg,
			unsigned char viewelo, unsigned char viewhat );

class palyaleiro;


