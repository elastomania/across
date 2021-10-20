#include	"all.h"

void showinstruct( void ) {
	blt8( Korny->picbuffer, Korny->ppic_help );

	int x1 = 160;
	int yo = 4;
	int dy = 24;

	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo,      "After you have eaten all the");
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy,   "foods, touch the flower!" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*2, "" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*3, "Use the four arrow keys and" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*4, "the space bar!" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*5, "" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*6, "For more info, go to help" );
	Korny->pabc_help->writekozep( Korny->picbuffer, x1, yo+dy*7, "from main menu!" );

	lassufizre( Korny->picbuffer, Korny->pal_help );
	int done = 0;
	while( !done ) {
		mk_emptychar();
		/*int c =*/ mk_getextchar();
		//if( c == MK_ESC || c == MK_ENTER )
		done = 1;
	}
}

// Ha text1 elobb van abc-ben, igazzal ter vissza:
static int abcbenelobb( char* text1, char* text2 ) {
	while( 1 ) {
		if( !*text1 )
			return 1;
		if( !*text2 )
			return 0;
		if( *text1 < *text2 )
			return 1;
		if( *text2 < *text1 )
			return 0;

		text1++;
		text2++;
	}
}

static int Megnemrand = 1;

int veletlen( void ) {
	// Veletlenszeru lejatszas:
	if( Megnemrand ) {
		Megnemrand = 0;
		srand( (unsigned)clock() );
	}

	int sum = 0;
	for( int i = 0; i < 100; i++ )
		sum += rand();
	while( sum < 0 )
		sum *= 3;
	return sum;
}

static void replay( void ) {
	strcpy( Korny->rubrikak[0], "Randomizer" );

	find_t ffblk;
	int done;
	if( Win_95 )
		done = _dos_findfirst( "*.rew", _A_NORMAL, &ffblk );
	else
		done = _dos_findfirst( "*.rec", _A_NORMAL, &ffblk );
	// Ez mindig eggyel tobb, mint valosag, mivel az elso
	// rubrika mindig 'Randomizer':
	int szamuk = 1;
	while( !done && szamuk < MAXRUBRIKASZAM ) {
		char* nev = Korny->rubrikak[szamuk];
		strcpy( nev, ffblk.name );
		// Leveszi kiterjesztest:
		int i = 0;
		while( nev[i] != '.' ) {
			if( !nev[i] )
				hiba( "replay-ban nincs pont nevben!: ", nev );
			i++;
		}
		nev[i] = 0;

		done = _dos_findnext( &ffblk );
		szamuk++;
		if( szamuk >= MAXRUBRIKASZAM-4 )
			done = 1;
	}

	if( szamuk < 2 )
		return;

	// ABC sorrendbe rendez:
	for( int i = 0; i < szamuk+1; i++ ) {
		for( int j = 1; j < szamuk-1; j++ ) {
			if( abcbenelobb( Korny->rubrikak[j+1], Korny->rubrikak[j] ) ) {
				// Csere:
				char tmp[100];
				strcpy( tmp, Korny->rubrikak[j] );
				strcpy( Korny->rubrikak[j], Korny->rubrikak[j+1] );
				strcpy( Korny->rubrikak[j+1], tmp );
			}
		}
	}

	int kurrens = 0;

	while( 1 ) {
		//Korny->pal_saveplay->set();
		blt8( Korny->picbuffer, Korny->ppic_saveplay );
		Korny->pabc_saveplay->write( Korny->picbuffer, 20, 2*SM-24,
										"Select replay file!" );

		// Valasztas:
		valaszt val;
		val.szam = szamuk;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		kurrens = val.valassz( Korny->pal_saveplay );

		if( kurrens < 0 ) {
			Kellobjectrendezni = 1;
			ervenytlenitpalya();
			return;
		}

		if( kurrens == 0 ) {
			// RANDOMIZER:
			// 0-tol kezdve szamolunk:
			int elozopl = -1;
			int elozoelottipl = -1;
			while( 1 ) {
				int pl = veletlen()%(szamuk-1);
				while( (pl == elozopl && szamuk > 2) ||
					   (pl == elozoelottipl && szamuk > 3) ) {
					pl = rand()%(szamuk-1);
				}
				elozoelottipl = elozopl;
				elozopl = pl;
				// Nev eloallitasa:
				char tmp[30];
				strcpy( tmp, Korny->rubrikak[pl+1] );
				strcat( tmp, ".rec" );
				// 0 param azt jelenti, hogy nem resource filebol olvas:
				long belyeg = Korny->prec->load( tmp, 0 );
				Kellobjectrendezni = !Korny->prec->regi;
				ervenytlenitpalya();
				int level = Korny->prec->getlevel();
				if( belyeg ) {
					if( !palyabe( level, belyeg ) ) {
						dialog320( "The name of the rec file:", tmp );
						break;
					}
				}
				else {
					if( !palyabe( level, belyeg ) )
						hiba( "replay-ben belyeg es !palyabe( level, belyeg )!" );
				}
				int parreplay = 1;
				Korny->prec->rewind();
				if( lejatszo( Korny->pleiro, &Korny->st,
								Korny->prec, parreplay ) )
					break;
			}
		}
		else {
			// Egy rec file lejatszasa:
			char tmp[30];
			strcpy( tmp, Korny->rubrikak[kurrens] );
			strcat( tmp, ".rec" );
			long belyeg = Korny->prec->load( tmp, 0 );
			Kellobjectrendezni = !Korny->prec->regi;
			ervenytlenitpalya();
			int level = Korny->prec->getlevel();
			if( belyeg ) {
				if( !palyabe( level, belyeg ) )
					continue;
			}
			else {
				if( !palyabe( level, belyeg ) )
					hiba( "replay-ben belyeg es !palyabe( level, belyeg )!" );
			}
			replayjelenlegi();
		}
	}
}

static void demo( void ) {
	int demoszam = 2;
	if( Sarvari )
		demoszam = 2;
	#ifdef W_95
		hiba( "Demoban most nem lehet!" );
		char demonevek[14][14] = { "w4.rew",
								   "w10.rew",
								   "w15.rew",
								   "w17.rew",
								   "w23.rew"
								 };
   #else
		/*char demonevek[14][14] = { "dw17.rec",
								   "dw20.rec",
								   "dw21.rec",
								   "dw23.rec",
								   "dw25.rec"
								 }; */
		char demonevek[14][14] = { "2.rec",
								   "22.rec",
								   "10.rec"
								 };
	#endif
	int elozodemo = -1;
	int elozoelotti = -1;
	/*while( 1 ) {
		int demo = veletlen()%demoszam;
		while( demo == elozodemo || demo == elozoelotti )
			demo = rand()%demoszam;
		elozoelotti = elozodemo;
		elozodemo = demo;
		Korny->prec->load( demonevek[demo], 1 );
		int level = Korny->prec->getlevel();
		palyabe( level, 1 );
		int parmcga = 1;
		int parreplay = 1;
		Korny->prec->rewind();
		if( lejatszo( Korny->pleiro, &Korny->st, parmcga,
							Korny->prec, parreplay ) )
			return;
	} */
	while( 1 ) {
		elozoelotti++;
		int demo = veletlen()%demoszam;
		while( demo == elozodemo )
			demo = rand()%demoszam;
		elozoelotti = elozodemo;
		elozodemo = demo;
		Korny->prec->load( demonevek[demo], 1 );
		Kellobjectrendezni = !Korny->prec->regi;
		ervenytlenitpalya();
		int level = Korny->prec->getlevel();
		palyabe( level );
		int parreplay = 1;
		Korny->prec->rewind();
		if( lejatszo( Korny->pleiro, &Korny->st,
							Korny->prec, parreplay ) ) {
			Kellobjectrendezni = 1;
			ervenytlenitpalya();
			return;
		}
	}
}

static void kilephakell( void ) {
	blt8( Korny->picbuffer, Korny->ppic_fomenu );
	Korny->pabc_fomenu->write( Korny->picbuffer, 50, 20, "Do you want to quit?" );
	//Korny->pal_fomenu->set();
	// Valasztas:
	valaszt val;
	val.szam = 2;
	val.kur = 0;
	val.egykepen = 6;
	val.x0 = 130;
	val.y0 = 70;
	val.dy = 35;
	val.hatter = Korny->picbuffer;
	val.pabc = Korny->pabc_fomenu;
	val.escelheto = 0;

	strcpy( Korny->rubrikak[0], "Yes" );
	strcpy( Korny->rubrikak[1], "No" );

	int eredmeny = val.valassz( Korny->pal_fomenu );

	if( eredmeny == 0 )
		kilep();
}

void mainmenu( void ) {
	State->save();
	valaszt val;
	int kurrens = 0;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_fomenu );
		Korny->pabc_fomenu->write( Korny->picbuffer, 70, 10, "Main menu" );
		//Korny->pal_fomenu->set();
		// Valasztas:
		val.szam = 6;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_fomenu;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "Play" );
		strcpy( Korny->rubrikak[1], "Replay" );
		strcpy( Korny->rubrikak[2], "Demo" );
		strcpy( Korny->rubrikak[3], "Options/Help" );
		strcpy( Korny->rubrikak[4], "Best times" );
		strcpy( Korny->rubrikak[5], "Editor" );

		int eredmeny = val.valassz( Korny->pal_fomenu );

		if( eredmeny >= 0 ) {
			// Kilepest kiveve:
			kurrens = eredmeny;
		}

		if( eredmeny == 0 )
			play();

		if( eredmeny == 1 ) {
			replay();
		}

		if( eredmeny == 2 ) {
			demo();
		}

		if( eredmeny == 3 ) {
			options();
		}

		if( eredmeny == 4 ) {
			besttimes();
		}

		if( eredmeny == 5 ) {
			editor();
		}

		if( eredmeny == -1 ) {
			State->save();
			kilephakell();
		}
	}
}