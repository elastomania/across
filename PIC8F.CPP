#include	"all.h"

pic8f::pic8f( int xsizepar, int ysizepar ) {
	x0 = y0 = 0;
	xsize = xsizepar;
	ysize = ysizepar;
	{ ppic = NEW(pic8*) pic8( 2*xsize, 2*ysize ); }
	if( !ppic )
		uzenet( "memory kep pic8f::pic8f-ban!" );
}

pic8f::~pic8f( void ) {
	if( ppic )
		DELETE ppic; }
}

int pic8f::getxsize( void ) {
	return xsize;
}

int pic8f::getysize( void ) {
	return ysize;
}

unsigned char pic8f::gpixel( int x, int y ) {
	#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::gpixel-ben bemeno negativ!" );
	#endif
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	return ppic->sormuttomb[y][x];
}

void pic8f::ppixel( int x, int y, unsigned char szin ) {
	#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::ppixel-ben bemeno negativ!" );
	#endif
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	ppic->sormuttomb[y][x] = szin;
}

void pic8f::vizvonalcpy( int x, int y, unsigned char* tomb ) {
	x = x%xsize;
	y = y%ysize;
	if( x < 0 )
		x += xsize;
	if( y < 0 )
		y += ysize;
	// Kozepso reszlet kirajzolasa
	bytecopy( &ppic->sormuttomb[y][x], tomb, xsize );
	// Utolso reszlet kirajzolasa
	bytecopy( &ppic->sormuttomb[y][x+xsize], tomb, xsize-x );
	// Elso reszlet kirajzolasa
	bytecopy( &ppic->sormuttomb[y][0], &tomb[xsize-x], x );

	// Ugyanez megismetelve:
	bytecopy( &ppic->sormuttomb[y+ysize][x], tomb, xsize );
	bytecopy( &ppic->sormuttomb[y+ysize][x+xsize], tomb, xsize-x );
	bytecopy( &ppic->sormuttomb[y+ysize][0], &tomb[xsize-x], x );
}

void pic8f::fuggvonalcpy( int x, int y, unsigned char* tomb ) {
	x = x%xsize;
	y = y%ysize;
	if( x < 0 )
		x += xsize;
	if( y < 0 )
		y += ysize;
	int x2 = x+xsize;
	// Kozepso reszlet:
	int hossz = ysize;
	for( int i = 0; i < hossz; i++ ) {
		unsigned char* psor = ppic->sormuttomb[i+y];
		psor[x] = tomb[i];
		psor[x2] = tomb[i];
	}
	// Utolso reszlet:
	hossz = ysize-y;
	for( i = 0; i < hossz; i++ ) {
		unsigned char* psor = ppic->sormuttomb[i+y+ysize];
		psor[x] = tomb[i];
		psor[x2] = tomb[i];
	}
	// Elso reszlet:
	hossz = y;
	for( i = 0; i < hossz; i++ ) {
		unsigned char* psor = ppic->sormuttomb[i];
		psor[x] = tomb[ysize-y+i];
		psor[x2] = tomb[ysize-y+i];
	}
}

void pic8f::vizszegmens( int x, int y, int size, unsigned char szin ) {
	#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::ppixel-ben bemeno negativ!" );
	#endif
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	for( int i = 0; i < size; i++ )
		ppic->sormuttomb[y][x+i] = szin;
}

void pic8f::fuggszegmens( int x, int y, int size, unsigned char szin ) {
	#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::ppixel-ben bemeno negativ!" );
	#endif
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;

	for( int i = 0; i < size; i++ )
		ppic->sormuttomb[y+i][x] = szin;
}

void pic8f::place( int x, int y ) {
	x0 = x%xsize;
	y0 = y%ysize;
	if( x0 < 0 )
		x0 += xsize;
	if( y0 < 0 )
		y0 += ysize;
}

// A kovetkezoek place alapjan dolgoznak:
void pic8f::vizvonalset( int x, int y, int hossz, unsigned char szin ) {
	#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::vizvonalset-ben bemeno negativ!" );
		if( hossz > xsize )
			hiba( "pic8f::vizvonalset-ben hossz > xsize!" );
	#endif
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	#ifdef TEST
		if( x+hossz >= x0+xsize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
		if( y >= y0+ysize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
	#endif
	memset( &ppic->sormuttomb[y][x], szin, hossz );
}

unsigned char* pic8f::getcim( int x, int y ) {
	/*#ifdef TEST
		if( x < 0 || y < 0 )
			hiba( "pic8f::getcim-ben bemeno negativ!" );
	#endif */
	x = x%xsize;
	y = y%ysize;
	if( x < 0 )
		x += xsize;
	if( y < 0 )
		y += ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	return &ppic->sormuttomb[y][x];
}

void pic8f::csakegyblt( pic8* pp, int x, int y ) {
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	#ifdef TEST
		if( x+pp->getxsize() > x0+xsize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
		if( y+pp->getysize() > y0+ysize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
	#endif
	blt8( ppic, pp, x, y );
}

void pic8f::csakegyblt( pic8* pp, int x, int y, int x1, int y1, int x2, int y2 ) {
	x = x%xsize;
	y = y%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	#ifdef TEST
		if( x2 < x1 || y2 < y1 )
			hiba( "pic8f::csakegyblt-ben x2 <= x1 || y2 <= y1!" );
		if( x+x2-x1 >= x0+xsize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
		if( y+y2-y1 >= y0+ysize )
			hiba( "pic8f::vizvonalset-ben vonal lelog keprol!" );
	#endif
	blt8( ppic, pp, x, y, x1, y1, x2, y2 );
}

void pic8f::freshrectangle( int x1, int y1, int x2, int y2 ) {
	if( x1 > x2 || y1 > y2 )
		hiba( "freshrectangle-ban meretek fel vannak cserelve!" );
	int xs = x2-x1+1;
	int ys = y2-y1+1;
	#ifdef TEST
		if( xs < 2 || ys < 2 )
			hiba( "freshrectangle-ban egyik meret kisebb mint 2!" );
	#endif
	int x = x1%xsize;
	int y = y1%ysize;
	if( x < x0 )
		x += xsize;
	if( y < y0 )
		y += ysize;
	#ifdef TEST
		if( x+xs-1 >= x0+xsize )
			hiba( "pic8f::freshrectangle-ben block lelog keprol!" );
		if( y+ys-1 >= y0+ysize )
			hiba( "pic8f::freshrectangle-ben block lelog keprol!" );
	#endif
	// X IRANYBAN SZABDALAS:
	int adx = -1, asx = -1, ahx = -1; // a block adatai:   d-dest, s-source,
	int bdx = -1, bsx = -1, bhx = -1; // b block adatai:         h-hossz
	asx = x+xsize;
	if( asx < 2*xsize ) {
		if( asx+xs-1 < 2*xsize ) {
			// Egy blokk, ami hatul van!:
			adx = x;
			ahx = 1;
			bdx = x+1;
			bsx = asx+1;
			bhx = xs-1;
		}
		else {
			// Ket blokk, ami hatul kezdodik!:
			adx = x;
			ahx = 2*xsize-asx;
			bdx = x+ahx;
			bsx = 0;
			bhx = xs-ahx;
		}
	}
	else {
		// Egy blokkbol megy masolas, ami elol van!:
		asx -= 2*xsize;
		adx = x;
		ahx = 1;
		bdx = x+1;
		bsx = asx+1;
		bhx = xs-1;
	}

	// Y IRANYBAN SZABDALAS:
	int ady = -1, asy = -1, ahy = -1; // a block adatai:   d-dest, s-source,
	int bdy = -1, bsy = -1, bhy = -1; // b block adatai:         h-hossz
	asy = y+ysize;
	if( asy < 2*ysize ) {
		if( asy+ys-1 < 2*ysize ) {
			// Egy blokk, ami hatul van!:
			ady = y;
			ahy = 1;
			bdy = y+1;
			bsy = asy+1;
			bhy = ys-1;
		}
		else {
			// Ket blokk, ami hatul kezdodik!:
			ady = y;
			ahy = 2*ysize-asy;
			bdy = y+ahy;
			bsy = 0;
			bhy = ys-ahy;
		}
	}
	else {
		// Egy blokkbol megy masolas, ami elol van!:
		asy -= 2*ysize;
		ady = y;
		ahy = 1;
		bdy = y+1;
		bsy = asy+1;
		bhy = ys-1;
	}

	blt8( ppic, ppic, adx, ady, asx, asy, asx+ahx-1, asy+ahy-1 );
	blt8( ppic, ppic, bdx, ady, bsx, asy, bsx+bhx-1, asy+ahy-1 );
	blt8( ppic, ppic, bdx, bdy, bsx, bsy, bsx+bhx-1, bsy+bhy-1 );
	blt8( ppic, ppic, adx, bdy, asx, bsy, asx+ahx-1, bsy+bhy-1 );
}



static void getvizvonal( int x, int y, int xsize, unsigned char* tomb,
			unsigned char viewhat, unsigned char viewelo,
			magassag* pmfugg, int zoom ) {
	kitoltcsikot( pmfugg, y, x, xsize, tomb, viewhat, viewelo, zoom );
}

static void getfuggvonal( int x, int y, int ysize, unsigned char* tomb,
			unsigned char viewhat, unsigned char viewelo,
			magassag* pmviz, int zoom ) {
	kitoltcsikot( pmviz, x, y, ysize, tomb, viewhat, viewelo, zoom );
}

int max(int value1, int value2) {
   return ( (value1 > value2) ? value1 : value2);
}

static unsigned char *Tomb = NULL, *Tombeg = NULL, *Tombfold = NULL;
static int Elsotolpic8f = 1;
static int Lefoglaltsize = 700;

// Bal also sarok koordinatajanak a valtozasa:
// Ha x1 vagy y1 kisebb -10000, egeszet ujra csinalja:
void tolpic8f( int x1, int y1, int x2, int y2, pic8f* pf,
				magassag* pmviz, magassag* pmfugg,
				unsigned char viewelo, unsigned char viewhat ) {
	pf->place( x2, y2 );
	int xsize = pf->getxsize();
	int ysize = pf->getysize();
	if( Elsotolpic8f ) {
		Elsotolpic8f = 0;
		{ Tomb = NEW(unsigned char*) unsigned char[Lefoglaltsize]; }
		{ Tombeg = NEW(unsigned char*) unsigned char[Lefoglaltsize]; }
		{ Tombfold = NEW(unsigned char*) unsigned char[Lefoglaltsize]; }
		if( !Tomb || !Tombeg || !Tombfold )
			uzenet( "memory lehet hogy Tombfold!" );
	}
	//if( Lefoglaltsize != max( xsize, ysize ) + 10 )
	//	hiba( "Lefoglaltsize != max( xsize, ysize ) + 10" );
	int egesz = 0;
	if( x1 < -10000 || y1 < -10000 )
		egesz = 1;
	//if( x2 < 0 || y2 < 0 )
	//	hiba( "tolpic8f-ben negativ koordinatak vannak megadva!" );
	if( abs( x2-x1 ) >= xsize/2 )
		egesz = 1;
	if( abs( y2-y1 ) >= ysize/2 )
		egesz = 1;
	if( egesz ) {
		for( int i = 0; i < ysize; i++ ) {
			getvizvonal( x2, y2+i, xsize, Tomb, viewhat, viewelo, pmfugg,
				Config.zoom );
			pf->vizvonalcpy( x2, y2+i, Tomb );
		}
		return;
	}
	int baloldalt = 1;
	if( x2 > x1 )
		baloldalt = 0;
	int hosszx = abs( x2-x1 );

	int lent = 1;
	if( y2 > y1 )
		lent = 0;
	int hosszy = abs( y2-y1 );

	if( baloldalt ) {
		for( int i = 0; i < hosszx; i++ ) {
			getfuggvonal( x2+i, y2, ysize, Tomb, viewhat, viewelo, pmviz,
				Config.zoom );
			pf->fuggvonalcpy( x2+i, y2, Tomb );
		}
	}
	else {
		for( int i = 0; i < hosszx; i++ ) {
			int j = x2+xsize-hosszx+i;
			getfuggvonal( j, y2, ysize, Tomb, viewhat, viewelo, pmviz,
					Config.zoom );
			pf->fuggvonalcpy( j, y2, Tomb );
		}
	}

	if( lent ) {
		for( int i = 0; i < hosszy; i++ ) {
			getvizvonal( x2, y2+i, xsize, Tomb, viewhat, viewelo, pmfugg,
					Config.zoom );
			pf->vizvonalcpy( x2, y2+i, Tomb );
		}
	}
	else {
		for( int i = 0; i < hosszy; i++ ) {
			int j = y2+ysize-hosszy+i;
			getvizvonal( x2, j, xsize, Tomb, viewhat, viewelo, pmfugg,
					Config.zoom );
			pf->vizvonalcpy( x2, j, Tomb );
		}
	}
}

