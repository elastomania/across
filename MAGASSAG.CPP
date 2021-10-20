#include	"all.h"

//double Magmaxelemszam = 7750;
double Magmaxelemszam = 16200;

magassag::magassag( int xsizepar ) {
	oszlopok = NULL;
	pcur = NULL;
	pelso = NULL;
	kovelem = 0;
	pcurhat = NULL;
	xsize = xsizepar;

	int ttmmpp = sizeof( ptrhat );
	if( ttmmpp > 4 )
		hiba( "magassag::magassag-ban ptrhat ) > 4!" );
	if( xsize > Magmaxelemszam )
		hiba( "ertekkeszlet tul nagy magassag::magassagban!" );

	// oszlopszamu mutato lefoglalasa:
	{ oszlopok = NEW(ptrhat*) ptrhat[xsize]; }
	if( !oszlopok )
		uzenet( "memory (oszlopok) magassag::magassagban!" );
	for( int i = 0; i < xsize; i++ )
		oszlopok[i] = NULL;

	// Elso hat tomb lefoglalasa:
	{ pelso = NEW(hattomb*) hattomb; }
	pcur = pelso;
	if( !pelso )
		uzenet( "memory (pelso) magassag::magassagban!" );
	pelso->pkov = NULL;
}

magassag::~magassag( void ) {
	DELETE oszlopok; }
	torolhatok();
}

int magassag::getxsize( void ) {
	return xsize;
}

hat* magassag::gethat( void ) {
	if( kovelem >= EGYTOMBHAT ) {
		// Uj tombot kell lefoglalni!:
		{ pcur->pkov = NEW(hattomb*) hattomb; }
		pcur = pcur->pkov;
		if( !pcur )
			uzenet( "memory (hattomb) magassag::gethat-ban!" );
		pcur->pkov = NULL;
		kovelem = 0;
	}
	pcur->hatok[kovelem].pkov = NULL;
	kovelem++;
	return &pcur->hatok[kovelem-1];
}

void magassag::torolhatok( void ) {
	pcur = pelso;
	while( pcur ) {
		hattomb* pkov = pcur->pkov;
		DELETE pcur; }
		pcur = pkov;
	}
}

void magassag::addszakasz( szakasz* psz, int felso ) {
	// Itt adunk be neki egy uj szakaszt:
	// Balrol jobbra mutasson:
	vekt2 v = psz->vv.v;
	vekt2 r = psz->vv.r;
	if( v.x < 0 ) {
		r = r+v;
		v = Vekt2null-v;
	}
	// Nem fuggoleges-e?:
	if( v.x < 0.01 )
		return;
	int x1 = r.x+1;
	int x2 = r.x+v.x;
	double kozelseg = 0.000001;
	if( fabs( double( x1 )-(r.x+1) ) < kozelseg )
			hiba( "Nagyon kozeli pont!" );
	if( fabs( double( x2 )-(r.x+v.x) ) < kozelseg )
			hiba( "Nagyon kozeli pont!" );
	// Nem log-e ki:
	if( x1 < 0 || x2 >= xsize )
		hiba( "Szakasz kilog ertekkeszletbol magassag::addszakasz-ban!" );
	// Egyenes egyenletenek parameterei:
	double m = v.y/v.x;
	double dx1 = r.x;
	double dx2 = r.x+v.x;
	double dy1 = r.y;
	double dy2 = r.y+v.y;
	double a = (dy2*dx1-dy1*dx2)/(dx1-dx2);
	for( int x = x1; x <= x2; x++ ) {
		int y = m*x+a+1.0;
		if( fabs( double(y)-m*x+a+1.0 ) < kozelseg )
			hiba( "Nagyon kozeli pont!" );
		// Hozzacsapjuk hatart oszlophoz:
		hat* phat = gethat();
		phat->y = y;
		phat->felso = felso;
		// Becsatoljuk sor vegere:
		if( !oszlopok[x] ) {
			oszlopok[x] = phat;
		}
		else {
			hat* pfuto = oszlopok[x];
			while( pfuto->pkov )
				pfuto = pfuto->pkov;
			pfuto->pkov = phat;
		}
	}
}

void magassag::rendezget( void ) {
	// Vegigmegy egyesevel minden oszlopon:
	for( int x = 0; x < xsize; x++ ) {
		// Eloszor is sorbarendezi hatarokat:
		// Megszamolja elemek szamat:
		if( x == 400 )
			semmi();
		int szam = 0;
		hat* pfuto = oszlopok[x];
		while( pfuto ) {
			pfuto = pfuto->pkov;
			szam++;
		}
		// Cserelget:
		for( int i = 0; i < szam+2; i++ ) {
			hat* pfuto = oszlopok[x];
			while( pfuto && pfuto->pkov ) {
				if( pfuto->y > pfuto->pkov->y ) {
					// Csere:
					int tmp = pfuto->y;
					pfuto->y = pfuto->pkov->y;
					pfuto->pkov->y = tmp;
					tmp = pfuto->felso;
					pfuto->felso = pfuto->pkov->felso;
					pfuto->pkov->felso = tmp;
				}
				pfuto = pfuto->pkov;
			}
		}
		// Most vegre sorba vannak rendezve!
		// Megszunteti egy y-nal tobbszoros hatarokat:
		int csinalt = 1;
		while( csinalt ) {
			csinalt = 0;
			hat* pfuto = oszlopok[x];
			hat* pelozo = NULL;
			while( pfuto && pfuto->pkov ) {
				int voltfutas = 0;
				if( pfuto->y == pfuto->pkov->y ) {
					// Egy y-on fekszenek:
					csinalt = 1;
					if( pfuto->felso == pfuto->pkov->felso ) {
						// Kihagyjuk az egyiket:
						pfuto->pkov = pfuto->pkov->pkov;
					}
					else {
						// Kihagyjuk mind a kettot:
						if( pelozo ) {
							// Normal eset:
							pelozo->pkov = pfuto->pkov->pkov;
							pfuto = pfuto->pkov->pkov;
						}
						else {
							// Lanc legelso ket elemet kell kihagyni:
							oszlopok[x] = pfuto->pkov->pkov;
							pfuto = pfuto->pkov->pkov;
						}
						voltfutas = 1;
					}
				}
				if( !voltfutas ) {
					pelozo = pfuto;
					pfuto = pfuto->pkov;
				}
			}
		}
	}
}

hat* magassag::getoszlopfirsthat( int x ) {
	#ifdef TEST
		if( x < 0 || x >= xsize )
			hiba( "magassag::initoszlop-ban x < 0 || x >= xsize!" );
	#endif
	return oszlopok[x];
}

void magassag::initoszlop( int x ) {
	#ifdef TEST
		if( x < 0 || x >= xsize )
			hiba( "magassag::initoszlop-ban x < 0 || x >= xsize!" );
	#endif
	pcurhat = oszlopok[x];
}

int magassag::gethat( int* pfelso, int* py ) {
	if( !pcurhat )
		return 0;
	*pfelso = pcurhat->felso;
	*py = pcurhat->y;
	pcurhat = pcurhat->pkov;
	return 1;
}

vekt2 magassagokeloallitasa( szakaszok* pszakok,
							magassag** ppmviz,
							magassag** ppmfugg ) {
	// Megkeresi burkolokat:
	pszakok->felsorolasresetszak();
	szakasz* psz = pszakok->getnextszak();
	if( !psz )
		hiba( "Nincs egyetlen szakasz sem definialva!" );
	double minx = psz->vv.r.x;
	double maxx = psz->vv.r.x;
	double miny = psz->vv.r.y;
	double maxy = psz->vv.r.y;
	while( psz ) {
		if( psz->vv.r.x < minx )
			minx = psz->vv.r.x;
		if( psz->vv.r.x > maxx )
			maxx = psz->vv.r.x;
		if( psz->vv.r.y < miny )
			miny = psz->vv.r.y;
		if( psz->vv.r.y > maxy )
			maxy = psz->vv.r.y;

		if( psz->vv.r.x+psz->vv.v.x < minx )
			minx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.x+psz->vv.v.x > maxx )
			maxx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.y+psz->vv.v.y < miny )
			miny = psz->vv.r.y+psz->vv.v.y;
		if( psz->vv.r.y+psz->vv.v.y > maxy )
			maxy = psz->vv.r.y+psz->vv.v.y;

		psz = pszakok->getnextszak();
	}

	vekt2 origo = vekt2( minx-400/Aranyx, miny-300/Aranyy );
	// Egy kis veletlenites!:
	origo.x += 0.0000145839857463;
	origo.y += 0.0000295874632574;

	double xsize = (maxx+400/Aranyx)-origo.x;
	double ysize = (maxy+300/Aranyy)-origo.y;

	// Lefoglalja magassagokat:
	{ *ppmviz = NEW(magassag*) magassag( xsize*Aranyx ); }
	{ *ppmfugg = NEW(magassag*) magassag( ysize*Aranyy ); }
	if( !ppmviz || !ppmfugg )
		uzenet( "memory magassag!" );

	// Magassagokba beeteti szakaszokat:
	pszakok->felsorolasresetszak();
	psz = pszakok->getnextszak();
	if( !psz )
		hiba( "Nincs egyetlen szakasz sem definialva!" );
	while( psz ) {
		vekt2 fold = forgatas90fokkal( psz->vv.v );
		// Vizszintes iranyu elintezes:
		szakasz sz;
		sz.vv.r = psz->vv.r-origo;
		sz.vv.r.x *= Aranyx;
		sz.vv.r.y *= Aranyy;
		sz.vv.v.x = psz->vv.v.x*Aranyx;
		sz.vv.v.y = psz->vv.v.y*Aranyy;
		int felso = fold.y < 0;

		(*ppmviz)->addszakasz( &sz, felso );

		// Fuggoleges iranyu elintezes:
		// Felcsereli x es y tengelyt:
		double tmp = sz.vv.r.x;
		sz.vv.r.x = sz.vv.r.y;
		sz.vv.r.y = tmp;
		tmp = sz.vv.v.x;
		sz.vv.v.x = sz.vv.v.y;
		sz.vv.v.y = tmp;

		felso = fold.x < 0;

		(*ppmfugg)->addszakasz( &sz, felso );

		psz = pszakok->getnextszak();
	}

	(*ppmviz)->rendezget();
	(*ppmfugg)->rendezget();

	return origo;
}


// tombbe masolja tombeg es tombfoldet:
void kitoltcsikot( magassag* pm, int x, int ystart, int ysize,
	  unsigned char* tomb, unsigned char* tombeg, unsigned char* tombfold) {
	int levego = 0;
	int elozolevego = 0;
	int y = 0;
	pm->initoszlop( x );
	// Addig megy, amig y el nem eri savot:
	while( y <= ystart ) {
		elozolevego = levego;
		if( !pm->gethat( &levego, &y ) ) {
			y = ERTEKKESZLET-1;
		}
	}
	// Addig megy, amig kitoltendo vege is meg savban van:
	int yucso = ystart-1;
	while( y < ystart+ysize ) {
		int size = y-(yucso+1);
		#ifdef TEST
		if( size < 1 ) {
			char tmp[80];
			sprintf( tmp, "kitoltcsikot-ban size < 1! y: %d, yucso: %d",
						y, yucso );
			hiba( tmp );
		}
		#endif
		if( elozolevego )
			bytecopy( tomb+yucso+1-ystart, tombeg+yucso+1-ystart, size );
		else
			bytecopy( tomb+yucso+1-ystart, tombfold+yucso+1-ystart, size );

		yucso = y-1;

		elozolevego = levego;
		if( !pm->gethat( &levego, &y ) ) {
			y = ERTEKKESZLET-1;
		}
	}
	// Utolso szakaszt is kitolti:
	int size = ystart+ysize-(yucso+1);
	if( elozolevego )
		bytecopy( tomb+yucso+1-ystart, tombeg+yucso+1-ystart, size );
	else
		bytecopy( tomb+yucso+1-ystart, tombfold+yucso+1-ystart, size );
}

static void zoomset( unsigned char* mut, int x1, unsigned char c, int size, int zoom ) {
	int x2 = x1+size-1;
	x1 /= zoom;
	x2 /= zoom;
	memset( mut+x1, c, x2-x1+1 );
}

// tombbe masolja hatter es eloter szineket view szamara!:
void kitoltcsikot( magassag* pm, int x, int ystart, int ysize,
	  unsigned char* tomb, unsigned char viewhat, unsigned char viewelo,
	  int zoom ) {
	x *= zoom;
	ystart *= zoom;
	ysize *= zoom;

	if( x < 0 || x >= pm->getxsize() ) {
		// Lelog keprol ez az oszlop, folddel kell feltolteni:
		memset( tomb, viewelo, ysize/zoom );
		return;
	}

	int levego = 0;
	int elozolevego = 0;
	int y = 0;
	pm->initoszlop( x );
	// Addig megy, amig y el nem eri savot:
	while( y <= ystart ) {
		elozolevego = levego;
		if( !pm->gethat( &levego, &y ) ) {
			y = ERTEKKESZLET-1;
		}
	}
	// Addig megy, amig kitoltendo vege is meg savban van:
	int yucso = ystart-1;
	while( y < ystart+ysize ) {
		int size = y-(yucso+1);
		#ifdef TEST
		if( size < 1 ) {
			return; // Ez itt most azert return, mert teljesen bizonytalan
					// hogy miert van?????????????????????????????
			/*char tmp[80];
			sprintf( tmp, "kitoltcsikot(view)-ban size < 1! y: %d, yucso: %d",
						y, yucso );
			hiba( tmp ); */
		}
		#endif
		if( elozolevego )
			zoomset( tomb, yucso+1-ystart, viewhat, size, zoom );
		else
			zoomset( tomb, yucso+1-ystart, viewelo, size, zoom );

		yucso = y-1;

		elozolevego = levego;
		if( !pm->gethat( &levego, &y ) ) {
			y = ERTEKKESZLET-1;
		}
	}
	// Utolso szakaszt is kitolti:
	int size = ystart+ysize-(yucso+1);
	if( elozolevego )
		zoomset( tomb, yucso+1-ystart, viewhat, size, zoom );
	else
		zoomset( tomb, yucso+1-ystart, viewelo, size, zoom );
}

/*static void kirajzkocka( int x, int y, int fold1, int fold2 ) {
	setcolor( ColorFG );
	int s = 15;
	int ox = 100+s*x;
	int oy = 400-s*y;
	putpixel( ox+s/2, oy+s/2, ColorFG );
	//fold1 = 0;
	if( fold1 ) {
		line( ox+2, oy+2, ox+s-3, oy+2 );
		line( ox+s-3, oy+2, ox+s-3, oy+s-3 );
		line( ox+s-3, oy+s-3, ox+2, oy+s-3 );
		line( ox+2, oy+s-3, ox+2, oy+2 );
	}
	if( fold2 ) {
		line( ox+4, oy+4, ox+s-5, oy+4 );
		line( ox+s-5, oy+4, ox+s-5, oy+s-5 );
		line( ox+s-5, oy+s-5, ox+4, oy+s-5 );
		line( ox+4, oy+s-5, ox+4, oy+4 );
	}
}

void kirajzszakasz( vekt2 r, vekt2 v ) {
	setcolor( ColorFG );
	double s = 15;
	r.x = 100+s/2+s*r.x + 0.5;
	r.y = 400+s/2-s*r.y + 0.5;
	v.x *= s;
	v.y *= -s;
	line( r.x, r.y, r.x+v.x, r.y+v.y );
}
*/

/*void testmagassag( void ) {
	{ szakaszok* pszakok = NEW(szakaszok*) szakaszok(); }
	if( !pszakok )
		uzenet( "memory!" );
	//pszakok->add( vekt2( 0.0001839765243, 0.0001539765243 ), vekt2( 0.0, 0.0000001 ) );

	pszakok->add( vekt2( 0.51, 10.5 ), vekt2( 10, -9 ) );
	pszakok->add( vekt2( 0.51, 10.5 ), vekt2( 10, 9 ), 1 );
	pszakok->add( vekt2( 20.51, 10.5 ), vekt2( -10, -9 ), 1 );
	pszakok->add( vekt2( 20.51, 10.5 ), vekt2( -10, 9 ) );

	pszakok->add( vekt2( 6, 10 ), vekt2( 4, 4 ) );
	pszakok->add( vekt2( 6, 10 ), vekt2( 4, -4 ), 1 );
	pszakok->add( vekt2( 14, 10 ), vekt2( -4, 4 ), 1 );
	pszakok->add( vekt2( 14, 10 ), vekt2( -4, -4 ) );

	//psz->add( vekt2( , ), vekt2( , ) );

	pszakok->rendez();

	magassag *pmviz, *pmfugg;

	vekt2 origo = magassagokeloallitasa( pszakok, &pmviz, &pmfugg );
	ide kene hatterszamitas is!

	int dx = -origo.x;
	int dy = -origo.y;

	// Kinyomat magassagok alapjan:
	for( int x = 0; x < 25; x++ ) {
		// Beolvassa oszlopot:
		unsigned char tomb[100];
		for( int i = 0; i < 25; i++ )
			tomb[i] = 2;
		if( x == 21 )
			semmi();
		kitoltcsikot( pmviz, dx+x, dy, 25, tomb );
		for( int y = 0; y < 25; y++ ) {
			kirajzkocka( x, y, tomb[y], 0 );
		}
	}

	// Kinyomat szelessegek alapjan:
	for( int y = 0; y < 25; y++ ) {
		// Beolvassa oszlopot:
		unsigned char tomb[100];
		for( int i = 0; i < 25; i++ )
			tomb[i] = 2;
		kitoltcsikot( pmfugg, dy+y, dx, 25, tomb );
		for( int x = 0; x < 25; x++ ) {
			kirajzkocka( x, y, 0, tomb[x] );
		}
	}

	// Kirajzolja szakaszokat:

	pszakok->felsorolasreset( -10000000.0, 10000000.0 );
	szakasz* psz = pszakok->getnext();
	if( !psz )
		hiba( "Nincs egyetlen szakasz sem definialva!" );
	while( psz ) {
		vekt2 r = psz->r-origo-vekt2( dx, dy );
		kirajzszakasz( r, psz->v );

		psz = pszakok->getnext();
	}

}
*/








