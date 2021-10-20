#include	"all.h"

static int Menulineszin = 0, Menuszin = 163, Menu_nyomottszin = 47;

pic8* Editbuffer = NULL;

topol* Ptop = NULL;

abc8 *Pabc1 = NULL, *Pabc2 = NULL;

gyuru* Pgy = NULL;
int K = 0;
int Fel = 0;

kerek* Pker = NULL;

int Valtozott = 0;

static unsigned char Hatterindex = 167;
//unsigned char Feketeindex = Hatterindex+128;

static int hanypalyavanmeg( void ) {
	jatekos* pjatekos = &State->jatekosok[int(State->jatekos)];
	int skippelt = 0;
	for( int i = 0; i < pjatekos->sikerespalyakszama; i++ ) {
		if( pjatekos->skippedtomb[i] )
			skippelt++;
	}
	return pjatekos->sikerespalyakszama-skippelt;
}

// EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER
// EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER
// EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER
// EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER
// EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER EGER

static int Egerkint = 0;

int Moux = 0, Mouy = 0, Kozelvan = 0;

static void elintezegymousepixelt( int x , int y ) {
	if( x < 0 || y < 0 || x > 639 || y > 479 )
		return;
	unsigned char szin = Editbuffer->gpixel( x, y );
	szin += 128;
	Editbuffer->ppixel( x, y, szin );
	Pscr8->ppixel( x, y, szin );
}

static void egeretrajzolja( void ) {
	int sugar = 4;
	for( int i = -sugar; i <= sugar; i++ ) {
		if( Kozelvan ) {
			elintezegymousepixelt( Moux+i, Mouy+i );
			if( i != 0 )
				elintezegymousepixelt( Moux+i, Mouy-i );
		}
		else {
			elintezegymousepixelt( Moux+i, Mouy );
			if( i != 0 )
				elintezegymousepixelt( Moux, Mouy+i );
		}
	}
}

// Egeret leveszi kepernyorol:
void push( void ) {
	if( !Egerkint )
		hiba( "push-ban !Egerkint!" );
	Egerkint = 0;

	egeretrajzolja();
}

// Egeret felteszi kepernyore:
void pop( void ) {
	if( Egerkint )
		hiba( "pop-ban Egerkint!" );
	Egerkint = 1;

	egeretrajzolja();
}

// TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP
// TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP
// TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP
// TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP TOOLHELP

static char Toolhelpszoveg[110] = "";

void toolhelp( char* text ) {
	if( text ) {
		if( strlen( text ) > 100 )
			hiba( "toolhelp-ben strlen( text ) > 100!", text );
		strcpy( Toolhelpszoveg, text );
	}
	int x1 = 1, y1 = 18, x2 = 638, y2 = Savy-2;
	Editbuffer->fillbox( x1, y1, x2, y2, Hatterindex );
	Pabc1->write( Editbuffer, 10, y1+12, Toolhelpszoveg );
	blt8( Pscr8, Editbuffer, x1, y1, x1, y1, x2, y2 );
}

// KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK
// KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK
// KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK
// KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK KIRAJZOLASOK

static int Kellredraw = 0;
void invalidate( void ) {
	Kellredraw = 1;
}

static int Kellredrawegesz = 0;
void invalidateegesz( void ) {
	Kellredrawegesz = 1;
}

static int Elsotool = 13, Toolszam = 8;
static int Menuszam = Elsotool+Toolszam, Menudy = 20;

static char Menuszovegek[21][15] = { // Masodik egy string hossza
	"Exit",
	"New",
	"Open",
	"Save As",
	"Save",
	"Save and Play",
	"Check Topology",
	"Level Name",
	"Zoom Out",
	"Zoom Fill",
	"Help",
	"",
	"",
	"Move",
	"Zoom In",
	"Create Vertex",
	"Delete Vertex",
	"Delete Polygon",
	"Create Food",
	"Create Killer",
	"Delete Object" };

int Tool = T_MOVE;

static void editbe_balmenu( void ) {
	Editbuffer->fillbox( 0, 0, 639, 479, Menuszin );
	// Eloszor is egy nagy keret:
	Editbuffer->line( 0, 0, 639, 0, Menulineszin );
	Editbuffer->line( 0, 479, 639, 479, Menulineszin );
	Editbuffer->line( 0, 0, 0, 479, Menulineszin );
	Editbuffer->line( 639, 0, 639, 479, Menulineszin );
	// Meg ket belso vonal:
	Editbuffer->line( 0, Savy-1, 639, Savy-1, Menulineszin );
	Editbuffer->line( Savx-1, Savy, Savx-1, 479, Menulineszin );
	for( int i = 0; i < Menuszam; i++ ) {
		// x1... stb a belso reszt jelenti:
		int x1 = 1;
		int y1 = Savy + i*Menudy+1;
		int x2 = Savx-2;
		int y2 = Savy + (i+1)*Menudy-1;
		if( i == Elsotool + Tool )
			Editbuffer->fillbox( x1, y1, x2, y2, Menu_nyomottszin );
		Editbuffer->line( x1, y2+1, x2, y2+1, Menulineszin );
		Pabc1->write( Editbuffer, 5, Savy + i*Menudy+14, Menuszovegek[i] );
	}
	// Meg benyomunk Save and Play felirat P-je ala egy vonalat:
	int x = 66;
	int y = Savy + 5*Menudy+15;
	Editbuffer->line( x, y, x+6, y, 247 );
}

static void ujrarajzol( int egesz ) {
	push();

	if( egesz )
		editbe_balmenu();

	Editbuffer->fillbox( Savx, Savy, 638, 478, Hatterindex );
	Editbuffer->fillbox( 1, 1, 638, Savy-2, Hatterindex );
	// Kiirjuk file nevet:
	Pabc1->write( Editbuffer, 10, 15, "File name:" );
	strupr( State->editfilenev );
	int len = 0;
	int xfile = 85;
	if( State->editfilenev[0] == 0 ) {
		Pabc2->write( Editbuffer, xfile, 15, "UNNAMED" );
		len = Pabc2->len( "UNNAMED" );
	}
	else {
		Pabc2->write( Editbuffer, xfile, 15, State->editfilenev );
		len = Pabc2->len( State->editfilenev );
	}
	if( Valtozott )
		Pabc1->write( Editbuffer, xfile+len+5, 15, "(Changed)" );
	else
		Pabc1->write( Editbuffer, xfile+len+5, 15, "(Unchanged)" );
	// Kiirjuk level nevet:
	Pabc1->write( Editbuffer, 260, 15, "Level name:" );
	Pabc2->write( Editbuffer, 340, 15, Ptop->levelname );
	// Kiirjuk tool szoveget:
	toolhelp();
	// Kiirjuk zoomot:
	Pabc1->write( Editbuffer, 520, 16, "Zoom:" );
	char text[30];
	sprintf( text, "%lf", getzoomvalue() );
	// Tizedespont utani jegyek szamat maximaljuk 2-re:
	if( *strstr( text, "." ) ) {
		*(strstr( text, "." ) + 2) = 0;
	}
	Pabc2->write( Editbuffer, 560, 16, text );

	Vonalcsakeditbe = 1; // Csak edit bufferbe rajzoljon kirajzol

	Ptop->kirajzol();
	if( Pgy ) {
		// Atalakitjuk szaggatotta amit kell:
		Pgy->kirajz( K, Fel, 0 );
		Pgy->kirajz( K, Fel, 1 );
	}

	Vonalcsakeditbe = 0;

	blt8( Pscr8, Editbuffer );

	Kellredraw = 0;
	if( egesz )
		Kellredrawegesz = 0;

	pop();
}



static void beallitkozelvane( int mx, int my ) {
	double x = konvx_i2d( mx );
	double y = konvy_i2d( my );

	int kozelvan = 0;
	if( Tool == T_MOVE || Tool == T_CREATE_VERT || Tool == T_MOVE ||
		Tool == T_DELETE_VERT || Tool == T_DELETE_POLY ) {
		if( Ptop->legkozpont( x, y, &K ) )
			kozelvan = 1;
	}
	if( Tool == T_MOVE || Tool == T_DELETE_KEREK && !kozelvan ) {
		if( Ptop->legkozkerek( x, y ) )
			kozelvan = 1;
	}
	Kozelvan  = kozelvan;
}

static void toolvaltas( int t ) {
	if( t == Tool )
		return;
	Tool = t;
	alaphelp();
	//kibalmenu();
	invalidateegesz();
}

//static int Mouseinited = 0;

void editor( void ) {
	if( !Editbuffer )
		Editbuffer = new pic8( 640, 480 );
	if( !Pabc1 ) {
		Pabc1 = new abc8( "kisbetu1.abc" );
		Pabc1->settav( 1 );
		Pabc2 = new abc8( "kisbetu2.abc" );
		Pabc2->settav( 1 );
	}

	setresdd( 4 ); // 640x480

	Korny->pal_editor->tomb[39*3] = 0;
	Korny->pal_editor->tomb[39*3+1] = 0;
	Korny->pal_editor->tomb[39*3+2] = 0;
	Korny->pal_editor->set();

	/*if( !Mouseinited ) {
		Mouseinited = 1;
		initmou( 640, 480 );
	} */
	initmou( 640, 480 );
	setmou( 320, 240 );
	getmou( &Moux, &Mouy );

	// Megprobaljuk kozepre hozni egeret:
	for( int i = 0; i < 1000; i++ )
		getmou( &Moux, &Mouy );
	int mmx = 355;
	int mmy = 265;
	Moux = mmx;
	Mouy = mmy;
	setmou( mmx, mmy );
	Moux = mmx;
	Mouy = mmy;

	editbe_balmenu();
	blt8( Pscr8, Editbuffer );

	// Egeret eloszor helyezzuk ki:
	Egerkint = 0;
	pop();

	Valtozott = 0;

	// Most megnezzuk, hogy megvannak-e palyai fazonnak:
	if( hanypalyavanmeg() < 5 ) {
		char tmp[50];
		if( hanypalyavanmeg() > 1 )
			sprintf( tmp, "You have completed %d levels so far.", hanypalyavanmeg() );
		else
			sprintf( tmp, "You have completed %d level so far.", hanypalyavanmeg() );
		dialog( "You must complete at least 5 levels before you can use the editor!",
				tmp );
		return;
	}

	// Beolvassuk file-t:
	kivalasztkezdetit();

	if( State->editfilenev[0] == 0 ) {
		Ptop = new topol;
		Valtozott = 0;
	}
	else {
		Ptop = new topol( State->editfilenev, 1 );
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
	}

	// Alaphelyzetbe allitunk egy-ket valtozot:
	Pgy = NULL;
	K = 0;
	Fel = 0;

	Tool = T_MOVE;
	alaphelp();

	kitolt();
	ujrarajzol( 1 );

	int egerballevoltnyomva = 0; // Eger gombja csak lemenetnel aktivizalodik
	int egerjobblevoltnyomva = 0;
	while( 1 ) {
		while( mk_kbhit() ) {
			int c = mk_getextchar();
			if( c == MK_ESC ) {
				if( Tool == T_MOVE )
					t_move_esc();
				if( Tool == T_ZOOMIN )
					t_zoomin_esc();
				if( Tool == T_CREATE_VERT )
					t_create_vert_esc();
			}
			if( c == ' ' )
				if( Tool == T_CREATE_VERT )
					t_create_vert_space();
			if( c == MK_ENTER )
				if( Tool == T_CREATE_VERT )
					t_create_vert_enter();
			if( c == 'p' ) {
				if( !(Pgy || Pker || Zoomfogva || Egypont) ) {
					editplay();
				}
			}
			// Csak debuggolashoz kellettek:
			/*void saveallapot( void );
			void loadallapot( void );
			if( c == 's' ) {
				saveallapot();
			}
			if( c == 'l' ) {
				loadallapot();
				invalidateegesz();
			} */
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
		if( balnyomva || jobbnyomva ) {
			int ujx = 0, ujy = 0;
			getmou( &ujx, &ujy );
			if( ujy < Savy )
				ujy = Savy;
			if( (Pgy || Pker || Zoomfogva || Egypont) && ujx < Savx ) {
				// Ha meg van valami fogva, nem mehetunk gombok fole:
				ujx = Savx;
				setmou( ujx, ujy );
			}
			if( ujx != Moux || ujy != Mouy ) {
				push();
				Moux = ujx;
				Mouy = ujy;
				pop();
			}
			if( ujx < Savx ) {
				// Billentyuk folott tortent lenyomas:
				for( int i = 0; i < Menuszam; i++ ) {
					if( Savy + i*Menudy <= ujy && ujy < Savy + (i+1)*Menudy ) {
						// i-edik gombot lenyomtak menuben:
						if( i == 0 && balnyomva )
							if( menu_exit() )
								return;
						if( i == 0 && jobbnyomva )
							menuhelp_exit();

						if( i == 1 && balnyomva )
							tolt_new();
						if( i == 1 && jobbnyomva )
							menuhelp_new();

						if( i == 2 && balnyomva )
							tolt_open();
						if( i == 2 && jobbnyomva )
							menuhelp_open();

						if( i == 3 && balnyomva )
							tolt_save_as( 1 );
						if( i == 3 && jobbnyomva )
							menuhelp_save_as();

						if( i == 4 && balnyomva )
							tolt_save( 1 );
						if( i == 4 && jobbnyomva )
							menuhelp_save();

						if( i == 5 && balnyomva ) {
							editplay();
							Tool = T_MOVE;
						}
						if( i == 5 && jobbnyomva )
							menuhelp_play();

						if( i == 6 && balnyomva )
							check_topology( 1 );
						if( i == 6 && jobbnyomva )
							menuhelp_check();

						if( i == 7 && balnyomva )
							change_level_name();
						if( i == 7 && jobbnyomva )
							menuhelp_level_name();

						if( i == 8 && balnyomva )
							menu_zoomout();
						if( i == 8 && jobbnyomva )
							menuhelp_zoomout();

						if( i == 9 && balnyomva ) {
							// Zoom Fill:
							kitolt();
							invalidate();
						}
						if( i == 9 && jobbnyomva )
							menuhelp_zoom_fill();

						if( i == 10 )
							edithelp();


						// Toolok:
						if( balnyomva ) {
							if( i >= Elsotool && i < Elsotool+Toolszam )
								toolvaltas( i-Elsotool );
						}
						else {
							// Jobb gomb nyomva:
							if( i == Elsotool+0 ) menuhelp_move();
							if( i == Elsotool+1 ) menuhelp_zoom_in();
							if( i == Elsotool+2 ) menuhelp_create_vertex();
							if( i == Elsotool+3 ) menuhelp_delete_vertex();
							if( i == Elsotool+4 ) menuhelp_delete_polygon();
							if( i == Elsotool+5 ) menuhelp_create_food();
							if( i == Elsotool+6 ) menuhelp_create_killer();
							if( i == Elsotool+7 ) menuhelp_delete_object();
						}
					}
				}
			}
			else {
				if( Tool == T_MOVE && balnyomva )
					t_move_nyomva( ujx, ujy );
				if( Tool == T_MOVE && jobbnyomva )
					t_move_esc();
				if( Tool == T_ZOOMIN && balnyomva )
					t_zoomin_nyomva( ujx, ujy );
				if( Tool == T_ZOOMIN && jobbnyomva )
					t_zoomin_esc();
				if( Tool == T_CREATE_VERT && balnyomva )
					t_create_vert_nyomva( ujx, ujy );
				if( Tool == T_CREATE_VERT && jobbnyomva )
					t_create_vert_esc();
				if( Tool == T_DELETE_VERT && balnyomva )
					t_delete_vert_nyomva( ujx, ujy );
				if( Tool == T_DELETE_POLY && balnyomva )
					t_delete_poly_nyomva( ujx, ujy );
				if( Tool == T_CREATE_FOOD && balnyomva )
					t_create_kerek_nyomva( ujx, ujy, 1 );
				if( Tool == T_CREATE_KILLER && balnyomva )
					t_create_kerek_nyomva( ujx, ujy, 0 );
				if( Tool == T_DELETE_KEREK && balnyomva )
					t_delete_kerek_nyomva( ujx, ujy );
			}
		}

		if( Kellredrawegesz || Kellredraw ) {
			if( Kellredrawegesz )
				ujrarajzol( 1 );
			else
				ujrarajzol( 0 );
			//continue;
		}

		// INNENTOL MAR CSAK EGER MOZGATASA AZ ESEMENY:

		int ujx = 0, ujy = 0;
		getmou( &ujx, &ujy );
		if( ujy < Savy )
			ujy = Savy;
		if( (Pgy || Pker || Zoomfogva || Egypont) && ujx < Savx ) {
			// Ha meg van valami fogva, nem mehetunk gombok fole:
			ujx = Savx;
			setmou( ujx, ujy );
		}
		if( ujx == Moux && ujy == Mouy )
			continue;
		// Eger mozdult:
		if( Tool == T_ZOOMIN ) {
			t_zoomin_mmove( ujx, ujy );
			continue;
		}
		if( Tool == T_CREATE_VERT && (Pgy || Egypont) ) {
			t_create_vert_mmove( ujx, ujy );
			continue;
		}
		if( !Pgy && !Pker ) {
			// Ha nincsen megfogva semmi, akkor itt rajzoljuk kurzort:
			push();
			beallitkozelvane( ujx, ujy );
			Moux = ujx;
			Mouy = ujy;
			pop();
		}
		else {
			// Ha meg van valami fogva, akkor tool rajzolja kurzort:
			if( Tool == T_MOVE )
				t_move_mmove( ujx, ujy );
		}
	}
}




