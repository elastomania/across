#include	"all.h"

void ido2string( long l, char* text ) {
	if( l < 0 )
		hiba( "ido2string-ben ido < 0!" );
	int szazad = int(l%100);
	l /= 100;
	int masodperc = int(l%60);
	l /= 60;
	int perc = int(l%60);
	l /= 60;
	if( l ) {
		szazad = 99;
		masodperc = 59;
		perc = 59;
	}
	text[0] = 0;
	char tmp[20];
	// perc:
	itoa( perc, tmp, 10 );
	if( !tmp[1] )
		strcat( text, "0" );
	strcat( text, tmp );
	strcat( text, ":" );
	// masodperc:
	itoa( masodperc, tmp, 10 );
	if( !tmp[1] )
		strcat( text, "0" );
	strcat( text, tmp );
	strcat( text, ":" );
	// szazad:
	itoa( szazad, tmp, 10 );
	if( !tmp[1] )
		strcat( text, "0" );
	strcat( text, tmp );
}

void levelbesttimes( int level ) {
	//char fejlec[100] = "Level ";
	//char tmp[10];
	//itoa( level+1, tmp, 10 );
	//strcat( fejlec, tmp );
	char fejlec[100];
	itoa( level+1, fejlec, 10 );
	strcat( fejlec, ": " );
	strcat( fejlec, getleveldescription( level ) );

	blt8( Korny->picbuffer, Korny->ppic_leveltimes );
	Korny->pabc_leveltimes->writekozep( Korny->picbuffer, 160, 10, fejlec );
	//Korny->pal_leveltimes->set();
	palyaidejei* pidok = &State->palyakidejei[level];
	if( pidok->idokszama == 0 )
		return;
	for( int i = 0; i < pidok->idokszama; i++ ) {
		jatekos* pjatekos = &State->jatekosok[int(pidok->nevindexek[i])];
		Korny->pabc_leveltimes->write( Korny->picbuffer, 30, 40+i*(SM+9),
														pjatekos->nev );
		char tmp[30];
		ido2string( pidok->idok[i], tmp );
		Korny->pabc_leveltimes->write( Korny->picbuffer, 200, 40+i*(SM+9), tmp );
	}
	lassufizre( Korny->picbuffer, Korny->pal_leveltimes );
	int done = 0;
	while( !done ) {
		mk_emptychar();
		int c = mk_getextchar();
		if( c == MK_ESC || c == MK_ENTER )
			done = 1;
	}
}

void besttimes( void ) {
	int palyaszam = 0;
	for( int i = 0; i < State->jatekosokszama; i++ ) {
		if( State->jatekosok[i].sikerespalyakszama > palyaszam )
			palyaszam = int(State->jatekosok[i].sikerespalyakszama);
	}
	if( palyaszam == 0 )
		return;
	if( palyaszam == 1 ) {
		levelbesttimes( 0 );
		return;
	}
	valaszt val;
	int kurrens = 0;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_times );
		Korny->pabc_times->write( Korny->picbuffer, 70, 10, "Times menu" );
		//Korny->pal_times->set();
		// Valasztas:
		val.szam = palyaszam;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 32;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_times;
		val.escelheto = 1;
		val.tomoren = 1;

		for( int i = 0; i < palyaszam; i++ ) {
			palyaidejei* pidok = &State->palyakidejei[i];
			int vanido = 1;
			if( pidok->idokszama == 0 )
				vanido = 0;
			jatekos* pjatekos = &State->jatekosok[int(pidok->nevindexek[0])];
			// Sorszam 1-tol:
			itoa( i+1, Korny->rubrikak[i], 10 );
			strcat( Korny->rubrikak[i], " " );
			// Level neve:
			strcat( Korny->rubrikak[i], getleveldescription( i ) );
			strcat( Korny->rubrikak[i], " " );
			while( Korny->pabc_times->len( Korny->rubrikak[i], -1 ) < 184 )
				strcat( Korny->rubrikak[i], " " );
			// Jatekos neve:
			if( vanido )
				strcat( Korny->rubrikak[i], pjatekos->nev );
			else
				strcat( Korny->rubrikak[i], "-" );

			/*strcat( Korny->rubrikak[i], " " );
			// Legjobb idok kiirasa: Sajnos most nem fer ki:
			char tmp[30];
			ido2string( pidok->idok[0], tmp );
			strcat( Korny->rubrikak[i], tmp );
			*/
		}

		int eredmeny = val.valassz( Korny->pal_times );

		if( eredmeny < 0 )
			return;

		kurrens = eredmeny;

		levelbesttimes( eredmeny );
	}
}

