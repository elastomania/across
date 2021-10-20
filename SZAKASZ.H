
struct szakasz;

struct targy;

struct vonal;

struct vonalnode {
	vonal* 	   pvonal;
	vonalnode* pnext;
};

typedef vonalnode* pvonalnode;

#define CSOMAGBANNODESZAM (500)
struct soknodecsomag {
	vonalnode nodetomb[CSOMAGBANNODESZAM];
	soknodecsomag* nextcsomag;
};

class topol;

class szakaszok {
	szakasz* tomb;
	int		 maxszam;
	int		 szam;
	int szakfuto;

	// Terbeli kereseshez:
	pvonalnode* tertomb;
	int xdim, ydim;
	double cellameret;
	vekt2 origo;
	vonalnode* nextnode; // getnext ez alapjan dolgozik!


	void beindexelvonalat( vonal* pv, double maxtav );
	void bekot( int cellax, int cellay, vonal* pv );

	// Terbeli kereseshez nodkezeles:
	soknodecsomag* pelsocsomag;
	int csomagbanbetelt;
	vonalnode* getnewnode( void );
	void deleteallnodes( void );
public:
	//szakaszok( void );
	szakaszok( FILE* h, int targyas );
	szakaszok( topol* ptop, int targyas );
	~szakaszok( void );
	void save( FILE* h, int targyas );
	//void add( vekt2 r, vekt2 v, int targyas );
	//void remove( szakasz* psz );

	// Terbeli fv.-ek:
	// rendez-t csak egyszer szabad hivni!
	void rendez( double maxtav );
	void felsorolasreset( vekt2 r );
	vonal* getnext( void );

	// Osszes szakaszon vegigmennek:
	void felsorolasresetszak( void );
	szakasz* getnextszak( void );

	// Visszaadja kajak szamat!:
	// setallaktiv allitja be motor kezdeti helyzetet is!:
	int setallaktiv( int targytipusszam, targytomb* ptt );

	void killerekelore( targytomb* ptt );

	szakasz* getptrszakasz( int sorszam );
	int getsorszam( szakasz* pszakasz );
};

struct vonal {
	vekt2 r, v, egyseg;
	double hossz;
};

struct szakasz {
public:
	vonal vv;
	targy*	 ptargy;
};

class leiro;

void loadszakaszok( char* nev, int kulso, szakaszok** ppszak,
			szakaszok** ppszakt );
void saveszakaszok( char* nev, szakaszok* pszak, szakaszok* pszakt );

// loadszakasz beleirja ptop->lezart-at ha topologia file-t olvas be:
// Ezt azutan afterplayext fuggveny hasznalja ki:
extern int Ucsoloadszakasz_lezart;






