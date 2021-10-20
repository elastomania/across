#include	"all.h"

#define	kb_data		0x060
#define kb_ctl      0x061
#define kb_reset    0x080
#define itc_eoi     32

static volatile int Irtam = 0, Marfut = 0;

void mk_wmkeydown( int kod );
void mk_wmkeyup( int kod );
void mk_wmchar( int kod );

#ifdef DOSX286
	REALPTR Old_real;
	PIHANDLER Old_prot;
#endif
#ifdef DOS
	void interrupt  (*cim)(...);
#endif
#ifdef WATCOM
	void (__interrupt __far *cim)();
#endif


#ifdef DOSX286
	static void interrupt far billkezelo( void ) {
#endif
#ifdef DOS
	static void interrupt far billkezelo( ... ) {
#endif
#ifdef WATCOM
	static void __interrupt billkezelo() {
#endif


	unsigned char  al, al2, al3;

	al = inp( kb_data );
	al2 = inp( kb_ctl );


	al3 = char(al2 | kb_reset);
	outp( kb_ctl, al3 );
	outp( kb_ctl, al2 );

	if( al < 128 ) {
		Irtam = 1;
		mk_wmkeydown( al );
	}
	else
		mk_wmkeyup( al-128 );
	outp( 0x020, itc_eoi );
}

void	billstart( void ) {
	delay( 200 );
	if( !Marfut ) {
		#ifdef DOSX286
			DosSetPassToProtVec( 9,
				(PIHANDLER)billkezelo,
				&Old_prot,
				&Old_real);
		#else
			cim = _dos_getvect( 9 );
			_dos_setvect( 9, billkezelo );
		#endif

		Marfut = 1;
	}
}

void    billstop( void ) {
	int		kesz;

	if( Marfut ) {
		kesz = 0;
		while( !kesz ) {
			Irtam = 0;
			kesz = 1;
			//for( i = 0; i < 128; i++ )
			//	if( leny[i] )
			//		kesz = 0;
			_disable();
			if( kesz && !Irtam ) {
				#ifdef DOSX286
					DosSetRealProtVec( 9, Old_prot, Old_real,
						NULL, NULL);
				#else
					_dos_setvect( 9, cim );
				#endif
				Marfut = 0;
			}
			_enable();
		}
	}
	Marfut = 0;
}

// MK_.H-ban levo fuggvenyek:

#define MAXKOD (126)
static volatile int Tomb[MAXKOD+1];

static volatile int Buffsize = 30;
static volatile int* Buffer = NULL;
static volatile int Charszam = 0;

static volatile char Konvtomb[] = "^^1234567890_^^^qwertyuiop^^^^asdfghjkl^^^^^zxcvbnm^.^^^^ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";

void mk_init( void ) {
	Konvtomb[1] = 1;    // ESC
	Konvtomb[28] = MK_ENTER;
	Konvtomb[72] = MK_UP;
	Konvtomb[80] = MK_DOWN;
	Konvtomb[75] = MK_LEFT;
	Konvtomb[77] = MK_RIGHT;
	Konvtomb[73] = MK_PGUP;
	Konvtomb[81] = MK_PGDOWN;
	Konvtomb[83] = MK_DEL;
	Konvtomb[14] = MK_BACKSPACE;

	Konvtomb[42] = 42;  // SHIFT
	Konvtomb[54] = 42;  // SHIFT

	for( int i = 0; i < MAXKOD; i++ )
		Tomb[i] = 0;

	{ Buffer = NEW(int*) int[Buffsize]; }
	if( !Buffer )
		hiba( "memory mk_init-ben!" );
	Charszam = 0;
}

static int konvtomk( int c ) {
	if( c == 331 ) return MK_LEFT;
	if( c == 333 ) return MK_RIGHT;
	if( c == 336 ) return MK_DOWN;
	if( c == 328 ) return MK_UP;
	if( c == 13 ) return MK_ENTER;
	if( c == 27 ) return MK_ESC;
	if( c == 329 ) return MK_PGUP;
	if( c == 337 ) return MK_PGDOWN;
	if( c == 339 ) return MK_DEL;
	if( c == 8 ) return MK_BACKSPACE;
	return c;
}

int mk_getextchar( void ) {
	if( Billint ) {
		while( 1 ) {
			if( Charszam > 0 ) {
				_disable();
				int c = Buffer[0];
				for( int i = 0; i < Charszam-1; i++ )
					Buffer[i] = Buffer[i+1];
				Charszam--;
				_enable();
				return c;
			}
		}
	}
	else {
		int c = getch();
		if( !c )
			c = 256+getch();
		c = konvtomk( c );
		// torlendo, torol
		/*if( c == 'q' ) {
			// Kiirjuk kepet lemezre:
			if( Editbuffer )
				Editbuffer->save( "snap_ed.pcx", Korny->pal_fomenu->tomb );
		} */
		return c;
	}
}

void mk_emptychar( void ) {
	if( Billint ) {
		_disable();
		Charszam = 0;
		_enable();
	}
	else {
		while( kbhit() ) {
			int c = getch();
			if( !c )
				c = 256+getch();
		}
		for( int c = 0; c < MAXKOD; c++ )
			Tomb[c] = 0;
	}
}

int mk_kbhit( void ) {
	if( Billint ) {
		if( Charszam > 0 )
			return 1;
		else
			return 0;
	}
	else {
		return kbhit();
	}
}

int mk_getstate( int kod ) {
	if( kod <= 0 || kod >= MAXKOD )
		hiba( "mk_getstate-ben kod nem ismert!" );
	if( Billint ) {
		return Tomb[kod];
	}
	else {
		// Eloszor is csinal egy kis vizsgalatot:
		while( kbhit() ) {
			int c = getch();
			if( !c )
				c = 256+getch();
			c = konvtomk( c );

			if( c > 0 && c < MAXKOD )
				Tomb[c] = 10;
		}
		if( Tomb[kod] > 0 ) {
			Tomb[kod]--;
			return 1;
		}
		return 0;
	}
}


// Kovetkezo billentyu fuggvenyek nem publikusak:

void mk_wmkeydown( int kod ) {
	if( kod < 0 || kod >= MAXKOD )
		return;
	int valodikod = Konvtomb[kod];
	if( valodikod == '^' )
		return;
	Tomb[valodikod] = 1;
	mk_wmchar( valodikod );
}

void mk_wmkeyup( int kod ) {
	if( kod < 0 || kod >= MAXKOD )
		return;
	int valodikod = Konvtomb[kod];
	if( valodikod == '^' )
		return;
	Tomb[valodikod] = 0;
}

void mk_wmchar( int kod ) {
	if( kod == 42 )
		return;
	if( Charszam < Buffsize ) {
		if( Tomb[42] ) {
			// Ha SHIFT le van nyomva:
			if( kod >= 0x61 && kod <= 0x7a )
				kod -= 0x20;
		}
		Buffer[Charszam] = kod;
		Charszam++;
	}
}

