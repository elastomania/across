#include	"all.h"

int skippelheto( int level ) {
	int maxskippelheto = 5;
	if( Sarvari )
		maxskippelheto = 3;
	jatekos* pjatekos = &State->jatekosok[int(State->jatekos)];
	if( pjatekos->sikerespalyakszama != level )
		hiba( "skippelheto-ben pjatekos->sikerespalyakszam != level!" );
	int eddigskippelt = 0;
	for( int i = 0; i < level; i++ ) {
		if( pjatekos->skippedtomb[i] )
			eddigskippelt++;
	}
	if( eddigskippelt > maxskippelheto )
		hiba( "eddigskippelt > maxskippelheto!" );
	int skippelheto = eddigskippelt < maxskippelheto;

	blt8( Korny->picbuffer, Korny->ppic_afterplaymenu );

	if( skippelheto ) {
		// Lehet meg skippelni!
		int x0 = 160;
		int y0 = 42;
		int dy = 35;
		char szam[10];
		itoa( maxskippelheto, szam, 10 );
		char tmp[100] = "Number of skips allowed: ";
		strcat( tmp, szam );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0, tmp );
		itoa( eddigskippelt, szam, 10 );
		strcpy( tmp, "Number of skips so far:   " );
		strcat( tmp, szam );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+dy, tmp );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+3*dy,
			"Press a key to continue!" );
	}
	else {
		// Nem lehet mar skippelni!
		int x0 = 160;
		int y0 = 10;
		int dy = 30;
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0,
			"You already have reached" );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+dy,
			"the maximum number of" );

		char szam[10];
		itoa( maxskippelheto, szam, 10 );
		char tmp[100] = "skips allowed (";
		strcat( tmp, szam );
		strcat( tmp, ")!" );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+2*dy, tmp );

		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+3*dy,
			"Fullfill a previously skipped" );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+4*dy,
			"level to skip this level!" );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, x0, y0+5*dy,
			"Press a key to continue!" );
	}


	// Kep kiteves:
	lassufizre( Korny->picbuffer, Korny->pal_afterplaymenu );

	mk_emptychar();
	mk_getextchar();

	return skippelheto;
}
