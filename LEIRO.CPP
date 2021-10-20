#include	"all.h"

int Displaysizex = 320, Displaysizey = 200;

void palyaleiro::foldbeolv_ratesz( int leiroindex ) {
	char foldnev[20] = "";
	switch( leiroindex ) {
		case 0:
			strcpy( foldnev, "4_fold1.pcx" );
			break;
		case 1:
			strcpy( foldnev, "4_fold2.pcx" );
			break;
		case 2:
			strcpy( foldnev, "4_fold3.pcx" );
			break;
		default:
			hiba( "foldbeolv-ban nincs meg leiroindex!" );
	}

	pic8* pfoldkis;
	if( leiroindex != 0 ) {
		{ pfoldkis = NEW(pic8*) pic8( foldnev ); }
	}
	else {
		// Eloszor teljes meretu kepet olvassuk be:
		pic8* pfoldnagy;
		{ pfoldnagy = NEW(pic8*) pic8( foldnev ); }
		// Lekicsinyitjuk:
		int kellmeret = 80;
		int kxsize = kellmeret*Aranyx/50.0;
		int kysize = kellmeret*Aranyy/50.0;
		{ pfoldkis = NEW(pic8*) pic8( kxsize, kysize ); }
		mintavetel8( pfoldkis, pfoldnagy );
		DELETE pfoldnagy; }
	}
	// Foldet x iranyban megsokszorozzuk:
	foldxalapsize = pfoldkis->getxsize();
	int szoros = 1;
	while( szoros*foldxalapsize < Displaysizex )
		szoros++;
	szoros++;
	if( pfold || pratesz )
		hiba( "palyaleiro::foldbeolv_ratesz-ben pfold || pratesz nem NULL!" );
	{ pfold = NEW(pic8*) pic8( szoros*foldxalapsize, pfoldkis->getysize() ); }
	for( int i = 0; i < szoros; i++ ) {
		blt8( pfold, pfoldkis, i*foldxalapsize, 0 );
	}
	DELETE pfoldkis; }

	{ pratesz = NEW(ratesz*)ratesz( Displaysizey, pfold, foldxalapsize ); }

	if( leiroindex == 0 ) {
		vieweloter  = 39;
		viewhatter  = 127;
		viewmotorindex = 18;
	}
	if( leiroindex == 1 ) {
		vieweloter  = 24;
		viewhatter  = 127;
		viewmotorindex = 7;
	}
	if( leiroindex == 2 ) {
		vieweloter  = 98;
		viewhatter  = 127;
		viewmotorindex = 18;
	}
}

static void loadnev( char* text, FILE* h ) {
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni sort filebol!" );
	char tmp[200];
	if( sscanf( sor, "%s", tmp ) != 1 )
		hiba( "loadnev-ben nem tudta feldolgozni sort!: ", sor );
	if( strlen( tmp ) > FILENEVHOSSZ )
		hiba( "loadnev-ben nev tul hosszu!: ", tmp );
	strcpy( text, tmp );
}

palyaleiro::palyaleiro( char* nev ) {
	pal = NULL;
	ptt = NULL;
	pvpic8f = NULL;
	pviewbuffer = NULL;
	//pegviz = NULL;
	//pegfugg = NULL;
	//pfoldviz = NULL;
	//pfoldfugg = NULL;

	peg = NULL;
	pfelho = NULL;
	pfold = NULL;

	pratesz = NULL;

	pkiskerek = NULL;
	pkisa = NULL;
	pkisb = NULL;
	pkisc = NULL;
	pkisd = NULL;
	pkise = NULL;
	pkiselsorud = NULL;
	pkishatsorud = NULL;
	pkisalkar = NULL;
	pkisfelkar = NULL;
	pkisfej = NULL;

	foldxalapsize = 0;

	int leiroindex = -1;
	if( strcmp( nev, "leir1.lei" ) == 0 )
		leiroindex = 0;
	if( strcmp( nev, "leir2.lei" ) == 0 )
		leiroindex = 1;
	if( strcmp( nev, "leir3.lei" ) == 0 )
		leiroindex = 2;
	if( leiroindex < 0 )
		hiba( "palyaleiro::palyaleiro-ban nem talalta meg nevet!: ", nev );


	FILE* h = qopen( nev, "rt" );
	if( !h )
		hiba( "Nem tudta megnyitni palyaleirot!: ", nev );



	{ pvpic8f = NEW(pic8f*) pic8f( Config.viewxsize, Config.viewysize ); }
	{ pviewbuffer = NEW(pic8*) pic8( Config.viewxsize, Config.viewysize ); }

	filenev tmp;
	loadnev( tmp, h );
	{ ptt = NEW(targytomb*) targytomb( tmp ); }

	// EG:
	loadnev( tmp, h );
	{ peg = NEW(pic8*) pic8( tmp ); }
	pcxtopal( tmp, &pal );
	// FELHO:
	loadnev( tmp, h );
	// Nagy felho:
	if( State->res > 2 ) {
		loadnev( tmp, h ); // Nagy felbontasban nagy felho felulir
	}
	else {
		filenev tmp2;
		loadnev( tmp2, h ); // Kis felbontasban nagy felho elveszik
	}
	{ pfelho = NEW(pic8*) pic8( tmp ); }
	unsigned char szin = pfelho->gpixel( 0, 0 );
	spriteosit( pfelho, szin );
	// FOLD:
	//loadnev( tmp, h ); fold neve most be van huzalozva:
	foldbeolv_ratesz( leiroindex );

	// Kerek:
	loadnev( tmp, h );
	{ pkiskerek = NEW(kiskep*) kiskep( tmp ); }
	// A:
	loadnev( tmp, h );
	{ pkisa = NEW(kiskep*) kiskep( tmp ); }
	// B:
	loadnev( tmp, h );
	{ pkisb = NEW(kiskep*) kiskep( tmp ); }
	// C:
	loadnev( tmp, h );
	{ pkisc = NEW(kiskep*) kiskep( tmp ); }
	// D:
	loadnev( tmp, h );
	{ pkisd = NEW(kiskep*) kiskep( tmp ); }
	// E:
	loadnev( tmp, h );
	{ pkise = NEW(kiskep*) kiskep( tmp ); }
	// Lyukindexeket a-bol atmasoljuk:
	pkisb->lyuk = pkisc->lyuk = pkisd->lyuk = pkise->lyuk = pkisa->lyuk;
	// Elsorud:
	loadnev( tmp, h );
	{ pkiselsorud = NEW(kiskep*) kiskep( tmp ); }
	// Hatsorud:
	loadnev( tmp, h );
	{ pkishatsorud = NEW(kiskep*) kiskep( tmp ); }
	// Alkar:
	loadnev( tmp, h );
	{ pkisalkar = NEW(kiskep*) kiskep( tmp ); }
	// Alkar:
	loadnev( tmp, h );
	{ pkisfelkar = NEW(kiskep*) kiskep( tmp ); }
	// Fej:
	loadnev( tmp, h );
	{ pkisfej = NEW(kiskep*) kiskep( tmp ); }

	// View indexek:
	vieweloter  = loadint( h );
	viewhatter  = loadint( h );
	viewmotorindex  = loadint( h );

	idoszin1 = loadint( h );
	idoszin2 = loadint( h );

	ugroindex = loadint( h );
	keretindex = loadint( h );

	int magicnumber = loadint( h );
	if( magicnumber != 17322 )
		hiba( "leiroban nem stimmel magic number!: ", nev );

	qclose( h );
}

palyaleiro::~palyaleiro( void ) {
	if( pal )
		DELETE pal; }
	if( ptt )
		DELETE ptt; }
	if( pvpic8f )
		DELETE pvpic8f; }
	if( pviewbuffer )
		DELETE pviewbuffer; }
	/*if( pegviz )
		DELETE pegviz; }
	if( pegfugg )
		DELETE pegfugg; }
	if( pfoldviz )
		DELETE pfoldviz; }
	if( pfoldfugg )
		DELETE pfoldfugg; } */

	if( peg )
		DELETE peg; }
	if( pfelho )
		DELETE pfelho; }
	if( pfold )
		DELETE pfold; }

	if( pratesz )
		DELETE pratesz; }

	if( pkiskerek )
		DELETE pkiskerek; }
	if( pkisa )
		DELETE pkisa; }
	if( pkisb )
		DELETE pkisb; }
	if( pkisc )
		DELETE pkisc; }
	if( pkisd )
		DELETE pkisd; }
	if( pkise )
		DELETE pkise; }
	if( pkiselsorud )
		DELETE pkiselsorud; }
	if( pkishatsorud )
		DELETE pkishatsorud; }
	if( pkisalkar )
		DELETE pkisalkar; }
	if( pkisfelkar )
		DELETE pkisfelkar; }
	if( pkisfej )
		DELETE pkisfej; }
}

// Csak foldet csereli ki Korny->leiroazonosito szerint:
// play-bol van hivva:
void csakfoldcsere( void ) {
	if( !Korny->pleiro->pfold || !Korny->pleiro->pratesz )
		hiba( "csakfoldcsere-ben !Korny->pleiro->pfold || !Korny->pleiro->pratesz!" );

	DELETE Korny->pleiro->pfold; }
	Korny->pleiro->pfold = NULL;
	DELETE Korny->pleiro->pratesz; }
	Korny->pleiro->pratesz = NULL;

	Korny->pleiro->foldbeolv_ratesz( Korny->leiroazonosito );
}


