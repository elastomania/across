
#define MAXRUBRIKAHOSSZ (40)
#define MAXRUBRIKASZAM (600)

typedef char rubrika[MAXRUBRIKAHOSSZ+2];

// Felhasznalja, Korny rubrikak-at es picbuffer-t!
class valaszt {
	void kirajz( int felso, ddpal* ppal );
public:
	int szam, kur, egykepen;
	int x0, y0, dy;
	pic8* hatter;
	abc8* pabc;
	int escelheto, tomoren;

	valaszt( void );
	// Visszaadja valasztott szamat, ESC eseten -1-t:
	// tobbrajz eseten tobbszor is kirajzolja kezdetben (Win95):
	int valassz( ddpal* ppal, int tobbrajz = 0 );
};

