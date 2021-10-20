#include	"all.h"

static void help( void ) {
	blt8( Korny->picbuffer, Korny->ppic_help );

	int x1 = 2;
	int x2 = 102;
	int yo = 4;
	int dy = 24;
	//Korny->pabc_help->write( Korny->picbuffer, x1+60, yo,      "Help" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo,   "UP, A" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo,   "- Accelerate" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy, "DOWN, Z" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy, "- Block Wheels" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*2, "SPACE" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*2, "- Turn Around" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*3, "RIGHT, M" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*3, "- Rotate Clockwise" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*4, "LEFT, N" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*4, "- Rotate AntiClock." );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*5, "S" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*5, "- Sound     Toggle" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*6, "V" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*6, "- PreView Toggle" );

	Korny->pabc_help->write( Korny->picbuffer, x1, yo+dy*7, "T" );
	Korny->pabc_help->write( Korny->picbuffer, x2, yo+dy*7, "- Time         Toggle" );

	//Korny->pal_help->set();
	lassufizre( Korny->picbuffer, Korny->pal_help );
	int done = 0;
	while( !done ) {
		mk_emptychar();
		int c = mk_getextchar();
		if( c == MK_ESC || c == MK_ENTER )
			done = 1;
	}
}

static int Elsorarajz = 1;

static void rarajzmot( pic8* ppic, int x1, int y1, int x2, int y2, double fakt ) {
	if( fakt > 1.0 )
		fakt = 1.0;
	if( x1 >= x2 || y1 >= y2 )
		hiba( "rarajzmot-ban x1 >= x2 || y1 >= y2!" );

	// Megcsinaljuk hatteret:
	unsigned char felszin = Korny->ppic_vollecbal->gpixel( 2, 2 );
	unsigned char keret = Korny->ppic_vollecbal->gpixel( 0, 0 );
	unsigned char vilagos = Korny->ppic_vollecbal->gpixel( 2, 1 );
	unsigned char sotet = Korny->ppic_vollecjobb->gpixel( 0, 2 );
	ppic->fillbox( x1, y1, x2, y2, felszin );
	// Keret:
	ppic->fuggszegmens( x1, y1, y2-y1+1, keret );
	ppic->fuggszegmens( x2, y1, y2-y1+1, keret );
	ppic->vizszegmens( x1, y1, x2-x1+1, keret );
	ppic->vizszegmens( x1, y2, x2-x1+1, keret );
	// Belso csikok:
	ppic->fuggszegmens( x1+1, y1+1, y2-y1-1, vilagos );
	ppic->fuggszegmens( x2-1, y1+1, y2-y1-1, sotet );
	ppic->vizszegmens( x1+1, y1+1, x2-x1-1, vilagos );
	ppic->vizszegmens( x1+1, y2-1, x2-x1-1, sotet );

	fakt *= 0.92;
	double xfelsize = (x2-x1+1)*fakt*0.5;
	double yfelsize = (y2-y1+1)*fakt*0.5;
	double xkozep = (x2+x1)*0.5;
	double ykozep = (y2+y1)*0.5;

	// Kis teglalap koordjai:
	x1 = xkozep-xfelsize;
	x2 = xkozep+xfelsize;
	y1 = ykozep-yfelsize;
	y2 = ykozep+yfelsize;

	if( Elsorarajz ) {
		Elsorarajz = 0;
		int xs = Korny->ppic_setsizemot->getxsize();
		int ys = Korny->ppic_setsizemot->getysize();
		unsigned char lyuk = Korny->ppic_setsizemot->gpixel( 0, 0 );
		for( int y = 0; y < ys; y++ ) {
			for( int x = 0; x < xs; x++ ) {
				if( Korny->ppic_setsizemot->gpixel( x, y ) == lyuk )
					Korny->ppic_setsizemot->ppixel( x, y, felszin );
			}
		}
	}

	mintavetel8( ppic, Korny->ppic_setsizemot, x1, y1, x2, y2 );
}

static void setresolution( void ) {
	valaszt val;
	blt8( Korny->picbuffer, Korny->ppic_times );
	Korny->pabc_times->write( Korny->picbuffer, 70, 10, "Video Mode" );
	//Korny->pal_times->set();
	// Valasztas:
	if( Win_95 ) {
		// WIN95-os valasztas:
		val.szam = 4;

		val.kur = -1;
		if( State->res == 1 )
			val.kur = 0;
		if( State->res == 2 )
			val.kur = 1;
		if( State->res == 3 )
			val.kur = 2;
		if( State->res == 4 )
			val.kur = 3;
		if( val.kur < 0 )
			hiba( "setresolution-ban val.kur < 0!" );

		val.egykepen = 6;
		val.x0 = 80;
		val.y0 = 60;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_times;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "320x200" );
		strcpy( Korny->rubrikak[1], "320x240" );
		strcpy( Korny->rubrikak[2], "640x400" );
		strcpy( Korny->rubrikak[3], "640x480" );

		int eredmeny = val.valassz( Korny->pal_times );

		if( eredmeny < 0 )
			return;

		if( eredmeny == 0 ) {
			State->res = 1;
		}
		if( eredmeny == 1 ) {
			State->res = 2;
		}
		if( eredmeny == 2 ) {
			State->res = 3;
		}
		if( eredmeny == 3 ) {
			State->res = 4;
		}
	}
	else {
		// DOS-os valasztas:
		val.szam = 3; // Ezt kell atirni 4-re, ha kell 320x240 DOS mod

		val.kur = -1;
		if( State->res == 1 )
			val.kur = 0;
		if( State->res == 2 )
			val.kur = 3;
		if( State->res == 3 )
			val.kur = 1;
		if( State->res == 4 )
			val.kur = 2;
		if( val.kur < 0 )
			hiba( "setresolution-ban val.kur < 0!" );

		val.egykepen = 6;
		val.x0 = 80;
		val.y0 = 60;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_times;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "320x200 (Standard)" );
		strcpy( Korny->rubrikak[1], "640x400    (VESA)" );
		strcpy( Korny->rubrikak[2], "640x480    (VESA)" );
		strcpy( Korny->rubrikak[3], "320x240 Ideiglenes" );

		int eredmeny = val.valassz( Korny->pal_times );

		if( eredmeny < 0 )
			return;

		if( eredmeny == 0 ) {
			State->res = 1;
		}
		if( eredmeny == 1 ) {
			State->res = 3;
		}
		if( eredmeny == 2 ) {
			State->res = 4;
		}
		if( eredmeny == 3 ) {
			State->res = 2;
		}
	}
}

static double setbikesize( double fakt, char* fejlec ) {
	while( 1 ) {
		// Szoveg kiiras:
		blt8( Korny->picbuffer, Korny->ppic_setsize );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 10, fejlec );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 120,
				"Use UP and DOWN to change" );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 145,
				"biker size!" );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 172,
				"Press enter when ready!" );

		// FAKT kirajzolas:
		int xo = 105;
		int yo = 40;
		int xs = 100;
		int ys = (195.0/266)*xs;
		rarajzmot( Korny->picbuffer, xo, yo, xo+xs, yo+ys, fakt/0.676 );

		// Kep kiteves:
		lassufizre( Korny->picbuffer, Korny->pal_setsize );

		mk_emptychar();
		int c = mk_getextchar();
		if( c == MK_UP ) {
			fakt *= 1.1;
			if( fakt > 0.676 )
				fakt = 0.676;
		}
		if( c == MK_DOWN ) {
			fakt /= 1.1;
			if( fakt < 0.5 )
				fakt = 0.5;
		}
		if( c == MK_ENTER || c == MK_ESC )
			return fakt;
	}
}

// VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME
// VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME
// VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME
// VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME VOLUME

static void rarajzvolume( pic8* ppic, int x1, int y, int x2, double volume ) {
	if( volume > 1.0 )
		volume = 1.0;
	if( volume < 0.0 )
		volume = 0.0;
	if( x1 >= x2 )
		hiba( "rarajzvolume-ban x1 >= x2!" );

	int gombxs = Korny->ppic_volgomb->getxsize();
	int gombys = Korny->ppic_volgomb->getysize();
	int lecxs = Korny->ppic_vollecjobb->getxsize();
	int lecys = Korny->ppic_vollecjobb->getysize();
	if( lecxs*2+10 >= x2-x1 )
		hiba( "rarajzvolume-ban lecxs*2+10 >= x2-x1!" );
	// Kitesszuk lec ket szelet:
	blt8( ppic, Korny->ppic_vollecbal, x1, y-lecys/2 );
	blt8( ppic, Korny->ppic_vollecjobb, x2-lecxs, y-lecys/2 );
	// Kitesszuk lec kozepet:
	for( int yf = 0; yf < lecys; yf++ ) {
		unsigned char szin = Korny->ppic_vollecbal->gpixel( lecxs, yf );
		ppic->vizszegmens( x1+lecxs, y+yf-lecys/2, x2-x1-2*lecxs, szin );
	}
	// Kitesszuk gombot:
	int eltolas = (x2-x1-2*lecxs-gombxs)*volume;
	blt8( ppic, Korny->ppic_volgomb, x1+lecxs+eltolas, y-gombys/2 );
}

static double setsoundvolume( double volume ) {
	while( 1 ) {
		// Szoveg kiiras:
		blt8( Korny->picbuffer, Korny->ppic_setsize );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 10,
			"Set Sound Volume!" );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 120,
				"Use UP and DOWN keys!" );
		Korny->pabc_setsize->writekozep( Korny->picbuffer, 160, 172,
				"Press enter when ready!" );

		// FAKT kirajzolas:
		int x1 = 90;
		int yo = 70;
		int x2 = 230;
		rarajzvolume( Korny->picbuffer, x1, yo, x2, volume );

		// Kep kiteves:
		lassufizre( Korny->picbuffer, Korny->pal_setsize );

		mk_emptychar();
		int c = mk_getextchar();
		double sebesseg = 0.04;
		if( c == MK_UP ) {
			volume += sebesseg;
			if( volume > 1.0 )
				volume = 1.0;
		}
		if( c == MK_DOWN ) {
			volume -= sebesseg;
			if( volume < 0.0 )
				volume = 0.0;
		}
		if( c == MK_ENTER || c == MK_ESC )
			return volume;
	}
}

// Kulon allithato:
/*void options( void ) {
	valaszt val;
	int kurrens = 0;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 95, 20, "Options" );
		//Korny->pal_playmenu->set();
		// Valasztas:
		val.szam = 3;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 48;
		val.y0 = 70;
		val.dy = 37;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		if( State->res == 1 )
			strcpy( Korny->rubrikak[0], "Video mode 320x200" );
		if( State->res == 2 )
			strcpy( Korny->rubrikak[0], "Video mode 320x240" );
		if( State->res == 3 )
			strcpy( Korny->rubrikak[0], "Video mode 640x400" );
		if( State->res == 4 )
			strcpy( Korny->rubrikak[0], "Video mode 640x480" );

		strcpy( Korny->rubrikak[1], "Bike Size    (Play)" );
		strcpy( Korny->rubrikak[2], "Bike Size (Replay)" );
		// Kirajzoljuk ket kis size teglalapot:
		int xo = 260;
		int yo = 100;
		int xs = 45;
		int ys = (195.0/266)*xs;
		rarajzmot( Korny->picbuffer, xo, yo, xo+xs, yo+ys, State->fakt_p );
		yo = 140;
		rarajzmot( Korny->picbuffer, xo, yo, xo+xs, yo+ys, State->fakt_r );


		kurrens = val.valassz( Korny->pal_playmenu );

		if( kurrens < 0 ) {
			State->save();
			return;
		}

		if( kurrens == 0 ) {
			setresolution();
		}
		if( kurrens == 1 ) {
			State->fakt_p = setbikesize( State->fakt_p,
									"Set Bike Size (Play)" );
		}
		if( kurrens == 2 ) {
			State->fakt_r = setbikesize( State->fakt_r,
									"Set Bike Size (Replay)" );
		}
	}
} */

// Egyben allithato:
void options( void ) {
	valaszt val;
	int kurrens = 0;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 95, 7, "Options/Help" );
		// Valasztas:
		val.szam = 5;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 48;
		val.y0 = 46;
		val.dy = 31;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		if( State->res == 1 )
			strcpy( Korny->rubrikak[0], "Video mode:     320x200" );
		if( State->res == 2 )
			strcpy( Korny->rubrikak[0], "Video mode:     320x240" );
		if( State->res == 3 )
			strcpy( Korny->rubrikak[0], "Video mode:     640x400" );
		if( State->res == 4 )
			strcpy( Korny->rubrikak[0], "Video mode:     640x480" );

		strcpy( Korny->rubrikak[1], "Bike Size:" );
		// Kirajzoljuk kis size teglalapot:
		int xo = 231;
		int yo = 71; //99
		int xs = 58;
		int ys = (195.0/266)*xs;
		rarajzmot( Korny->picbuffer, xo, yo, xo+xs, yo+ys, State->fakt/0.751 );

		// Kirajzoljuk sound volume-ot:
		strcpy( Korny->rubrikak[2], "Sound Volume:" );
		int x1 = 225;
		yo = 123; // 158
		int x2 = 295;
		rarajzvolume( Korny->picbuffer, x1, yo, x2, State->soundvolume );

		// Jatekos valasztas:
		jatekos* pj = &State->jatekosok[State->jatekos];
		sprintf( Korny->rubrikak[3], "Player:   %s", pj->nev );

		// Help szoveget is kiiratjuk:
		strcpy( Korny->rubrikak[4], "Help" );

		kurrens = val.valassz( Korny->pal_playmenu );

		if( kurrens < 0 ) {
			State->save();
			return;
		}

		if( kurrens == 0 ) {
			setresolution();
		}
		if( kurrens == 1 ) {
			State->fakt = setbikesize( State->fakt,
									"Set Bike Size" );
		}
		if( kurrens == 2 ) {
			State->soundvolume = setsoundvolume( State->soundvolume );
		}
		if( kurrens == 3 ) {
			jatekosvalasztas( 1 );
		}
		if( kurrens == 4 ) {
			help();
		}
	}
}


