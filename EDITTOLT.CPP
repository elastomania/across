#include	"all.h"

static const unsigned short Save_as_box_szin = 215, Save_as_szelszin = 0;

static unsigned short Openlistaszin = 173, Openlistainvszin = 205;

static int Maxnev = 1000; // Nem lehet nagyobb 3000-nel!
static char* Soknev = NULL;

static void abcberendez( int nevszam ) {
	for( int i = 0; i < nevszam+3; i++ ) {
		for( int j = 0; j < nevszam-1; j++ ) {
			// Ha j es j+1 edik elem forditva van visszacsereljuk oket:
			char* nev1 = &Soknev[j*10];
			char* nev2 = &Soknev[(j+1)*10];
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

static void betolt( char* nev ) {
	if( !Ptop )
		hiba( "uoiwfiufre" );
	if( !*nev )
		hiba( "oiufewguyxygxw" );
	char tmp[15];
	if( strlen( nev ) > 8 )
		hiba( "iovffiufi" );
	strcpy( tmp, nev );
	strcat( tmp, ".lev" );

	delete Ptop;
	Ptop = new topol( tmp, 1 );

	Valtozott = 0;
	if( strlen( tmp ) > 12 )
		hiba( "betolt-ben strlen( tmp ) > 12!" );
	strcpy( State->editfilenev, tmp );
	if( Ptop->lezart ) {
		char szoveg[40];
		sprintf( szoveg, "The file '%s' is locked, you cannot edit it!",
											State->editfilenev );
		dialog( szoveg );
		delete Ptop;
		Ptop = new topol;
		Valtozott = 0;
		State->editfilenev[0] = 0;
	}

	// Most megnezzuk hogy nincs-e tul sok pontja palyanak:
	Ptop->checklevelforsarvari();

	kitolt();
}

static void igazit( int* pkur, int* pfelso, int nevszam, int helyszam ) {
	if( *pkur < 0 )
		*pkur = 0;
	if( *pkur > nevszam-1 )
		*pkur = nevszam-1;
	if( *pfelso > *pkur )
		*pfelso = *pkur;
	if( *pfelso < *pkur-helyszam+1 )
		*pfelso = *pkur-helyszam+1;
}

static void boxbarajz( pic8* ppic, box abox, unsigned char szin, int fel ) {
	int x1 = abox.x1; int y1 = abox.y1;
	int x2 = abox.x2; int y2 = abox.y2;
	int szeltav = 16;
	x1 += szeltav; x2 -= szeltav;
	y1 += 5; y2 -= 8;
	int xkozep = (x1+x2)/2;
	for( int xfut = x1; xfut <= xkozep; xfut++ ) {
		int x = xfut;
		int y = y1 + (y2-y1)/double(xkozep-x1)*(x-x1);
		if( fel ) {
			y = abox.y2-(y-abox.y1);
			ppic->ppixel( x, y, szin );
			ppic->ppixel( x, y-3, szin );
			x = xkozep + (xkozep-x);
			ppic->ppixel( x, y, szin );
			ppic->ppixel( x, y-3, szin );
		}
		else {
			ppic->ppixel( x, y, szin );
			ppic->ppixel( x, y+3, szin );
			x = xkozep + (xkozep-x);
			ppic->ppixel( x, y, szin );
			ppic->ppixel( x, y+3, szin );
		}
	}
}

void tolt_open( void ) {
	invalidateegesz();
	if( Valtozott ) {
		if( dialog( "There are unsaved changes in the level file.",
					"If you open another file, you will loose these changes.",
					"Do you still want to continue?",
					"GOMBOK", "Yes", "No" ) == 1 )
		return;
	}
	if( !Soknev ) {
		Soknev = new char[Maxnev*10+100];
		if( !Soknev )
			hiba( "Nincs eleg memoria tolt_open-ben (hjfdf)!" );
	}
	for( int i = 0; i < Maxnev*10+10; i++ )
		Soknev[i] = 0;
	int done;
	find_t ffblk;
	done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
	int nevszam = 0;
	while( !done ) {
		if( strlen( ffblk.name ) > 14 )
			hiba( "ijhfiyewpp" );
		char* nev = &Soknev[nevszam*10];
		strcpy( nev, ffblk.name );
		// Leveszi kiterjesztest:
		int i = 0;
		while( nev[i] != '.' ) {
			if( !nev[i] )
				hiba( "Nincs pont nevben! (yewruyrewuy): ", nev );
			i++;
		}
		nev[i] = 0;
		if( strlen( nev ) > 8 )
			hiba( "iuoriureiur" );

		done = _dos_findnext( &ffblk );
		nevszam++;
		if( nevszam >= Maxnev ) {
			dialog( "Number of files > 1000 in open dialog box!",
					"Could not load all the files!" );
			done = 1;
		}
	}
	if( nevszam < 1 ) {
		dialog( "There is not any level file (*.lev) in this directory!" );
		return;
	}

	abcberendez( nevszam );

	int kur = 0; // Ha megtalalja eddigit, akkor kur-t raallitja
	for( i = 0; i < nevszam; i++ ) {
		char tmp[50];
		strcpy( tmp, &Soknev[i*10] );
		strcat( tmp, ".lev" );
		if( strcmpi( tmp, State->editfilenev ) == 0 )
			kur = i;
	}

	//State->editfilenev[0] = 0; Igy nem lesz unnamed CANCEL utan

	// Koordok megadasa:
	int helyszam = 10;
	int dy = 20;
	int x1 = 200;
	int y1 = 100;
	int x2 = 401;
	int y2 = 174+helyszam*dy;
	int lx1 = x1+10;
	int ly1 = y1+37;
	int lx2 = lx1+100;
	int ly2 = ly1+helyszam*dy;

	box boxfel = { x1+10, y1 + 11, x1+110, y1 + 31 };
	box boxle =  { x1+10, y2 - 30, x1+110, y2 -10 };
	box boxcancel = { x1+121, (y2+y1)/2-10, x1+121 + 70, (y2+y1)/2+10 };

	int felso = 0;
	int eddigfelso = -1;
	int eddigkur = 0;
	mk_emptychar();
	int egerballevoltnyomva = 0;
	while( 1 ) {
		// Nezzuk gombokat:
		while( mk_kbhit() ) {
			int c = mk_getextchar();
			if( c == MK_ESC )
				return;
			if( c == MK_ENTER ) {
				igazit( &kur, &felso, nevszam, helyszam );
				betolt( &Soknev[kur*10] );
				return;
			}
			if( c == MK_UP ) kur--;
			if( c == MK_PGUP ) kur -= helyszam-1;
			if( c == MK_DOWN ) kur++;
			if( c == MK_PGDOWN ) kur += helyszam-1;
		}
		// Eger lenyomas:
		int balnyomva = 0;
		if( getbutbmou() && !egerballevoltnyomva ) {
			egerballevoltnyomva = 1;
			balnyomva = 1;
		}
		else {
			egerballevoltnyomva = 0;
		}
		if( balnyomva ) {
			int ujx = 0, ujy = 0;
			getmou( &ujx, &ujy );
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
			// Most nezzuk meg, hogy hova kattintott:
			if( boxban( ujx, ujy, boxle ) )
				kur += helyszam-1;
			if( boxban( ujx, ujy, boxfel ) )
				kur -= helyszam-1;
			if( boxban( ujx, ujy, boxcancel ) )
				return;
			if( ujx > lx1 && ujx < lx2 && ujy > ly1 && ujy < ly2 ) {
				// Lista folott nyomta meg gombot:
				if( ujy < ly1 + dy*helyszam ) {
					// Egy nevet el is talalt:
					int index = (ujy-ly1)/dy;
					index += felso;
					if( index < nevszam ) {
						betolt( &Soknev[index*10] );
						return;
					}
				}
			}
		}
		igazit( &kur, &felso, nevszam, helyszam );
		if( felso != eddigfelso || kur != eddigkur ) {
			// Kirajzoljuk, mivel valtozott kep:
			push();
			kirajzbox( Editbuffer, x1, y1, x2, y2, Dialogszin, Dialogszelszin );
			kirajzbox( Editbuffer, lx1, ly1, lx2, ly2, Openlistaszin,  Dialogszelszin );
			for( int i = 0; i < helyszam && i < nevszam; i++ ) {
				if( i + felso == kur ) {
					// Ez most kuurens nev:
					Editbuffer->fillbox( lx1+1, ly1+i*dy + 1,
							lx2-1, ly1+(i+1)*dy-1, Openlistainvszin );
				}
				Pabc2->write( Editbuffer, lx1+3, ly1+15+i*dy, &Soknev[(i+felso)*10] );
			}
			// Most jonnek gombok:
			kirajzbox( Editbuffer, boxfel, Buttonszin, Dialogszelszin );
			boxbarajz( Editbuffer, boxfel, Dialogszelszin, 1 );
			kirajzbox( Editbuffer, boxle, Buttonszin, Dialogszelszin );
			boxbarajz( Editbuffer, boxle, Dialogszelszin, 0 );
			kirajzbox( Editbuffer, boxcancel, Buttonszin, Dialogszelszin );
			Pabc2->writekozep( Editbuffer, (boxcancel.x1+boxcancel.x2)/2,
										  boxcancel.y1 + 15, "CANCEL" );

			blt8( Pscr8, Editbuffer, x1, y1, x1, y1, x2, y2 );
			pop();

			eddigfelso = felso;
			eddigkur = kur;
		}
		int ujx = 0, ujy = 0;
		getmou( &ujx, &ujy );
		if( ujx != Moux && ujy != Mouy ) {
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
		}
	}
}

// Editfilenev alapjan megprobal josolni, majd oda teszi eredmenyt:
void kivalasztkezdetit( void ) {
	invalidateegesz();
	if( !Soknev ) {
		Soknev = new char[Maxnev*10+100];
		if( !Soknev )
			hiba( "Nincs eleg memoria tolt_open-ben (hjfdf)!" );
	}
	for( int i = 0; i < Maxnev*10+10; i++ )
		Soknev[i] = 0;
	int done;
	find_t ffblk;
	done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
	int nevszam = 0;
	while( !done ) {
		if( strlen( ffblk.name ) > 14 )
			hiba( "ijhfiyewpp" );
		char* nev = &Soknev[nevszam*10];
		strcpy( nev, ffblk.name );

		// Leveszi kiterjesztest:
		int i = 0;
		while( nev[i] != '.' ) {
			if( !nev[i] )
				hiba( "Nincs pont nevben! (yewruyrewuy): ", nev );
			i++;
		}
		nev[i] = 0;
		if( strlen( nev ) > 8 )
			hiba( "iuoriureiur" );

		done = _dos_findnext( &ffblk );
		nevszam++;
		if( nevszam >= Maxnev ) {
			dialog( "Number of files > 1000 in open dialog box!",
					"Could not load all the files!" );
			done = 1;
		}
	}
	if( nevszam < 1 ) {
		State->editfilenev[0] = 0;
		return;
	}

	abcberendez( nevszam );

	int kur = 0; // Ha megtalalja eddigit, akkor kur-t raallitja
	for( i = 0; i < nevszam; i++ ) {
		char tmp[50];
		strcpy( tmp, &Soknev[i*10] );
		strcat( tmp, ".lev" );
		if( strcmpi( tmp, State->editfilenev ) == 0 )
			kur = i;
	}

	State->editfilenev[0] = 0;

	// Koordok megadasa:
	int helyszam = 10;
	int dy = 20;
	int x1 = 200;
	int y1 = 100;
	int x2 = 401;
	int y2 = 174+helyszam*dy;
	int lx1 = x1+10;
	int ly1 = y1+37;
	int lx2 = lx1+100;
	int ly2 = ly1+helyszam*dy;

	box boxfel = { x1+10, y1 + 11, x1+110, y1 + 31 };
	box boxle =  { x1+10, y2 - 30, x1+110, y2 -10 };
	box boxcancel = { x1+121, (y2+y1)/2-10, x1+121 + 70, (y2+y1)/2+10 };
	box boxnew = { x1+121, (y2+y1)/2-40, x1+121 + 70, (y2+y1)/2-20 };

	int felso = 0;
	int eddigfelso = -1;

	int eddigkur = 0;
	mk_emptychar();
	int egerballevoltnyomva = 0;
	while( 1 ) {
		// Nezzuk gombokat:
		while( mk_kbhit() ) {
			int c = mk_getextchar();
			if( c == MK_ESC )
				return;
			if( c == MK_ENTER ) {
				igazit( &kur, &felso, nevszam, helyszam );
				char* nev = &Soknev[kur*10];
				if( strlen( nev ) > 8 )
					hiba( "iovfgsdgffiufi" );
				strcpy( State->editfilenev, nev );
				strcat( State->editfilenev, ".lev" );
				return;
			}
			if( c == MK_UP ) kur--;
			if( c == MK_PGUP ) kur -= helyszam-1;
			if( c == MK_DOWN ) kur++;
			if( c == MK_PGDOWN ) kur += helyszam-1;
		}
		// Eger lenyomas:
		int balnyomva = 0;
		if( getbutbmou() && !egerballevoltnyomva ) {
			egerballevoltnyomva = 1;
			balnyomva = 1;
		}
		else {
			egerballevoltnyomva = 0;
		}
		if( balnyomva ) {
			int ujx = 0, ujy = 0;
			getmou( &ujx, &ujy );
			Moux = ujx;
			Mouy = ujy;
			// Most nezzuk meg, hogy hova kattintott:
			if( boxban( ujx, ujy, boxle ) )
				kur += helyszam-1;
			if( boxban( ujx, ujy, boxfel ) )
				kur -= helyszam-1;
			if( boxban( ujx, ujy, boxcancel ) )
				return;
			if( boxban( ujx, ujy, boxnew ) ) {
				State->editfilenev[0] = 0;
				return;
			}
			if( ujx > lx1 && ujx < lx2 && ujy > ly1 && ujy < ly2 ) {
				// Lista folott nyomta meg gombot:
				if( ujy < ly1 + dy*helyszam ) {
					// Egy nevet el is talalt:
					int index = (ujy-ly1)/dy;
					index += felso;
					if( index < nevszam ) {
						char* nev = &Soknev[index*10];
						if( strlen( nev ) > 8 )
							hiba( "iovfgsdgffiufi" );
						strcpy( State->editfilenev, nev );
						strcat( State->editfilenev, ".lev" );
						return;
					}
				}
			}
		}
		igazit( &kur, &felso, nevszam, helyszam );
		if( felso != eddigfelso || kur != eddigkur ) {
			// Kirajzoljuk, mivel valtozott kep:
			push();
			kirajzbox( Editbuffer, x1, y1, x2, y2, Dialogszin, Dialogszelszin );
			kirajzbox( Editbuffer, lx1, ly1, lx2, ly2, Openlistaszin,  Dialogszelszin );
			for( int i = 0; i < helyszam && i < nevszam; i++ ) {
				if( i + felso == kur ) {
					// Ez most kuurens nev:
					Editbuffer->fillbox( lx1+1, ly1+i*dy + 1,
							lx2-1, ly1+(i+1)*dy-1, Openlistainvszin );
				}
				Pabc2->write( Editbuffer, lx1+3, ly1+15+i*dy, &Soknev[(i+felso)*10] );
			}
			// Most jonnek gombok:
			kirajzbox( Editbuffer, boxfel, Buttonszin, Dialogszelszin );
			boxbarajz( Editbuffer, boxfel, Dialogszelszin, 1 );
			kirajzbox( Editbuffer, boxle, Buttonszin, Dialogszelszin );
			boxbarajz( Editbuffer, boxle, Dialogszelszin, 0 );
			kirajzbox( Editbuffer, boxcancel, Buttonszin, Dialogszelszin );
			Pabc2->writekozep( Editbuffer, (boxcancel.x1+boxcancel.x2)/2,
										  boxcancel.y1 + 15, "CANCEL" );
			kirajzbox( Editbuffer, boxnew, Buttonszin, Dialogszelszin );
			Pabc2->writekozep( Editbuffer, (boxnew.x1+boxnew.x2)/2,
										  boxnew.y1 + 15, "NEW" );


			blt8( Pscr8, Editbuffer, x1, y1, x1, y1, x2, y2 );
			pop();

			eddigfelso = felso;
			eddigkur = kur;
		}
		int ujx = 0, ujy = 0;
		getmou( &ujx, &ujy );
		if( ujx != Moux && ujy != Mouy ) {
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
		}
	}
}

void tolt_new( void ) {
	invalidateegesz();
	if( Valtozott ) {
		if( dialog( "There are unsaved changes in the level file.",
					"Do you still want to continue?",
					"GOMBOK", "Yes", "No" ) == 1 )
		return;
	}

	if( !Ptop )
		hiba( "uofggiwfiufre" );

	delete Ptop;
	Ptop = new topol;
	State->editfilenev[0] = 0;
	kitolt();
	Valtozott = 0;
}

// Ha sikeresen mentett, akkor igaz:
int tolt_save( int lokkolhato ) {
	invalidateegesz();
	if( State->editfilenev[0] == 0 ) {
		// Ez a file meg nincsen elnevezve:
		return tolt_save_as( lokkolhato );
	}
	Ptop->save( State->editfilenev );
	Valtozott = 0;
	return 1;
}

// Ha sikeresen mentett, akkor igaz:
int tolt_save_as( int lokkolhato ) {
	invalidateegesz();
	// Itt most be kell olvasnunk egy nevet:
	box boxnagy = { 20, 200, 600, 380 };
	if( !lokkolhato )
		boxnagy.y2 = 300;
	box boxsave = { 320-30, 273, 320+30, 290 };
	box boxsavelock = { 320-45, 340, 320+45, 357 };

	push();
	kirajzbox( Editbuffer, boxnagy, Dialogszin, Dialogszelszin );
	kirajzbox( Editbuffer, boxsave, Buttonszin, Dialogszelszin );
	if( lokkolhato )
		kirajzbox( Editbuffer, boxsavelock, Buttonszin, Dialogszelszin );
	Pabc2->writekozep( Editbuffer, (boxnagy.x2+boxnagy.x1)/2, 220, "Type in the file name and press ENTER or click on SAVE, or press ESC to cancel!" );
	if( lokkolhato )
		Pabc2->writekozep( Editbuffer, (boxnagy.x2+boxnagy.x1)/2, 320, "If you click on the SAVE LOCKED button, the saved file won't be modifiable anymore. " );
	Pabc2->writekozep( Editbuffer, (boxsave.x2+boxsave.x1)/2, boxsave.y1+13, "SAVE" );
	if( lokkolhato )
		Pabc2->writekozep( Editbuffer, (boxsavelock.x2+boxsavelock.x1)/2, boxsavelock.y1+13, "SAVE LOCKED" );

	blt8( Pscr8, Editbuffer, boxnagy.x1, boxnagy.y1, boxnagy.x1, boxnagy.y1, boxnagy.x2, boxnagy.y2 );
	pop();

	mk_emptychar();
	char nev[10] = "";
	int i = 0;
	int egerballevoltnyomva = 0;
	int egerjobblevoltnyomva = 0;
	char eddignev[10] = "$%...%^&";
	while( 1 ) {
		if( strcmp( eddignev, nev ) != 0 ) {
			strcpy( eddignev, nev );

			int ex1 = boxnagy.x1+100;
			int ey1 = boxnagy.y1+30;
			int ex2 = boxnagy.x2-100;
			int ey2 = boxnagy.y1+55;
			//Editbuffer->fillbox( ex1, ey1, ex2, ey2, Buttonszin );
			blt8( Pscr8, Editbuffer, ex1, ey1, ex1, ey1, ex2, ey2 );
			Pabc2->write( Pscr8, 290, 250, nev );
			char tmp[40];
			strcpy( tmp, nev );
			tmp[i] = 0;
			Pabc2->write( Pscr8, 290+Pabc1->len( tmp ), 255, "-" );
		}
		while( mk_kbhit() ) {
			int c = mk_getextchar();
			if( c == MK_ESC )
				return 0;
			if( c == MK_ENTER ) {
				if( nev[0] == 0 )
					continue;
				// Most mentunk:
				// Eloallitjuk nevet:
				strcat( nev, ".lev" );
				if( access( nev, 0 ) == 0 )
					if( dialog( "File exists, overwrite?", nev,
										"GOMBOK", "Yes", "No" ) == 1 )
						return 0;
				Ptop->save( nev );
				strcpy( State->editfilenev, nev );
				Valtozott = 0;
				return 1;
			}
			if( ( c >= 'A' && c <= 'Z' ) ||
				( c >= 'a' && c <= 'z' ) ||
				( c >= '0' && c <= '9' ) ) {
				if( strlen( nev ) >= 8 )
					continue;
				// c-t beillesztjuk i. helyre:
				int hossz = strlen( nev );
				for( int j = hossz; j >= i; j-- )
					nev[j+1] = nev[j];
				nev[i] = c;
				i++;
			}
			if( c == MK_BACKSPACE ) {
				// Torles:
				if( nev[0] == 0 || i <= 0 )
					continue;
				// Kiszedjuk i-1-edik helyrol karaktert:
				int hossz = strlen( nev );
				for( int j = i-1; j < hossz; j++ )
					nev[j] = nev[j+1];
				i--;
			}
			if( c == MK_DEL ) {
				// Torles:
				if( nev[0] == 0 || i >= strlen( nev ) )
					continue;
				// Kiszedjuk i-1-edik helyrol karaktert:
				int hossz = strlen( nev );
				for( int j = i; j < hossz; j++ )
					nev[j] = nev[j+1];
			}
			if( c == MK_LEFT && i > 0 ) {
				i--;
				eddignev[0] = 0;
			}
			if( c == MK_RIGHT && i < strlen( nev ) ) {
				i++;
				eddignev[0] = 0;
			}
		}
		// Eger lenyomas:
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

		if( jobbnyomva )
			return 0;

		if( balnyomva ) {
			int ujx = 0, ujy = 0;
			getmou( &ujx, &ujy );
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
			// Most nezzuk meg, hogy hova kattintott:
			if( boxban( ujx, ujy, boxsave ) ) {
				if( nev[0] == 0 )
					continue;
				// Most mentunk:
				// Eloallitjuk nevet:
				strcat( nev, ".lev" );
				if( access( nev, 0 ) == 0 )
					if( dialog( "File exists, overwrite?", nev,
										"GOMBOK", "Yes", "No" ) == 1 )
					continue;
				Ptop->save( nev );
				strcpy( State->editfilenev, nev );
				Valtozott = 0;
				return 1;
			}
			if( lokkolhato && boxban( ujx, ujy, boxsavelock ) ) {
				// Lockoltan mentjuk el:
				if( nev[0] == 0 )
					continue;
				// Most mentunk:
				// Eloallitjuk nevet:
				strcat( nev, ".lev" );
				if( strcmpi( nev, State->editfilenev ) == 0 ) {
					dialog( "You must give a name that is different from the name of the edited file!" );
					return 0;
				}
				if( dialog(
"WARNING: The file you save will be locked. You won't be able to load it into the editor!",
"You should save a locked file only if you want to give it to somebody",
"and don't want him/her to modify your level file.",
"It is recommended that you save and keep the original (not locked) file",
"in case you want to modify it.",
"After saving the file, the name of the content of the editor will remain the original.",
"",
"Do you want to continue?",


										"GOMBOK", "Yes", "No" ) == 1 )
						return 0;
				if( access( nev, 0 ) == 0 )
					if( dialog( "File exists, overwrite?", nev,
										"GOMBOK", "Yes", "No" ) == 1 )
						return 0;
				Ptop->lezart = 1;
				Ptop->save( nev );
				Ptop->lezart = 0;
				//strcpy( State->editfilenev, nev );
				return 1;
			}
		}
		int ujx = 0, ujy = 0;
		getmou( &ujx, &ujy );
		if( ujx != Moux && ujy != Mouy ) {
			push();
			Moux = ujx;
			Mouy = ujy;
			pop();
		}
	}
}

void change_level_name( void ) {
	push(); // Itt levesszuk egeret es nem is tesszuk vissza vegeig:

	invalidateegesz();
	// Itt most be kell olvasnunk egy nevet:
	int x1 = 50;
	int y1 = 200;
	int x2 = 600;
	int y2 = 280;
	Editbuffer->fillbox( x1, y1, x2, y2, Save_as_box_szin );

	blt8( Pscr8, Editbuffer, x1, y1, x1, y1, x2, y2 );
	Pscr8->line( x1, y1, x2, y1, Save_as_szelszin );
	Pscr8->line( x1, y2, x2, y2, Save_as_szelszin );
	Pscr8->line( x1, y1, x1, y2, Save_as_szelszin );
	Pscr8->line( x2, y1, x2, y2, Save_as_szelszin );
	Pabc2->writekozep( Pscr8, (x2+x1)/2, 220, "Type in the name of the level and press ENTER, or press ESC to cancel!" );

	mk_emptychar();
	char nev[100];
	strcpy( nev, Ptop->levelname );
	int i = 0;
	while( 1 ) {
		blt8( Pscr8, Editbuffer, x1+1, y1+30, x1+1, y1+30, x2-1, y2-1 );
		Pabc2->write( Pscr8, 300, 250, nev );
		char tmp[40];
		strcpy( tmp, nev );
		tmp[i] = 0;
		Pabc2->write( Pscr8, 300+Pabc1->len( tmp ), 255, "-" );
		int c = mk_getextchar();
		if( c == MK_ESC ) {
			pop();
			return;
		}
		if( c == MK_ENTER ) {
			if( nev[0] == 0 )
				continue;
			strcpy( Ptop->levelname, nev );
			Valtozott = 1;
			pop();
			return;
		}
		if( ( c >= 'A' && c <= 'Z' ) ||
			( c >= 'a' && c <= 'z' ) ||
			( c >= '0' && c <= '9' ) ||
			c == ' ' ) {
			if( strlen( nev ) >= LEVELNAMEHOSSZ )
				continue;
			// c-t beillesztjuk i. helyre:
			int hossz = strlen( nev );
			for( int j = hossz; j >= i; j-- )
				nev[j+1] = nev[j];
			nev[i] = c;
			i++;
		}
		if( c == MK_BACKSPACE ) {
			// Torles:
			if( nev[0] == 0 || i <= 0 )
				continue;
			// Kiszedjuk i-1-edik helyrol karaktert:
			int hossz = strlen( nev );
			for( int j = i-1; j < hossz; j++ )
				nev[j] = nev[j+1];
			i--;
		}
		if( c == MK_DEL ) {
			// Torles:
			if( nev[0] == 0 || i >= strlen( nev ) )
				continue;
			// Kiszedjuk i-1-edik helyrol karaktert:
			int hossz = strlen( nev );
			for( int j = i; j < hossz; j++ )
				nev[j] = nev[j+1];
		}
		if( c == MK_LEFT && i > 0 )
			i--;
		if( c == MK_RIGHT && i < strlen( nev ) )
			i++;
	}
}


