
class targytomb;

void resetleptet( void );
void leptet( double most, double dt, int gaz, int fek,
					int ugrik1, int ugrik2 );
// Kor1-bol eloallitja Fejr-et!:
void Kor12Fejr( void );

// 0-meghalt, 2-semmi kulonos:
int vizsgalat( double t, targytomb* ptt );

void kiszamolsurlodast( double* pseb, double* pero );
