
class ratesz {
	pic8* ptext;
	int textxalapsize;
	int ysize;
	int y0;
	int elsohivas;
	int* elsotomb;

	hat** sorhatok;

	void beallitsort( int y, magassag* pm );
public:
	ratesz( int ysizepar, pic8* ptextpar, int textxalapsizepar );
	~ratesz( void );
	void reset( void );
	void teddra( int x, int y, pic8* phatter, magassag* pm );
};


