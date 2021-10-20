
class conf {
public:
	void load( char* nev );
	void save( char* nev );
	int viewxsize, viewysize;
	int viewxorig, viewyorig;
	int viewxtolas;
	int zoom;
};


extern conf Config;

