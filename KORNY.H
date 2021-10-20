
class kornyezet {
public:
	pic8* ppic_fomenu;
	abc8* pabc_fomenu;
	ddpal* pal_fomenu;
	ddpal* pal_editor;

	pic8* ppic_load;
	abc8* pabc_load;
	ddpal* pal_load;

	pic8* ppic_help;
	abc8* pabc_help;
	ddpal* pal_help;

	pic8* ppic_saveplay;
	abc8* pabc_saveplay;
	ddpal* pal_saveplay;

	//pic8* ppic_loadplay;
	//abc8* pabc_loadplay;
	//unsigned char* pal_loadplay;

	pic8* ppic_playmenu;
	abc8* pabc_playmenu;
	ddpal* pal_playmenu;

	pic8* ppic_afterplaymenu;
	abc8* pabc_afterplaymenu;
	ddpal* pal_afterplaymenu;

	pic8* ppic_newjatekos;
	abc8* pabc_newjatekos;
	ddpal* pal_newjatekos;

	pic8* ppic_jatekosval;
	abc8* pabc_jatekosval;
	ddpal* pal_jatekosval;

	pic8* ppic_times;
	abc8* pabc_times;
	ddpal* pal_times;

	pic8* ppic_leveltimes;
	abc8* pabc_leveltimes;
	ddpal* pal_leveltimes;

	pic8* ppic_setsize;
	abc8* pabc_setsize;
	ddpal* pal_setsize;
	pic8* ppic_setsizemot;
	unsigned char setsizeindex;
	pic8* ppic_vollecbal; // Eggyel szelesebb jobbra, amit kozepre megy
	pic8* ppic_vollecjobb;
	pic8* ppic_volgomb;

	pic8* picbuffervalaszt;
	pic8* picbuffer;
	rubrika* rubrikak;

	palyaleiro* pleiro;
	int leiroazonosito;

	st320 st;
	int levelpszak;
	//unsigned long belyegpszak;

	recorder* prec;

	char legjobbido[10];

	int resbeolvlevel; // 0 -> text, 1 -> 320x200, 3 -> 640x400

	double fakt_beolvlevel;

	// Hatterkeppel kapcsolatos dolgok:
	double lassitas;
	int hatteralapx, hatteralapy;

	kornyezet( void );
	~kornyezet( void );

	void hatterszamitas( double tartxsize, double tartysize );
};

void pleasewait( char* nev );

extern kornyezet* Korny; // teljes.cpp-ben van inicializalva.


void dialog320( char* text0, char* text1 = NULL, char* text2 = NULL,
				char* text3 = NULL, char* text4 = NULL,
				char* text5 = NULL, char* text6 = NULL );


/* Ez volt regen:
extern char Palyanevek[PALYASZAM][MAXRUBRIKAHOSSZ-5];
extern int Leiroazonositotomb[PALYASZAM];
extern char Palyatextnevtomb[PALYASZAM][20]; */

extern char Leironevtomb[5][20];

struct leveltomb_s {
	int ujpalya;
	int leiroazonosito;
	char description[MAXRUBRIKAHOSSZ];
	char filenev[12];
};

extern leveltomb_s Leveltomb[100];
extern int Levellista[100]; // Leveltombbe indexel

// Visszaadja level leirasat (nem szabad bantani):
char* getleveldescription( int level );

