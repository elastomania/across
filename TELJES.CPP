#include	"all.h"

state* State = NULL;
kornyezet* Korny = NULL;

void cheatplayer( void ) {
	// Egyetlen jatekosnak sem szabad lennie:
	if( State->jatekosokszama != 0 )
		hiba( "cheatplayer-ben mar van valaki bejegyezve!" );

	State->jatekosokszama = 1;

	int hanypalya = Palyaszam;
	hanypalya = 40;

	jatekos* pjat = &State->jatekosok[0];
	pjat->sikerespalyakszama = hanypalya;
	strcpy( pjat->nev, "Cheat" );
	pjat->jelenlegipalya = 0;


	for( int i = 0; i < hanypalya; i++ ) {
		palyaidejei* pidok = &State->palyakidejei[i];
		if( pidok->idokszama != 0 )
			hiba( "cheatplayer-ben bug!" );
		pidok->idokszama = 1;
		pidok->idok[0] = 180000l;
	}
}


void teljes( void ) {
	{ State = NEW(state*) state; }
	if( !State )
		uzenet( "memory" );

	//State->save(); // Itt lehet ures state.dat file-t csinalni

	{ Korny = NEW(kornyezet*) kornyezet(); }
	if( !Korny )
		uzenet( "memory" );

	// Ez itt egy kicsit gany, itt lehet cheat playert eloallitani:
	//cheatplayer();

	if( State->jatekosokszama == 0 )
		newjatekos();
	else
		jatekosvalasztas();
	hiba( "Ide nem kellett volna jonnie!" );
}

void kilep( void ) {
	hiba( "Kiir szoveg!" );
}


