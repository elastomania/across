#include	"all.h"

static double Szorzo = 1000.0;

void loadtargy( FILE* h, targy* ptargy ) {
	if( !ptargy )
		hiba( "loadtargyban!" );
	char sor[200];
	// mozgo:
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->mozgo ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	// szam:
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->szam ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	// sebesseg:
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	long sebesseg = -1;
	if( sscanf( sor, "%ld", &sebesseg ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	ptargy->sebesseg = sebesseg/Szorzo;
	// fazis:
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	long fazis = -1;
	if( sscanf( sor, "%ld", &fazis ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	ptargy->fazis = fazis/Szorzo;
	// odavissza:
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->odavissza ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
}

void savetargy( FILE* h, targy* ptargy ) {
	if( !ptargy )
		hiba( "savetargyban!" );
	fprintf( h, "%d\n", ptargy->mozgo );
	fprintf( h, "%d\n", ptargy->szam );
	long sebesseg = ptargy->sebesseg*Szorzo;
	fprintf( h, "%ld\n", sebesseg );
	long fazis = ptargy->fazis*Szorzo;
	fprintf( h, "%ld\n", fazis );
	fprintf( h, "%d\n", ptargy->odavissza );
}

short loadint( FILE* h ) {
	int ertek = -1;
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni sort filebol!" );
	if( sscanf( sor, "%d", &ertek ) != 1 )
		hiba( "loadint-ben nem tudta feldolgozni sort!: ", sor );
	if( ertek < 0 )
		hiba( "loadint-ben ertek < 0!: ", sor );
	return short(ertek);
}

void loadstring( FILE* h, char* text, int maxhossz ) {
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni sort filebol!" );
	char tmp[100];
	if( sscanf( sor, "%s", tmp ) != 1 )
		hiba( "loadstring-ben nem tudta feldolgozni sort!: ", sor );
	if( strlen( tmp ) > maxhossz )
		hiba( "loadstring-ben strlen( tmp ) > maxhossz!: ", tmp );
	strcpy( text, tmp );
}

static int loadtargytipus( targytipus* ptipus, FILE* h ) {
	int sorszam = loadint( h );
	ptipus->tipus = loadint( h );
	ptipus->sugar = loadint( h ) * 0.01;
	ptipus->kerekerz = loadint( h );
	ptipus->fejerz = loadint( h );
	ptipus->frameseb = loadint( h );
	ptipus->pottyindex = loadint( h );
	ptipus->frameszam = loadint( h );
	if( ptipus->frameszam > MAXFRAMESZAM )
		hiba( "ptipus->frameszam > MAXFRAMESZAM" );
	if( ptipus->frameszam < 1 )
		hiba( "ptipus->frameszam < 1" );

	// A vegleges kepek meretei:
	int kellx = ptipus->sugar*2*Aranyx;
	int kelly = ptipus->sugar*2*Aranyy;

	for( int i = 0; i < ptipus->frameszam; i++ ) {
		char sor[200];
		if( fgets( sor, 190, h ) == NULL )
			hiba( "Nem tudott olvasni frame nevet filebol!" );
		char nev[200];
		if( sscanf( sor, "%s", nev ) != 1 )
			hiba( "targytomb konstban nem tudta feldolgozni sort!: ", sor );
		pic8* ptmp = NULL;
		{ ptmp = NEW(pic8*) pic8( nev ); }
		unsigned char lyukindex = ptmp->gpixel( 0, 0 );
		{ ptipus->spriteok[i] = NEW(pic8*) pic8( kellx, kelly ); }
		mintavetel8( ptipus->spriteok[i], ptmp );
		DELETE ptmp; }
		// Fejjel lefele forditjuk sprite-ot:
		pic8* ppic = ptipus->spriteok[i];
		int xsize = ppic->getxsize();
		int ysize = ppic->getysize();
		if( i == 0 ) {
			ptipus->dx = xsize*0.5;
			ptipus->dy = ysize*0.5;
		}
		for( int y = 0; y < (ysize>>1); y++ ) {
			for( int x = 0; x < xsize; x++ ) {
				unsigned char szin1 = ppic->gpixel( x, y );
				unsigned char szin2 = ppic->gpixel( x, ysize-1-y );
				ppic->ppixel( x, ysize-1-y, szin1 );
				ppic->ppixel( x, y, szin2 );
			}
		}
		// Kiterjesztes megkeresese:
		int j = strlen( nev ) - 1;
		while( j > 0 ) {
			if( nev[j] == '.' ) {
				if( strcmpi( nev+j, ".pcx" ) == 0 ) {
					// Ki kell lyukasztani sprite-ta!:
					spriteosit( ptipus->spriteok[i], lyukindex );
				}
				break;
			}
			j--;
		}
	}
	return sorszam;
}

static void felszabtargytipus( targytipus* ptipus ) {
	for( int i = 0; i < MAXFRAMESZAM; i++ ) {
		if( ptipus->spriteok[i] )
			DELETE ptipus->spriteok[i]; }
	}
}


targytomb::targytomb( char* nev ) {
	{ targytipusok = NEW(targytipus*) targytipus[MAXTARGYTIPUSSZAM]; }
	if( !targytipusok )
		uzenet( "memory" );

	// Nullazas:
	for( int i = 0; i < MAXTARGYTIPUSSZAM; i++ )
		memset( &targytipusok[i], 0, sizeof( targytipus ) );

	FILE* h = qopen( nev, "rt" );
	if( !h )
		hiba( "Nem tudta megnyitni file-t!: ", nev );

	// Beolvassa targytipusok szamat:
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targytipusszamot!" );
	if( sscanf( sor, "%d", &targytipusszam ) != 1 )
		hiba( "targytomb konstban nem tudta feldolgozni sort!: ", sor );

	if( targytipusszam < 0 || targytipusszam > MAXTARGYTIPUSSZAM )
		hiba( "targytipusszam > MAXTARGYTIPUSSZAM!" );

	// Most jonnek sorra targytipusok:
	for( i = 0; i < targytipusszam; i++ ) {
		if( loadtargytipus( &targytipusok[i], h ) != i )
			hiba( "targytipus sorszama nem stimmel!" );
	}

	qclose( h );
}

targytomb::~targytomb( void ) {
	for( int i = 0; i < targytipusszam; i++ )
		felszabtargytipus( &targytipusok[i] );
	DELETE targytipusok; }
}


