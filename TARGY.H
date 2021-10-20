
short loadint( FILE* h );
void loadstring( FILE* h, char* text, int maxhossz );

struct targy {
	int mozgo;
	int szam;
	double sebesseg;
	double fazis;
	int odavissza;
	int aktiv; // Minden menet elott be kell allitani 1-be!
};

void loadtargy( FILE* h, targy* ptargy );
void savetargy( FILE* h, targy* ptargy );

#define MAXFRAMESZAM (20)

// A targytomb fileban sorszam altalaban a kovetkezo keppen alakul:
// Az editorban ezt a szamot kell beirni)
// 0-cel, 1-kaja, 2-halalos, 3-kezdo

enum { T_CEL = 1, T_KAJA, T_HALALOS, T_KEZDO };

struct targytipus {
	int tipus;
	double sugar; //*10
	int kerekerz;
	int fejerz;
	double dx, dy; //*10
	double frameseb;
	unsigned char pottyindex;
	int frameszam;
	pic8* spriteok[MAXFRAMESZAM];
};

#define MAXTARGYTIPUSSZAM (10)

class targytomb {
public:
	int targytipusszam;
	targytipus *targytipusok;
	targytomb( char* nev );
	~targytomb( void );
};


