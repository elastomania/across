#include	"all.h"

static int Maxnevhossz = 8; // Ez kisebb kell hogy legyen JATEKOSNEVHOSSZ-nal!

static void betuki( int oszlop, char c ) {
	int x = 50+oszlop*19;
	int y = 6*SM + 8;
	blt8( Korny->picbuffer, Korny->ppic_newjatekos, x, y, x, y, x+19, y+30 );
	if( c == '_' ) {
		c = '-';
		y += 11;
	}
	char tmp[2] = "Q";
	tmp[0] = c;
	Korny->pabc_newjatekos->write( Korny->picbuffer, x, y, tmp );
	lassufizre( Korny->picbuffer, Korny->pal_newjatekos );
}

// Siker eseten igazzal ter vissza:
static int bevesznevet( char* nev, int visszater ) {
	int i = 0;
	betuki( 0, '_' );
	while( 1 ) {
		mk_emptychar();
		int c = mk_getextchar();
		if( c == MK_ESC ) {
			if( visszater )
				return 0;
			kilep();
		}
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
			if( i >= Maxnevhossz )
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

void newjatekos( int visszater ) {
	// Ellenorzes:
	if( State->jatekosokszama >= MAXJATEKOS-1 )
		uzenet( "Sorry, no more players can get onto the list!" );
	int maxrub = MAXRUBRIKASZAM;
	if( MAXJATEKOS > maxrub )
		hiba( "MAXJATEKOS > MAXRUBRIKASZAM!" );

	//Korny->pal_newjatekos->set();
	blt8( Korny->picbuffer, Korny->ppic_newjatekos );
	Korny->pabc_newjatekos->write( Korny->picbuffer, 20, 2*SM, "To exit press ESC!" );
	Korny->pabc_newjatekos->write( Korny->picbuffer, 20, 5*SM, "Please enter your name:" );
	lassufizre(  Korny->picbuffer, Korny->pal_newjatekos );
	lassufizre(  Korny->picbuffer, Korny->pal_newjatekos );
	if( bevesznevet( State->jatekosok[int(State->jatekosokszama)].nev, visszater ) ) {
		State->jatekos = State->jatekosokszama;
		State->jatekosokszama++;
	}
	else {
		visszater = 1; // Ha elescapelte, akkor visszamegyunk menube
	}

	if( State->jatekosokszama == 0 )
		kilep();

	if( visszater )
		return;

	mainmenu();
}

void jatekosvalasztas( int visszater ) {
	//Korny->pal_jatekosval->set();
	blt8( Korny->picbuffer, Korny->ppic_jatekosval );
	Korny->pabc_jatekosval->writekozep( Korny->picbuffer, 160, 10, "Choose player!" );

	// Valasztas:
	valaszt* pval = NULL;
	{ pval = NEW(valaszt*) valaszt; }
	pval->szam = int(State->jatekosokszama+1);
	pval->kur = int(State->jatekos+1);
	pval->egykepen = 5;
	pval->x0 = 50;
	pval->y0 = 50;
	pval->dy = 30;
	pval->hatter = Korny->picbuffer;
	pval->pabc = Korny->pabc_jatekosval;
	pval->escelheto = 1;

	strcpy( Korny->rubrikak[0], "Create New Player" );
	for( int i = 0; i < State->jatekosokszama; i++ ) {
		if( strlen( State->jatekosok[i].nev ) > MAXRUBRIKAHOSSZ )
			hiba( "Tul hosszu nev jatekosvalasztas-ban!" );
		strcpy( Korny->rubrikak[i+1], State->jatekosok[i].nev );
	}

	int eredmeny = pval->valassz( Korny->pal_jatekosval, 1 );
	DELETE pval; }

	if( eredmeny < 0 ) {
		if( visszater )
			return;
		kilep();
	}
	if( eredmeny == 0 ) {
		newjatekos( visszater );
		return;
	}

	State->jatekos = eredmeny-1;

	if( visszater )
		return;

	mainmenu();
}


