
class texture {
	pic8* ppic;
public:
		texture( pic8* ppic, int inverz );
		~texture( void );
	void getvonal( int x, int y, int xsize, unsigned char* tomb );
};




void hattereloallitas( palyaleiro* pleiro, pic8* peg, pic8* pfold );