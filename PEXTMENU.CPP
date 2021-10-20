#include	"all.h"

//static void playextfile( long belyeg ) {
//	palyabe( -1, belyeg );

static long playextfile( char* filenev ) {
	if( !palyabe( -1, 0, filenev ) )
		hiba( "playextfile-ban !palyabe( -1, 0, filenev )!" );
	int parreplay = 0;
	Korny->prec->erase( 0 );
	long ido = lejatszo( Korny->pleiro, &Korny->st,
						Korny->prec, parreplay );
	return ido;
}

// Nincs h-ja, hanem prototype ki van irva play.cpp-ben:
/*void playextmenu( void ) {
	static long* belyegtomb = NULL;
	if( !belyegtomb ) {
		belyegtomb = new long[MAXRUBRIKASZAM];
		if( !belyegtomb )
			hiba( "playextmenu-ben nincs eleg memoria belyegtomb-nek!" );
	}
	// Kiirjuk osszes lev file nevet ide:
	find_t ffblk;
	int done;
	done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
	int szam = 0;
	int kezdo = 0;
	while( !done ) {
		char* filenev = ffblk.name;
		char* levelnev = getlevelname( filenev );
		if( !levelnev ) {
			// Kiirunk egy hibauzenetet kepre:
			blt8( Korny->picbuffer, Korny->ppic_load );
			int y0 = 20;
			int dy = 30;
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+0*dy, "There was an error" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+1*dy,  "loading the file:" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+2*dy,  filenev );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+3*dy,  "It is not a valid level file!" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+4*dy,  "Press a key to continue!" );
			lassufizre( Korny->picbuffer, Korny->pal_load );
			mk_emptychar();
			mk_getextchar();
			done = _dos_findnext( &ffblk );
			continue;
		}

		if( strlen( levelnev ) > MAXRUBRIKAHOSSZ )
			hiba( "mdvihgfpo!" );
		strcpy( Korny->rubrikak[szam], levelnev );

		belyegtomb[szam] = getbelyeg( filenev );

		if( strcmp( State->playextfilenev, filenev ) == 0 )
			kezdo = szam;

		done = _dos_findnext( &ffblk );
		szam++;
		if( szam >= MAXRUBRIKASZAM-4 )
			done = 1;
	}

	// Feltoltottuk neveket, jon valasztas:
	valaszt val;
	val.szam = szam;
	val.kur = kezdo;
	val.egykepen = 6;
	val.x0 = 50;
	val.y0 = 40;
	val.dy = 25;
	val.hatter = Korny->picbuffer;
	val.pabc = Korny->pabc_playmenu;
	val.escelheto = 1;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 10, 10, "Select external file!" );
		int eredmeny = val.valassz( Korny->pal_playmenu );
		if( eredmeny < 0 )
			return;
		val.kur = eredmeny;
		getbelyegheznev( belyegtomb[eredmeny], State->playextfilenev );
		playextfile( belyegtomb[eredmeny] );
	}
} */

// Igaz, ha ujra jatszik, hamis ha listat ker:
static int afterplayext( char* filenev, long ido ) {
	int kurrens = 0;
	while( 1 ) {
		// Megirjuk tablat:
		blt8( Korny->picbuffer, Korny->ppic_afterplaymenu );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, 160, 10,
							   getlevelname( filenev ) );
		// Kiirjuk ha lockolt:
		if( Ucsoloadszakasz_lezart )
			Korny->pabc_afterplaymenu->write( Korny->picbuffer,
					9, 173, "Locked" );

		// Ido feliratot keszitunk:
		char idoszov1[30] = "", idoszov2[30] = "";
		if( ido < 0 ) {
			strcpy( idoszov1, "You Failed" );
			strcpy( idoszov2, " to Finish!" );
		}
		else {
			strcpy( idoszov1, "Your time:" );
			ido2string( ido, idoszov2 );
		}

		Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 104, idoszov1 );
		Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 134, idoszov2 );

		//Korny->pal_afterplaymenu->set();
		// Valasztas:
		valaszt val;
		val.szam = 4;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_afterplaymenu;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "Play again" );
		strcpy( Korny->rubrikak[1], "Replay" );
		strcpy( Korny->rubrikak[2], "Save play" );
		strcpy( Korny->rubrikak[3], "File list" );
		//strcpy( Korny->rubrikak[4], "Main menu" );

		int eredmeny = val.valassz( Korny->pal_afterplaymenu );
		kurrens = eredmeny;

		if( eredmeny < 0 || eredmeny == 3 )
			return 0; // Lista
		if( eredmeny == 0 )
			return 1; // Play again
		if( eredmeny == 1 ) {
			replayjelenlegi(); // Replay
			continue;
		}
		if( eredmeny == 2 ) {
			unsigned long belyeg = getbelyeg( filenev );
			saveplay( belyeg ); // Save play
			continue;
		}
		hiba( "Itt nem szabadna lennie (afterplayext)!" );
	}
}

static void abcberendez( int szam ) {
	for( int i = 0; i < szam+3; i++ ) {
		for( int j = 0; j < szam-1; j++ ) {
			// Ha j es j+1 edik elem forditva van visszacsereljuk oket:
			char* nev1 = Korny->rubrikak[j];
			char* nev2 = Korny->rubrikak[j+1];
			if( strcmp( nev1, nev2 ) > 0 ) {
				// Csere:
				char tmp[20];
				strcpy( tmp, nev1 );
				strcpy( nev1, nev2 );
				strcpy( nev2, tmp );
			}
		}
	}
}

// Nincs h-ja, hanem prototype ki van irva play.cpp-ben:
void playextmenu( void ) {
	while( 1 ) {
		// Kiirjuk osszes lev file nevet ide:
		find_t ffblk;
		int done;
		done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
		int szam = 0;
		while( !done ) {
			char* filenev = ffblk.name;
			//char* levelnev = getlevelname( filenev );
			/*if( !levelnev ) {
				// Kiirunk egy hibauzenetet kepre:
				blt8( Korny->picbuffer, Korny->ppic_load );
				int y0 = 20;
				int dy = 30;
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+0*dy, "There was an error" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+1*dy,  "loading the file:" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+2*dy,  filenev );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+3*dy,  "It is not a valid level file!" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+4*dy,  "Press a key to continue!" );
				lassufizre( Korny->picbuffer, Korny->pal_load );
				mk_emptychar();
				mk_getextchar();
				done = _dos_findnext( &ffblk );
				continue;
			}

			if( strlen( levelnev ) > MAXRUBRIKAHOSSZ )
				hiba( "mdvihgfpo!" );
			strcpy( Korny->rubrikak[szam], levelnev ); */

			strcpy( Korny->rubrikak[szam], filenev );

			done = _dos_findnext( &ffblk );
			szam++;
			if( szam >= MAXRUBRIKASZAM-4 )
				done = 1;
		}

		if( szam <= 0 )
			return;

		abcberendez( szam );

		int kezdo = 0; // Ha megtalalja eddigit, akkor kur-t raallitja
		for( int i = 0; i < szam; i++ ) {
			char tmp[50];
			strcpy( tmp, Korny->rubrikak[i] );
			//strcat( tmp, ".lev" );
			if( strcmpi( tmp, State->playextfilenev ) == 0 )
				kezdo = i;
		}

		// Feltoltottuk neveket, jon valasztas:
		valaszt val;
		val.szam = szam;
		val.kur = kezdo;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 10, 10, "Select external file!" );
		int eredmeny = val.valassz( Korny->pal_playmenu );
		if( eredmeny < 0 )
			return;

		char filenev[20];
		if( strlen( Korny->rubrikak[eredmeny] ) > 15 )
			hiba( "playextmenu-ben Korny->rubrikak[eredmeny] ) > 15!" );
		strcpy( filenev, Korny->rubrikak[eredmeny] );

		strcpy( State->playextfilenev, filenev );

		while( 1 ) {
			long ido = playextfile( filenev );
			// Igaz, ha ujra jatszik, hamis ha listat ker:
			if( !afterplayext( filenev, ido ) )
				break;
		}
	}
}






