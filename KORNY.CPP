#include	"all.h"

void pleasewait( char* nev ) {
	if( !Korny )
		hiba( "pleasewait-ben Korny == NULL!" );
	blt8( Korny->picbuffer, Korny->ppic_load );
	if( nev ) {
		Korny->pabc_load->writekozep( Korny->picbuffer, 160, 65, "Loading" );
		Korny->pabc_load->writekozep( Korny->picbuffer, 160, 95, nev );
		Korny->pabc_load->writekozep( Korny->picbuffer, 160, 125, "Please Wait!" );
	}
	else {
		// Ide most nem johet!
		Korny->pabc_load->writekozep( Korny->picbuffer, 160, 80, "Loading," );
		Korny->pabc_load->writekozep( Korny->picbuffer, 160, 110, "Please Wait!" );
	}
	//Korny->pal_load->set();
	lassufizre( Korny->picbuffer, Korny->pal_load );
}

kornyezet::kornyezet( void ) {
	int negymegas = 0;
	if( access( "negymeg.inf", 0 ) == 0 )
		negymegas = 1;

	prec = NULL;
	pleiro = NULL;

	ppic_fomenu = NULL;
	pabc_fomenu = NULL;
	pal_fomenu = NULL;
	pal_editor = NULL;

	ppic_load = NULL;
	pabc_load = NULL;
	pal_load = NULL;

	ppic_help = NULL;
	pabc_help = NULL;
	pal_help = NULL;

	ppic_saveplay = NULL;
	pabc_saveplay = NULL;
	pal_saveplay = NULL;

	//ppic_loadplay = NULL;
	//pabc_loadplay = NULL;
	//pal_loadplay = NULL;

	ppic_newjatekos = NULL;
	pabc_newjatekos = NULL;
	pal_newjatekos = NULL;

	ppic_jatekosval = NULL;
	pabc_jatekosval = NULL;
	pal_jatekosval = NULL;

	ppic_playmenu = NULL;
	pabc_playmenu = NULL;
	pal_playmenu = NULL;

	ppic_afterplaymenu = NULL;
	pabc_afterplaymenu = NULL;
	pal_afterplaymenu = NULL;

	ppic_times = NULL;
	pabc_times = NULL;
	pal_times = NULL;

	ppic_leveltimes = NULL;
	pabc_leveltimes = NULL;
	pal_leveltimes = NULL;

	ppic_setsize = NULL;
	ppic_setsizemot = NULL;
	pabc_setsize = NULL;
	pal_setsize = NULL;
	ppic_vollecbal = NULL;
	ppic_vollecjobb = NULL;
	ppic_volgomb = NULL;

	resbeolvlevel = 0;

	fakt_beolvlevel = 0.4;

	// BEOLVASSA KEPEKET, PALOKAT ES ABCKET

	pic8* p_intro = NULL;
	{ p_intro = NEW(pic8*) pic8( "intro.pcx" ); }
	ddpal* pp_intro = NULL;
	pcxtopal( "intro.pcx", &pp_intro ); // Intronak nincs abcje!

	//pp_intro->set();
	lassufizre( p_intro, pp_intro ); // Gyorsan mar ki is rakja intro kepet


	pic8* p_hisco1 = NULL;
	{ p_hisco1 = NEW(pic8*) pic8( "hisco1.pcx" ); }
	ddpal* pp_hisco1 = NULL;
	pcxtopal( "hisco1.pcx", &pp_hisco1 );
	abc8* pa_hisco1 = NULL;
	{ pa_hisco1 = NEW(abc8*) abc8( "hisco1.abc" ); }

	// ShortDognak irtam csak bele, ki kell kommentezni:
	/*pa_hisco1->write( p_intro, 10, 100, "Snap version" );
	pa_hisco1->write( p_intro, 10, 140, "Please do not distribute!" );
	pa_hisco1->write( p_hisco1, 10, 100, "Snap version" );
	pa_hisco1->write( p_hisco1, 10, 140, "Please do not distribute!" );
	lassufizre( p_intro, pp_intro ); // Ujra kirakjuk intro kepet
	*/

	pic8* p_hisco2 = NULL;
	{ p_hisco2 = NEW(pic8*) pic8( "hisco2.pcx" ); }
	ddpal* pp_hisco2 = NULL;
	pcxtopal( "hisco2.pcx", &pp_hisco2 );
	abc8* pa_hisco2 = NULL;
	{ pa_hisco2 = NEW(abc8*) abc8( "hisco2.abc" ); }

	/*pic8* p_loading = NULL;
	{ p_loading = NEW(pic8*) pic8( "loading.pcx" ); }
	ddpal* pp_loading = NULL;
	pcxtopal( "loading.pcx", &pp_loading );
	abc8* pa_loading = NULL;
	{ pa_loading = NEW(abc8*) abc8( "loading.abc" ); } */

	pic8* p_mainmenu = NULL;
	{ p_mainmenu = NEW(pic8*) pic8( "mainmenu.pcx" ); }
	ddpal* pp_mainmenu = NULL;
	pcxtopal( "mainmenu.pcx", &pp_mainmenu );
	pcxtopal( "mainmenu.pcx", &pal_editor );
	abc8* pa_mainmenu = NULL;
	{ pa_mainmenu = NEW(abc8*) abc8( "mainmenu.abc" ); }

	pic8* p_utomenu = NULL;
	{ p_utomenu = NEW(pic8*) pic8( "utomenu.pcx" ); }
	ddpal* pp_utomenu = NULL;
	pcxtopal( "utomenu.pcx", &pp_utomenu );
	abc8* pa_utomenu = NULL;
	{ pa_utomenu = NEW(abc8*) abc8( "utomenu.abc" ); }

	pic8* p_oreg = NULL;
	{ p_oreg = NEW(pic8*) pic8( "oreg.pcx" ); }
	ddpal* pp_oreg = NULL;
	pcxtopal( "oreg.pcx", &pp_oreg );
	abc8* pa_oreg = NULL;
	{ pa_oreg = NEW(abc8*) abc8( "oreg.abc" ); }


	{ picbuffer = NEW(pic8*) pic8( 320, 200 ); }
	if( !picbuffer )
		uzenet( "memory" );

	// KIOSZTJA KEPEKET:

	// Fomenu:
	ppic_fomenu = p_mainmenu;
	pabc_fomenu = pa_mainmenu;
	pal_fomenu = pp_mainmenu;

	// Afterplaymenu mindig fomenuvel egyenlo:
	ppic_afterplaymenu = p_utomenu;
	pabc_afterplaymenu = pa_utomenu;
	pal_afterplaymenu = pp_utomenu;

	// load:
	ppic_load = p_hisco1;
	pabc_load = pa_hisco1;
	pal_load = pp_hisco1;

	// loadplay mindig ugyanaz, mint load:
	//ppic_loadplay = ppic_load;
	//pabc_loadplay = pabc_load;
	//pal_loadplay = pal_load;

	// help:
	ppic_help = p_oreg;
	pabc_help = pa_oreg;
	pal_help = pp_oreg;

	// saveplay:
	ppic_saveplay = p_oreg;
	pabc_saveplay = pa_oreg;
	pal_saveplay = pp_oreg;


	// leveltimes:
	ppic_leveltimes = p_hisco1;
	pabc_leveltimes = pa_hisco1;
	pal_leveltimes = pp_hisco1;

	// times:
	ppic_times = p_hisco2;
	pabc_times = pa_hisco2;
	pal_times = pp_hisco2;

	// Newjatekos:
	ppic_newjatekos = p_oreg;
	pabc_newjatekos = pa_oreg;
	pal_newjatekos = pp_oreg;

	// Jatekosval:
	ppic_jatekosval = p_oreg;
	pabc_jatekosval = pa_oreg;
	pal_jatekosval = pp_oreg;

	// Playmenu:
	ppic_playmenu = p_oreg;
	pabc_playmenu = pa_oreg;
	pal_playmenu = pp_oreg;

	// Setsize:
	ppic_setsize = p_oreg;
	{ ppic_setsizemot = NEW(pic8*) pic8( "setsimot.pcx" ); }
	// Volume reszek:
	// Gomb:
	{ ppic_volgomb = NEW(pic8*) pic8( "gomb.pcx" ); }
	spriteosit( ppic_volgomb, ppic_volgomb->gpixel( 0, 0 ) );
	// Lec bal:
	{ ppic_vollecbal = NEW(pic8*) pic8( "lecbal.pcx" ); }
	if( !(ppic_vollecbal->getxsize() % 2) )
		hiba( "lec xsize paros!" );
	// Lec jobb:
	{ ppic_vollecjobb = NEW(pic8*) pic8( "lecjobb.pcx" ); }

	setsizeindex = ppic_setsizemot->gpixel( 0, 0 );
	pabc_setsize = pa_oreg;
	pal_setsize = pp_oreg;


	// VEGYES:
	{ picbuffervalaszt = NEW(pic8*) pic8( 320, 200 ); }
	{ rubrikak = NEW(rubrika*) rubrika[MAXRUBRIKASZAM]; }
	if( !picbuffervalaszt || !rubrikak )
		uzenet( "memory" );

	if( negymegas ) {
		{ prec = NEW(recorder*) recorder( 7185, 1500 ); }
	}
	else {
		{ prec = NEW(recorder*) recorder( 14370, 3000 ); }
	}


	{ pleiro = NEW(palyaleiro*) palyaleiro( Leironevtomb[0] ); }
	leiroazonosito = 0;

	levelpszak = 0;
	loadszakaszok( "paly01.txt", 0, &Pszak, &Pszakt );
	if( Fejsugar > Kor2.sugar )
		Pszak->rendez( Fejsugar );
	else
		Pszak->rendez( Kor2.sugar );
	st.origo = magassagokeloallitasa( Pszak, &st.pmviz, &st.pmfugg );
	hatterszamitas( st.pmviz->getxsize(), st.pmfugg->getxsize() );
	legjobbido[0] = 0;
}

kornyezet::~kornyezet( void ) {
	// Nem torlok semmit, mivel ugyis csak kilepeskor hivodik meg!
}

void kornyezet::hatterszamitas( double tartxsize, double tartysize ) {
	// Egy kis ellenorzes:
	if( !pleiro || !pleiro->peg )
		hiba( "kornyezet::hatterszamitas-ban!: ",
				"!pleiro || !pleiro->ppicbuffer || !pleiro->peg" );

	// Megengedheto lassitasok kiszamitasa:
	// X iranyban:
	double kiskepxsize = Displaysizex;
	double nagykepxsize = pleiro->peg->getxsize();

	if( kiskepxsize+4 > nagykepxsize )
		hiba( "kornyezet::hatterszamitas-ban kiskepxsize+4 > nagykepxsize!" );
	if( tartxsize < 100 )
		hiba( "kornyezet::hatterszamitas-ban tartxsize < 100!" );

	double lassitasx = (nagykepxsize-kiskepxsize-4) / tartxsize;

	// Y iranyban:
	double kiskepysize = Displaysizey;
	double nagykepysize = pleiro->peg->getysize();

	if( kiskepysize+4 > nagykepysize )
		hiba( "kornyezet::hatterszamitas-ban kiskepysize+4 > nagykepysize!" );
	if( tartysize < 100 )
		hiba( "kornyezet::hatterszamitas-ban tartysize < 100!" );

	double lassitasy = (nagykepysize-kiskepysize-4) / tartysize;

	// Most meghatarozzuk lassitas merteket:
	lassitas = 0.1; // Ez maximalis lassitas
	if( lassitasx < lassitas )
		lassitas = lassitasx;
	if( lassitasy < lassitas )
		lassitas = lassitasy;

	// Most meghatarozott lassitasbol kiszamoljuk hatteralap paramokat:
    // X iranyban:
	double felnel = lassitas*tartxsize/2.0+kiskepxsize/2.0;
	hatteralapx = nagykepxsize/2.0-felnel;
	// Ellenorzes:
	if( hatteralapx < 0 )
		hiba( "kornyezet::hatterszamitas-ban hatteralapx < 0!" );
	if( tartxsize*lassitas+kiskepxsize > nagykepxsize )
		hiba( "kornyezet::hatterszamitas-ban!: ",
					"tartxsize*lassitas+kiskepxsize > nagykepxsize!" );

	// Y iranyban:
	felnel = lassitas*tartysize/2.0+kiskepysize/2.0;
	hatteralapy = nagykepysize/2.0-felnel;
	// Ellenorzes:
	if( hatteralapy < 0 )
		hiba( "kornyezet::hatterszamitas-ban hatteralapy < 0!" );
	if( tartysize*lassitas+kiskepysize > nagykepysize )
		hiba( "kornyezet::hatterszamitas-ban!: ",
					"tartysize*lassitas+kiskepysize > nagykepysize!" );
}

#define DIALOG320SOR (7)

void dialog320( char* text0, char* text1, char* text2, char* text3,
				char* text4, char* text5, char* text6 ) {
	char* sortomb[DIALOG320SOR];
	sortomb[0] = text0;
	sortomb[1] = text1;
	sortomb[2] = text2;
	sortomb[3] = text3;
	sortomb[4] = text4;
	sortomb[5] = text5;
	sortomb[6] = text6;
	int sorszam = 0;
	for( int i = 0; i < DIALOG320SOR; i++ ) {
		if( sortomb[i] )
			sorszam++;
		else
			break;
	}
	if( sorszam <= 0 )
		hiba( "iutgev!" );
	int dy = 32;
	int y0 = 160-dy*sorszam;

	blt8( Korny->picbuffer, Korny->ppic_playmenu );
	for( i = 0; i < sorszam; i++ )
		Korny->pabc_playmenu->writekozep( Korny->picbuffer,
										160, y0+i*dy, sortomb[i] );
	lassufizre( Korny->picbuffer, Korny->pal_playmenu );
	mk_emptychar();
	mk_getextchar();
}


leveltomb_s Leveltomb[100] = {
	// 0.
	{ 0, 0, "Warm Up",	 		   "paly01.txt" },
	{ 0, 0, "Flat Track",          "paly02.txt" },
	{ 0, 1, "Twin Peaks",          "paly03.txt" },
	{ 0, 0, "Over and Under",      "paly04.txt" },
	{ 0, 1, "Uphill Battle",       "paly05.txt" },
	{ 0, 0, "Long Haul",           "paly06.txt" },
	{ 0, 0, "Hi-Flyer",            "paly07.txt" },
	{ 0, 1, "Tunnel Terror",       "paly08.txt" },
	{ 0, 1, "The Steppes",         "paly09.txt" },
	{ 0, 1, "Islands in the Sky",  "paly10.txt" },
	// 10.
	{ 0, 1, "Hill Legend",         "paly11.txt" },
	{ 0, 0, "Fair Trial",          "paly12.txt" },
	{ 0, 0, "Loop-de-Loop",        "paly13.txt" },
	{ 0, 1, "Slippery Slope",      "paly14.txt" },
	{ 0, 1, "The Turnaround",      "paly15.txt" },
	{ 0, 1, "Hangman",             "paly16.txt" },
	{ 0, 1, "On Top",              "paly17.txt" },
	{ 0, 1, "Slalom",              "paly18.txt" },
	{ 0, 0, "Quick Round",         "paly19.txt" },
	{ 0, 0, "Ramp Frenzy",         "paly20.txt" },
	// 20.
	{ 0, 1, "Precarious",          "paly21.txt" },
	{ 0, 1, "Circuitous",          "paly22.txt" },
	{ 0, 0, "The Enigma",          "paly23.txt" },
	{ 0, 0, "Expert System",       "paly24.txt" },
	{ 0, 1, "The End",        		"paly25.txt" },
	{ 0, 1, "CSTXT 01",        		"CS01.txt" },
	{ 0, 1, "Hang Tight",      		"CS02.txt" },
	{ 0, 1, "CSTXT 03",        		"CS03.txt" },
	{ 0, 1, "Serpents Tale",  		"CS04.txt" },
	{ 0, 0, "Fruit in the Den",     "CS05.txt" },
	// 30.
	{ 0, 1, "Shelf Life",      		"CS06.txt" },
	{ 0, 1, "Apple Harvest",       	"CS07.txt" },
	{ 0, 1, "Double Trouble",       "CS08.txt" },
	{ 0, 0, "Zig Zag",        		"CS09.txt" },
	{ 0, 0, "Bounce Back",        	"CS10.txt" },
	{ 0, 0, "Enduro", 		    	"CS11.txt" },
	//{ 0, 0, "Chute the Breeze",     "CS11.txt" },
	{ 0, 0, "Enigma II",          	"paly26.txt" },
	{ 0, 1, "Turnaround II",    	"paly27.txt" },
	{ 0, 1, "Ures hely",          	"nincs.txt" },
	{ 0, 1, "Ures hely",          	"nincs.txt" }, // Ez hasznalva van
	// 40.
	// Level palyak:
	{ 1, 1, "Tricks Abound",		"proba01.lev" },
	{ 1, 1, "CSLPRO 02",			"proba02.lev" },
	{ 1, 0, "Spiral",				"proba03.lev" },
	{ 1, 1, "CSLPRO 04",			"proba04.lev" },
	{ 1, 1, "New Wave",				"proba05.lev" },
	{ 1, 1, "CSLPRO 06",			"proba06.lev" },
	{ 1, 1, "CSLPRO 07",			"proba07.lev" },
	{ 1, 1, "Thorny Route",			"proba08.lev" },
	{ 1, 1, "Curvaceous",			"proba09.lev" },
	{ 1, 0, "Jaws",					"proba10.lev" },
	// 50.
	{ 1, 1, "Freefall",				"proba11.lev" },
	{ 1, 1, "He He",				"proba12.lev" },
	{ 1, 1, "CSLPRO 13",			"proba13.lev" },
	{ 1, 1, "Labyrinth Pro",		"proba14.lev" },
	{ 1, 1, "Animal Farm",    		"proba15.lev" },
	{ 1, 1, "Framework",			"proba16.lev" },
	{ 1, 1, "Headbanger",			"proba17.lev" },
	{ 1, 1, "More Levels",	    	"visit.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	// 60.
	{ 1, 1, "CSL NINCS",	    	"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",	    	"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",	    	"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",	    	"proba00.lev" },
	// 70.
	{ 1, 1, "Labyrinth",			"bal01.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" },
	{ 1, 1, "CSL NINCS",			"proba00.lev" }
};

int Levellista[100] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	/*10:*/
	10, // Hill Legend
	12, // Loop-the-Loop
	28, // Number of the Beast
	44, // New Wave
	70, // Labyrinth
	42, // Spiral
#ifdef SARVARI
	24, // The End
#else
	37, // The Turnaround II (a modositott)
#endif
	15, // Hangman
	17, // Slalom
	18, // Quick Round
	/*20:*/
	19, // Ramp Frenzy
	20, // Precarious
	21, // Circuitous
	30, // Shelf Life
	34, // Bounce Back
	56, // HeadBanger
	54, // Dinoszaurusz
	33, // Zig Zag
	53, // Labyrinth Pro
	29, // Apple in the Den
	/*30:*/
	49, // Jaws
	48, // Curvaceous
	32, // Double Trouble
	55, // Framework
	35, // Chute the Breeze
	51, // HEHE
	50, // Freefall
	36, // Enigma II
	23, // Expert System
	40, // Tricks Abound
	/*40:*/
	26, // Hang Tight
	31, // Apple Harvest
	57, // Visit our homepage
	39, // EZ MAR NEM PALYA
	39, 39, 39, 39, 39, 39,
	/*50:*/
	39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	/*60:*/
	39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	/*70:*/
	39, 39, 39, 39, 39, 39, 39, 39, 39, 39,

	// Ezek mar csak lejatszhato palyak:
	/*80:*/
	22, // The Enigma: regi Enigma
	11, // Fair Trial
	13, // Slippery Slope
	16,  // On Top
	14,  // The Turnaround (regi)

	39, 39,
};

char* getleveldescription( int level ) {
	if( level < 0 || level >= MAXPALYASZAM )
		hiba( "getleveldescription-ban level < 0 || level >= MAXPALYASZAM!" );
	return Leveltomb[Levellista[level]].description;
}

// PALYASZAM csak maximumkent van ideirva:
char Leironevtomb[5][20] = {
	"leir1.lei",
	"leir2.lei",
	"leir3.lei"
};

