#include	"all.h"

#ifdef SARVARI
	int Palyaszam = 17; // End-del egyutt
#else
	int Palyaszam = 43; // Vizittel egyutt
#endif


static void titkosread( void* mut, int hossz, FILE* h ) {
	if( fread( mut, 1, hossz, h ) != hossz )
		hiba( "Nem ment olvasas state file-bol!" );
	unsigned char* pc = (unsigned char*)mut;
	short a = 23, b = 9782, c = 3391;

	for( int i = 0; i < hossz; i++ ) {
		pc[i] ^= a;
		a %= c;
		b += a*c;
		a = 31*b+c;
	}
}

static void titkoswrite( void* mut, int hossz, FILE* h ) {
	unsigned char* pc = (unsigned char*)mut;
	short a = 23, b = 9782, c = 3391;

	for( int i = 0; i < hossz; i++ ) {
		pc[i] ^= a;
		a %= c;
		b += a*c;
		a = 31*b+c;
	}
	if( fwrite( mut, 1, hossz, h ) != hossz )
		hiba( "Nem ment iras state file-ba!" );
	a = 23; b = 9782; c = 3391;

	for( i = 0; i < hossz; i++ ) {
		pc[i] ^= a;
		a %= c;
		b += a*c;
		a = 31*b+c;
	}
}

static long Magicnumber_state_s = 123432111l;
static long Magicnumber_state_r = 123432222l;

static char Statenev[] = "state.dat";

state::state( void ) {
	memset( palyakidejei, 0, sizeof( palyakidejei ) );
	memset( jatekosok, 0, sizeof( jatekosok ) );
	jatekosokszama = 0;
	jatekos = 0;
	res = 1;
	fakt = 0.60; // 0.68
	soundvolume = 1.0;
	// Itt hagyni kell fopen-t!!!!!!!!!!!!!!!!!!!!:
	//#ifndef VEGLEGES
	if( access( Statenev, 0 ) != 0 )
		return;
	//#endif
	FILE* h = fopen( Statenev, "rb" );
	if( !h )
		hiba( "Nem nyilik state file!: ", Statenev );

	titkosread( palyakidejei, sizeof( palyakidejei ), h );
	titkosread( jatekosok, sizeof( jatekosok ), h );
	titkosread( &jatekosokszama, sizeof( long ), h );
	titkosread( &jatekos, sizeof( long ), h );
	titkosread( &res, 2, h );
	// res konvertalasa, mivel DOS-ban nincs 320x240-es mod:
	if( !Win_95 && res == 2 )
		res = 1;
	short tmp;
	titkosread( &tmp, 2, h );
	fakt = tmp*0.001;
	titkosread( &tmp, 2, h );
	soundvolume = tmp*0.001;

	titkosread( &editfilenev, 20, h );

	// Magic number:
	long magicnumber = 0;
	if( fread( &magicnumber, 1, sizeof( long ), h ) !=
					sizeof( long ) )
		hiba( "Nem megy beolvasas state file-ba!: ", Statenev );
	if( Sarvari ) {
		if( magicnumber != Magicnumber_state_s )
			uzenet( "Corrupt STATE.DAT file!" );
	}
	else {
		if( magicnumber != Magicnumber_state_s &&
			magicnumber != Magicnumber_state_r )
			uzenet( "Corrupt STATE.DAT file!" );
	}

	fclose( h );
}

state::~state( void ) {
}

void state::save( void ) {
	// Itt hagyni kell fopen-t!!!!!!!!!!!!!!!!!!!!:
	FILE* h = fopen( Statenev, "wb" );
	if( !h )
		uzenet( "Could not open for write file!: ", Statenev );

	titkoswrite( palyakidejei, sizeof( palyakidejei ), h );
	titkoswrite( jatekosok, sizeof( jatekosok ), h );
	titkoswrite( &jatekosokszama, sizeof( long ), h );
	titkoswrite( &jatekos, sizeof( long ), h );

	titkoswrite( &res, 2, h );
	short tmp = 1000*fakt;
	titkoswrite( &tmp, 2, h );
	tmp = 1000*soundvolume;
	titkoswrite( &tmp, 2, h );

	titkoswrite( &editfilenev, 20, h );

	long magicnumber = Magicnumber_state_r;
	if( Sarvari )
		magicnumber = Magicnumber_state_s;
	if( fwrite( &magicnumber, 1, sizeof( long ), h ) !=
					sizeof( long ) )
		hiba( "Nem megy beolvasas state file-ba!: ", Statenev );

	fclose( h );
}








