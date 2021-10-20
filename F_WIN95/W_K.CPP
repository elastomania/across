#include	"w_all.h"

static int Maxkod = 500;
static int* Tomb = NULL;

static int Buffsize = 30;
static int* Buffer = NULL;
static int Charszam = 0;

void mk_init( void ) {
	if( Tomb )
		hiba( "mk_init-ben Tomb != NULL!" );

	Tomb = new int[Maxkod];
	if( !Tomb )
		hiba( "memory mk_init-ben!" );
	for( int i = 0; i < Maxkod; i++ )
		Tomb[i] = 0;

	Buffer = new int[Buffsize];
	if( !Buffer )
		hiba( "memory mk_init-ben!" );
	Charszam = 0;
}

int mk_getextchar( void ) {
	while( 1 ) {
		mv_check();
		if( Charszam > 0 ) {
			int c = Buffer[0];
			for( int i = 0; i < Charszam-1; i++ )
				Buffer[i] = Buffer[i+1];
			Charszam--;
			return c;
		}
	}
}

void mk_emptychar( void ) {
	mv_check();
	Charszam = 0;
}

int mk_kbhit( void ) {
	mv_check();
	if( Charszam > 0 )
		return 1;
	else
		return 0;
}

int mk_getstate( int kod ) {
	if( kod >= 0 && kod < Maxkod )
		return Tomb[kod];
	hiba( "mk_getstate-ben kod nem ismert!" );
	return 0;
}


// Kovetkezo billentyu fuggvenyek nem publikusak:
void mk_wmkeydown( int kod ) {
	if( kod >= 0 && kod < Maxkod )
		Tomb[kod] = 1;
	if( kod == MK_UP )
		mk_wmchar( MK_UP );
	if( kod == MK_DOWN )
		mk_wmchar( MK_DOWN );
	if( kod == MK_LEFT )
		mk_wmchar( MK_LEFT );
	if( kod == MK_RIGHT )
		mk_wmchar( MK_RIGHT );
	if( kod == MK_PGUP )
		mk_wmchar( MK_PGUP );
	if( kod == MK_PGDOWN )
		mk_wmchar( MK_PGDOWN );
}

void mk_wmkeyup( int kod ) {
	if( kod >= 0 && kod < Maxkod )
		Tomb[kod] = 0;
}

void mk_wmchar( int kod ) {
	if( Charszam < Buffsize ) {
		Buffer[Charszam] = kod;
		Charszam++;
	}
}








