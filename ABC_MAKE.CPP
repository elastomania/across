#include	"all.h"

#define SORSZAM (9)
#define BETUSZAM (10)    // 320x200
//#define BETUSZAM (11)  // 640x480

static int Megjelenit = 1;

static void abcbesprite( FILE* h, pic8* ppic, unsigned char c, short y ) {
	if( fwrite( "EGYMIX", 7, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: " );
		fclose( h );
		return;
	}
	if( fwrite( &c, 1, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: " );
		fclose( h );
		return;
	}
	if( fwrite( &y, 2, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: " );
		fclose( h );
		return;
	}
	if( !ppic->save( ".spr", NULL, h ) ) {
		hiba( "Nem tudta kiirni SPRITE-ot abc file-ba!: " );
		return;
	}
}

static int belelog_fugg( int x, int y1, int y2, pic8* ppic,
											unsigned char lyuk ) {
	if( y2< y1 )
		hiba( "belelog_fugg-ben y2< y1!" );
	for( int y = y1; y <= y2; y++ )
		if( lyuk != ppic->gpixel( x, y ) )
			return 1;
	return 0;
}

static int belelog_viz( int x1, int x2, int y, pic8* ppic,
											unsigned char lyuk ) {
	if( x2<= x1 )
		hiba( "belelog_viz-ben x2<= x1!" );
	for( int x = x1; x <= x2; x++ )
		if( lyuk != ppic->gpixel( x, y ) )
			return 1;
	return 0;
}


// Igazzal ter vissza, ha ment sprite (ez alapjan lehet novelni szam-ot):
static int egysprite( pic8* pnpic, unsigned char lyuk, char c, int xk, int yk,
								int xsize, int ysize, FILE* h, int refy ) {
	if( c == ' ' )
		return 0;
	int sugar = 5;
	int x1 = xk-sugar;
	int x2 = xk+sugar;
	int y1 = yk-sugar;
	int y2 = yk+sugar;
	// Ide kirajzoljuk keretet:
	Pscr8->line( x1, y1, x2, y1, 100 );
	Pscr8->line( x1, y2, x2, y2, 100 );
	Pscr8->line( x1, y1, x1, y2, 100 );
	Pscr8->line( x2, y1, x2, y2, 100 );
	int voltvaltozas = 1;
	while( voltvaltozas ) {
		voltvaltozas = 0;
		if( kbhit() ) {
			getch();
			semmi();
		}
		// Ellenorzes:
		if( x1 >= x2 )
			hiba( "egysprite-ban x1 >= x2!" );
		if( y1 > y2 )
			hiba( "egysprite-ban y1 > y2!" );
		if( x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0 )
			hiba( "egysprite-ban x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0!" );
		if( x1 >= xsize-1 || x2 >= xsize-1 || y1 >= ysize-1 || y2 >= ysize-1 )
			hiba( "egysprite-ban x1 >= xsize-1 || x2 >= xsize-1 || y1 >= ysize-1 || ...!" );
		// Tagit:
		if( belelog_fugg( x2+1, y1, y2, pnpic, lyuk ) ) {
			x2++; voltvaltozas = 1; }
		if( belelog_fugg( x1-1, y1, y2, pnpic, lyuk ) ) {
			x1--; voltvaltozas = 1; }
		if( belelog_viz( x1, x2, y2+1, pnpic, lyuk ) ) {
			y2++; voltvaltozas = 1; }
		if( belelog_viz( x1, x2, y1-1, pnpic, lyuk ) ) {
			y1--; voltvaltozas = 1; }
		// Szukit:
		if( !belelog_fugg( x2, y1, y2, pnpic, lyuk ) ) {
			x2--; voltvaltozas = 1; }
		if( !belelog_fugg( x1, y1, y2, pnpic, lyuk ) ) {
			x1++; voltvaltozas = 1; }
		if( !belelog_viz( x1, x2, y2, pnpic, lyuk ) ) {
			y2--; voltvaltozas = 1; }
		if( !belelog_viz( x1, x2, y1, pnpic, lyuk ) ) {
			y1++; voltvaltozas = 1; }
	}
	// Megvannak hatarok:
	pic8* pkpic = new pic8( x2-x1+1, y2-y1+1 );
	if( !pkpic )
		hiba( "Nem tudott lefoglalni kiskepnek!" );
	blt8( pkpic, pnpic, -x1, -y1 );

	spriteosit( pkpic, lyuk );

	if( Megjelenit ) {
		static int s_xcur = 50;
		s_xcur += 6;
		//blt8( Pscr8, pnpic );
		blt8( Pscr8, pkpic, s_xcur, 100+y1-refy );
		getch();
	}

	abcbesprite( h, pkpic, c, y1-refy );
	delete pkpic;
	return 1;
}

// d_v.cpp-bol van hivva ha kell:
void kulonexe_abcmake( void ) {
	pic8* pnpic = new pic8( "pcx.pcx" );
	if( !pnpic )
		hiba( "Hasznalat: PCX.PCX kepbol csinal ABC.ABC-t!: " );

	// 320x200-as ABC:
	char betuk[SORSZAM][BETUSZAM+1] = { "abcdefghij",
						  "klmnopqrst",
						  "uvwxyz    ",
						  "ABCDEFGHIJ",
						  "KLMNOPQRST",
						  "UVWXYZ-   ",   // "UVWXYZ-+=*"
						  "0123456789",
						  ":;`\",.<>/?",
						  "!@#$%^&() " };
						  //"[]{}|~    "

	int ytomb[SORSZAM] = { 10,  	// abc
						   44,      // klm
						   78,      // uvw
						   113,     // ABC
						   147,     // KLM
						   181,     // UVW
						   214,     // 012
						   249,     // :
						   283 };   // !

	// Beadjuk sarokbetuk kozepeinek koordinatait:
	int x1 = 27, y1 = 21;
	int x2 = 399, y2 = 292;

	// 640x480-as editor ABC-hez:
	/*char betuk[SORSZAM][BETUSZAM+1] = {
						  "ABCDEFGHIJK",
						  "LMNOPQRSTUV",
						  "WXYZ       ",
						  "  abcdefghi",
						  "jklmnopqrst",
						  "uvwxyz     ",
						  "    0123456",
						  "789.:- ,/?!",
						  "@()*+=     " };

	int ytomb[SORSZAM] = { 70,
						   94,
						   118,
						   142,
						   166,
						   190,
						   214,
						   238,
						   261 };

	// Beadjuk sarokbetuk kozepeinek koordinatait:
	int x1 = 55, y1 = 65;
	int x2 = 215, y2 = 257;      */

	unsigned char lyuk = pnpic->gpixel( 0, 0 );

	int xsize = pnpic->getxsize();
	int ysize = pnpic->getysize();

	setresdd( 4 );

	blt8( Pscr8, pnpic );

	// Megnyitja abc-t irasra:
	char* abcneve = "abc.abc";
	FILE* h = fopen( abcneve, "wb" );
	if( !h ) {
		hiba( "Nem tudta megnyitni abc-t irasra!: ", abcneve );
		return;
	}
	if( fwrite( "RA1", 4, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: ", abcneve );
		fclose( h );
		return;
	}

	// Kitoltetlenul hagyja egyenlore mixek szamat:
	long szamhely = ftell( h );
	short szam = 0;
	if( fwrite( &szam, 2, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: ", abcneve );
		fclose( h );
		return;
	}

	for( int y = 0; y < SORSZAM; y++ ) {
		for( int x = 0; x < BETUSZAM; x++ ) {
			int xk = x1 + double(x2-x1)/(BETUSZAM-1) * x;
			int yk = y1 + double(y2-y1)/(SORSZAM-1) * y;
			if( egysprite( pnpic, lyuk, betuk[y][x],
								xk, yk, xsize, ysize, h, ytomb[y] ) )
				szam++;
		}
	}

	// Utolag kitolti mixek szamat:
	if( fseek( h, szamhely, SEEK_SET ) != 0 )
		hiba( "Nem megy fseek!" );
	if( fwrite( &szam, 2, 1, h ) != 1 ) {
		hiba( "Nem tudja irni abc-t!: ", abcneve );
		fclose( h );
		return;
	}

	fclose( h );

	hiba( "Befejezte ABC konvertalast!" );
}




