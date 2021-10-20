#include    "all.h"

// szamitogep
// monitor

void semmi( void ) {
}

#ifdef SARVARI
	int Sarvari = 1;
#else
	int Sarvari = 0;
#endif

int Sajatugras = 0; // (forditva mint regen) Ha igaz, akkor jo ugras (right-clockwise)
int Otszoros = 0; // Ha igaz, mindig otszor rakja ki palyat
int Dupladuplabuffer = 0; // Ha igaz duplan huzza az idot (Win95)
int Snapenabled = 0; // Ha igaz, akkor m-re kiment egy kepet

static void ugraseldontes( void ) {
	if( access( "fordit.inf", 0 ) == 0 )
		Sajatugras = 0;
	else
		Sajatugras = 1;
}

// magassagokeloallitasa-bol kimasolva!!!!!:
/*static void elemszam( char* text, szakaszok* pszakok ) {
	pszakok->felsorolasresetszak();
	szakasz* psz = pszakok->getnextszak();
    if( !psz )
        hiba( "Nincs egyetlen szakasz sem definialva!" );
	double minx = psz->vv.r.x;
	double maxx = psz->vv.r.x;
	double miny = psz->vv.r.y;
	double maxy = psz->vv.r.y;
	while( psz ) {
		if( psz->vv.r.x < minx )
			minx = psz->vv.r.x;
		if( psz->vv.r.x > maxx )
			maxx = psz->vv.r.x;
		if( psz->vv.r.y < miny )
			miny = psz->vv.r.y;
		if( psz->vv.r.y > maxy )
			maxy = psz->vv.r.y;

		if( psz->vv.r.x+psz->vv.v.x < minx )
			minx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.x+psz->vv.v.x > maxx )
			maxx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.y+psz->vv.v.y < miny )
			miny = psz->vv.r.y+psz->vv.v.y;
		if( psz->vv.r.y+psz->vv.v.y > maxy )
			maxy = psz->vv.r.y+psz->vv.v.y;

		psz = pszakok->getnextszak();
    }

    vekt2 origo = vekt2( minx-400/Aranyx, miny-300/Aranyy );
    // Egy kis veletlenites!:
    origo.x += 0.0000145839857463;
    origo.y += 0.0000295874632574;

    double xsize = (maxx+400/Aranyx)-origo.x;
    double ysize = (maxy+300/Aranyy)-origo.y;

	// Lefoglalja magassagokat:
	int xszazalek = 100.0*(xsize*Aranyx)/Magmaxelemszam;
	int yszazalek = 100.0*(ysize*Aranyy)/Magmaxelemszam;
	sprintf( text, "Kihasznaltsag szazalekban: x = %d, y = %d", xszazalek,
																yszazalek );
} */

/*
q7/8
	232, 473
o0/1
	336, 343
n8/9
	241, 245, 246, 580, 611, 982
o8/9
	376, 379, 634
*/

void mv_main( void ) {
	//initqopen();

	#ifdef WATCOM
	if( access( "nemmem.inf", 0 ) != 0 ) {
		// Ha nem talalja nemmem.inf-et:
		// mem Totall Free 11600K-nal meg eppen elmegy, 101 mar uzenetet ir:
		int memkell = 100;
		char* tomb[110];
		for( int i = 0; i < memkell; i++ ) {
			{ tomb[i] = NEW(char*) char[100000]; }
			if( !tomb[i] ) {
				char tmp[200];
				sprintf( tmp, "You need about %d00K more free memory!\n", memkell-i );
				uzenet( tmp );
			}
		}
		for( i = 0; i < memkell; i++ )
			DELETE tomb[i]; }
	}
	#endif

	if( access( "otszor.inf", 0 ) == 0 )
		Otszoros = 1;

	if( access( "ddbuffer.inf", 0 ) == 0 )
		Dupladuplabuffer = 1;

	if( access( "snap.en", 0 ) == 0 )
		Snapenabled = 1;

	// Ezek play.cpp-ben ugyis mindig meg vannak ismetelve:
	Config.viewxsize = 100;
	Config.viewysize = 40;
	Config.viewxorig = 30;
	Config.viewyorig = 159;
	Config.viewxtolas = 160;
	Config.zoom = 10;

	initadatok();

	ugraseldontes();

	teljes();

	// Editor:
	/*char* palyanev = NULL;
	char* leironev = NULL;
	#ifndef WATCOM
		if( _argc != 3 )
			hiba( "Meg kell adni palya es leiro nevet!" );
		palyanev = _argv[1];
		leironev = _argv[2];
	#endif

	palyaleiro leiro( leironev, 0 );
	st320 st;

	while( 1 ) {
		int kov = editor( palyanev, &leiro );
		if( kov == 0 ) {
			loadszakaszok( palyanev, &Pszak, &Pszakt, &Phordok, &leiro );
			char kihasznaltsag[40];
			elemszam( kihasznaltsag, Pszak );
			//DELETE Pszak; } Pszak = NULL;
			//DELETE Pszakt; } Pszakt = NULL;
			//DELETE Phordok; } Phordok = NULL;
			hiba( kihasznaltsag );
		}
		if( kov == 1 ) {
			loadszakaszok( palyanev, &Pszak, &Pszakt, &Phordok, &leiro );
			if( Fejsugar > Kor2.sugar )
				Pszak->rendez( Phordok, Fejsugar );
			else
				Pszak->rendez( Phordok, Kor2.sugar );
			lejatszo( &leiro, &st, 0, NULL, 0 );
			DELETE Pszak; Pszak = NULL; }
			DELETE Pszakt; Pszakt = NULL; }
			DELETE Phordok; Phordok = NULL; }
			mk_emptychar();
		}
	} */
}

