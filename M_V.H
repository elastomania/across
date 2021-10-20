// m_v.h

extern int Win_95;

void mv_main( void ); // Ennek nem szabad visszajonnie!
void mv_check( void ); // Ezt neha meg kell hivni!
void mv_exit( void ); // Ezzel kell kilepni!

// 182*sec-et adja vissza idot tortresszel egyutt!
// d_stoppe.cpp-ben vannak megvalositva:
double mv_stopperido( void );
void mv_startstopper( void );
// Ezt valojaban csak crc fuggvenyek hasznaljak veletlen generalasara:
void getbiostort( unsigned short* pbios, unsigned short* ptort );

void hiba( char* text1, char* text2 = NULL, char* text3 = NULL );
void uzenet( char* text1, char* text2 = NULL, char* text3 = NULL );

int s_random( int ); // Ugyanaz mint random, de az nincs WATCOM-ban



