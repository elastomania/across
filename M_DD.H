// m_dd.h

class pic8;
class palyaleiro;
struct st320;
class recorder;

class ddpal {
	friend void lassufizre( pic8* ppic, ddpal* ppal );
	//friend long lejatszo( palyaleiro* pleiro, st320* pst, int mcga,
	//									recorder* prec, int replay );
public:
	#ifdef W_95
		void* pddpal;
	#else
		unsigned char* tomb;
	#endif

	ddpal( unsigned char* tomb );
   ~ddpal( void );
   void set( void );
};

unsigned char** lockbackbuffer( int* pxs, int* pys );
void unlockbackbuffer( void ); // Ki is teszi

// res: 1 -> 320x200, 4 -> 640x480
void setresdd( int res );

