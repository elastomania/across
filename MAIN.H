
#ifdef __cplusplus
#define CFUGGVENY extern "C"
#else
#define CFUGGVENY
#endif

#define SARVARI
extern int Sarvari;

extern int Billint; // Billentyuzet interraptos!

extern int Sajatugras; // Ha 1, akkor left->pozitiv forgas
extern int Otszoros; // Ha igaz, mindig otszor rakja ki palyat
extern int Dupladuplabuffer; // Ha igaz duplan huzza az idot (Win95)
extern int Snapenabled; // Ha igaz, akkor m-re kiment egy kepet

#define bytecopy(a,b,c) memcpy( a, b, c )

void semmi( void );

void inkremental( void );
extern long	Inkremens;


