#include	"all.h"

//static int Kep[25][25];

//static void kirajzkocka( int x, int y, int fold );
//static void kirajzszakasz( vekt2 r, vekt2 v );

static void sorki( int x1, int x2, int y, int szin, pic8f* ppic8f ) {
	#ifdef TEST
		// Sajnos nem tudok teljes ellenorzest csinalni!:
		if( x1 > x2+1 )
			hiba( "teglaban (sorki) x1 > x2!" );
		if( x2-x1 > ppic8f->getxsize() )
			hiba( "teglaban (sorki) x2-x1 > xsize!" );
	#endif
	if( x1 > x2 )
		return;
	ppic8f->vizvonalset( x1, y, x2-x1+1, szin );
	/*for( int x = x1; x <= x2; x++ ) {
		if( x < 0 || x >= 25 )
			hiba( "x kilog sorki-ban!" );
		if( y < 0 || y >= 25 )
			hiba( "y kilog sorki-ban!" );
		Kep[x][y] = szin;
	}
	*/
}

// Csak teszthez kell, kirajzolja oldalakat:
/*static void teglaoldalak( vekt2 A, vekt2 B, vekt2 C, vekt2 D ) {
	kirajzszakasz( A, B-A );
	kirajzszakasz( B, C-B );
	kirajzszakasz( C, D-C );
	kirajzszakasz( D, A-D );
} */

static void reszletki( double y0, double yv, double ax0, double am,
						 double bx0, double bm, int szin, pic8f* ppic8f ) {
	// Ellenorzes:
	#ifdef TEST
	#endif

	// a-nak kell balra lennie:
	double ytavkozep = (yv-y0)/2;
	double axkozep = ax0+am*ytavkozep;
	double bxkozep = bx0+bm*ytavkozep;
	if( axkozep > bxkozep ) {
		// a, b csere:
		double tmp = ax0;
		ax0 = bx0;
		bx0 = tmp;
		tmp = am;
		am = bm;
		bm = tmp;
	}
	// Felveszi y iranyu egesz hatarokat:
	int y1 = y0+1;
	int y2 = yv;
	// Modositja x0-okat:
	double ykulonbseg = y1-y0;
	ax0 += am*ykulonbseg+1.0;
	bx0 += bm*ykulonbseg;
	// Felvesszuk long valtozokat:
	long lax = ax0*65536.0;
	long lbx = bx0*65536.0;
	long lam = am*65536.0;
	long lbm = bm*65536.0;
	// Vegigmegy sorokon:
	for( int y = y1; y <= y2; y++ ) {
		sorki( int(lax>>16), int(lbx>>16), y, szin, ppic8f );
		lax += lam;
		lbx += lbm;
	}
}

void tegla( vekt2 r, vekt2 v, vekt2 v2fel, int szin, pic8f* ppic8f ) {
	if( abs( v ) < 0.1 )
		return;
	#ifdef TEST
		if( abs( v2fel ) < 0.1 )
			hiba( "Tul kicsi szelesseg tegla-ban!" );
	#endif
	// v-nek felfele kell mutatnia:
	if( v.y < 0 ) {
		r = r+v;
		v = Vekt2null-v;
	}
	// v2fel-nek felfele kell mutatnia:
	if( v2fel.y < 0 ) {
		v2fel = Vekt2null-v2fel;
	}
	// ABCD pontok eloallitasa:
	vekt2 A = r+v+v2fel;
	vekt2 B = r+v-v2fel;
	vekt2 D = r+v2fel;
	vekt2 C = r-v2fel;

	//teglaoldalak( A, B, C, D );

	// Most A legfelul, C legalul van.
	// B-nek kell D felett lennie:
	if( B.y < D.y ) {
		// Csere B, D:
		vekt2 tmp2 = B;
		B = D;
		D = tmp2;
	}

	// Most felulrol lefele a kovetkezo a nem szigoru sorrend: ABDC
	// es A szemben van C-vel!

	// Megnezzuk, hogy nem tul merolegesek-e az oldalai tengelyekkel:
	if( fabs( v.x ) < 0.1 || fabs( v.y ) < 0.1 ) {
		// Egy reszletben kuldjuk ki egeszet:
		reszletki( C.y, A.y, A.x, 0, C.x, 0, szin, ppic8f );
		return;
	}
	// Kiszamitjuk B2 es D2 pontok x koordinatajat:
	// (Ezek vizszintesen egymagassagban fekszenek veluk a szemben levo oldalon)
	double D2x = C.x+(B.x-C.x)*(D.y-C.y)/(B.y-C.y);
	double B2x = A.x+(D.x-A.x)*(B.y-A.y)/(D.y-A.y);
	// Kiszamitjuk ket meredekseget:
	double mDC = (D.x-C.x)/(D.y-C.y);
	double mBC = (B.x-C.x)/(B.y-C.y);
	// Most pedig harom reszletben kirajzoljuk teglalapot:
	//			y0   yv  ax0 am bx0 bm
	// Also:
	reszletki( C.y, D.y, C.x, mDC, C.x, mBC, szin, ppic8f );
	// Kozepso:
	reszletki( D.y, B.y, D.x, mBC, D2x, mBC, szin, ppic8f );
	// Felso:
	reszletki( B.y, A.y, B2x, mBC, B.x, mDC, szin, ppic8f );
}

/*static void kirajzkocka( int x, int y, int fold ) {
	setcolor( ColorFG );
	int s = 15;
	int ox = 100+s*x;
	int oy = 400-s*y;
	putpixel( ox+s/2, oy+s/2, ColorFG );
	if( fold ) {
		line( ox+2, oy+2, ox+s-3, oy+2 );
		line( ox+s-3, oy+2, ox+s-3, oy+s-3 );
		line( ox+s-3, oy+s-3, ox+2, oy+s-3 );
		line( ox+2, oy+s-3, ox+2, oy+2 );
	}
}

static void kirajzszakasz( vekt2 r, vekt2 v ) {
	setcolor( ColorFG );
	double s = 15;
	r.x = 100+s/2+s*r.x + 0.5;
	r.y = 400+s/2-s*r.y + 0.5;
	v.x *= s;
	v.y *= -s;
	line( r.x, r.y, r.x+v.x, r.y+v.y );
}
*/

/*void testtegla( void ) {
	for( int x = 0; x < 25; x++ ) {
		for( int y = 0; y < 25; y++ ) {
			Kep[x][y] = 0;
		}
	}

	double alfa = 0;
	int i = 0;
	while( 1 ) {
		setcolor( ColorBG );
		for( int y = 0; y < 480; y++ )
			line( 0, y, 639, y );

		for( x = 0; x < 25; x++ ) {
			for( int y = 0; y < 25; y++ ) {
				Kep[x][y] = 0;
			}
		}

		if( i == 3 )
			semmi();
		tegla( vekt2( 12, 12 ), vekt2( 7*cos( alfa ), 7*sin( alfa ) ), 3, 1 );

		for( x = 0; x < 25; x++ ) {
			for( int y = 0; y < 25; y++ ) {
				kirajzkocka( x, y, Kep[x][y] );
			}
		}
		if( getch() == 'q' )
			hiba( "q" );

		alfa += 0.1;
		i++;
	}
}
*/