
#define FILENEVHOSSZ (17)

typedef char filenev[FILENEVHOSSZ+1];

extern int Displaysizex, Displaysizey;

class ddpal;

class palyaleiro {
public:
	ddpal* pal;
	targytomb* ptt;
	pic8f* pvpic8f;
	pic8* pviewbuffer;
	pic8* peg;
	pic8* pfelho;
	pic8* pfold;
	int foldxalapsize;

	ratesz* pratesz;

	//texture *pegviz, *pegfugg, *pfoldviz, *pfoldfugg;
	double kerekdx, kerekdy;
	//double fejdx, fejdy;

	kiskep* pkiskerek;
	kiskep* pkisa;
	kiskep* pkisb;
	kiskep* pkisc;
	kiskep* pkisd;
	kiskep* pkise;
	kiskep* pkiselsorud;
	kiskep* pkishatsorud;
	kiskep* pkisalkar;
	kiskep* pkisfelkar;
	kiskep* pkisfej;

	unsigned char vieweloter;
	unsigned char viewhatter;

	unsigned char viewmotorindex;

	unsigned char idoszin1;
	unsigned char idoszin2;

	unsigned char ugroindex;
	unsigned char keretindex;

	palyaleiro( char* nev );
	~palyaleiro( void );
	void foldbeolv_ratesz( int leiroindex );
};

// Csak foldet csereli ki Korny->leiroazonosito szerint:
void csakfoldcsere( void );

