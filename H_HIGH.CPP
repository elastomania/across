#include	"all.h"

int Hangenabled = 0;

int Mute = 1;

#define WAVBANKSZAM (20)
typedef wav* wavramut;
wavramut Wavbank[WAVBANKSZAM];

//wav *Pberreg = NULL;

static wav *Pw1 = NULL, *Pw2 = NULL, *Pw3 = NULL, *Pw4 = NULL, *Psurl = NULL;
static wav2 *Pw42 = NULL;

static int Mix = 100, I1 = 7526,
					  I2 = 34648,
					  I3 = 38766,
					  I4 = 14490,
					  I5 = 18906;

static double Egypermix = 1.0/Mix;

static int Buffsize = 500;
static int Atmenet = 499;

static int Hanghighbevoltkapcsolva = 0;

static int Wavszam = 0;
#define MAXWAV  (5)
static int Ezmegy[MAXWAV];
static int Kovhang[MAXWAV];
static unsigned long Hangerok[MAXWAV];
static wav *Pwavok[MAXWAV];

static int Elsostart = 1;

void starthanghigh( void ) {
	if( !Elsostart )
		hiba( "starthanghigh-ban !Elsostart!" );
	Elsostart = 0;
	if( !Hangenabled )
		return;

	Mute = 1;
	if( !starthang( Buffsize ) ) {
		Hangenabled = 0;
		return;
	}
	if( Hanghighbevoltkapcsolva )
		hiba( "Hanghighbevoltkapcsolva igaz starthanghigh-ban!" );
	Hanghighbevoltkapcsolva = 1;
	for( int i = 0; i < WAVBANKSZAM; i++ ) {
		Wavbank[i] = NULL;
	}
	{ Wavbank[WAV_UTODES] = NEW(wav*) wav( "utodes.wav", 0.25 ); }
	{ Wavbank[WAV_TORES] = NEW(wav*) wav( "torik.wav", 0.34 ); }
	{ Wavbank[WAV_SIKER] = NEW(wav*) wav( "siker.wav", 0.8 ); } // 0.34
	{ Wavbank[WAV_EVES] = NEW(wav*) wav( "eves.wav", 0.5 ); }
	{ Wavbank[WAV_FORDULAS] = NEW(wav*) wav( "fordul.wav", 0.3 ); }
	{ Wavbank[WAV_UGRAS1] = NEW(wav*) wav( "ugras.wav", 0.34 ); }
	{ Wavbank[WAV_UGRAS2] = Wavbank[WAV_UGRAS1]; }

	{ Psurl = NEW(wav*) wav( "dorzsol.wav", 0.44 ); }
	Psurl->loopol( Mix );

	double berregero = 0.26;
	{ Pw1 = NEW(wav*) wav( "harl.wav", berregero, 0, I1+Mix ); }
	{ Pw2 = NEW(wav*) wav( "harl.wav", berregero, I1, I2 ); }
	{ Pw3 = NEW(wav*) wav( "harl.wav", berregero, I2-Mix, I3 ); }
	{ Pw4 = NEW(wav*) wav( "harl2.wav", berregero, I4, I5 ); }

	Pw2->loopol( Mix );
	//Pw2->save( "alacsony.wav" );
	Pw4->loopol( Mix );
	Pw3->vegereilleszt( Pw4, Mix );

	{ Pw42 = NEW(wav2*) wav2( Pw4 ); }
	Pw42->reset();

	for( i = 0; i < MAXWAV; i++ ) {
		Ezmegy[i] = 0;
		Kovhang[i] = 0;
		Hangerok[i] = 0;
		Pwavok[i] = NULL;
	}
	Mute = 0;
}

void stophanghigh( void ) {
	if( !Hangenabled )
		return;
	if( !Hanghighbevoltkapcsolva )
		hiba( "Hanghighbevoltkapcsolva hamis stophanghigh-ban!" );
	stophang();
	for( int i = 0; i < WAVBANKSZAM; i++ ) {
		if( Wavbank[i] ) {
			DELETE Wavbank[i]; }
			Wavbank[i] = NULL;
		}
	}
}

static int Gazhang = 0;

static double Frekvenciamost = 1.0;
static double Frekvenciakell = 1.0;

// frekvencia 1.0-tol 2.0-ig valtozik csak:
void setmotor( double frekvencia, int gaz ) {
	if( !Hangenabled )
		return;

	Gazhang = gaz;
	if( frekvencia > 2.0 )
		frekvencia = 2.0;
	if( frekvencia < 1.0 )
		frekvencia = 1.0;
	Frekvenciakell = frekvencia;
}

static double Surleromost = 0.0;
static double Surlerokell = 0.0;

void setsurlodas( double ero ) {
	if( ero > 1.0 )
		ero = 1.0;
	if( ero < 0 )
		hiba( "ero < 0!" );
	Surlerokell = ero;
}

void startwave( int wavazonosito, double hangero ) {
	if( !Hangenabled || Mute )
		return;

	if( hangero <= 0.0 || hangero >= 1.0 )
		hiba( "wav: hangero <= 0.0 || hangero >= 1.0!" );

	wav* pwav = Wavbank[wavazonosito];

	if( Wavszam >= MAXWAV )
		return;

	_disable();
	for( int i = 0; i < MAXWAV; i++ ) {
		if( !Ezmegy[i] ) {
			// i. csatorna szabad!
			Wavszam++;
			Ezmegy[i] = 1;
			Kovhang[i] = 0;
			Pwavok[i] = pwav;
			Hangerok[i] = hangero*65536.0;
			_enable();
			return;
		}
	}
	_enable();
	hiba( "Wavszam szerint meg van csat, de nincs!" );
}

int vanmegwav( void ) {
	if( Hangenabled && Wavszam > 0 )
		return 1;
	else
		return 0;
}

// hangero meg van szorozva 65536-tal:
static void wavadd( short* buff, short* tomb,
			long size, unsigned long hangero ) {
	for( unsigned long i = 0; i < size; i++ ) {
		buff[i] += short((tomb[i]*hangero)>>16);
	}
}

enum allapot { A_INDIT = 0, A_ALACSONY, A_ATMENETBE, A_ATMENET,
					A_MAGAS, A_VISSZAMENET };

static int Allapot = A_INDIT, I_indit, I_alacsony, I_atmenet;

void startmotor( void ) {
	Allapot = A_INDIT;
	I_indit = 0;
}

static void shortcpy( short* dest, short* sour, int number ) {
	if( number == 0 )
		return;
	memcpy( dest, sour, number*2 );
}

static void motorelintezes( short* sbuff, int gaz ) {
	//for( int j = 0; j < Buffsize; j++ )
	//	sbuff[j] = 0;
	//return;

	// Most mindig erre a fv.-hivasra vonatkozik!:
	int counter = 0;
	int masolando = -1;
	int ucsoutani = -1;
	int novekmeny = -1;
	int i = -1;
	while( 1 ) {
		switch( Allapot ) {
			case A_INDIT:
				if( I_indit+Buffsize > Pw1->size ) {
					// Meg most befejezi A_INDIT-ot:
					int masolando = Pw1->size-I_indit;
					shortcpy( &sbuff[counter], &Pw1->tomb[I_indit], masolando );
					counter += masolando;
					Allapot = A_ALACSONY;
					I_alacsony = Mix;
				}
				else {
					// Most teljes egeszeben indit-bol megy:
					int masolando = Buffsize-counter;
					shortcpy( &sbuff[counter], &Pw1->tomb[I_indit], masolando );
					I_indit += masolando;
					return;
				}
				break;
			case A_ALACSONY:
				if( gaz ) {
					Allapot = A_ATMENETBE;
					I_atmenet = 0;
				}
				else {
					// Korbe-korbe:
					int masolando = Buffsize-counter;
					if( masolando > Pw2->size-I_alacsony ) {
						// Most ujra elejere kell meg ugrani:
						masolando = Pw2->size-I_alacsony;
						shortcpy( &sbuff[counter], &Pw2->tomb[I_alacsony], masolando );
						counter += masolando;
						I_alacsony = 0;
					}
					else {
						// Most egy darabban lehet masolni alacsonybol:
						shortcpy( &sbuff[counter], &Pw2->tomb[I_alacsony], masolando );
						I_alacsony += masolando;
						return;
					}
				}
				break;
			case A_ATMENETBE:
				// alacsonyt egyre kisebb sullyal veszi,
				// atmenetet pedig egyre nagyobbal:
				masolando = Buffsize-counter;
				ucsoutani = I_atmenet+masolando;
				if( ucsoutani > Mix ) {
					// Meg most befejezodik mixeles:
					ucsoutani = Mix;
					Allapot = A_ATMENET;
				}
				novekmeny = 0;
				for( i = I_atmenet; i < ucsoutani; i++ ) {
					if( I_alacsony >= Pw2->size )
						I_alacsony = 0;
					double arany = i*Egypermix;
					sbuff[counter+novekmeny] = arany*Pw3->tomb[i] +
									(1-arany)*Pw2->tomb[I_alacsony];
					I_alacsony++;
					novekmeny++;
				}
				counter += novekmeny;
				I_atmenet += novekmeny;
				if( counter+novekmeny == Buffsize )
					return;
				break;
			case A_ATMENET:
				masolando = Buffsize-counter;
				if( masolando > Pw3->size-I_atmenet ) {
					// Mar most at kell terni magas-ra:
					masolando = Pw3->size-I_atmenet;
					shortcpy( &sbuff[counter], &Pw3->tomb[I_atmenet], masolando );
					counter += masolando;
					Allapot = A_MAGAS;
					Pw42->reset( Mix );
					Frekvenciamost = 1.0;
				}
				else {
					// Most meg teljes egeszeben atmenet megy ki:
					shortcpy( &sbuff[counter], &Pw3->tomb[I_atmenet], masolando );
					I_atmenet += masolando;
					return;
				}
				break;
			case A_MAGAS:
				if( !gaz ) {
					Allapot = A_ALACSONY;
					I_alacsony = 0;
				}
				else {
					int masolando = Buffsize-counter;
					long dtmost = 65536.0*Frekvenciamost;
					long dtkell = 65536.0*Frekvenciakell;
					long ddt = 0;
					if( masolando > 30 ) {
						ddt = (dtkell-dtmost)/((double)masolando);
					}
					for( int i = 0; i < masolando; i++ ) {
						sbuff[counter+i] = Pw42->getnextsample( dtmost );
						dtmost += ddt;
					}
					Frekvenciamost = ((double)dtmost)/65536.0;
					return;
				}
				break;
		}
	}
}

static int Surltart = 0;

// Ennek hozza kell adnia eddigi ertekhez:
static void surlodaselintezes( short* sbuff ) {
	/*if( !Ppic8 ) {
		{ Ppic8 = NEW(pic8*) pic8( 320, 2 ); }
		for( int i = 0; i < 320; i++ ) {
			Ppic8->ppixel( i, 0, 0 );
			Ppic8->ppixel( i, 1, 0 );
		}
	}
	int hatar = Surlerokell*320;
	blt8( Pscr8, Ppic8, hatar, 100 ); */


	if( Surlerokell < 0.1 && Surleromost < 0.1 ) {
		Surleromost = 0.0;
		return;
	}

	long eromost = 65536.0*Surleromost;
	long erokell = 65536.0*Surlerokell;
	long dero = (erokell-eromost)/((double)Buffsize);
	int meret = Psurl->size;
	for( int i = 0; i < Buffsize; i++ ) {
		long ertek = Psurl->tomb[Surltart];
		Surltart++;
		if( Surltart >= meret )
			Surltart = 0;

		ertek *= eromost;
		sbuff[i] += (short)(ertek >> 16);

		eromost += dero;
	}
	Surleromost = ((double)eromost)/65536.0;
}

void callbackhang( short* sbuff, unsigned char* cbuff ) {
	if( !Hangenabled )
		hiba( "callbackhang, pedig !Hangenabled!" );
	#ifdef TEST
	if( Atmenet >= Buffsize )
		hiba( "Atmenet >= Buffsize!" );
	#endif
	memset( sbuff, 0, Buffsize*2 );
	if( Mute ) {
		// Nem kell hang:
		if( Wavszam > 0 ) {
			Wavszam = 0;
			for( int i = 0; i < MAXWAV; i++ )
				Ezmegy[i] = 0;
		}
		if( cbuff )
			memset( cbuff, 128, Buffsize );
		else
			memset( sbuff, 0, Buffsize*2 );
		return;
	}

	motorelintezes( sbuff, Gazhang );
	surlodaselintezes( sbuff );

	// Wavok lejatszasa:
	for( int i = 0; i < MAXWAV; i++ ) {
		if( Ezmegy[i] ) {
			int darab = Buffsize;
			if( darab > Pwavok[i]->size-Kovhang[i] ) {
				darab = Pwavok[i]->size-Kovhang[i];
				Ezmegy[i] = 0;
				Wavszam--;
				if( Wavszam < 0 )
					hiba( "Wavszam < 0 !" );
			}
			wavadd( sbuff, &Pwavok[i]->tomb[Kovhang[i]], darab, Hangerok[i] );
			Kovhang[i] += darab;
		}
	}

	if( cbuff ) {
		for( int i = 0; i < Buffsize; i++ ) {
			cbuff[i] = (unsigned char)((sbuff[i]>>8)+128);
		}
	}
}


void hangosdelay( long t ) {
	if( !Hangenabled ) {
		delay( t );
		return;
	}
	for( int i = 0; i < t/20; i++ ) {
		delay( 20 );
		hang();
	}
}

