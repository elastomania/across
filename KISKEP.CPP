#include	"all.h"

kiskep::kiskep( char* pic8nev ) {
	tomb = NULL;
	pic8* ppic = NULL;
	{ ppic = NEW(pic8*) pic8( pic8nev ); }

	lyuk = ppic->gpixel( 0, 0 );

	xsize = ppic->getxsize();
	ysize = ppic->getysize();
	if( xsize > 255 || ysize > 255 )
		hiba( "kiskepben size > 255!" );
	hossz = ysize*256l;
	{ tomb = NEW(unsigned char*) unsigned char[unsigned(hossz)]; }
	if( !tomb )
		uzenet( "memory!" );
	for( int y = 0; y < ysize; y++ )
		for( int x = 0; x < xsize; x++ )
			tomb[y*256+x] = ppic->gpixel( x, y );

	DELETE ppic; }
}

kiskep::~kiskep( void ) {
	if( tomb )
		DELETE tomb; }
}
