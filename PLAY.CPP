#include	"all.h"

static void betuki( int oszlop, char c ) {
	int x = 50+oszlop*19;
	int y = 6*SM + 8;
	blt8( Korny->picbuffer, Korny->ppic_saveplay, x, y, x, y, x+19, y+30 );
	if( c == '_' ) {
		c = '-';
		y += 11;
	}
	char tmp[2] = "Q";
	tmp[0] = c;
	Korny->pabc_newjatekos->write( Korny->picbuffer, x, y, tmp );
	lassufizre( Korny->picbuffer, Korny->pal_newjatekos );
}

static int bevesznevet( char* nev ) {
	int i = 0;
	betuki( 0, '_' );
	while( 1 ) {
		mk_emptychar();
		int c = mk_getextchar();
		if( c == MK_ESC )
			return 0;
		if( c == MK_ENTER ) {
			// ENTER
			if( i > 0 ) {
				nev[i] = 0;
				return 1;
			}
		}
		if( ( c >= 'A' && c <= 'Z' ) ||
			( c >= 'a' && c <= 'z' ) ||
			( c >= '0' && c <= '9' ) ) {
			if( i >= 8 )
				continue;
			betuki( i, c );
			nev[i] = c;
			i++;
			betuki( i, '_' );
		}
		if( c == MK_BACKSPACE ) {
			// <- (torles):
			if( i > 0 ) {
				betuki( i, ' ' );
				i--;
				betuki( i, '_' );
			}
		}
	}
}

static int Eltolas = 15;

static void konvoda( char* text ) {
	int betuszam = 'z'-'A'+1;
	while( *text ) {
		if( *text != 0x20 ) {
			int sorszam = *text-'A'; // sorszam legkisebb erteke 0
			sorszam = betuszam-1-sorszam; // Megforditjuk sorrendet
			sorszam += Eltolas;
			sorszam %= betuszam;
			if( sorszam < 0 || sorszam >= betuszam )
				hiba( "B508976!" );
			*text = (char)('A' + sorszam);
		}
		text++;
	}
}

static void konvback( char* text ) {
	int betuszam = 'z'-'A'+1;
	while( *text ) {
		if( *text != 0x20 ) {
			int sorszam = *text-'A'; // sorszam legkisebb erteke 0
			sorszam -= Eltolas;
			sorszam += betuszam*2;
			sorszam %= betuszam;
			sorszam = betuszam-1-sorszam; // Megforditjuk sorrendet
			if( sorszam < 0 || sorszam >= betuszam )
				hiba( "B508976!" );
			*text = (char)('A' + sorszam);
		}
		text++;
	}
}

/*static void gratulacio( void ) {
	blt8( Korny->picbuffer, Korny->ppic_fomenu );

	int x1 = 160;
	int yo = 4;
	int dy = 24;

	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo,      "Congratulations!");
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy,   "You have just completed" );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*2, "the final level." );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*3, "" );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*4, "Check out readme.txt on" );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*5, "how you can get some" );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*6, "more levels for free!" );
	Korny->pabc_fomenu->writekozep( Korny->picbuffer, x1, yo+dy*7, "Press 'c' to continue!" );

	lassufizre( Korny->picbuffer, Korny->pal_fomenu );
	while( 1 ) {
		mk_emptychar();
		int c = mk_getextchar();
		if( c == 'c' || c == 'C' )
			return;
	}
} */

void saveplay( long belyeg ) {
	blt8( Korny->picbuffer, Korny->ppic_saveplay );
	Korny->pabc_saveplay->write( Korny->picbuffer, 20, 2*SM, "To exit press ESC!" );
	Korny->pabc_saveplay->write( Korny->picbuffer, 20, 5*SM, "Please enter filename:" );
	lassufizre( Korny->picbuffer, Korny->pal_saveplay );
	char tmp[20] = "";
	if( !bevesznevet( tmp ) )
		return;
	if( strcmp( tmp, "memtocpy" ) == 0 ) {
		// Itt most kiirja specialis uzenetet:

		blt8( Korny->picbuffer, Korny->ppic_saveplay );
		//char* sor1 = "This is the executable code";
		//char* sor2 = "of    Action SuperCross";
		//char* sor3 = "copyrighted in 1997"
		//char* sor4 = "by    Balazs Rozsa"
		//char* sor1 = "vbaW aW Vbe eRegUVih^e g[fe";
		//char* sor2 = "[d    OgVa[3 wUZeXMX[WW";
		//char* sor3 = "g[ZQXacbVef a3 3333";
		//char* sor4 = "hQ    Ni^iPW x[PWi";

		//vbaW aW Vbe eRegUVih^e g[fe|
		//vbaW|
		// aW |
		//Vbe |
		//eReg|
		//UVih|
		//^e g|
		//[fe|
		char sor1[30];
		sor1[0] = 'v'; sor1[1] = 'b'; sor1[2] = 'a'; sor1[3] = 'W';
		sor1[4] = ' '; sor1[5] = 'a'; sor1[6] = 'W'; sor1[7] = ' ';
		sor1[8] = 'V'; sor1[9] = 'b'; sor1[10] = 'e'; sor1[11] = ' ';
		sor1[12] = 'e'; sor1[13] = 'R'; sor1[14] = 'e'; sor1[15] = 'g';
		sor1[16] = 'U'; sor1[17] = 'V'; sor1[18] = 'i'; sor1[19] = 'h';
		sor1[20] = '^'; sor1[21] = 'e'; sor1[22] = ' '; sor1[23] = 'g';
		sor1[24] = '['; sor1[25] = 'f'; sor1[26] = 'e'; sor1[27] = 0;

		//[d    OgVa[3 wUZeXMX[WW|
		//[d  |
		//  Og|
		//Va[3|
		// wUZ|
		//eXMX|
		//[WW|
		char sor2[30];
		sor2[0] = '['; sor2[1] = 'd'; sor2[2] = ' '; sor2[3] = ' ';
		sor2[4] = ' '; sor2[5] = ' '; sor2[6] = 'O'; sor2[7] = 'g';
		sor2[8] = 'V'; sor2[9] = 'a'; sor2[10] = '['; sor2[11] = '3';
		sor2[12] = ' '; sor2[13] = 'w'; sor2[14] = 'U'; sor2[15] = 'Z';
		sor2[16] = 'e'; sor2[17] = 'X'; sor2[18] = 'M'; sor2[19] = 'X';
		sor2[20] = '['; sor2[21] = 'W'; sor2[22] = 'W'; sor2[23] = 0;

		//g[ZQXacbVef a3 3333|
		//g[ZQ|
		//Xacb|
		//Vef |
		//a3 3|
		//333|
		char sor3[30];
		sor3[0] = 'g'; sor3[1] = '['; sor3[2] = 'Z'; sor3[3] = 'Q';
		sor3[4] = 'X'; sor3[5] = 'a'; sor3[6] = 'c'; sor3[7] = 'b';
		sor3[8] = 'V'; sor3[9] = 'e'; sor3[10] = 'f'; sor3[11] = ' ';
		sor3[12] = 'a'; sor3[13] = '3'; sor3[14] = ' '; sor3[15] = '3';
		sor3[16] = '3'; sor3[17] = '3'; sor3[18] = '3'; sor3[19] = 0;

		//hQ    Ni^iPW x[PWi|
		//hQ  |
		//  Ni|
		//^iPW|
		// x[P|
		//Wi|
		char sor4[30];
		sor4[0] = 'h'; sor4[1] = 'Q'; sor4[2] = ' '; sor4[3] = ' ';
		sor4[4] = ' '; sor4[5] = ' '; sor4[6] = 'N'; sor4[7] = 'i';
		sor4[8] = '^'; sor4[9] = 'i'; sor4[10] = 'P'; sor4[11] = 'W';
		sor4[12] = ' '; sor4[13] = 'x'; sor4[14] = '['; sor4[15] = 'P';
		sor4[16] = 'W'; sor4[17] = 'i'; sor4[18] = 0;

		konvoda( sor1 );
		konvoda( sor2 );
		konvoda( sor3 );
		konvoda( sor4 );
		sor3[15] = '1';
		sor2[11] = 'n';
		sor3[16] = '9';
		sor3[17] = '9';
		sor3[13] = 'n';
		sor3[18] = '7';
		Korny->pabc_saveplay->writekozep( Korny->picbuffer, 160, 3*SM, sor1 );
		Korny->pabc_saveplay->writekozep( Korny->picbuffer, 160, 5*SM, sor2 );
		Korny->pabc_saveplay->writekozep( Korny->picbuffer, 160, 7*SM, sor3 );
		Korny->pabc_saveplay->writekozep( Korny->picbuffer, 160, 9*SM, sor4 );
		konvback( sor1 );
		konvback( sor2 );
		konvback( sor3 );
		konvback( sor4 );
		lassufizre( Korny->picbuffer, Korny->pal_saveplay );
		while( 1 ) {
			mk_emptychar();
			int c = mk_getextchar();
			if( c == MK_ESC )
				return;
		}
	}
	if( Win_95 )
		strcat( tmp, ".rew" );
	else
		strcat( tmp, ".rec" );
	Korny->prec->save( tmp, belyeg );
}

static void setglobals( int res, double fakt ) {
	// Displaysizeok:
	Displaysizex = -1;
	if( res == 1 ) {
		Displaysizex = 320;
		Displaysizey = 200;
	}
	if( res == 2 ) {
		Displaysizex = 320;
		Displaysizey = 240;
	}
	if( res == 3 ) {
		Displaysizex = 640;
		Displaysizey = 400;
	}
	if( res == 4 ) {
		Displaysizex = 640;
		Displaysizey = 480;
	}
	if( Displaysizex < 1 )
		hiba( "palyabe-ben Displaysizex < 1!" );

	// Aranyok:
	Arany = 100.0;
	Arany *= Displaysizex/320.0;
	Arany *= fakt;
	Aranyx = Arany*320.0/640.0;
	Aranyy = Aranyx*Displaysizey/Displaysizex*(640.0/480.0);

	// Zoomok:
	if( res == 1 ) {
		Config.viewxsize = 100;
		Config.viewysize = 40;
		Config.viewxorig = 30;
		Config.viewyorig = 159;
		Config.viewxtolas = 160;
	}
	if( res == 2 ) {
		Config.viewxsize = 100;
		Config.viewysize = 40;
		Config.viewxorig = 30;
		Config.viewyorig = 159;
		Config.viewxtolas = 160;
	}
	if( res == 3 ) {
		Config.viewxsize = 200;
		Config.viewysize = 80;
		Config.viewxorig = 60;
		Config.viewyorig = 319;
		Config.viewxtolas = 320;
	}
	if( res == 4 ) {
		Config.viewxsize = 200;
		Config.viewysize = 80;
		Config.viewxorig = 60;
		Config.viewyorig = 319;
		Config.viewxtolas = 320;
	}
	//Config.zoom = 10;
	// Minel nagyobb zoom, annal nagyobb resz latszik ablakban:
	if( res == 3 || res == 4 )
		Config.zoom = 0.28*Aranyx*0.5;
	else
		Config.zoom = 0.28*Aranyx;

	// Motor origok:
	//double motorhezag = 0.22 + 0.08*(fakt-0.5)*2.0;
	// double motorhezag = 0.14 + 0.13*(fakt-0.5)*2.0;
	double motorhezag = 0.15 + 0.13*(fakt-0.5)*2.0;

	double xdsize = Displaysizex/Aranyx;
	double ydsize = Displaysizey/Aranyy;
	Mo_y = ydsize*0.5;
	Mo_bal = xdsize*motorhezag;
	double xdjobb = xdsize-Mo_bal;
	Mo_dx = xdjobb-Mo_bal;
}

void ervenytlenitpalya( void ) {
	Korny->levelpszak = -30;
}

// Vagy level == -1 es belyeg != 0 es filenev == NULL,
// vagy level >= 0 < 100 es belyeg = 0 es filenev == NULL,
// vagy level == -1 es belyeg = 0 es filenev ervenyes,
// Hamis eseten nem sikerult betolteni palyat (rec filehoz nincs lev):
int palyabe( int level, unsigned long belyeg, char* filenev ) {
	if( (level >= 0 && belyeg) ||
		(level >= 0 && filenev) ||
		(filenev && belyeg) )
		hiba( "palyabe-ben tul sok param van beallitva!" );
	if( level < 0 && !belyeg && !filenev )
		hiba( "palyabe-ben level < 0 && !belyeg && !filenev!" );

	char* kiirnev = "External level";
	leveltomb_s *plev = NULL;
	if( level >= 0 ) {
		if( level >= MAXPALYASZAM )
			hiba( "palyabe-ben level >= MAXPALYASZAM!" );
		plev = &Leveltomb[Levellista[level]];
		kiirnev = getleveldescription( level );
	}

	int ujleiroazonosito = 2;
	if( level >= 0 )
		ujleiroazonosito = plev->leiroazonosito;

	int ujraegesz = 0;
	int ujtopol = 0;

	if( Korny->resbeolvlevel != State->res ) {
		ujraegesz = 1;
	}

	double fakt;
	fakt = State->fakt;
	if( Korny->fakt_beolvlevel != fakt ) {
		ujraegesz = 1;
	}

	Korny->fakt_beolvlevel = fakt;
	Korny->resbeolvlevel = State->res;

	setglobals( State->res, fakt );

	if( ujraegesz ) {
		pleasewait( kiirnev );
		Korny->leiroazonosito = ujleiroazonosito;
		DELETE Korny->pleiro; }
		{ Korny->pleiro = NEW(palyaleiro*) palyaleiro( Leironevtomb[ujleiroazonosito] ); }
	}
	else {
		if( Korny->leiroazonosito != ujleiroazonosito ) {
			// Most csak a foldet kell kicserelni:
			pleasewait( kiirnev );
			Korny->leiroazonosito = ujleiroazonosito;
			ujtopol = 1;
			csakfoldcsere();
		}
	}

	// Kell-e topologia csere:
	if( ujraegesz )
		ujtopol = 1;
	if( Korny->levelpszak != level )
		ujtopol = 1;
	if( belyeg )
		ujtopol = 1;
	if( filenev ) // External file-nal mindig ujraolvasunk
		ujtopol = 1;
	if( ujtopol ) {
		char tmpnev[20] = "";
		char* nev = NULL;
		if( level >= 0 )
			nev = plev->filenev;
		else {
			if( belyeg ) {
				nev = tmpnev;
				if( !getbelyegheznev( belyeg, nev ) ) {
					dialog320( "Level file not found!",
							   "Read file EDITOR.TXT!" );
					return 0;
				}
			}
			else
				nev = filenev;
		}

		DELETE Pszak; } Pszak = NULL;
		DELETE Pszakt; } Pszakt = NULL;
		DELETE Korny->st.pmviz; }
		DELETE Korny->st.pmfugg; }

		Korny->levelpszak = level;
		if( level >= 0 )
			loadszakaszok( nev, 0, &Pszak, &Pszakt );
		else
			loadszakaszok( nev, 1, &Pszak, &Pszakt );

		if( Fejsugar > Kor2.sugar )
			Pszak->rendez( Fejsugar );
		else
			Pszak->rendez( Kor2.sugar );
		Korny->st.origo = magassagokeloallitasa( Pszak,
				&Korny->st.pmviz, &Korny->st.pmfugg );
		Korny->hatterszamitas(
					Korny->st.pmviz->getxsize(),
					Korny->st.pmfugg->getxsize() );
	}
	Korny->legjobbido[0] = 0;
	if( level >= 0 ) {
		long idokszama = State->palyakidejei[unsigned(level)].idokszama;
		if( idokszama > 0 ) {
			ido2string( State->palyakidejei[unsigned(level)].idok[0],
								Korny->legjobbido );
		}
	}
	return 1;
}

// Idot beirja tablaba, ha kell:
static void idoelintezes( long ido, char* valasz1, char* valasz2, char* valasz3, int level ) {
	valasz1[0] = 0;
	valasz2[0] = 0;
	valasz3[0] = 0;
	if( ido < 0 ) {
		strcpy( valasz1, "You Failed" );
		strcpy( valasz2, " to Finish!" );
		return;
	}
	if( ido == 0 )
		hiba( "idoelintezes-ben ido == 0!" );

	ido2string( ido, valasz1 );

	palyaidejei* pidok = &State->palyakidejei[level];
	if( pidok->idokszama == MAXIDOK && pidok->idok[MAXIDOK-1] < ido )
		return;
	// Felkerult tablara!
	if( pidok->idokszama == 0 ) {
		// Elso ido:
		pidok->idokszama = 1;
		pidok->idok[0] = ido;
		pidok->nevindexek[0] = State->jatekos;
		strcat( valasz2, "Best Time!" );
		return;
	}
	// Tablan van mar mas ido is:
	// Uzenetek eldontese:
	int elozott  = 0;
	if( pidok->idok[unsigned(pidok->idokszama-1)] > ido ) {
		// Vkit leelozott:
		elozott  = 1;
	}
	if( pidok->idok[0] > ido ) {
		// Legjobb ido!:
		strcat( valasz2, "Best Time!" );
	}
	else {
		if( elozott ) {
			strcat( valasz2, "You Made the" );
			strcat( valasz3, "Top Six!" );
		}
	}
	// Beillesztjuk uj idot vegere!:
	if( pidok->idokszama == MAXIDOK ) {
		pidok->idok[MAXIDOK-1] = ido;
		pidok->nevindexek[MAXIDOK-1] = State->jatekos;
	}
	else {
		pidok->idok[int(pidok->idokszama)] = ido;
		pidok->nevindexek[int(pidok->idokszama)] = State->jatekos;
		pidok->idokszama++;
	}
	// Sorrendbe allitjuk idoket:
	for( int i = 0; i < MAXIDOK+1; i++ ) {
		for( int j = 0; j < pidok->idokszama-1; j++ ) {
			if( pidok->idok[j] > pidok->idok[j+1] ) {
				// Csere:
				long tmp = pidok->idok[j];
				pidok->idok[j] = pidok->idok[j+1];
				pidok->idok[j+1] = tmp;

				tmp = pidok->nevindexek[j];
				pidok->nevindexek[j] = pidok->nevindexek[j+1];
				pidok->nevindexek[j+1] = tmp;
			}
		}
	}
}

void replaykorny( void ) {
	int level = Korny->prec->getlevel();
	if( !palyabe( level ) )
		hiba( "replaykorny-ben !palyabe( level )!" );
	while( 1 ) {
		int parreplay = 1;
		Korny->prec->rewind();
		if( lejatszo( Korny->pleiro, &Korny->st, Korny->prec, parreplay ) )
			return;
	}
}

// A jelenleg beolvasott palyan jatszik vissza:
// Csak afterplay menu replay-ebol szabad hasznalni:
void replayjelenlegi( void ) {
	while( 1 ) {
		int parreplay = 1;
		Korny->prec->rewind();
		if( lejatszo( Korny->pleiro, &Korny->st, Korny->prec, parreplay ) )
			return;
	}
}

// 0-fomenu, 1-this level, 2-next level, 3-skip level
static int afterplay( int level, int nextdefault,
						char* valasz1, char* valasz2, char* valasz3 ) {
	jatekos* pjatekos = &State->jatekosok[int(State->jatekos)];
	int nextisvan = 0;
	if( pjatekos->sikerespalyakszama > level &&
		level < Palyaszam-1 )
		nextisvan = 1;
	int skipisvan = 0;
	if( !nextisvan && level < Palyaszam-1 ) {
		skipisvan = 1;
	}
	int kurrens = nextdefault;
	// Ellenorzes:
	if( nextdefault && !nextisvan )
		hiba( "nextdefault && !nextisvan!" );
	if( skipisvan && nextisvan )
		hiba( "skipisvan && nextisvan!" );
	while( 1 ) {
		char fejlec[100] = "Level ";
		char tmp[10];
		itoa( level+1, tmp, 10 );
		strcat( fejlec, tmp );
		strcat( fejlec, ": " );
		strcat( fejlec, getleveldescription( level ) );

		blt8( Korny->picbuffer, Korny->ppic_afterplaymenu );
		Korny->pabc_afterplaymenu->write( Korny->picbuffer, 28, 10,
		fejlec );
		if( valasz3[0] ==  0 ) {
			Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 104, valasz1 );
			if( strcmp( valasz2, "Best Time!" ) == 0 )
				Korny->pabc_afterplaymenu->write( Korny->picbuffer, 196, 134, valasz2 );
			else
				Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 134, valasz2 );
		}
		else {
			Korny->pabc_afterplaymenu->write( Korny->picbuffer, 196, 76, valasz1 );
			Korny->pabc_afterplaymenu->write( Korny->picbuffer, 164, 106, valasz2 );
			Korny->pabc_afterplaymenu->write( Korny->picbuffer, 196, 136, valasz3 );
		}
		//Korny->pal_afterplaymenu->set();
		// Valasztas:
		valaszt val;
		val.szam = 5+nextisvan+skipisvan;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_afterplaymenu;
		val.escelheto = 1;

		if( nextisvan || skipisvan ) {
			strcpy( Korny->rubrikak[0], "Play again" );
			if( nextisvan )
				strcpy( Korny->rubrikak[1], "Play next" );
			else
				strcpy( Korny->rubrikak[1], "Skip level" );
			strcpy( Korny->rubrikak[2], "Replay" );
			strcpy( Korny->rubrikak[3], "Save play" );
			strcpy( Korny->rubrikak[4], "Best times" );
			strcpy( Korny->rubrikak[5], "Main menu" );
		}
		else {
			strcpy( Korny->rubrikak[0], "Play again" );
			strcpy( Korny->rubrikak[1], "Replay" );
			strcpy( Korny->rubrikak[2], "Save play" );
			strcpy( Korny->rubrikak[3], "Best times" );
			strcpy( Korny->rubrikak[4], "Main menu" );
		}
		int eredmeny = val.valassz( Korny->pal_afterplaymenu );
		kurrens = eredmeny;
		// eredmeny-t nextisvan esetere konvertalja:
		if( eredmeny > 0 && !nextisvan && !skipisvan )
			eredmeny++;

		if( eredmeny < 0 || eredmeny == 5 )
			return 0;
		if( eredmeny == 0 )
			return 1;
		if( eredmeny == 1 ) {
			// Next, vagy skip:
			if( !nextisvan && !skipisvan )
				hiba( "Itt nem kene lennie (!nextisvan && !skipisvan)!" );
			if( nextisvan )
				return 2;
			// SKIP:
			if( skippelheto( level ) )
				return 3;
		}
		if( eredmeny == 2 )
			replaykorny();
		if( eredmeny == 3 ) {
			//if( !Sarvari || level != Palyaszam-1 )
			if( level != Palyaszam-1 )
				saveplay( 0 ); // utolso palyat nem lehet menteni!
		}
		if( eredmeny == 4 )
			levelbesttimes( level );
	}
}

static void playlevel( int level ) {
	jatekos* pjatekos = &State->jatekosok[int(State->jatekos)];
	while( 1 ) {
		if( !palyabe( level ) )
			hiba( "playlevel-ben !palyabe( level )!" );
		int parreplay = 0;
		Korny->prec->erase( level );
		long ido = lejatszo( Korny->pleiro, &Korny->st,
						Korny->prec, parreplay );
		char valasz1[50] = "";
		char valasz2[50] = "";
		char valasz3[50] = "";
		idoelintezes( ido, valasz1, valasz2, valasz3, level );

		int ujpalya = 0;
		if( ido > 0 ) {
			// Sikeres volt szint:
			pjatekos->skippedtomb[level] = 0;
			if( pjatekos->sikerespalyakszama == level ) {
				// Novelte eggyel sikeres palyak szamat:
				pjatekos->sikerespalyakszama++;
				State->save();
				if( pjatekos->sikerespalyakszama < Palyaszam ) {
					ujpalya = 1;
				}
				else {
					// Most eloszor sikeresen teljesitette utolso palyat:

				}
			}
		}
		int eredmeny = afterplay( level, ujpalya, valasz1, valasz2, valasz3 );
		Korny->prec->erase( 0 );
		if( eredmeny == 0 ) {
			pjatekos->jelenlegipalya = level+ujpalya;
			return;
		}
		if( eredmeny == 2 ) {
			level++;
			pjatekos->jelenlegipalya = level;
		}
		if( eredmeny == 3 ) {
			// Skip level:
			if( level != pjatekos->sikerespalyakszama )
				hiba( "level != pjatekos->sikerespalyakszama! 674" );
			pjatekos->skippedtomb[level] = 1;
			level++;
			pjatekos->jelenlegipalya = level;
			pjatekos->sikerespalyakszama++;
			State->save();
		}
	}
}

void play( void ) {
	while( 1 ) {
		jatekos* pjatekos = &State->jatekosok[int(State->jatekos)];
		if( pjatekos->sikerespalyakszama == 0 ) {
			showinstruct();
			playlevel( 0 );
			return;
		}

		int kirpalyszam = int(pjatekos->sikerespalyakszama)+1;
		if( kirpalyszam > Palyaszam )
			kirpalyszam = Palyaszam;

		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 70, 10, "Select level!" );
		//Korny->pal_playmenu->set();
		// Valasztas:
		valaszt val;
		val.szam = kirpalyszam+1;
		val.kur = int(pjatekos->jelenlegipalya)+1;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "External file" );
		for( int i = 0; i < kirpalyszam; i++ ) {
			char tmp[MAXRUBRIKAHOSSZ];
			itoa( i+1, tmp, 10 );
			strcat( tmp, " " );
			if( pjatekos->skippedtomb[i] )
				strcat( tmp, "SKIPPED!" );
			else
				strcat( tmp, getleveldescription( i ) );
			if( strlen( tmp ) > MAXRUBRIKAHOSSZ-5 )
				hiba( "play-ben! strlen( tmp ) > MAXRUBRIKAHOSSZ-5!" );
			strcpy( Korny->rubrikak[i+1], tmp );
		}

		int eredmeny = val.valassz( Korny->pal_playmenu );

		if( eredmeny < 0 )
			return;

		if( eredmeny > 0 ) {
			playlevel( eredmeny-1 );
			return;
		}

		// External file lejatszast valasztotta, ilyenkor ha visszajon
		// folytatodik tovabb:
		pjatekos->jelenlegipalya = -1;
		void playextmenu( void );
		playextmenu();
	}
}



