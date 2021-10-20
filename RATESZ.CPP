#include	"all.h"


static int negmodulusis( int osztando, int oszto ) {
	if( oszto <= 0 )
		hiba( "negmodulusis-ben oszto <= 0!" );
	if( osztando >= 0 )
		return osztando % oszto;
	// osztando most mar negativ!
	int mod = oszto-((-osztando)%oszto);
	#ifdef TEST
		if( mod > oszto || mod <= 0 )
			hiba( "negmodulusis-ben mod > oszto || mod <= 0!" );
	#endif
	if( mod >= oszto )
		mod = 0;
	return mod;
}

ratesz::ratesz( int ysizepar, pic8* ptextpar, int textxalapsizepar ) {
	ptext = ptextpar;
	textxalapsize = textxalapsizepar;
	ysize = ysizepar;
	sorhatok = NULL;
	y0 = 0;
	elsohivas = 1;

	{ sorhatok = NEW(ptrhat*) ptrhat[ysize]; }
	if( !sorhatok )
		uzenet( "memory (sorhatok) ratesz::ratesz-ben!" );
	for( int i = 0; i < ysize; i++ )
		sorhatok[i] = NULL;

	{ elsotomb = NEW(int*) int[ysize]; }
	if( !elsotomb )
		uzenet( "memory (elsotomb) ratesz::ratesz-ben!" );
}

ratesz::~ratesz( void ) {
	if( sorhatok )
		DELETE sorhatok; }
	if( elsotomb )
		DELETE elsotomb; }
}

void ratesz::reset( void ) {
	elsohivas = 1;
}

void ratesz::beallitsort( int y, magassag* pm ) {
	//if( y < 0 )
	//	hiba( "y < 0 ratesz::beallitsort-ban!" );
	// torlendo:
	//if( y == 333 )
	//	semmi();
	unsigned yindex = negmodulusis( y , ysize );
	elsotomb[yindex] = 1;
	sorhatok[yindex] = pm->getoszlopfirsthat( y );
}

void ratesz::teddra( int x, int y, pic8* phatter, magassag* pm ) {
	// Egy kis ellenorzes:
	//if( x < 0 || y < 0 )
	//	hiba( "ratesz::teddra-ban x < 0 || y < 0!" );
	if( ysize != phatter->getysize() )
		hiba( "ratesz::teddra-ban ysize != phatter->getysize()!" );
	if( Displaysizex != phatter->getxsize() )
		hiba( "ratesz::teddra-ban Displaysizex != phatter->getxsize()!" );
	if( Displaysizey != phatter->getysize() )
		hiba( "ratesz::teddra-ban Displaysizey != phatter->getysize()!" );
	// Nem ervenyes sorok beallitasa uj y fuggvenyeben:
	if( elsohivas ) {
		elsohivas = 0;
		y0 = y - 1000;
		if( y0 < 0 )
			y0 = y + 1000;
	}
	// Beallitjuk sorokat:
	if( y < y0 ) {
		for( int futy = y; futy < y0 && futy < y + ysize; futy++ ) {
			beallitsort( futy, pm );
		}
		y0 = y;
	}
	if( y > y0 ) {
		if( y >= y0 + ysize ) {
			// Egesz ujrairodik:
			for( int futy = y; futy < y+ysize; futy++ ) {
				beallitsort( futy, pm );
			}
		}
		else {
			// Csak reszlet irodik ujra:
			for( int futy = y0+ysize; futy < y+ysize; futy++ ) {
				beallitsort( futy, pm );
			}
		}
		y0 = y;
	}
	// Kirajzoljuk sorokat:
	int foldkezdx = negmodulusis( x, textxalapsize );
	#ifdef TEST
		if( foldkezdx+Displaysizex > ptext->getxsize() )
			hiba( "ratesz::teddra-ban foldkezdx+Displaysizex > ptext->getxsize()" );
	#endif
	int xutani = x+phatter->getxsize();
	//int stopsor = 300;
	int kellhangsor = y+201;
	hang();
	for( int futy = y; futy < y+ysize; futy++ ) {
		if( y == kellhangsor )
			hang();

		//if( futy - y == stopsor )
		//	semmi();

		// Beallitjuk hatter es text sorokra mutato mutatokat:
		int yindex = negmodulusis( futy, ysize );
		unsigned char* phattersor = phatter->getptr( futy-y );

		int yfold = negmodulusis( futy, ptext->getysize() );
		unsigned char* pfoldsor = ptext->getptr( yfold );
		// Kov sor utan pfoldsor karaktersorra fog mutatni, amely display
		// 0. soraval fog kezdodni:
		pfoldsor += foldkezdx;

		// Egy sor elintezese:
		hat* phat = sorhatok[yindex];
		int elso = elsotomb[yindex];
		#ifdef TEST
			if( !elso && !phat )
				hiba( "ratesz::teddra-ban !elso && !phat!" );
		#endif
		if( elso && !phat ) {
			// Ebben a sorban nincs elatmenet, tehat tisztan fold jon ide:
			memcpy( phattersor, pfoldsor, Displaysizex );
			continue;
		}

		// Innentol phat != NULL
		// Megnezzuk, hogy nem leptunk-e tul vissza, mert akkor ujra kell
		// kitolteni sort:
		if( !elso && x < phat->y ) {
			beallitsort( futy, pm );
			phat = sorhatok[yindex];
			elso = elsotomb[yindex];
		}
		// Most biztosan elol all phat:
		#ifdef TEST
			if( !phat || (!elso && x < phat->y) )
				hiba( "ratesz::teddra-ban !phat || (!elso && phat->y < x)!" );
		#endif
		int futx = x;
		hat* phatkov = phat->pkov;

		// Atlepjuk azokat a hat-okat, amelyek tul koran vannak:
		while( phatkov && phatkov->y < x ) {
			phat = sorhatok[yindex] = phatkov;
			phatkov = phatkov->pkov;
			elso = elsotomb[yindex] = 0;
		}
		// Visszalepunk, ha kell:
		if( x < phat->y ) {
			#ifdef TEST
				if( !elso )
					hiba( "ratesz::teddra-ban !elso, pedig x < phat->y!" );
			#endif
			phatkov = phat;
			phat = NULL;
		}
		// Innentol phat lehet NULL:
		while( 1 ) {
			// Most phat es phatkov kozrefogjak futx-et (phat lehet NULL):
			int fold = 1;
			if( elso ) {
				if( phat && phat->felso && phat->y <= futx )
					fold = 0;
			}
			else {
				fold = !phat->felso;
			}
			if( !phatkov || phatkov->y >= xutani ) {
				// Sor vegeig kitolt:
				if( fold )
					memcpy( phattersor+(futx-x), pfoldsor+(futx-x), xutani-futx );
				break;
			}
			// Kitolt phatkov->y -ig:
			if( fold )
				memcpy( phattersor+(futx-x), pfoldsor+(futx-x), phatkov->y-futx );
			futx = phatkov->y;
			phat = phatkov;
			phatkov = phatkov->pkov;
		}
	}
	hang();
}

