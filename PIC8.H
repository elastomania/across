
// Ezt nyugodtan ki lehet venni, ha nem jonnek hibauzenetek:
#define PIC8TEST

typedef unsigned char* puchar;

class pic8 {
public:
	// Baratok:
	friend void blt8( pic8* pd, pic8* ps, int x = 0, int y = 0,
		int x1 = -10000, int y1 = -10000, int x2 = -10000, int y2 = -10000 );
	//friend void blt8fizrefejjelle( pic8* ps );
	friend void spriteosit( pic8* ppic, int szin );
	friend void pic8f::csakegyblt( pic8* pp, int x, int y );
	friend void setmode8_l( int res );
protected:
	// Sajat:
	int fizkep;
	int nemdestrukt;
	int lefoglal( long xsize, long ysize );
	void voxbeolvas( char* nev );
	int voxsave( char* nev );
	void mixbeolvas( char* nev );
	void spritebeolvas( char* nev, FILE* h );
	int spritesave( char* nev, FILE* h );
	void pcxbeolvas( char* nev );
	int pcxsave( char* nev, unsigned char* pal );
public: // Csak gany miatt public!
	short xsize, ysize;
	puchar* sormuttomb;
private:
	puchar* szegmuttomb;
	long     szegnum;
	// Sprite reszek:
	unsigned char* sprite;
	unsigned short spritehossz;
public:
	int success;
	pic8( int xsize, int ysize );
	// Mar meglevo tombre epiti ra pic8-at, destruktor nem torli:
	pic8( int xsize, int ysize, unsigned char** ppc );
	// VOX ,MIX, SPR es PCX-eket ismeri:
	pic8( char* nev, FILE* h = NULL );
	~pic8( void );
	// VOX, SPR es PCX-et ismeri:
	int save( char* nev, unsigned char* pal = NULL, FILE* h = NULL );
	int getxsize( void );
	int getysize( void );
	void ppixel( int x, int y, unsigned char szin );
	unsigned char gpixel( int x, int y );
	unsigned char* getptr( int y );
	void fillbox( int x1, int y1, int x2, int y2, unsigned char szin );
	void line( int x1, int y1, int x2, int y2, unsigned char szin );
	void fuggszegmens( int x, int y, int size, unsigned char szin );
	void vizszegmens( int x, int y, int size, unsigned char szin );
};

// Fizikai kepernyore mutat:
//extern pic8* Pscr8;

class ddpal;

unsigned char* spriteadat8( pic8* pmask, unsigned char szin, unsigned short* pspritehossz );
int pcxtopal( char* nev, unsigned char* pal );
int pcxtopal( char* nev, ddpal** ppddpal );

void mintavetel8( pic8* pdest, pic8* psour, int x1, int y1, int x2, int y2 );
void mintavetel8( pic8* pdest, pic8* psour );

void lassufizre( pic8* ppic, ddpal* ppal );
