#include	"all.h"

static int Highres;

static int Vonalvizhossz, Vonalfugghossz;

static int Digdx, Kozdx, Koztesdx;

int Digitxsize, Digitysize;

static void kikettospont( int x, int y, unsigned char szin,
												pic8* ppic8 ) {
	if( Highres ) {
		ppic8->ppixel( x+17, y+9, szin );
		ppic8->ppixel( x+18, y+9, szin );
		ppic8->ppixel( x+17, y+10, szin );
		ppic8->ppixel( x+18, y+10, szin );

		ppic8->ppixel( x+18, y+23, szin );
		ppic8->ppixel( x+17, y+23, szin );
		ppic8->ppixel( x+18, y+24, szin );
		ppic8->ppixel( x+17, y+24, szin );
	}
	else {
		ppic8->ppixel( x+10, y+3, szin );
		ppic8->ppixel( x+10, y+8, szin );
	}
}

static void kiszegmens( int szam, int x, int y, unsigned char szin,
													pic8* ppic8 ) {
	switch( szam ) {
		case 0:
			ppic8->fuggszegmens( x,
								  y+1,
								  Vonalfugghossz, szin );
			break;
		case 1:
			ppic8->fuggszegmens( x+Vonalvizhossz+1,
								  y+1,
								  Vonalfugghossz, szin );
			break;
		case 2:
			ppic8->fuggszegmens( x,
								  y+Vonalfugghossz+2,
								  Vonalfugghossz, szin );
			break;
		case 3:
			ppic8->fuggszegmens( x+Vonalvizhossz+1,
								  y+Vonalfugghossz+2,
								  Vonalfugghossz, szin );
			break;
		case 4:
			ppic8->vizszegmens( x+1,
								 y,
								 Vonalvizhossz, szin );
			break;
		case 5:
			ppic8->vizszegmens( x+1,
								 y+Vonalfugghossz+1,
								 Vonalvizhossz, szin );
			break;
		case 6:
			ppic8->vizszegmens( x+1,
								 y+Vonalfugghossz+Vonalfugghossz+2,
								 Vonalvizhossz, szin );
			break;
		default:
			hiba( "kiszegmens-ben szegmens out of range!" );
			break;
	}
}

static void kiegydigit( int c, int x, int y, unsigned char szin,
													pic8* ppic8 ) {
	switch( c ) {
		case '0':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 0, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '1':
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			break;
		case '2':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 0, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '3':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '4':
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			break;
		case '5':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '6':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 0, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '7':
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '8':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 0, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		case '9':
			kiszegmens( 4, x, y, szin, ppic8 );
			kiszegmens( 1, x, y, szin, ppic8 );
			kiszegmens( 5, x, y, szin, ppic8 );
			kiszegmens( 2, x, y, szin, ppic8 );
			kiszegmens( 3, x, y, szin, ppic8 );
			kiszegmens( 6, x, y, szin, ppic8 );
			break;
		default:
			hiba( "kiegydigit-ben c out of range!" );
	}
}

void kidigit( char* time1, double time2d, unsigned char szin1,
				unsigned char szin2, pic8* ppic8, int res ) {
	int x, y;

	if( res == 1 || res == 2 ) {
		// Low resolution:
		Highres = 0;

		x = 14;
      if( res == 1 )
			// 200
			y = 170;
		else
			// 240
			y = 210;

		Vonalvizhossz = 5,
		Vonalfugghossz = 8;

		Digdx = 8,
		Kozdx = 12,
		Koztesdx = 182;

		Digitxsize = Digdx*7+Kozdx*4+Koztesdx,
		Digitysize = 2*Vonalfugghossz+5;
	}
	else {
		// High resolution:
		Highres = 1;

		x = 28;
		if( res == 3 )
			// 400
			y = 340;
		else
			// 480
			y = 420;

		Vonalvizhossz = 10,
		Vonalfugghossz = 16;

		Digdx = 16,
		Kozdx = 24,
		Koztesdx = 364;

		Digitxsize = Digdx*7+Kozdx*4+Koztesdx,
		Digitysize = 2*Vonalfugghossz+5;
	}


	long l2 = time2d*100.0/(182.0*0.0024);
	char time2[20];
	ido2string( l2, time2 );
	// Ket ido elo van allitva!
	// Elso:
	if( time1[0] != 0 ) {
		kiegydigit( time1[0], x, y, szin1, ppic8 );
		x += Digdx;
		kiegydigit( time1[1], x, y, szin1, ppic8 );
		kikettospont( x, y, szin1, ppic8 );
		x += Kozdx;
		kiegydigit( time1[3], x, y, szin1, ppic8 );
		x += Digdx;
		kiegydigit( time1[4], x, y, szin1, ppic8 );
		kikettospont( x, y, szin1, ppic8 );
		x += Kozdx;
		kiegydigit( time1[6], x, y, szin1, ppic8 );
		x += Digdx;
		kiegydigit( time1[7], x, y, szin1, ppic8 );
	}
	else {
		x += Digdx*3+Kozdx*2;
	}

	x += Koztesdx;
	// Masodik:
	kiegydigit( time2[0], x, y, szin2, ppic8 );
	x += Digdx;
	kiegydigit( time2[1], x, y, szin2, ppic8 );
	kikettospont( x, y, szin2, ppic8 );
	x += Kozdx;
	kiegydigit( time2[3], x, y, szin2, ppic8 );
	x += Digdx;
	kiegydigit( time2[4], x, y, szin2, ppic8 );
	kikettospont( x, y, szin2, ppic8 );
	x += Kozdx;
	kiegydigit( time2[6], x, y, szin2, ppic8 );
	x += Digdx;
	kiegydigit( time2[7], x, y, szin2, ppic8 );
}


