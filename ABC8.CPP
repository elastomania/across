#include	"all.h"

static int Hibaismeretlenkar = 0;

 
abc8::abc8( char* nev ) {
	success = 0;
	tav = 0;
	ppsprite = NULL;
	ytomb = NULL;
	{ ppsprite = NEW(ptrpic8*) ptrpic8[256]; }
	if( !ppsprite ) {
		uzenet( "memory" );
		return;
	}
	for( int i = 0; i < 256; i++ )
		ppsprite[i] = NULL;
	{ ytomb = NEW(short*) short[256]; }
	if( !ytomb ) {
		uzenet( "memory" );
		return;
	}
	for( i = 0; i < 256; i++ )
		ytomb[i] = 0;
	FILE* h = qopen( nev, "rb" );
	if( !h ) {
		hiba( "Nem tudta megnyitni abc konstruktor file-t!: ", nev );
		return;
	}
	char tmp[20];
	if( fread( tmp, 4, 1, h ) != 1 ) {
		hiba( "Nem tudja olvasni abc-t!: ", nev );
		qclose( h );
		return;
	}
	if( strcmp( tmp, "RA1" ) != 0 ) {
		hiba( "Nem ervenyes abc file elso negy byte alapjan!: ", nev );
		qclose( h );
		return;
	}
	short darabszam = 0;
	if( fread( &darabszam, 2, 1, h ) != 1 ) {
		hiba( "Nem tudja olvasni abc-t!: ", nev );
		qclose( h );
		return;
	}
	if( darabszam <= 0 || darabszam > 256 )
		hiba( "abc file darabszama nem stimmel!" );
	for( int kkk = 0; kkk < darabszam; kkk++ ) {
		// Sorban beolvassa betuk spritejait:
		if( fread( tmp, 7, 1, h ) != 1 ) {
			hiba( "Nem tudja olvasni abc-t!: ", nev );
			qclose( h );
			return;
		}
		if( strcmp( tmp, "EGYMIX" ) != 0 ) {
			hiba( "Nem talalta meg EGYMIX-et!: ", nev );
			qclose( h );
			return;
		}
		unsigned char c = -1;
		if( fread( &c, 1, 1, h ) != 1 ) {
			hiba( "Nem tudja olvasni abc-t!: ", nev );
			qclose( h );
			return;
		}
		if( fread( &ytomb[c], 2, 1, h ) != 1 ) {
			hiba( "Nem tudja olvasni abc-t!: ", nev );
			qclose( h );
			return;
		}
		if( ppsprite[c] ) {
			hiba( "Egy karakter ketszer van meg abc file-ban!: ", nev );
			return;
		}
		{ ppsprite[c] = NEW(pic8*) pic8( ".spr", h ); }
		if( !ppsprite[c]->success ) {
			hiba( "Nem tudta beolvasni SPRITE-ot abc file-bol!: ", nev );
			return;
		}
	}
	success = 1;
	qclose( h );
}

abc8::~abc8( void ) {
	if( ppsprite ) {
		for( int i = 0; i < 256; i++ ) {
			if( ppsprite[i] ) {
				DELETE ppsprite[i]; }
				ppsprite[i] = NULL;
			}
		}
		DELETE ppsprite; }
		ppsprite = NULL;
	}
	if( ytomb ) {
		DELETE ytomb; }
		ytomb = NULL;
	}
}

static int Spacehossz = 5;

void abc8::write( pic8* pdest, int x, int y, char* text, int tavpar ) {
	if( !success ) {
		hiba( "abc8::write, pedig nem success!" );
		return;
	}
	if( tavpar == -1000 )
		tavpar = tav;
	while( *text ) {
		int index = (unsigned char)*text;
		if( index == ' ' ) {
			x += Spacehossz;
			text++;
			continue;
		}
		if( !ppsprite[index] ) {
			if( Hibaismeretlenkar ) {
				hiba( "abc8::len-ben ismeretlen karakter!" );
				return;
			}
			else {
				text++;
				continue;
			}
		}
		blt8( pdest, ppsprite[index], x, y+ytomb[index] );
		//blt8( pdest, ppsprite[index], x, y );
		x += tavpar+ppsprite[index]->getxsize();

		text++;
	}
}

int abc8::len( char* text, int tavpar ) {
	if( !success ) {
		hiba( "abc8::len, pedig nem success!" );
		return 0;
	}
	if( tavpar == -1000 )
		tavpar = tav;
	int hossz = 0;
	while( *text ) {
		int index = (unsigned char)*text;
		if( !ppsprite[index] ) {
			if( index == ' ' ) {
				hossz += Spacehossz;
				text++;
				continue;
			}
			if( Hibaismeretlenkar ) {
				hiba( "abc8::len-ben ismeretlen karakter!" );
				return 0;
			}
			else {
				text++;
				continue;
			}
		}
		hossz += tavpar+ppsprite[index]->getxsize();

		text++;
	}
	return hossz;
}

void abc8::settav( int tavpar ) {
	tav = tavpar;
}

void abc8::writekozep( pic8* pdest, int x, int y, char* text, int tavpar ) {
	int hossz = len( text, tav );
	write( pdest, x-hossz/2, y, text, tavpar );
}


