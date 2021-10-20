#include	"all.h"

static int Elemszam = 8;
static int X0 = 300, Y0 = 40, Xhossz = 300, Dy = 20, Dxszov = 5, Dyszov = 5;
static int Dxbetu = 8;

static void kiirsor( int sorszam, char* text, float ertek, int kurx, int kury ) {
	kurzorki( kurx, kury );
	setwritemode( COPY_PUT );
	setcolor( ColorBG );
	// Torles:
	for( int y = Y0+sorszam*Dy+1; y < Y0+(sorszam+1)*Dy; y++ )
		line( X0+1, y, X0+Xhossz-1, y );

	setcolor( ColorFG );

	char tmp[50];
	char tmp2[20];

	strcpy( tmp, text );
	if( ertek >= -0.1 ) {
		sprintf( tmp2, "%f", ertek );
		strcat( tmp, tmp2 );
	}

	outtextxy( X0+Dxszov, Y0+sorszam*Dy+Dyszov, tmp );

	setwritemode( XOR_PUT );
	setcolor( EGA_WHITE );
	kurzorki( kurx, kury );
}

static int char2szam( double* pd, char* text, int hossz ) {
	*pd = -1.0;
	if( hossz <= 0 )
		return 0;
	int cur = 0;
	double ertek = 0;
	int egeszek = 1;
	double helyiertek = 0.1;
	while( cur < hossz && text[cur] != ' ' ) {
		char c = text[cur];
		int jokar = 0;
		if( c == '.' ) {
			jokar = 1;
			if( !egeszek )
				return 0;
			egeszek = 0;
		}
		if( c >= '0' && c <= '9' ) {
			jokar = 1;
			int jegy = c-'0';
			if( egeszek ) {
				ertek *= 10.0;
				ertek += jegy;
			}
			else {
				ertek += jegy*helyiertek;
				helyiertek *= 0.1;
			}
		}

		if( !jokar )
			return 0;
		cur++;
	}
	*pd = ertek;
	return 1;
}

static void kibetu( int pos, char c ) {
	setwritemode( COPY_PUT );
	setcolor( ColorBG );
	// Torles:
	for( int y = Y0+1; y < Y0+Dy; y++ )
		line( X0+Dxszov+pos*Dxbetu, y, X0+Dxszov+(pos+1)*Dxbetu-1, y );

	setcolor( ColorFG );

	char tmp[5] = "A";
	tmp[0] = c;
	outtextxy( X0+Dxszov+pos*Dxbetu, Y0+Dyszov, tmp );

	setwritemode( XOR_PUT );
	setcolor( EGA_WHITE );
}

#define SORHOSSZ (12)
static double inputszam( double esces, int kurx, int kury ) {
	kurzorki( kurx, kury );
	char sor[SORHOSSZ+10];
	int kov = 0;
	kibetu( kov, '_' );
	while( 1 ) {
		int c = mk_getextchar();
		if( (c >= '0' && c <= '9') || c == '.' ) {
			// Szamjegy vagy '.':
			if( kov < SORHOSSZ ) {
				sor[kov] = c;
				kibetu( kov, c );
				kov++;
				kibetu( kov, '_' );
			}
		}
		if( c == 8 ) {
			// <- (torles):
			if( kov > 0 ) {
				kibetu( kov, ' ' );
				kov--;
				kibetu( kov, '_' );
			}
		}
		if( c == MK_ESC ) {
			kurzorki( kurx, kury );
			kiirsor( 0, "  ", -1.0, kurx, kury );
			return esces;
		}
		if( c == MK_ENTER ) {
			double ertek;
			if( char2szam( &ertek, sor, kov ) ) {
				kurzorki( kurx, kury );
				kiirsor( 0, "  ", -1.0, kurx, kury );
				return ertek;
			}
		}
	}
}

static int getsorszam( int x, int y ) {
	y = 479-y;
	if( x < X0 || x > X0+Xhossz-1 || y < Y0 || y > Y0+Elemszam*Dy-1 )
		return -1;
	return (y-Y0)/Dy;
}



static void kiirtargy( targy* pt, int kurx, int kury ) {
	kurzorki( kurx, kury );
	setwritemode( COPY_PUT );
	setcolor( ColorFG );

	// Keret:
	for( int y = 0; y < Elemszam; y++ ) {
		line( X0, Y0+y*Dy, X0+Xhossz, Y0+y*Dy );
		line( X0, Y0+y*Dy+1, X0, Y0+(y+1)*Dy-1 );
		line( X0+Xhossz, Y0+y*Dy+1, X0+Xhossz, Y0+(y+1)*Dy-1 );
	}
	line( X0, Y0+Elemszam*Dy, X0+Xhossz, Y0+y*Dy );

	setwritemode( XOR_PUT );
	setcolor( EGA_WHITE );
	kurzorki( kurx, kury );

	// Szamok:
	kiirsor( 0, "  ", -1.0, kurx, kury );
	kiirsor( 1, "Mozgo:      ", pt->mozgo, kurx, kury );
	kiirsor( 2, "Szam:       ", pt->szam, kurx, kury );
	kiirsor( 3, "Sebesseg:   ", pt->sebesseg, kurx, kury );
	kiirsor( 4, "Fazis:      ", pt->fazis, kurx, kury );
	kiirsor( 5, "Oda-vissza: ", pt->odavissza, kurx, kury );
	kiirsor( 6, "OK", -1.0, kurx, kury );
	kiirsor( 7, "CANCEL", -1.0, kurx, kury );
}

//kurzorki( x, y );

void edittargy( targy* pt ) {
	int x, y;
	getmousepos( &x, &y );
	kurzorki( x, y );

	targy eredeti = *pt;
	kiirtargy( pt, x, y );
	while( 1 ) {
		if( getbutbmou() ) {
			while( getbutbmou() )
				;
			switch( getsorszam( x, y ) ) {
				case 1:
					pt->mozgo = inputszam( pt->mozgo, x, y ) + 0.1;
					kiirsor( 1, "Mozgo:      ", pt->mozgo, x, y );
					break;
				case 2:
					pt->szam = inputszam( pt->szam, x, y ) + 0.1;
					kiirsor( 2, "Szam:       ", pt->szam, x, y );
					break;
				case 3:
					pt->sebesseg = inputszam( pt->sebesseg, x, y );
					kiirsor( 3, "Sebesseg:   ", pt->sebesseg, x, y );
					break;
				case 4:
					pt->fazis = inputszam( pt->fazis, x, y );
					kiirsor( 4, "Fazis:      ", pt->fazis, x, y );
					break;
				case 5:
					pt->odavissza = inputszam( pt->odavissza, x, y );
					kiirsor( 5, "Oda-vissza: ", pt->odavissza, x, y );
					break;
				case 6:
					return;
				case 7:
					*pt = eredeti;
					return;
			}
		}
		else {
			// Nem nyomtunk meg gombot:
			int xelozo = x;
			int yelozo = y;
			getmousepos( &x, &y );
			if( x != xelozo || y != yelozo ) {
				kurzorki( xelozo, yelozo );
				kurzorki( x, y );
			}
		}
	}
}

