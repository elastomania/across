#include	"all.h"

#define TIMER_0_8253 (0x40)
#define MODE_8253 (0x43)
#define TIMER_COUNT (0x6c)

void getbiostort( unsigned short* pbios, unsigned short* ptort ) {
	#ifdef WATCOM
		long cimlong = 0x046c;
		unsigned short* pmem = (unsigned short*)cimlong;
	#else
		unsigned short* pmem = (unsigned short*)MK_FP( 0x040, 0x6c );
	#endif
	_disable();
	*pbios = *pmem;
	outp( MODE_8253, 0 );
	*ptort = (unsigned)inp( TIMER_0_8253 );
	*ptort += ((unsigned short)inp( TIMER_0_8253 ))<<8;
	_enable();
	*ptort = 0xffff-*ptort;
}

static unsigned short Lastbioscount = 0,
					  Kezdetibioscount = 0,
					  Kezdetitort = 0;
static double Lastt = 0;
static double Hozzaad = 0.0;

void mv_startstopper( void ) {
	getbiostort( &Kezdetibioscount, &Kezdetitort );
	Lastbioscount = Kezdetibioscount;
	Hozzaad = 0.0;
	Lastt = 0.0;
}

double mv_stopperido( void ) {
	int done = 0;
	double t;
	while( !done ) {
		unsigned short bioscount, tort;
		getbiostort( &bioscount, &tort );
		if( bioscount < Lastbioscount )
			Hozzaad += Lastbioscount;
		Lastbioscount = bioscount;
		t = Hozzaad + double(Lastbioscount) - double(Kezdetibioscount);
		t += (((double)tort)-((double)Kezdetitort)) * (1.0/65536.0);
		if( t >= Lastt )
			done = 1;
	}
	if( t > Lastt + 10.0 ) {// kb. fel masodpercet maradhat ki maximum
		double dt = t - (Lastt+10.0);
		t -= dt;
		Hozzaad -= dt;
	}
	Lastt = t;
	return t*10.0;
}

void stopperproba( void ) {
	mv_startstopper();
	for( int i = 0; i < 20; i++ ) {
		long l = mv_stopperido();
		printf( "%ld\n", l );
	}
	double t = mv_stopperido();
	while( !kbhit() ) {
		double ujt = mv_stopperido();
		if( ujt < t ) {
			printf( "ujt < t!\n" );
			printf( "Lastbioscount: %ld\n", long(Lastbioscount) );
			printf( "Kezdetibioscount: %ld\n", long(Kezdetibioscount) );
			printf( "Kezdetitort: %ld\n", long(Kezdetitort) );
			long l = (ujt-t)*1000.0;
			printf( "Kulonbseg: %ld\n", l );
		}
		t = ujt;
	}
	printf( "%ld\n", long(mv_stopperido()) );
	getch();
	exit( 0 );
}


void midnightteszt( void ) {
	int elozoszam = 0;
	while( 1 ) {
		if( kbhit() )
			exit( 0 );
		unsigned short bioscount, tort;
		getbiostort( &bioscount, &tort );
		int szam = bioscount;
		if( elozoszam != szam )
			printf( "%d\n", szam );
		elozoszam = szam;
	}
}



