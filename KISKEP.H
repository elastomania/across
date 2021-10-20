
// Egy szegmensben tarolja kepeket, maximalis meretei 256-nal kissebbek, es
// sorok pont 256 byte-onkent kovetkeznek!

class kiskep {
public:
	unsigned char lyuk;
	unsigned char* tomb;
	int xsize, ysize;
	long hossz;
	kiskep( char* pic8nev );
	~kiskep( void );
};