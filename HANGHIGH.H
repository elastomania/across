
enum { WAV_UTODES = 1, WAV_TORES, WAV_SIKER, WAV_EVES, WAV_FORDULAS,
	   WAV_UGRAS1, WAV_UGRAS2 };

extern int Hangenabled;
extern int Mute;

void starthanghigh( void );
void stophanghigh( void );

void startmotor( void );
// frekvencia 1.0-tol 2.0-ig valtozik csak:
void setmotor( double frekvencia, int gaz );

void setsurlodas( double ero );

void startwave( int wavazonosito, double hangero );
int vanmegwav( void );

void hangosdelay( long t );

