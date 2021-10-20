

#define MAXPONT (1000) // Ennyit csak akkor foglal le, ha kell

extern int Maxpont_sarvari; // 10

class topol;

class gyuru {
	int lefoglaltmaxpont;
public:
	int pontszam;
	vekt2* ponttomb;

	// Fuggvenyek:
	gyuru( void );
	gyuru( FILE* h );
	~gyuru( void );

	int getpontszam( void );
	void save( FILE* h, topol* ptop );
	void ujkoord( int k, double x, double y );
	void kirajz( int k, int fel, int szaggatott = 0 );
	void osszrajz( void );
	// Sikeres-e azt adja vissza:
	int beszur( int k ); // k pont utan beszur egyet ugyanolyan koordokkal
	void torol( int k );
	double legkozpont( double x, double y, int* pk );
	int metszesszam( vekt2 r1, vekt2 v1 );
	// Ez csak akkor tekinti metszesnek, ha nem csak eppen erintik egymast:
	int metszeshely( vekt2 r1, vekt2 v1, int kihagy, vekt2* pmetszes );
	// akkor ad igazat, ha korbejaraskor poligon belseje bal oldalon van;
	int pozitivkorjaras( void );
	vekt2 sulypont( void );
	// Beadott px1, stb-bol indul ki:
	void getminmax( double* px1, double* py1, double* px2, double* py2 );
	double belyegszamitas( void );
	void szetvalaszt( void );
};

#define MAXKEREK (52)

extern int Maxkerek_sarvari; // 8

// T_CEL = 1, T_KAJA, T_HALALOS, T_KEZDO (targy.h-ban van definialva)

class kerek {
public:
	vekt2 r;
	int tipus;

	kerek( double x, double y, int tipus );
	kerek( FILE* h );
	void kirajzol( void );
	void save( FILE* h );
	double belyegszamitas( void );
};

#define MAXGYURU (300)

extern int Maxgyuru_sarvari; // 3

typedef gyuru* ptrgyuru;
#define LEVELNAMEHOSSZ (14)

class topol {
	double belyegszamitas( void );
public:
	int hibas, lezart;
	ptrgyuru ptomb[MAXGYURU]; // Ez csak gany miatt public
	kerek* kerektomb[MAXKEREK];
	char levelname[LEVELNAMEHOSSZ+1];

	topol( void );
	topol( char* filenev, int kulso ); // kulso: qopen vagy nem
	~topol( void );
	gyuru* legkozpont( double x, double y, int* pk, double* ptav = NULL );
	kerek* legkozkerek( double x, double y, double* ptav = NULL );
	void save( char* filenev );
	void kirajzol( void );
	int levegoben( gyuru* pgy, vekt2* pr = NULL ); // pgy levegoben van-e. Akkor folddarab
	void getminmax( double* px1, double* py1, double* px2, double* py2,
									int kerekkel );

	// Ha Sarvari, megvizsgalja hogy palya megfelel-e sarvari verzionak:
	// Ha nem, akkor kilep corrupt hibauzenettel:
	void checklevelforsarvari( void );
};

unsigned long getbelyeg( char* nev );
// Igazzal ter vissza, ha megvan nev;
int getbelyegheznev( unsigned long belyeg, char* nev );

// Csak external file-hoz:
// Ha NULL, akkor valami baj volt vele:
char* getlevelname( char* nev );

// Belso belyegszamitasokhoz kell:
/*unsigned long double2belyeg( double* px );
unsigned long double2belyeg( long l );*/




