#include	"all.h"

#define BYTEN (1)

// Fizikai kepernyore mutat:
pic8* Pscr8 = NULL;
static pic8* Pegyszervett8 = NULL;

// Res: 0 -> text, 1 -> 320x200, 3 ->640x400, 3 ->640x480:
static int Res = 0;

static int Granszorzo = 1;

// ELOSZOR DRV FUNCTIONS:

static void setmodevesa8( int res ) {
	if( res != 1 && res != 2 && res != 3 && res != 4 )
		hiba( "setmodevesa8_l-ban res != 1 && res != 2 && res != 3 && res != 4!" );

	unsigned short modszam = 0;
	if( res == 1 || res == 2 ) {
		initmode320x200_drv();
		return;
	}
	if( res == 3 )
		modszam = 0x100;
	if( res == 4 )
		modszam = 0x101;
	if( !modszam )
		hiba( "setvesamode8-ben ismeretlen mod!" );

	unsigned short retvalue = 1000;

	/*#ifdef WATCOM
		union	REGS	inregs, outregs;
		inregs.x.eax = 0x13;
		inregs.x.ebx = 0;
		int386( 16, &inregs, &outregs );
	#else
		asm {
			push ax; push bx; push cx; push dx; push di; push si;
		};
		_AX = 0x13;
		_BX = 0;
		asm int 16;
		asm {
			pop si; pop di; pop dx; pop cx; pop bx; pop ax;
		};
	#endif*/

	#ifdef WATCOM
		union	REGS	inregs, outregs;
		inregs.x.eax = 0x4f02;
		inregs.x.ebx = modszam;
		int386( 16, &inregs, &outregs );
		retvalue = outregs.x.eax;
	#else

	asm {
		push ax
		push bx
		push cx
		push dx
	};
	_AX = 0x4f02;
	_BX = modszam;
	asm	int 16;
	retvalue = _AX;

	asm {
		pop dx
		pop cx
		pop bx
		pop ax
	};
	#endif

	if( retvalue != 0x4f ) {
		char tmp[10];
		itoa( retvalue, tmp, 16 );
		uzenet( "VESA driver initialization failure!" );
	}

	// Granularitas beallitasa:
	int granularitas = 64;
	#ifdef WATCOM
		int getgranularitas( void );
		granularitas = getgranularitas();
	#endif

	Granszorzo = 1;
	switch( granularitas ) {
		case 1: Granszorzo = 64; break;
		case 2: Granszorzo = 32; break;
		case 4: Granszorzo = 16; break;
		case 8: Granszorzo = 8; break;
		case 16: Granszorzo = 4; break;
		case 32: Granszorzo = 2; break;
		case 64: Granszorzo = 1; break;
	}
}

static void bankvaltas( unsigned short bankszam ) {
	bankszam *= Granszorzo;

	unsigned short retvalue = 1000;

	#ifdef WATCOM
		union	REGS	inregs, outregs;
		inregs.x.eax = 0x4f05;
		inregs.x.ebx = 0;
		inregs.x.edx = bankszam;
		int386( 16, &inregs, &outregs );
		retvalue = outregs.x.eax;
	#else
		asm {
			push ax
			push bx
			push cx
			push dx
		};
		_AX = 0x4f05;
		_BX = 0;
		_DX = bankszam;
		asm	int 16;
		retvalue = _AX;

		asm {
			pop dx
			pop cx
			pop bx
			pop ax
		};
	#endif


	if( retvalue != 0x4f ) {
		char tmp[10];
		itoa( retvalue, tmp, 10 );
		uzenet( "VESA driver failure!" );
	}
}

// INNENTOL LOW LEVEL FUNCTIONS:

// y-al lehet indexelni oket:
static long *Torott = NULL,     // boole
			*Kezdocim = NULL,   // pixel-ben megadva
			*Toresx = NULL,     // pixel-ben megadva
			*Bankszam = NULL;   // egesz

static int Xsize = -1, Ysize = -1;
static int Kurbank = 0;

static unsigned char* Basemutato = NULL;

void setmode8_l( int res ) {
	if( res != 1 && res != 2 && res != 3 && res != 4 )
		hiba( "setmode8_l-ban res != 1 && res != 2 && res != 3 && res != 4!" );

	//if( Res == res )
	//	return;
	Res = res;

	unsigned short szegmens;
	#ifdef DOSX286
		DosMapRealSeg( 0x0a000, 0x010000l, &szegmens );
		Basemutato = (unsigned char*)MK_FP( szegmens, 0 );
	#endif
	#ifdef DOS
		szegmens = 0x0a000;
		Basemutato = (unsigned char*)MK_FP( szegmens, 0 );
	#endif
	#ifdef WATCOM
		szegmens = 0x0a000;
		Basemutato = (unsigned char*)(szegmens*16);
	#endif

	if( Basemutato == NULL )
		hiba( "setmode8_l-ben Basemutato == NULL!" );

	if( res == 1 ) {
		Xsize = 320;
		Ysize = 200;
	}
	// Ezt valojaban 320x200-as modban fogjuk megjeleniteni vagassal:
	if( res == 2 ) {
		Xsize = 320;
		Ysize = 240;
	}
	if( res == 3 ) {
		Xsize = 640;
		Ysize = 400;
	}
	if( res == 4 ) {
		Xsize = 640;
		Ysize = 480;
	}

	setmodevesa8( res );

	// Mindig csak egyszer kell kitolteni tomboket, mivel csak 640x480-hoz
	// vesszuk fel tomboket, ami jo 640x400-hoz is, mivel 640 marad:
	if( Res == 1 || Res == 2 || Torott )
		return;

	// Ideiglenesen (fv. vegen visszaallitjuk):
	Ysize = 480;

	{ Torott = NEW(long*) long[Ysize]; }
	{ Kezdocim = NEW(long*) long[Ysize]; }
	{ Toresx = NEW(long*) long[Ysize]; }
	{ Bankszam = NEW(long*) long[Ysize]; }
	if( !Torott || !Kezdocim || !Toresx || !Bankszam ) {
		hiba( "Nem sikerult lefoglalni initmode_l-ben!" );
		return;
	}
	bankvaltas( 0 );
	Kurbank = 0;

	int soreltolas = BYTEN*Xsize;
	for( long y = 0; y < Ysize; y++ ) {
		long soreleje = y*soreltolas; // byte-ban megadva (mar *2)
		long sorutolso = y*soreltolas+Xsize*BYTEN-1; // byte-ban megadva (mar *2)
		long bankeleje = soreleje>>16;
		long bankutolso = sorutolso>>16;
		if( bankeleje == bankutolso ) {
			// Nincs benne tores:
			Torott[int(y)] = 0;
			Kezdocim[int(y)] = soreleje-(bankeleje<<16); // Meg byte-ban
			//if( Kezdocim[int(y)] % BYTEN )
			//	hiba( "setmode8_l-ben Kezdocim[int(y)] % BYTEN!" );
			Kezdocim[int(y)] = Kezdocim[int(y)]/BYTEN; // Pixelben megadva
			Toresx[int(y)] = 0;
			Bankszam[int(y)] = bankeleje;
		}
		else {
			// Van benne tores:
			Torott[int(y)] = 1;
			Kezdocim[int(y)] = soreleje-(bankeleje<<16);
			//if( Kezdocim[int(y)] % BYTEN )
			//	hiba( "setmode8_l-ben Kezdocim[int(y)] % BYTEN!" );
			Kezdocim[int(y)] = Kezdocim[int(y)]/BYTEN; // Pixelben megadva
			Toresx[int(y)] = 0x10000l-Kezdocim[int(y)]*BYTEN;
			//if( Toresx[int(y)] % BYTEN )
			//	hiba( "setmode8_l-ben Toresx[int(y)] % BYTEN!" );
			Toresx[int(y)] = Toresx[int(y)]/BYTEN;
			Bankszam[int(y)] = bankeleje;
		}
	}
	if( Pegyszervett8 == NULL ) {
		{ Pegyszervett8 = NEW(pic8*) pic8( 10, 10 ); }
		Pegyszervett8->fizkep = 1;
		Pegyszervett8->xsize = Xsize;
		Pegyszervett8->ysize = Ysize;
	}
	Pscr8 = Pegyszervett8;
	// Mivel csak ideiglenesen volt mindenkeppen 480:
	if( res == 3 )
		Ysize = 400;
}


void putline8_l( int x, int y, unsigned char* tomb, int xsize ) {
	#ifdef PICLOWTEST
	if( Res == 0 ) {
		hiba( "putbytes8_l-ben nem Res == 0!" );
		return;
	}
	if( x < 0 || x+xsize-1 >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "putbytes8_l-ben kepen kivulre iras!" );
		return;
	}
	#endif
	if( Res == 1 ) {
		// 320x200 mod:
		memcpy( Basemutato+320*y+x, tomb, xsize*BYTEN );
		return;
	}
	if( Res == 2 ) {
		// 320x240 mod, de valojaban 320x200-as mod:
		if( y >= 200 )
			return; // Ha lelog, kihagyjuk:
		else
			memcpy( Basemutato+320*y+x, tomb, xsize*BYTEN );
		return;
	}
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bankvaltas( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
		return;
	}

	// A sor torott:
	// Elejenek elintezese:
	int maradekx = xsize;
	int tombfelex = 0;
	if( x < Toresx[unsigned(y)] ) {
		// Elso bankban biztos van:
		if( Kurbank != Bankszam[y] ) {
			bankvaltas( unsigned(Bankszam[y]) );
			Kurbank = int(Bankszam[y]);
		}
		int atlog = 0;
		if( x+xsize-1 >= Toresx[unsigned(y)] )
			atlog = 1;
		if( !atlog ) {
			memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
			return;
		}
		else {
			int xs = int(Toresx[unsigned(y)]-x);
			memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xs*BYTEN );
			maradekx = xsize-xs;
			tombfelex = xs;
		}
	}

	// Masodik bank elintezese (biztos, hogy ebben is van mit tenni!):
	if( Kurbank != Bankszam[y]+1 ) {
		bankvaltas( unsigned(Bankszam[y]+1) );
		Kurbank = int(Bankszam[y]+1);
	}
	int atlog = 0;
	if( x < Toresx[unsigned(y)] )
		atlog = 1;
	if( !atlog ) {
		memcpy( Basemutato+unsigned(x-Toresx[y]), tomb, xsize*BYTEN );
	}
	else {
		// Atlog:
		memcpy( Basemutato, tomb+tombfelex, maradekx*BYTEN );
	}
}

void getline8_l( int x, int y, unsigned char* tomb, int xsize ) {
	#ifdef PICLOWTEST
	if( Res == 0 ) {
		hiba( "getbytes8_l-ben nem Res == 0!" );
		return;
	}
	if( x < 0 || x+xsize-1 >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "getbytes8_l-ben kepen kivulrol olvasas!" );
		return;
	}
	#endif
	if( Res == 1 ) {
		// 320x200 mode:
		memcpy( tomb, Basemutato+y*320+x, xsize*BYTEN );
		return;
	}
	if( Res == 2 ) {
		// 320x240-es mod, de valojaban 320x200-as mod:
		if( y >= 200 )
			memset( tomb, 0, xsize*BYTEN );
		else
			memcpy( tomb, Basemutato+y*320+x, xsize*BYTEN );
		return;
	}
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		memcpy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize*BYTEN );
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bankvaltas( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		memcpy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize*BYTEN );
		return;
	}

	// A sor torott:
	// Elejenek elintezese:
	int maradekx = xsize;
	int tombfelex = 0;
	if( x < Toresx[unsigned(y)] ) {
		// Elso bankban biztos van:
		if( Kurbank != Bankszam[y] ) {
			bankvaltas( unsigned(Bankszam[y]) );
			Kurbank = int(Bankszam[y]);
		}
		int atlog = 0;
		if( x+xsize-1 >= Toresx[unsigned(y)] )
			atlog = 1;
		if( !atlog ) {
			memcpy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xsize*BYTEN );
			return;
		}
		else {
			int xs = int(Toresx[unsigned(y)]-x);
			memcpy( tomb, Basemutato+unsigned(Kezdocim[y])+x, xs*BYTEN );
			maradekx = xsize-xs;
			tombfelex = xs;
		}
	}

	// Masodik bank elintezese (biztos, hogy ebben is van mit tenni!):
	if( Kurbank != Bankszam[y]+1 ) {
		bankvaltas( unsigned(Bankszam[y]+1) );
		Kurbank = int(Bankszam[y]+1);
	}
	int atlog = 0;
	if( x < Toresx[unsigned(y)] )
		atlog = 1;
	if( !atlog ) {
		memcpy( tomb, Basemutato+unsigned(x-Toresx[y]), xsize*BYTEN );
	}
	else {
		// Atlog:
		memcpy( tomb+tombfelex, Basemutato, maradekx*BYTEN );
	}
}

void putpixel8_l( int x, int y, unsigned char szin ) {
	#ifdef PICLOWTEST
	if( Res == 0 ) {
		hiba( "putbytes8_l-ben nem Res == 0!" );
		return;
	}
	if( x < 0 || x >= Xsize || y < 0 || y >= Ysize ) {
		hiba( "putpixel8_l-ben kepen kivulre iras!" );
		return;
	}
	#endif
	if( Res == 1 ) {
		// 320x200 mod:
		//memcpy( Basemutato+320*y+x, tomb, xsize*BYTEN );
		Basemutato[320*y+x] = szin;
		return;
	}
	if( Res == 2 ) {
		// 320x240 mod, de valojaban 320x200-as mod:
		if( y >= 200 )
			return; // Ha lelog, kihagyjuk:
		else
			//memcpy( Basemutato+320*y+x, tomb, xsize*BYTEN );
			Basemutato[320*y+x] = szin;
		return;
	}
	// Ez a leggyakoribb eset:
	if( !Torott[y] && Kurbank == Bankszam[y] ) {
		//memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
		Basemutato[unsigned(Kezdocim[y])+x] = szin;
		return;
	}
	if( !Torott[y] ) {
		// Csak bankot kell valtani!:
		bankvaltas( unsigned(Bankszam[y]) );
		Kurbank = int(Bankszam[y]);
		//memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
		Basemutato[unsigned(Kezdocim[y])+x] = szin;
		return;
	}

	// A sor torott:
	if( x < Toresx[unsigned(y)] ) {
		// Elso bankban biztos van:
		if( Kurbank != Bankszam[y] ) {
			bankvaltas( unsigned(Bankszam[y]) );
			Kurbank = int(Bankszam[y]);
		}
		//memcpy( Basemutato+unsigned(Kezdocim[y])+x, tomb, xsize*BYTEN );
		Basemutato[unsigned(Kezdocim[y])+x] = szin;
		return;
	}

	// Masodik bank elintezese (biztos, hogy ebben is van mit tenni!):
	if( Kurbank != Bankszam[y]+1 ) {
		bankvaltas( unsigned(Bankszam[y]+1) );
		Kurbank = int(Bankszam[y]+1);
	}
	//memcpy( Basemutato+unsigned(x-Toresx[y]), tomb, xsize*BYTEN );
	Basemutato[unsigned(x-Toresx[y])] = szin;
}

void settextmode( void ) {
	Res = 0;
	#ifdef WATCOM
		union	REGS	inregs, outregs;
		inregs.x.eax = 3;
		int386( 16, &inregs, &outregs );
	#else
		asm {
			push ax; push bx; push cx; push dx; push di; push si;
		};
		_AX = 3;
		asm int 16;
		asm {
			pop si; pop di; pop dx; pop cx; pop bx; pop ax;
		};
	#endif
}

