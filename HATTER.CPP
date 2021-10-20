#include	"all.h"

texture::texture( pic8* ppicpar, int inverz ) {
	ppic = NULL;
	if( inverz ) {
		{ ppic = NEW(pic8*) pic8( ppicpar->getysize(), ppicpar->getxsize() ); }
		for( int y = 0; y < ppic->getysize(); y++ ) {
			for( int x = 0; x < ppic->getxsize(); x++ ) {
				ppic->ppixel( x, y, ppicpar->gpixel( y, x ) );
			}
		}
	}
	else {
		{ ppic = NEW(pic8*) pic8( ppicpar->getxsize(), ppicpar->getysize() ); }
		for( int y = 0; y < ppic->getysize(); y++ ) {
			for( int x = 0; x < ppic->getxsize(); x++ ) {
				ppic->ppixel( x, y, ppicpar->gpixel( x, y ) );
			}
		}
	}
}

texture::~texture( void ) {
	if( ppic )
		DELETE ppic ;}
}

/*void texture::getvonal( int x1, int y, int hossz, unsigned char* tomb ) {
	unsigned char* psor = ppic->sormuttomb[y%ppic->getysize()];
	int x2 = x1+hossz-1;
	int xs = ppic->getxsize();
	if( x2 <= x1 )
		hiba( "texture::getvonal-ban x2 <= x1!" );
	if( x2/xs <= x1/xs )
		hiba( "texture::getvonal-ban x2/xs <= x1/xs!" );
	// Elso cella elintezese:
	int sorkezd = x1%xs;
	int size = xs-sorkezd;
	bytecopy( tomb, &psor[sorkezd], size );
	// Sok teljes cella elintezese:
	int xtombkezd = x1+size;
	int darab = x2/xs-xtombkezd/xs;
	if( darab < 0 )
		hiba( "texture::getvonal-ban darab < 0!" );
	for( int i = 0; i < darab; i++ ) {
		bytecopy( &tomb[xtombkezd-x1], psor, xs );
		xtombkezd += xs;
	}
	// Utolso cella elintezese:
	size = x2-xtombkezd+1;
	bytecopy( &tomb[xtombkezd-x1], psor, size );
}*/


void texture::getvonal( int x1, int y, int hossz, unsigned char* tomb ) {
	unsigned char* psor = ppic->sormuttomb[y%ppic->getysize()];
	int x2 = x1+hossz-1;
	int xs = ppic->getxsize();
	if( x2 <= x1 )
		hiba( "texture::getvonal-ban x2 <= x1!" );
	int xkezd = x1;
	while( 1 ) {
		if( xkezd > x2 )
			return;
		int x = xkezd%xs;
		int size = xs-x;
		if( xkezd+size > x2 )
			size = x2-xkezd+1;
		bytecopy( &tomb[xkezd-x1], &psor[x], size );
		xkezd += size;
	}
}

void hattereloallitas( palyaleiro* pleiro, pic8* peg, pic8* pfold ) {
	{ pleiro->pegviz = NEW(texture*) texture( peg, 0 ); }
	{ pleiro->pegfugg = NEW(texture*) texture( peg, 1 ); }
	{ pleiro->pfoldviz = NEW(texture*) texture( pfold, 0 ); }
	{ pleiro->pfoldfugg = NEW(texture*) texture( pfold, 1 ); }
}



