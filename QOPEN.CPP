#include	"all.h"

static int Egybol = 1;

static char Nagyfilenev[] = "across.res";

struct fileleiro {
	char nev[16];
	long kezdet, hossz;
};

static fileleiro* Leirotomb = NULL;

#define FILESZAM (150)

#define MAGICNUMBER (1347839l)

static long Fileszam = 0;

static void titkosit( void ) {
	short a = 23, b = 9782, c = 3391;

	if( !Leirotomb )
		hiba( "titkosit-ban !Leirotomb!" );
	int leiromeret = sizeof( fileleiro );
	for( int i = 0; i < FILESZAM; i++ ) {
		unsigned char* pc = (unsigned char*)(&Leirotomb[i]);
		for( int j = 0; j < leiromeret; j++ ) {
			pc[j] ^= a;
			a %= c;
			b += a*c;
			a = 31*b+c;
		}
	}
}

static int Qopeninited = 0;

void initqopen( void ) {
	if( Qopeninited )
		hiba( "initqopen tobbszor hivva!" );
	Qopeninited = 1;

	Egybol = 1;
	if( access( "files.inf", 0 ) == 0 )
		Egybol = 0;

	if( !Egybol )
		return;

	{ Leirotomb = NEW(fileleiro*) fileleiro[FILESZAM]; }
	if( !Leirotomb )
		uzenet( "initqopen-ben memory!" );
	FILE* h = fopen( Nagyfilenev, "rb" );
	if( !h )
		uzenet( "Missing file!: ", Nagyfilenev );

	if( fread( &Fileszam, 1, 4, h ) != 4 )
		hiba( "initqopen-ben nem sikerult olvasni!" );
	if( Fileszam <= 0 || Fileszam > FILESZAM )
		hiba( "Fileszam <= 0 || Fileszam > FILESZAM!" );

	long hossz = sizeof( fileleiro[FILESZAM] );
	if( hossz > 64000l )
		hiba( "Tul hosszu Leirotomb initqopen-ben!" );
	if( fread( Leirotomb, 1, unsigned(hossz), h ) != hossz )
		hiba( "initqopen-ben nem sikerult olvasni!" );
	titkosit();

	long magicnumber = 0;
	if( fread( &magicnumber, 1, 4, h ) != 4 )
		hiba( "initqopen-ben nem sikerult olvasni!" );
	if( magicnumber != MAGICNUMBER )
		hiba( "initqopen-ben nem stimmel magicnumber!" );

	fclose( h );
}

static char* Csomagbuffer = NULL;

void becsomagol( void ) {
	{ Csomagbuffer = NEW(char*) char[60000u]; }
	if( !Csomagbuffer )
		uzenet( "becsomagol-ban memory!" );
	{ Leirotomb = NEW(fileleiro*) fileleiro[FILESZAM]; }
	if( !Leirotomb )
		uzenet( "becsomagol-ban memory!" );
	char tmp[30] = "..\\";
	strcat( tmp, Nagyfilenev );
	FILE* h = fopen( tmp, "wb" );
	if( !h )
		hiba( "becsomagol-ban nem sikerult megnyitni!" );

	Fileszam = 0;
	if( fwrite( &Fileszam, 1, 4, h ) != 4 )
		hiba( "becsomagol-ban nem sikerult irni!" );

	long hossz = sizeof( fileleiro[FILESZAM] );
	if( hossz > 64000l )
		hiba( "Tul hosszu Leirotomb initqopen-ben!" );
	if( fwrite( Leirotomb, 1, unsigned(hossz), h ) != hossz )
		hiba( "becsomagol-ban nem sikerult irni!" );

	long magicnumber = MAGICNUMBER;
	if( fwrite( &magicnumber, 1, 4, h ) != 4 )
		hiba( "becsomagol-ban nem sikerult irni!" );

	find_t ffblk;
	int done;
	done = _dos_findfirst( "*.*", _A_NORMAL, &ffblk );
	int kov = 0;
	while( !done ) {
		if( kov == FILESZAM )
			hiba( "becsomagolban tul sok file!" );
		if( strlen( ffblk.name ) > 12 )
			hiba( "ffblk.name ) > 12!" );
		strcpy( Leirotomb[kov].nev, ffblk.name );
		Leirotomb[kov].kezdet = ftell( h );

		// File bemasolasa:
		FILE* hs = fopen( ffblk.name, "rb" );
		if( !hs )
			hiba( "becsomagolban nem nyilik file!: ", ffblk.name );
		// Masolas:
		while( 1 ) {
			long hossz = fread( Csomagbuffer, 1, 60000u, hs );
			if( !hossz )
				break;
			if( fwrite( Csomagbuffer, 1, unsigned(hossz), h ) != hossz )
				hiba( "Nem megy iras becsomagolban!" );
		}
		fclose( hs );

		Leirotomb[kov].hossz = ftell( h ) - Leirotomb[kov].kezdet;
		if( Leirotomb[kov].hossz <= 0 )
			hiba( "Hossz <= 0!" );

		done = _dos_findnext( &ffblk );
		kov++;
	}

	if( fseek( h, 0, SEEK_SET ) != 0 )
		hiba( "fseek 0-t adott vissza!" );
	Fileszam = kov;
	if( fwrite( &Fileszam, 1, 4, h ) != 4 )
		hiba( "becsomagol-ban nem sikerult irni!" );

	titkosit();
	if( fwrite( Leirotomb, 1, unsigned(hossz), h ) != hossz )
		hiba( "becsomagol-ban nem sikerult irni!" );
	titkosit();

	fclose( h );
}

#define MAXNYITVA (3)
static FILE* Htomb[MAXNYITVA];
static int Indextomb[MAXNYITVA];

static int Nyitva = 0;

FILE* qopen( char* nev, char* paramok ) {
	if( !Qopeninited )
		hiba( "qopen initqopen nelkul!" );
	if( Nyitva  == MAXNYITVA )
		hiba( "qopen-ben Nyitva == MAXNYITVA!" );
	if( strcmp( paramok, "rb" ) != 0 && strcmp( paramok, "rt" ) )
		hiba( "qopen-ben sem rb, sem rt!: ", nev );

	if( Egybol ) {
		for( int i = 0; i < Fileszam; i++ ) {
			if( strcmpi( nev, Leirotomb[i].nev ) == 0 ) {
				Htomb[Nyitva] = fopen( Nagyfilenev, paramok );
				Indextomb[Nyitva] = i;
				if( !Htomb[Nyitva] )
					hiba( "Nem sikerult nagy tombot megnyitni!: ",
													Nagyfilenev );
				if( fseek( Htomb[Nyitva], Leirotomb[i].kezdet, SEEK_SET ) != 0 )
					hiba( "Nem mukodik fseek qopen-ben!" );

				Nyitva++;
				return Htomb[Nyitva-1];
			}
		}
		hiba( "qopen nem talalta meg nagyfileban file-t!: ", nev );
		return NULL;
	}
	else {
		Nyitva++;
		char tmp[30] = "..\\files\\";
		if( strlen( nev ) > 12 || nev[0] == '.' )
			hiba( "Hibas nev qopen-ben!: ", nev );
		strcat( tmp, nev );
		return fopen( tmp, paramok );
	}
}

void qclose( FILE* h ) {
	if( !Qopeninited )
		hiba( "qclose initqopen nelkul!" );
	if( !Nyitva )
		hiba( "qclose-ban !Nyitva!" );
	if( Egybol ) {
		for( int i = 0; i < Nyitva; i++ ) {
			if( Htomb[i] == h ) {
				fclose( h );
				for( int j = i; j < Nyitva-1; j++ ) {
					Htomb[j] = Htomb[j+1];
					Indextomb[j] = Indextomb[j+1];
				}
				Nyitva--;
				return;
			}
		}
		hiba( "qclose nem talalta meg h-t!" );
	}
	else {
		Nyitva--;
		fclose( h );
	}
}

int qseek( FILE *h, long offset, int whence ) {
	if( whence != SEEK_SET && whence != SEEK_END && whence != SEEK_CUR )
		hiba( "whence != SEEK_SET && whence != SEEK_END && whence != SEEK_CUR!" );
	if( Egybol ) {
		for( int i = 0; i < Nyitva; i++ ) {
			if( Htomb[i] == h ) {
				int index = Indextomb[i];
				if( whence == SEEK_SET ) {
					return fseek( Htomb[i], Leirotomb[index].kezdet+offset,
																SEEK_SET );
				}
				if( whence == SEEK_END ) {
					return fseek( Htomb[i], Leirotomb[index].kezdet +
								Leirotomb[index].hossz + offset, SEEK_SET );
				}
				return fseek( Htomb[i], offset, SEEK_CUR );
			}
		}
		hiba( "qclose nem talalta meg h-t!" );
		return NULL;
	}
	else {
		return fseek( h, offset, whence );
	}
}

