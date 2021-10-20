#include	"all.h"

void valaszt::kirajz( int felso, ddpal* ppal ) {
	blt8( Korny->picbuffervalaszt, hatter );
	for( int i = 0; i < egykepen && i < szam-felso; i++ ) {
		if( tomoren )
			pabc->write( Korny->picbuffervalaszt, x0, y0+i*dy, Korny->rubrikak[felso+i], -2 );
		else
			pabc->write( Korny->picbuffervalaszt, x0, y0+i*dy, Korny->rubrikak[felso+i] );
	}
	// Jel kirajzolasa:
	char tmp[2] = "A";
	tmp[0] = '@';
	int szelesseg = pabc->len( tmp );                  // x0-37
	pabc->write( Korny->picbuffervalaszt, (x0-szelesseg)/2, y0+(kur-felso)*dy+0, tmp );
	lassufizre( Korny->picbuffervalaszt, ppal );
}

valaszt::valaszt( void ) {
	tomoren = 0;
}


// Visszaadja valasztott szamat, ESC eseten -1-t:
int valaszt::valassz( ddpal* ppal, int tobbrajz ) {
	if( szam < 0 || szam > MAXRUBRIKASZAM )
		hiba( "valassz-ban szam nem stimmel!" );

	int felso = kur-egykepen/2;
	int maxfelso = szam-egykepen;
	if( felso > maxfelso )
		felso = maxfelso;
	if( felso < 0 )
		felso = 0;
	// felso be van allitva!
	kirajz( felso, ppal );
	if( tobbrajz )
		kirajz( felso, ppal );
	while( 1 ) {
		mk_emptychar();
		int c = mk_getextchar();

		//FILE* h = fopen( "billkod.txt", "at" );
		//fprintf( h, "%d\n", c );
		//fclose( h );

		/*if( c == '1' )
			Korny->picbuffervalaszt->save( "menu1.pcx", ppal->tomb );
		if( c == '2' )
			Korny->picbuffervalaszt->save( "menu2.pcx", ppal->tomb );
		if( c == '3' )
			Korny->picbuffervalaszt->save( "menu3.pcx", ppal->tomb );
		if( c == '4' )
			Korny->picbuffervalaszt->save( "menu4.pcx", ppal->tomb );
		if( c == '5' )
			Korny->picbuffervalaszt->save( "menu5.pcx", ppal->tomb );
		if( c == '6' )
			Korny->picbuffervalaszt->save( "menu6.pcx", ppal->tomb );
		if( c == '7' )
			Korny->picbuffervalaszt->save( "menu7.pcx", ppal->tomb );
		if( c == '8' )
			Korny->picbuffervalaszt->save( "menu8.pcx", ppal->tomb );*/


		if( c == MK_ESC && escelheto )
			return -1;
		if( c == MK_ENTER )
			return kur;
		if( c == MK_UP ) {
			if( kur > 0 ) {
				kur--;
				if( kur < felso )
					felso--;
				kirajz( felso, ppal );
			}
		}
		if( c == MK_PGUP ) {
			if( kur > 0 ) {
				kur -= egykepen;
				if( kur < 0 )
					kur = 0;
				if( kur < felso )
					felso = kur;
				kirajz( felso, ppal );
			}
		}
		if( c == MK_DOWN ) {
			if( kur < szam-1 ) {
				kur++;
				if( kur > felso+egykepen-1 )
					felso++;
				kirajz( felso, ppal );
			}
		}
		if( c == MK_PGDOWN ) {
			if( kur < szam-1 ) {
				kur += egykepen;
				if( kur >= szam )
					kur = szam-1;
				if( kur > felso+egykepen-1 )
					felso = kur-egykepen+1;
				kirajz( felso, ppal );
			}
		}
	}
}

