#include	"all.h"

#define MAGICNUMBER (4796277l)

//#define Darabszam (10000u)
//#define Hangdarabszam (3000u)

static unsigned Darabszam = 0;
static unsigned Hangdarabszam = 0;

static double Valtrate = 60;
static double Recorderszorzo = Valtrate/(182.0*0.0024);

recorder::recorder( unsigned darabszam, unsigned hangdarabszam ) {
	Darabszam = darabszam;
	Hangdarabszam = hangdarabszam;
	pk1rx = NULL;
	pk1ry = NULL;
	pk2rx = NULL;
	pk2ry = NULL;
	pk4rx = NULL;
	pk4ry = NULL;
	pk2alfa = NULL;
	pk1alfa = NULL;
	pk4alfa = NULL;
	pfrekvencia = NULL;
	pgaz = NULL;
	psurlseb = NULL;
	psurlero = NULL;


	phangok = NULL;

	betoltve = 0;
	hangbetoltve = 0;

	// Ellenorzes:
	long hossz = sizeof( float );
	if( hossz != 4 )
		hiba( "float hossz != 4!" );
	hossz = long(Darabszam)*sizeof( float );
	if( hossz > 64000l )
		hiba( "Darabszam*sizeof( float )!" );

	hossz = long(Hangdarabszam)*sizeof( egyhang );
	if( hossz > 64000l )
		hiba( "Hangdarabszam*sizeof( egyhang )!" );

	{ pk1rx = NEW(float*) float[Darabszam]; }
	{ pk1ry = NEW(float*) float[Darabszam]; }
	{ pk2rx = NEW(float*) float[Darabszam]; }
	{ pk2ry = NEW(float*) float[Darabszam]; }
	{ pk4rx = NEW(float*) float[Darabszam]; }
	{ pk4ry = NEW(float*) float[Darabszam]; }
	{ pk2alfa = NEW(float*) float[Darabszam]; }
	{ pk1alfa = NEW(float*) float[Darabszam]; }
	{ pk4alfa = NEW(float*) float[Darabszam]; }
	{ pfrekvencia = NEW(float*) float[Darabszam]; }
	{ pgaz = NEW(char*) char[Darabszam]; }
	{ phatra = NEW(char*) char[Darabszam]; }
	{ psurlseb = NEW(float*) float[Darabszam]; }
	{ psurlero = NEW(float*) float[Darabszam]; }

	{ phangok = NEW(egyhang*) egyhang[Hangdarabszam]; }

	if( !pk1rx || !pk1ry || !pk2rx || !pk2ry || !pk4rx || !pk4ry ||
	!pk2alfa || !pk1alfa|| !pk4alfa || !pfrekvencia || !pgaz || !phatra ||
	!phangok || !psurlseb || !psurlero )
		uzenet( "memory" );
}

recorder::~recorder( void ) {
	// Ez most nincs megcsinalva!
}

void recorder::erase( long parlevel ) {
	level = parlevel;
	betoltve = 0;
	hangbetoltve = 0;
	ucsokiadva = 0;
	hangkov = 0;
}

void recorder::rewind( void ) {
	ucsokiadva = 0;
	hangkov = 0;
}

int recorder::recall( double dt, hanginfo* phinfo ) {
	if( betoltve <= 0 )
		hiba( "recall-ban betoltve <= 0!" );

	long index = Recorderszorzo*dt;
	if( index < 0 )
		index = 0;
	if( ucsokiadva ) {
		phinfo->frekvencia = 1.0;
		phinfo->gaz = 0;
		return 0;
	}

	if( index >= betoltve-1 ) {
		index = betoltve-1;
		ucsokiadva = 1;
	}

	Kor1.r.x = pk1rx[int(index)];
	Kor1.r.y = pk1ry[int(index)];
	Kor2.r.x = pk2rx[int(index)];
	Kor2.r.y = pk2ry[int(index)];
	Kor4.r.x = pk4rx[int(index)];
	Kor4.r.y = pk4ry[int(index)];
	Kor2.alfa = pk2alfa[int(index)];
	Kor1.alfa = pk1alfa[int(index)];
	Kor4.alfa = pk4alfa[int(index)];

	Hatra = phatra[int(index)];

	phinfo->frekvencia = pfrekvencia[int(index)];
	phinfo->gaz = pgaz[int(index)];
	phinfo->surlseb = psurlseb[int(index)];
	phinfo->surlero = psurlero[int(index)];


	return 1;
}

void recorder::store( double dt, hanginfo* phinfo ) {
	long index = Recorderszorzo*dt;
	if( index < 0 )
		index = 0;
	if( index >= Darabszam )
		// betelt:
		return;
	if( index < betoltve )
		hiba( "recorder::store-ban index < betoltve!" );
	for( long i = betoltve; i <= index; i++ ) {
		pk1rx[int(i)] = Kor1.r.x;
		pk1ry[int(i)] = Kor1.r.y;
		pk2rx[int(i)] = Kor2.r.x;
		pk2ry[int(i)] = Kor2.r.y;
		pk4rx[int(i)] = Kor4.r.x;
		pk4ry[int(i)] = Kor4.r.y;
		pk2alfa[int(i)] = Kor2.alfa;
		pk1alfa[int(i)] = Kor1.alfa;
		pk4alfa[int(i)] = Kor4.alfa;
		phatra[int(i)] = Hatra;

		pfrekvencia[int(i)] = phinfo->frekvencia;
		pgaz[int(i)] = phinfo->gaz;
		psurlseb[int(i)] = phinfo->surlseb;
		psurlero[int(i)] = phinfo->surlero;
	}
	betoltve = index;
}

void recorder::addhang( double t, char wavindex, double hangero, int objszam ) {
	if( hangbetoltve >= Hangdarabszam )
		return;
	if( hangbetoltve > 0 ) {
		if( phangok[unsigned(hangbetoltve-1)].t > t+0.00001 ) {
			char tmp[50];
			double t2 = phangok[unsigned(hangbetoltve-1)].t;
			sprintf( tmp, "t1: kulonbseg: %f\n", float( t - t2 ) );
			hiba( tmp );
			//hiba( "recorder::addhang-ban ido forditott sorrendben!" );
		}
	}
	phangok[unsigned(hangbetoltve)].t = t;
	phangok[unsigned(hangbetoltve)].wavindex = wavindex;
	phangok[unsigned(hangbetoltve)].hangero = hangero;
	phangok[unsigned(hangbetoltve)].objszam = objszam;
	hangbetoltve++;
}

// Ha van hang, akkor igazzal ter vissza:
int recorder::gethang( double t, char* pwavindex, double* phangero,
												int* pobjszam ) {
	if( hangkov < hangbetoltve ) {
		if( phangok[unsigned(hangkov)].t <= t ) {
			*pwavindex = phangok[unsigned(hangkov)].wavindex;
			*phangero = phangok[unsigned(hangkov)].hangero;
			*pobjszam = phangok[unsigned(hangkov)].objszam;
			hangkov++;
			return 1;
		}
	}
	return 0;
}

int recorder::getlevel( void ) {
	return int(level);
}

// demo eseten resource file-bol nyit:
// belyeg-et adja vissza, vagy 0-at:
long recorder::load( char* nev, int demo ) {
	FILE* h;
	if( demo )
		h = qopen( nev, "rb" );
	else
		h = fopen( nev, "rb" );
	if( !h )
		hiba( "Nem sikerult megnyitni recorded file-t!: ", nev );
	betoltve = 0;
	if( fread( &betoltve, 1, 4, h ) != 4 )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( betoltve <= 0 )
		hiba( "Nem stimmel betoltve recorded file-ban!: ", nev );
	int kihagy = 0;
	if( betoltve > Darabszam ) {
		kihagy = (unsigned)betoltve - Darabszam;
		betoltve = Darabszam;
	}

	long verzio = 0;
	if( fread( &verzio, 1, 4, h ) != 4 )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );

	long belyeg = 0;
	if( verzio < 30 ) {
		// 1.0 es 1.1-es verziokban nem volt verzio, hanem ez valojaban
		// palya szama (level) 1-24-ig.
		level = verzio;
		verzio = 100;
		if( level < 0 || level > 24 )
			hiba( "level < 0 || level > 24!" );
	}
	else {
		// Verzio 120:
		if( fread( &belyeg, 1, 4, h ) != 4 )
			hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
		if( fread( &level, 1, 4, h ) != 4 )
			hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
		if( !belyeg && (level < 0 || level >= MAXPALYASZAM) )
			hiba( "belyeg es level nem stimmel recorded file-ban!: ", nev );
		if( belyeg && level != -1 )
			hiba( "belyeg es level nem stimmel recorded file-ban (2)!: ", nev );
	}

	if( verzio != 100 && verzio != 120 )
		uzenet( "Recorded file version is too new!", nev );

	if( verzio == 100 )
		regi = 1; // Lejatszonak ezek utan nem szabad rendeznie objokat
	else
		regi = 0;

	if( verzio == 100 ) {
		// Ide kene palya konverziot csinalni.
		if( level < 0 || level > 23 )
			hiba( "Hibas recorded file !", nev );
		switch( level ) {
			// Ezek meg most is jatszhatoak:
			case 12: level = 11; break; // Loop-de-Loop
			case 15: level = 17; break; // Hangman
			case 17: level = 18; break; // Slalom
			case 18: level = 19; break; // Quick Round
			case 19: level = 20; break; // Ramp Frenzy
			case 20: level = 21; break; // Precarious
			case 21: level = 22; break; // Circuitous
			case 23: level = 38; break; // Expert System

			// Ezek most mar nem jatszhatoak:
			case 22: level = 80; break; // The Enigma
			case 11: level = 81; break; // Fair Trial
			case 13: level = 82; break; // Slippery Slope
			case 16: level = 83; break; // On Top
			case 14: level = 84; break; // The Turnaround (regi)
		}
	}

	if( Sarvari && !belyeg && (level > 15 && level != 22) )
		uzenet( "This REC file can only be replayed with a registered version of the game!",
					nev );

	long hossz = betoltve*sizeof( float );
	long kihagyhossz = kihagy*sizeof( float );

	if( fread( pk1rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk1ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk2rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk2ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk4rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk4ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );

	if( fread( pk1alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk2alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pk4alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( phatra, 1, unsigned(betoltve), h ) != betoltve ) // karakter
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagy, SEEK_CUR );

	if( fread( pfrekvencia, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( pgaz, 1, unsigned(betoltve), h ) != betoltve ) // karakter
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagy, SEEK_CUR );

	if( fread( psurlseb, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );
	if( fread( psurlero, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );

	// Hangok:
	if( fread( &hangbetoltve, 1, 4, h ) != 4 )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( hangbetoltve < 0 )
		hiba( "hangbetoltve nem stimmel loadban!" );
	kihagy = 0;
	if( hangbetoltve > Hangdarabszam ) {
		kihagy = (unsigned)hangbetoltve - Hangdarabszam;
		hangbetoltve = Hangdarabszam;
	}
	hossz = hangbetoltve*sizeof( egyhang );
	kihagyhossz = kihagy*sizeof( egyhang );
	if( fread( phangok, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( kihagy ) qseek( h, kihagyhossz, SEEK_CUR );

	long magic = 0;
	if( fread( &magic, 1, 4, h ) != 4 )
		hiba( "Nem sikerult olvasni recorded file-bol!: ", nev );
	if( magic != MAGICNUMBER )
		hiba( "magic != MAGICNUMBER" );

	if( demo )
		qclose( h );
	else
		fclose( h );

	return belyeg;
}

/* 1.0 es 1.1 eseten ez volt:
	h = fopen( nev, "wb" );
	if( !h )
		hiba( "Nem sikerult megnyitni irasra recorded file-t!: ", nev );

	if( fwrite( &betoltve, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( &level, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );


	long hossz = betoltve*sizeof( float );

	if( fwrite( pk1rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
*/

void recorder::save( char* nev, long belyeg ) {
	if( betoltve == 0 )
		hiba( "save-ben betoltve == 0!" );
	FILE* h;
	// Itt meg kell hagyni fopen-t!:
	h = fopen( nev, "wb" );
	if( !h )
		hiba( "Nem sikerult megnyitni irasra recorded file-t!: ", nev );

	if( fwrite( &betoltve, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	// Verzio csak 1.2-tol kezdodoen jon bele:
	// Elozoleg itt level volt, ami biztos hogy kisebb 120-nal:
	long verzio = 120;
	if( fwrite( &verzio, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );


	if( fwrite( &belyeg, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	if( belyeg )
		level = -1;
	if( fwrite( &level, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	long hossz = betoltve*sizeof( float );

	if( fwrite( pk1rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk1ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk2rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk2ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk4rx, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk4ry, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	if( fwrite( pk1alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk2alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pk4alfa, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( phatra, 1, unsigned(betoltve), h ) != betoltve ) // karakter
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	if( fwrite( pfrekvencia, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( pgaz, 1, unsigned(betoltve), h ) != betoltve ) // karakter
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	if( fwrite( psurlseb, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	if( fwrite( psurlero, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	// Hangok:
	if( fwrite( &hangbetoltve, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );
	hossz = hangbetoltve*sizeof( egyhang );
	if( fwrite( phangok, 1, unsigned(hossz), h ) != hossz )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	long magic = MAGICNUMBER;
	if( fwrite( &magic, 1, 4, h ) != 4 )
		hiba( "Nem sikerult irni recorded file-ba!: ", nev );

	fclose( h );
}

// WAV GYUJTO:

static int Begyujtve = 0;

#define GYUJTODARAB (200)

static int Azonosito[GYUJTODARAB];
static double Hangero[GYUJTODARAB];
static int Objszam[GYUJTODARAB];

void startwavegyujto( int wavazonosito, double hangero, int objszam ) {
	if( Begyujtve < GYUJTODARAB ) {
		Azonosito[Begyujtve] = wavazonosito;
		Hangero[Begyujtve] = hangero;
		Objszam[Begyujtve] = objszam;
		Begyujtve++;
	}
}

void resetwavegyujto( void ) {
	Begyujtve = 0;
}

// Igaz, ha van bent wav:
int getwavegyujto( int* pwavazonosito, double* phangero, int* pobjszam ) {
	if( Begyujtve == 0 )
		return 0;
	*pwavazonosito = Azonosito[0];
	*phangero = Hangero[0];
	*pobjszam = Objszam[0];
	Begyujtve--;
	for( int i = 0; i < Begyujtve; i++ ) {
		Azonosito[i] = Azonosito[i+1];
		Hangero[i] =   Hangero[i+1];
		Objszam[i] = Objszam[i+1];
	}
	return 1;
}

