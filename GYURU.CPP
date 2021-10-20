#include	"all.h"

// Ide balszog tesz be akkor pontot, ha talal tul hegyes szoget:
// Check hivja gyuru->pozitivkorjaras-on keresztul:
int Vanhegyesszog = 0;
vekt2 Rhegyesszog;

gyuru::gyuru( void ) {
	pontszam = 0;
	lefoglaltmaxpont = 10;
	ponttomb = new vekt2[lefoglaltmaxpont];
	for( int i = 0; i < lefoglaltmaxpont; i++ ) {
		ponttomb[i].x = 0;
		ponttomb[i].y = 0;
   }
   // Most csak beteszunk harom pontot:
   ponttomb[0].x = -24;
   ponttomb[0].y = -8;
   ponttomb[1].x = 24;
   ponttomb[1].y = -8;
   ponttomb[2].x = 24;
   ponttomb[2].y = 2;
   ponttomb[3].x = -24;
   ponttomb[3].y = 2;
   pontszam = 4;
}

gyuru::~gyuru( void ) {
	if( ponttomb )
		delete ponttomb;
	ponttomb = NULL;
}

void gyuru::ujkoord( int k, double x, double y ) {
	if( k < 0 || k >= pontszam )
		hiba( "gyuru::ujkoord-ban k < 0 || k >= pontszam!" );
	ponttomb[k].x = x;
	ponttomb[k].y = y;
}

void gyuru::kirajz( int k, int fel, int szaggatott ) {
	if( k < 0 || k >= pontszam )
   	hiba( "gyuru::kirajz-ban k < 0 || k >= pontszam!" );
	if( (fel && k == pontszam-1) || (!fel && k == 0) ) {
		// Elejet a vegevel kell osszekotni:
		vonalrajz( ponttomb[pontszam-1], ponttomb[0], szaggatott );
   }
   else {
	// Normal osszekotes:
	  if( fel )
		vonalrajz( ponttomb[k], ponttomb[k+1], szaggatott );
		else
		vonalrajz( ponttomb[k-1], ponttomb[k], szaggatott );
   }
}

void gyuru::osszrajz( void ) {
	if( pontszam < 3 || pontszam > MAXPONT )
		hiba( "gyuru::osszrajz-ban pontszam < 3 || pontszam > MAXPONT!" );
	for( int i = 0; i < pontszam; i++ )
		kirajz( i, 1, 0 );
}


// k pont utan beszur egyet ugyanolyan koordokkal
int gyuru::beszur( int k ) {
	if( pontszam+1 > MAXPONT ) {
		dialog( "You cannot add more points to this GYURU!" );
		return 0;
	}
	if( k >= pontszam )
		hiba( "gyuru::beszur-ban k >= pontszam!" );

	if( pontszam >= lefoglaltmaxpont ) {
		// Ujra fel kell venni pontok tombjet, csak most tobb ponttal:
		lefoglaltmaxpont += 10;
		vekt2* ujponttomb = new vekt2[lefoglaltmaxpont];
		for( int i = 0; i < lefoglaltmaxpont; i++ ) {
			ujponttomb[i].x = 0;
			ujponttomb[i].y = 0;
		}
		for( i = 0; i < pontszam; i++ ) {
			ujponttomb[i].x = ponttomb[i].x;
			ujponttomb[i].y = ponttomb[i].y;
		}
		delete ponttomb;
		ponttomb = ujponttomb;
	}

	for( int i = pontszam-1; i >= k; i-- ) {
		ponttomb[i+1].x = ponttomb[i].x;
		ponttomb[i+1].y = ponttomb[i].y;
   }
   pontszam++;
   return 1;
}

void gyuru::torol( int k ) {
	if( k < 0 || k >= pontszam )
		hiba( "gyuru::torol-ben k < 0 || k >= pontszam!" );
	if( pontszam <= 3 )
		return;
	for( int i = k; i < pontszam-1; i++ ) {
		ponttomb[i].x = ponttomb[i+1].x;
		ponttomb[i].y = ponttomb[i+1].y;
   }
   pontszam--;
}

double gyuru::legkozpont( double x, double y, int* pk ) {
	double tav = 1000000000000000.0;
	*pk = 0;
	for( int i = 0; i < pontszam; i++ ) {
		double dx = ponttomb[i].x-x;
		double dy = ponttomb[i].y-y;
		double ujtav = dx*dx + dy*dy;
		if( ujtav < tav ) {
			tav = ujtav;
			*pk = i;
		}
   }
   return sqrt( tav );
}

int gyuru::metszesszam( vekt2 r1, vekt2 v1 ) {
	int metszes = 0;
	for( int i = 0; i < pontszam; i++ ) {
		double x1 = ponttomb[i].x;
		double y1 = ponttomb[i].y;
		double x2, y2;
		if( i < pontszam-1 ) {
			// Nem az utolso pont:
			x2 = ponttomb[i+1].x;
			y2 = ponttomb[i+1].y;
		}
		else {
			// Az utolso pont:
			x2 = ponttomb[0].x;
			y2 = ponttomb[0].y;
		}
		vekt2 r2( x1, y1 );
		vekt2 v2 = vekt2( x2, y2 ) - r2;
		if( vektormetszes( r1, v1, r2, v2 ) )
			metszes++;
	}
	return metszes;
}

int gyuru::metszeshely( vekt2 r1, vekt2 v1, int kihagy, vekt2* phely ) {
	// Eloszor felvesszuk allando szakasz min es max pontjait:
	double minx;
	if( v1.x >= 0 )
		minx = r1.x;
	else
		minx = r1.x+v1.x;
	double maxx;
	if( v1.x >= 0 )
		maxx = r1.x+v1.x;
	else
		maxx = r1.x;
	// Y:
	double miny;
	if( v1.y >= 0 )
		miny = r1.y;
	else
		miny = r1.y+v1.y;
	double maxy;
	if( v1.y >= 0 )
		maxy = r1.y+v1.y;
	else
		maxy = r1.y;

	// Vegigmegyunk gyuru pontjain:
	for( int i = 0; i < pontszam; i++ ) {
		if( i == kihagy )
			continue;

		// Szomszedos szakaszokat nem vizsgalunk:
		if( kihagy >= 0 ) {
			if( kihagy == 0 ) {
				if( i == pontszam - 1 )
					continue;
			}
			else {
				if( i == kihagy - 1 )
					continue;
			}

			if( kihagy == pontszam - 1 ) {
				if( i == 0 )
					continue;
			}
			else {
				if( i == kihagy + 1 )
					continue;
			}
		}

		double x1 = ponttomb[i].x;
		double y1 = ponttomb[i].y;
		double x2, y2;
		if( i < pontszam-1 ) {
			// Nem az utolso pont:
			x2 = ponttomb[i+1].x;
			y2 = ponttomb[i+1].y;
		}
		else {
			// Az utolso pont:
			x2 = ponttomb[0].x;
			y2 = ponttomb[0].y;
		}

		// Eloalltak masodik szakasz pontjai is, megnezzuk hatha annyira
		// kilognak egymasbol, hogy rogton kizarhatjuk metszest:
		if( (x1 < minx && x2 < minx) ||
			(x1 > maxx && x2 > maxx) ||
			(y1 < miny && y2 < miny) ||
			(y1 > maxy && y2 > maxy) )
			continue;

		vekt2 r2( x1, y1 );
		vekt2 v2 = vekt2( x2, y2 ) - r2;
		if( vektormetszes3( r1, v1, r2, v2 ) ) {
			*phely = crossingpoint( r1, v1, r2, v2 );
			return 1;
		}
	}
	return 0;

}

int gyuru::getpontszam( void ) {
	return pontszam;
}

gyuru::gyuru( FILE* h ) {
	pontszam = 0;
	lefoglaltmaxpont = 0;
	ponttomb = NULL;

	if( fread( &pontszam, 1, sizeof( pontszam ), h ) != sizeof( pontszam ) )
		hiba( "gyuru::gyuru-ben nem megy file olvasas!" );

	if( pontszam < 3 || pontszam > MAXPONT ) {
		hiba( "gyuru::gyuru-ben pontszam < 3 || pontszam > MAXPONT!" );
	}

	lefoglaltmaxpont = pontszam+10;
	ponttomb = new vekt2[lefoglaltmaxpont];
	for( int i = 0; i < lefoglaltmaxpont; i++ ) {
		ponttomb[i].x = 0;
		ponttomb[i].y = 0;
	}
	if( fread( ponttomb, 1, sizeof( vekt2 ) * pontszam, h ) !=
											sizeof( vekt2 ) * pontszam )
						hiba( "gyuru::gyuru-ben nem megy file olvasasa!" );
}

void gyuru::save( FILE* h, topol* ptop ) {
	// Ugy kell elmenteni, hogy ha sorban megyunk vegig pontokon,
	// akkor mindig baloldalon legyen fold:
	// Eloszor megnezzuk, hogy vonal ureg, vagy folddarab-e:
	int levegoben = ptop->levegoben( this ); // Ha levegoben, akkor folddarab

	int pozitivjaras = pozitivkorjaras();

	int csere = (levegoben && !pozitivjaras) || (!levegoben && pozitivjaras);

	csere = !csere; // mivel fejjel lefele fogjuk beolvasni

	if( csere ) {
		//dialog( "Most cserel." );

		// Meg kell cserelni pontok sorrendjet:
		for( int i = 0; i < pontszam/2; i++ ) {
			int j = pontszam-1-i;
			vekt2 tmpv = ponttomb[i];
			ponttomb[i] = ponttomb[j];
			ponttomb[j] = tmpv;
		}
	}

	// Itt jon a mentes lenyegeben:
	if( fwrite( &pontszam, 1, sizeof( pontszam ), h ) != sizeof( pontszam ) )
		hiba( "gyuru::save-ben nem megy file-ba iras!" );
	if( fwrite( ponttomb, 1, sizeof( vekt2 ) * pontszam, h ) !=
											sizeof( vekt2 ) * pontszam )
						hiba( "gyuru::save-ben nem megy file-ba iras!" );
}

// Visszaadja b pontban fekvo belso szoget, ha belso oldal bal oldal a, b, c
// haladas eseten:
static double balszog( vekt2* pa, vekt2 b, vekt2 c ) {
		while( 1 ) {
		double dy1 = pa->y-b.y;
		double dx1 = pa->x-b.x;
		if( dy1 == 0.0 && dx1 == 0 )
			// Ezt mar topologia check-ben meg kellett volna szuntetni:
			hiba( "balszog-ben ket pont egybe esik!" );
		double alfa = atan2( dy1, dx1 );
		double dy2 = c.y-b.y;
		double dx2 = c.x-b.x;
		if( dy2 == 0.0 && dx2 == 0 )
			// Ezt mar topologia check-ben meg kellett volna szuntetni:
			hiba( "balszog-ben ket pont egybe esik!" );
		double beta = atan2( dy2, dx2 );
		double szog = alfa-beta;
		if( szog < 0 )
			szog += 2*K_pi;
		if( szog > 2*K_pi - 0.0000001 )
			szog -= 2*K_pi;
		if( szog < 0.0000002 ) {
			// Szog tul hegyes, egyik pontjat arrebb rakjuk:
			static const double egytav = 0.0002;
			double tav = egytav + egytav*s_random( 1000 )/1000.0;
			vekt2 meroleges = *pa - b;
			meroleges = forgatas90fokkal( meroleges );
			meroleges.normal();
			*pa = *pa + meroleges*egytav;
			//dialog( "Arrebb raktuk!" );

			continue;
		}
		return szog;
	}
}

static void szetvalaszt_fv( vekt2* pa, vekt2* pb ) {
	static const double egytav = 0.0002;
	if( fabs( pa->x-pb->x ) < 0.0000002 && fabs( pa->y-pb->y ) < 0.0000002 ) {
		// Egy helyen vannak:
		pa->x += egytav + egytav*s_random( 1000 )/1200.0;
		pa->y += egytav + egytav*s_random( 1000 )/1200.0;
	}
}

void gyuru::szetvalaszt( void ) {
	for( int i = 0; i < pontszam; i++ ) {
		if( i < pontszam-1 )
			szetvalaszt_fv( &ponttomb[i], &ponttomb[i+1] );
		else
			szetvalaszt_fv( &ponttomb[i], &ponttomb[0] );
	}
}

// akkor ad igazat, ha korbejaraskor poligon belseje bal oldalon van;
int gyuru::pozitivkorjaras( void ) {
	// Sorban osszeadjuk belso szogeket:
	double sum = 0.0;
	for( int i = 0; i < pontszam-2; i++ ) {
		sum += balszog( &ponttomb[i], ponttomb[i+1], ponttomb[i+2] );
	}
	sum += balszog( &ponttomb[pontszam-2], ponttomb[pontszam-1], ponttomb[0] );
	sum += balszog( &ponttomb[pontszam-1], ponttomb[0], ponttomb[1] );
	double maradek = sum-pontszam*K_pi;
	if( fabs( fabs( maradek ) - 2*K_pi ) > 0.1 ) {
		/*char tmp[100];
		sprintf( tmp, "Nagy az elteres, x100 = %d!", int(fabs( fabs( maradek ) - 2*K_pi )*100) );
		dialog( tmp );*/
		return 0; // Ilyenkor biztos hogy van egymast metszo ket vonal
	}
	if( maradek > 0.0 )
		return 0;
	else
		return 1;
}

vekt2 gyuru::sulypont( void ) {
	vekt2 suly( 0.0, 0.0 );
	for( int i = 0; i < pontszam; i++ ) {
		suly = suly + ponttomb[i];
	}
	suly = suly * (1.0/pontszam);
	return suly;
}

void gyuru::getminmax( double* px1, double* py1, double* px2, double* py2 ) {
	for( int i = 0; i < pontszam; i++ ) {
		double x = ponttomb[i].x;
		double y = ponttomb[i].y;
		if( *px1 > x )
			*px1 = x;
		if( *py1 > y )
			*py1 = y;
		if( *px2 < x )
			*px2 = x;
		if( *py2 < y )
			*py2 = y;
	}
}

double gyuru::belyegszamitas( void ) {
	double belyeg = 0;
	for( int i = 0; i < pontszam; i++ ) {
		belyeg += ponttomb[i].x;
		belyeg += ponttomb[i].y;
	}
	return belyeg;
}


