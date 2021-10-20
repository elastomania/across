#include	"all.h"

static long *Torott = NULL, *Kezdocim = NULL, *Toresx = NULL, *Bankszam = NULL;

static int Xsize, Ysize, Modeinited = 0;
static int Kurbank = 0;

static unsigned char* Basemutato = NULL;

void initmode_l( int soreltolas, int color ) {
	unsigned short szegmens;
	#ifdef DOSX286
		DosMapRealSeg( 0x0a000, 0x010000l, &szegmens );
	#else
		szegmens = 0x0a000;
	#endif
	Basemutato = (unsigned char*)MK_FP( szegmens, 0 );
	#ifdef WATCOM
		Basemutato = (unsigned char*)0x0a0000;
	#endif


	int xsize = SCREENXSIZE;
	int ysize = SCREENYSIZE;
	Modeinited = 0;
	initmode_drv( xsize, ysize, color );
	{ Torott = NEW(long*) long[ysize]; }
	{ Kezdocim = NEW(long*) long[ysize]; }
	{ Toresx = NEW(long*) long[ysize]; }
	{ Bankszam = NEW(long*) long[ysize]; }
	if( !Torott || !Kezdocim || !Toresx || !Bankszam ) {
		uzenet( "memory initmode_l-ben!" );
		return;
	}
	bank_drv( 0 );
	Kurbank = 0;
	for( long y = 0; y < ysize; y++ ) {
		long soreleje = y*soreltolas;
		long sorutolso = y*soreltolas+xsize-1;
		long bankeleje = soreleje>>16;
		long bankutolso = sorutolso>>16;
		if( bankeleje == bankutolso ) {
			// Nincs benne tores:
			Torott[int(y)] = 0;
			Kezdocim[int(y)] = soreleje-(bankeleje<<16);
			Toresx[int(y)] = 0;
			Bankszam[int(y)] = bankeleje;
		}
		else {
			// Van benne tores:
			Torott[int(y)] = 1;
			Kezdocim[int(y)] = soreleje-(bankeleje<<16);
			Toresx[int(y)] = 0x10000l-Kezdocim[int(y)];
			Bankszam[int(y)] = bankeleje;
		}
	}
	Xsize = xsize;
	Ysize = ysize;
	Modeinited = 1;
}

void closegraph_l( void ) {
	if( Torott )
		DELETE Torott; }
	if( Kezdocim )
		DELETE Kezdocim; }
	if( Toresx )
		DELETE Toresx; }
	if( Bankszam )
		DELETE Bankszam; }
	Torott = Kezdocim = Toresx = Bankszam = 0;
	if( !Modeinited )
		return;
	closegraph_drv();
	Modeinited = 0;
}

void ppixel_l( int x, int y, int szin ) {
	#ifdef PICLOWTEST
	if( !Modeinited ) {
		hiba( "ppixel_l-ben nem Modeinited!" );
		return;
	}
	if( x < 0 || x >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "ppixel_l-ben kepbol kiiras!" );
		return;
	}
	#endif
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		Basemutato[unsigned(Kezdocim[y]+x)] = szin;
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bank_drv( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		Basemutato[unsigned(Kezdocim[y]+x)] = szin;
		return;
	}
	// A sor torott:
	int bank = int(Bankszam[unsigned(y)]);
	long cim = Kezdocim[unsigned(y)];
	if( x >= Toresx[unsigned(y)] ) {
		// A masodik bankban van pont!
		bank++;
		cim = -Toresx[y];
	}
	if( Kurbank != bank ) {
		// Bankvaltas:
		bank_drv( bank );
		Kurbank = bank;
	}
	Basemutato[unsigned(cim+x)] = szin;
}

int gpixel_l( int x, int y ) {
	#ifdef PICLOWTEST
	if( !Modeinited ) {
		hiba( "gpixel_l-ben nem Modeinited!" );
		return 0;
	}
	if( x < 0 || x >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "gpixel_l-ben kepen kivulrol olvasas!" );
		return 0;
	}
	#endif
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		return Basemutato[unsigned(Kezdocim[y]+x)];
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bank_drv( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		return Basemutato[unsigned(Kezdocim[y]+x)];
	}
	// A sor torott:
	int bank = int(Bankszam[unsigned(y)]);
	long cim = Kezdocim[unsigned(y)];
	if( x >= Toresx[unsigned(y)] ) {
		// A masodik bankban van pont!
		bank++;
		cim = -Toresx[y];
	}
	if( Kurbank != bank ) {
		// Bankvaltas:
		bank_drv( bank );
		Kurbank = bank;
	}
	return Basemutato[unsigned(cim+x)];
}

void putbytes_l( int x, int y, unsigned char* tomb, int xsize ) {
	#ifdef PICLOWTEST
	if( !Modeinited ) {
		hiba( "putbytes_l-ban nem Modeinited!" );
		return;
	}
	if( x < 0 || x+xsize-1 >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "putbytes_l-ban kepen kivulre iras!" );
		return;
	}
	#endif
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		bytecopy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize );
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bank_drv( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		bytecopy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize );
		return;
	}

	// A sor torott:
	// Elejenek elintezese:
	int maradekx = xsize;
	int tombfelex = 0;
	if( x < Toresx[unsigned(y)] ) {
		// Elso bankban biztos van:
		if( Kurbank != Bankszam[y] ) {
			bank_drv( unsigned(Bankszam[y]) );
			Kurbank = int(Bankszam[y]);
		}
		int atlog = 0;
		if( x+xsize-1 >= Toresx[unsigned(y)] )
			atlog = 1;
		if( !atlog ) {
			bytecopy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize );
			return;
		}
		else {
			int xs = int(Toresx[unsigned(y)]-x);
			bytecopy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xs );
			maradekx = xsize-xs;
			tombfelex = xs;
		}
	}

	// Masodik bank elintezese (biztos, hogy ebben is van mit tenni!):
	if( Kurbank != Bankszam[y]+1 ) {
		bank_drv( unsigned(Bankszam[y]+1) );
		Kurbank = int(Bankszam[y]+1);
	}
	int atlog = 0;
	if( x < Toresx[unsigned(y)] )
		atlog = 1;
	if( !atlog ) {
		bytecopy( Basemutato+unsigned(x-Toresx[y]), tomb, xsize );
	}
	else {
		// Atlog:
		bytecopy( Basemutato, tomb+tombfelex, maradekx );
	}
}

void getbytes_l( int x, int y, unsigned char* tomb, int xsize ) {
	#ifdef PICLOWTEST
	if( !Modeinited ) {
		hiba( "getbytes_l-ban nem Modeinited!" );
		return;
	}
	if( x < 0 || x+xsize-1 >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "getbytes_l-ban kepen kivulre iras!" );
		return;
	}
	#endif
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		bytecopy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize );
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bank_drv( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		bytecopy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize );
		return;
	}

	// A sor torott:
	// Elejenek elintezese:
	int maradekx = xsize;
	int tombfelex = 0;
	if( x < Toresx[unsigned(y)] ) {
		// Elso bankban biztos van:
		if( Kurbank != Bankszam[y] ) {
			bank_drv( unsigned(Bankszam[y]) );
			Kurbank = int(Bankszam[y]);
		}
		int atlog = 0;
		if( x+xsize-1 >= Toresx[unsigned(y)] )
			atlog = 1;
		if( !atlog ) {
			bytecopy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize );
			return;
		}
		else {
			int xs = int(Toresx[unsigned(y)]-x);
			bytecopy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xs );
			maradekx = xsize-xs;
			tombfelex = xs;
		}
	}

	// Masodik bank elintezese (biztos, hogy ebben is van mit tenni!):
	if( Kurbank != Bankszam[y]+1 ) {
		bank_drv( unsigned(Bankszam[y]+1) );
		Kurbank = int(Bankszam[y]+1);
	}
	int atlog = 0;
	if( x < Toresx[unsigned(y)] )
		atlog = 1;
	if( !atlog ) {
		bytecopy( tomb, Basemutato+unsigned(x-Toresx[y]), xsize );
	}
	else {
		// Atlog:
		bytecopy( tomb+tombfelex, Basemutato, maradekx );
	}
}




