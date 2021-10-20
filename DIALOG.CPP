#include	"all.h"

unsigned char Dialogszin = 215;
unsigned char Dialogszelszin = 0;
unsigned char Buttonszin = 217;

void kirajzbox( pic8* ppic, int x1, int y1, int x2, int y2,
			unsigned char felulet, unsigned char szele ) {
	ppic->fillbox( x1, y1, x2, y2, felulet );
	ppic->line( x1, y1, x2, y1, szele );
	ppic->line( x1, y2, x2, y2, szele );
	ppic->line( x1, y1, x1, y2, szele );
	ppic->line( x2, y1, x2, y2, szele );
}

void kirajzbox( pic8* ppic, box boxa, unsigned char felulet,
									  unsigned char szele ) {
	kirajzbox( ppic, boxa.x1, boxa.y1, boxa.x2, boxa.y2, felulet, szele );
}

int boxban( int x, int y, box boxa ) {
	if( x >= boxa.x1 && x <= boxa.x2 &&
		y >= boxa.y1 && y <= boxa.y2 )
		return 1;
	else
		return 0;
}

int dialog( char* text1, char* text2, char* text3,
			 char* text4, char* text5, char* text6,
			 char* text7, char* text8, char* text9,
			 char* text10, char* text11, char* text12,
			 char* text13, char* text14, char* text15,
			 char* text16, char* text17, char* text18,
			 char* text19, char* text20, char* text21,
			 char* text22, char* text23, char* text24 ) {
	char* texttomb[30] = { text1, text2, text3, text4, text5, text6,
					   text7, text8, text9, text10, text11, text12,
					   text13, text14, text15, text16, text17, text18,
					   text19, text20, text21, text22, text23, text24 };
	int textszam = 24;
	for( int i = 0; i < 24; i++ ) {
		if( !texttomb[i] ) {
			textszam = i;
			break;
		}
	}
	char* gombtomb[30];
	for( i = 0; i < 24; i++ )
		gombtomb[i] = NULL;
	int gombszam = 0;
	for( i = 0; i < textszam; i++ ) {
		if( strcmp( texttomb[i], "GOMBOK" ) == 0 ) {
			gombszam = textszam-i-1;
			if( gombszam <= 0 )
				hiba( "dialog-ban gombszam <= 0!" );
			textszam = i;
			for( int j = 0; j < gombszam; j++ )
				gombtomb[j] = texttomb[textszam+j+1];
			break;
		}
	}
	if( gombszam == 0 ) {
		gombszam = 1;
		gombtomb[0] = "OK";
	}

	int visszater = 0;
	if( gombszam == 1 && strcmpi( gombtomb[0], "VISSZATER" ) == 0 ) {
		gombszam = 0;
		visszater = 1;
	}


	if( textszam < 1 )
		hiba( "iuofrf" );

	// Beallitottuk tomboket!!!

	push();

	invalidateegesz();

	int xsize = 0;
	for( i = 0; i < textszam; i++ )
		if( xsize < Pabc2->len( texttomb[i] ) + 16 )
			xsize = Pabc2->len( texttomb[i] ) + 16;

	int dy = 20;
	int ysize = textszam*dy + 50;
	int x1 = 320-xsize/2;
	int y1 = 240-ysize/2;
	int x2 = 320+xsize/2;
	int y2 = 240+ysize/2;
	kirajzbox( Editbuffer, x1, y1, x2, y2, Dialogszin, Dialogszelszin );
	for( i = 0; i < textszam; i++ )
		Pabc2->writekozep( Editbuffer, 320, y1+22+i*dy, texttomb[i] );

	// Kirajzoljuk gombokat is:
	int gx1tomb[15], gx2tomb[15], gy1 = y2-30, gy2 = y2-10;
	int gdx = 80;
	if( gombszam == 1 ) {
		gx1tomb[0] = 320;
	}
	if( gombszam == 2 ) {
		gx1tomb[0] = 320-gdx/2;
		gx1tomb[1] = 320+gdx/2;
	}
	if( gombszam == 3 ) {
		gx1tomb[0] = 320-gdx;
		gx1tomb[1] = 320;
		gx1tomb[2] = 320+gdx;
	}
	if( gombszam > 3 )
		hiba( "uofiufriufr" );
	for( i = 0; i < gombszam; i++ ) {
		int xs = Pabc2->len( gombtomb[i] ) + 10;
		gx2tomb[i] = gx1tomb[i] + xs/2;
		gx1tomb[i] = gx1tomb[i] - xs/2;
		kirajzbox( Editbuffer, gx1tomb[i], gy1, gx2tomb[i], gy2, Buttonszin, Dialogszelszin );
		Pabc2->writekozep( Editbuffer, (gx2tomb[i]+gx1tomb[i])/2, gy1+14, gombtomb[i] );
	}

	blt8( Pscr8, Editbuffer );
	pop();

	if( visszater )
		return 0;

	// Most figyeljuk user mit csinal:
	int egerballevoltnyomva = 0; // Eger gombja csak lemenetnel aktivizalodik
	int egerjobblevoltnyomva = 0; // Eger gombja csak lemenetnel aktivizalodik
	while( 1 ) {
		while( mk_kbhit() ) {
			int c = mk_getextchar();
			if( gombszam == 1 ) {
				if( c == MK_ESC || c == MK_ENTER )
					return 0;
			}
			for( int i = 0; i < gombszam; i++ )
				if( c == tolower( *(gombtomb[i]) ) )
					return i;
		}
		// Megnezzuk eger gombok lemenetelet:
		int balnyomva = 0;
		if( getbutbmou() && !egerballevoltnyomva ) {
			egerballevoltnyomva = 1;
			balnyomva = 1;
		}
		else {
			egerballevoltnyomva = 0;
		}
		int jobbnyomva = 0;
		if( getbutjmou() && !egerjobblevoltnyomva ) {
			egerjobblevoltnyomva = 1;
			jobbnyomva = 1;
		}
		else {
			egerjobblevoltnyomva = 0;
		}

		if( jobbnyomva ) {
			if( gombszam == 1 )
				return 0;
		}

		if( balnyomva ) {
			int ujx = 0, ujy = 0;
			getmou( &ujx, &ujy );
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
			// Most nezzuk meg, hogy hova kattintott:
			for( int i = 0; i < gombszam; i++ ) {
				if( ujx >= gx1tomb[i] && ujx <= gx2tomb[i] &&
					ujy >= gy1 && ujy <= gy2 )
						return i;
			}
			continue;
		}
		int ujx = 0, ujy = 0;
		getmou( &ujx, &ujy );
		if( ujx == Moux && ujy == Mouy )
			continue;
		push();
		Moux = ujx;
		Mouy = ujy;
		pop();
	}
}




