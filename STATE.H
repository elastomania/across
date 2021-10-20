
#define MAXJATEKOS (50)
#define JATEKOSNEVHOSSZ (14)

#define MAXPALYASZAM (90) // Ennyit soha nem er el

extern int Palyaszam; // Ez fugg attol, hogy shareware, vagy regisztralt

struct jatekos {
	char nev[JATEKOSNEVHOSSZ+2];
	char skippedtomb[((MAXPALYASZAM/4)+1)*4];
	long sikerespalyakszama;
	long jelenlegipalya; // -1 eseten external files
};

#define MAXIDOK (6)

struct palyaidejei {
	long idokszama;
	long idok[MAXIDOK];
	long nevindexek[MAXIDOK];
};

class state {
public:
	palyaidejei palyakidejei[MAXPALYASZAM];
	jatekos jatekosok[MAXJATEKOS];
	long jatekosokszama;
	long jatekos;
	short res;
	double fakt;
	double soundvolume;
	char editfilenev[20];
	char playextfilenev[20];
	state( void );
	~state( void );
	void save( void );
};

extern state* State; // teljes.cpp-ben van inicializalva!


