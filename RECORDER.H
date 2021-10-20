
struct hanginfo {
	double frekvencia;
	char gaz;
	double surlseb, surlero;
};

struct egyhang {
	double t;
	short objszam;
	char wavindex;
	float hangero;
};

class recorder {
	float *pk1rx, *pk1ry, *pk2rx, *pk2ry, *pk4rx, *pk4ry,
		*pk2alfa, *pk4alfa, *pk1alfa;
	float *pfrekvencia;
	char *pgaz, *phatra;
	float *psurlseb, *psurlero;

	long betoltve;
	int ucsokiadva;
	long level;

	egyhang* phangok;
	long hangbetoltve;
	long hangkov;
public:
	int regi; // Ha igaz, akkor lejatszonak nem szabad rendeznie objokat

	recorder( unsigned darabszam, unsigned hangdarabszam );
	~recorder( void );
	void erase( long level );
	void rewind( void );
	int recall( double dt, hanginfo* phinfo );
	void store( double dt, hanginfo* phinfo );

	void addhang( double t, char wavindex, double hangero, int objszam );
	// Ha van hang, akkor igazzal ter vissza:
	int gethang( double t, char* pwavindex, double* phangero, int* pobjszam );

	int getlevel( void );
	// demo eseten resource file-bol nyit:
	// belyeg-et adja vissza, vagy 0-at:
	long load( char* nev, int demo );
	void save( char* nev, long belyeg );
};


void startwavegyujto( int wavazonosito, double hangero, int objszam );
void resetwavegyujto( void );
// Igaz, ha van bent wav:
int getwavegyujto( int* pwavazonosito, double* phangero, int* pobjszam );


