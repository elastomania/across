#include	"all.h"

#ifdef WATCOM
	static int Szakaszmaxszam = 5130;
#else
	static int Szakaszmaxszam = 1040;
#endif

// loadszakasz beleirja ptop->lezart-at ha topologia file-t olvas be:
// Ezt azutan afterplayext fuggveny hasznalja ki:
int Ucsoloadszakasz_lezart = 0;

/*szakaszok::szakaszok( void ) {
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vekt2( 0, 0 );
	nextnode = NULL;
	pelsocsomag = NULL;
	csomagbanbetelt = 0;


	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );
} */

szakaszok::szakaszok( FILE* h, int targyas ) {
	// Elejen ugyanaz, mint void konstruktor:
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vekt2( 0, 0 );
	nextnode = NULL;
	pelsocsomag = NULL;
	csomagbanbetelt = 0;

	#ifndef WATCOM
		// Fel lehet emelni 64000-re:
		if( long(Szakaszmaxszam) * sizeof( szakasz ) > 64000l )
			hiba( "szakaszok::szakaszok-ban long(Szakaszmaxszam) * sizeof( szakasz ) > 64000l!" );
	#endif

	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );

	char sor[200];
	// Verzio beolvasasa:
	if( fgets( sor, 190, h ) == NULL ) {
		hiba( "Nem tudott olvasni txt file-bol!" );
		return;
	}
	int verzio = 0;
	char verstring[200];
	if( sscanf( sor, "%s %d\n", verstring, &verzio ) != 2 ) {
		hiba( "Nem tudta feldolgozni sort!: ", sor );
		return;
	}
	if( verzio < 3 || verzio > 5 ) {
		hiba( "Verzio nem stimmel!: ", sor );
		return;
	}
	if( strcmp( verstring, "Verzio:" ) != 0 ) {
		hiba( "Nem tudta beolvasni verziot!: ", sor );
		return;
	}

	// Elemszam beolvasasa:
	if( fgets( sor, 190, h ) == NULL ) {
		hiba( "Nem tudott olvasni txt file-bol!" );
		return;
	}
	if( sscanf( sor, "%d\n", &szam ) != 1 ) {
		hiba( "Nem tudta feldolgozni sort!: ", sor );
		return;
	}

	if( szam >= Szakaszmaxszam )
		hiba( "Nincs eleg hely szakaszoknak!" );

	for( i = 0; i < szam; i++ ) {
		if( fgets( sor, 190, h ) == NULL ) {
			hiba( "Nem tudott olvasni txt file-bol!" );
			return;
		}

		long lx = 0;
		long ly = 0;
		long ldx = 0;
		long ldy = 0;
		if( verzio <= 1 )
			hiba( "Verzio kisebb 2!" );
		int balfold = 1;
		if( verzio < 5 ) {
			if( sscanf( sor, "%d %ld %ld %ld %ld", &balfold,
					&lx, &ly, &ldx, &ldy ) != 5 ) {
				hiba( "Nem tudta feldolgozni sort!: ", sor );
				return;
			}
		}
		else {
			if( sscanf( sor, "%ld %ld %ld %ld",
					&lx, &ly, &ldx, &ldy ) != 4 ) {
				hiba( "Nem tudta feldolgozni sort!: ", sor );
				return;
			}
		}
		double szorzo = 1.0/100.0;
		if( verzio > 2 )
			szorzo = 1.0/100000.0;
		if( verzio <= 3 ) {
			tomb[i].vv.r.x = lx*szorzo;
			tomb[i].vv.r.y = ly*szorzo;
			tomb[i].vv.v.x = ldx*szorzo;
			tomb[i].vv.v.y = ldy*szorzo;
		}
		else {
			tomb[i].vv.r.x = lx*szorzo;
			tomb[i].vv.r.y = ly*szorzo;
			tomb[i].vv.v.x = ldx*szorzo - tomb[i].vv.r.x;
			tomb[i].vv.v.y = ldy*szorzo - tomb[i].vv.r.y;
		}
		if( !balfold ) {
			tomb[i].vv.r = tomb[i].vv.r + tomb[i].vv.v;
			tomb[i].vv.v = Vekt2null - tomb[i].vv.v;
		}

		// Targy beolvasasa:
		if( targyas ) {
			{ tomb[i].ptargy = NEW(targy*) targy; }
			loadtargy( h, tomb[i].ptargy );
		}
	}
}

szakaszok::szakaszok( topol* ptop, int targyas ) {
	// Elejen ugyanaz, mint void konstruktor:
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vekt2( 0, 0 );
	nextnode = NULL;
	pelsocsomag = NULL;
	csomagbanbetelt = 0;

	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );

	if( targyas ) {
		for( i = 0; i < MAXKEREK; i++ ) {
			kerek* pker = ptop->kerektomb[i];
			if( !pker )
				continue;
			if( szam >= Szakaszmaxszam )
				hiba( "szakaszok::szakaszok-ban szam >= Szakaszmaxszam (ker)!" );
			tomb[szam].vv.r = pker->r;
			tomb[szam].vv.r.y = -tomb[szam].vv.r.y; // fejjel lefele
			tomb[szam].vv.v = vekt2( 0.0, 0.0 );

			{ tomb[szam].ptargy = NEW(targy*) targy; }
			if( !tomb[szam].ptargy )
				hiba( "Nincs eleg memoria szakaszok::szakaszok-ban ptargynak!" );

			if( pker->tipus < T_CEL || pker->tipus > T_KEZDO )
				hiba( "szakaszok::szakaszok-ban pker->tipus < T_CEL || pker->tipus > T_KEZDO!" );

			tomb[szam].ptargy->mozgo = 0;
			tomb[szam].ptargy->szam = pker->tipus-T_CEL;
			tomb[szam].ptargy->sebesseg = 0.0;
			tomb[szam].ptargy->fazis = 0.0;
			tomb[szam].ptargy->odavissza = 0;
			tomb[szam].ptargy->aktiv = 0;

			szam++;
		}
	}
	else {
		// Normal szakaszok beolvasasa:
		for( i = 0; i < MAXGYURU; i++ ) {
			gyuru* pgy = ptop->ptomb[i];
			if( !pgy )
				continue;
			for( int j = 0; j < pgy->pontszam; j++ ) {
				if( szam >= Szakaszmaxszam )
					hiba( "szakaszok::szakaszok-ban z >= Szakaszmaxszam!" );

				vekt2 r1, r2;
				if( j < pgy->pontszam - 1 ) {
					r1 = pgy->ponttomb[j];
					r2 = pgy->ponttomb[j+1];
				}
				else {
					r1 = pgy->ponttomb[j];
					r2 = pgy->ponttomb[0];
				}
				tomb[szam].vv.r = r1;
				tomb[szam].vv.v = r2-r1;

				tomb[szam].vv.r.y = -tomb[szam].vv.r.y; // fejjel lefele
				tomb[szam].vv.v.y = -tomb[szam].vv.v.y; // fejjel lefele

				szam++;
			}
		}
	}
}

szakaszok::~szakaszok( void ) {
	if( tomb ) {
		for( int i = 0; i < szam; i++ ) {
			if( tomb[i].ptargy )
				DELETE tomb[i].ptargy; }
		}
		DELETE tomb; }
	}
	if( tertomb ) {
		DELETE tertomb; }
	}
	deleteallnodes();
}

void szakaszok::save( FILE* h, int targyas ) {
	fprintf( h, "Verzio: %d\n", int(5) );
	fprintf( h, "%d\n", szam );
	for( int i = 0; i < szam; i++ ) {
		double szorzo = 100000.0;
		long lx1 = tomb[i].vv.r.x*szorzo;
		long ly1 = tomb[i].vv.r.y*szorzo;
		long lx2 = (tomb[i].vv.r.x+tomb[i].vv.v.x)*szorzo;
		long ly2 = (tomb[i].vv.r.y+tomb[i].vv.v.y)*szorzo;
		fprintf( h, "%ld %ld %ld %ld\n",
			lx1, ly1, lx2, ly2 );
		if( targyas ) {
			savetargy( h, tomb[i].ptargy );
		}
		else {
			if( tomb[i].ptargy )
				hiba( "szakaszok::save-ben !targyas, megis van benne targy!" );
		}
	}
}

/*void szakaszok::add( vekt2 r, vekt2 v, int targyas ) {
	if( szam >= maxszam ) {
		hiba( "Nincs eleg hely uj szakasz hozzaadasahoz!" );
		return;
	}
	tomb[szam].vv.r = r;
	tomb[szam].vv.v = v;
	if( targyas ) {
		{ tomb[szam].ptargy = NEW(targy*) targy; }
		tomb[szam].ptargy->mozgo = 0;
		tomb[szam].ptargy->szam = 0;
		tomb[szam].ptargy->sebesseg = 0.0;
		tomb[szam].ptargy->fazis = 0.0;
		tomb[szam].ptargy->odavissza = 1;
	}
	else
		tomb[szam].ptargy = NULL;
	szam++;
} */

/*void szakaszok::remove( szakasz* psz ) {
	for( int i = 0; i < szam; i++ ) {
		if( &tomb[i] == psz ) {
			if( tomb[i].ptargy )
				DELETE tomb[i].ptargy; }
			for( int j = i; j < szam-1; j++ )
				tomb[j] = tomb[j+1];
			szam--;
			return;
		}
	}
	hiba( "szakaszok::remove-ban nem talalja torlendo szakaszt!" );
} */

// NODE-kezeles:

/*#define CSOMAGBANNODESZAM (500)
struct soknodecsomag {
	vonalnode nodetomb[CSOMAGBANNODESZAM];
	soknodecsomag* nextcsomag;
};
soknodecsomag* pelsocsomag;
int csomagbanbetelt;
*/

vonalnode* szakaszok::getnewnode( void ) {
	if( !pelsocsomag ) {
		{ pelsocsomag = NEW(soknodecsomag*) soknodecsomag; }
		if( !pelsocsomag )
			uzenet( "memory!" );
		pelsocsomag->nextcsomag = NULL;
		csomagbanbetelt = 0;
	}
	soknodecsomag* curcsomag = pelsocsomag;
	while( curcsomag->nextcsomag )
		curcsomag = curcsomag->nextcsomag;
	if( csomagbanbetelt == CSOMAGBANNODESZAM ) {
		{ curcsomag->nextcsomag = NEW(soknodecsomag*) soknodecsomag; }
		curcsomag = curcsomag->nextcsomag;
		if( !curcsomag )
			uzenet( "memory!" );
		curcsomag->nextcsomag = NULL;
		csomagbanbetelt = 0;
	}
	csomagbanbetelt++;
	return &curcsomag->nodetomb[csomagbanbetelt-1];
}

void szakaszok::deleteallnodes( void ) {
	soknodecsomag* curcsomag = pelsocsomag;
	pelsocsomag = NULL;
	while( curcsomag ) {
		soknodecsomag* elozocsomag = curcsomag;
		curcsomag = curcsomag->nextcsomag;
		DELETE elozocsomag; }
	}
}



// Egy cellaba bekoti vonalat, ha meg nincs benne es rajta van tablan:
void szakaszok::bekot( int cellax, int cellay, vonal* pv ) {
	#ifdef TEST
		if( cellax < 0 || cellay < 0 )
			hiba( "cellax < 0 || cellay < 0!" );
	#endif
	if( cellax >= xdim || cellay >= ydim )
		return;
	vonalnode* pujnode = getnewnode();
	pujnode->pnext = NULL;
	pujnode->pvonal = pv;
	vonalnode* pn = tertomb[xdim*cellay+cellax];
	if( !pn ) {
		tertomb[xdim*cellay+cellax] = pujnode;
		return;
	}
	while( pn->pnext ) {
		pn = pn->pnext;
	}
	// Most pn az utolso node-ra mutat!
	pn->pnext = pujnode;
}

// Egy vonalat hozzarendel osszes olyan cellahoz, amihez koze lehet:
void szakaszok::beindexelvonalat( vonal* pv, double maxtav ) {
	// Kitolti vonal egyseg es hossz rubrikait:
	pv->hossz = abs( pv->v );
	if( pv->hossz < 0.00000001 )
		hiba( "Vonal hossza tul kicsi!" );
	pv->egyseg = egys( pv->v );

	vekt2 v = pv->v;
	vekt2 r = pv->r - origo;

	v = v*(1/cellameret);
	r = r*(1/cellameret);
	maxtav *= 1.5/cellameret;
	// Ezentul cellameretet lehet 1.0-nak venni!
	int invertalt = 0;
	if( fabs( v.y ) > fabs( v.x ) ) {
		invertalt = 1;
		double tmpd = v.x;
		v.x = v.y;
		v.y = tmpd;
		tmpd = r.x;
		r.x = r.y;
		r.y = tmpd;
	}
	// Most x iranyban biztosan hosszabb vagy egyenlo y iranynal!
	if( v.x < 0 ) {
		// Vegpont csere:
		r = r+v;
		v = Vekt2null-v;
	}
	// Most v x-pozitiv iranyba mutat:
	// Kiszamoljuk y = m*x + y0 fuggveny parametereit:
	double m = v.y/v.x;
	double y0 = r.y-m*r.x;
	double xkezdo = r.x-maxtav;
	int cellax = 0;
	if( xkezdo > 0 )
		cellax = xkezdo;
	if( r.x+v.x+maxtav < 0 )
		hiba( "szakaszok::beindexelvonalat-ban r.x+v.x+maxtav < 0!" );
	int ucsocellax = r.x+v.x+maxtav;
	while( cellax <= ucsocellax ) {
		double y1 = m*cellax + y0;
		double y2 = m*(cellax+1) + y0;
		// y1-nek kissebnek kell lennie y2-nel:
		if( y1 > y2 ) {
			double tmpd = y1;
			y1 = y2;
			y2 = tmpd;
		}
		y1 -= maxtav;
		y2 += maxtav;
		#ifdef TEST
			if( y1 > y2 )
				hiba( "Bug!" );
		#endif
		if( y2 < 0.0 )
			hiba( "Bug!" );
		int cellay = 0;
		if( y1 > 0 )
			cellay = y1;
		int ucsocellay = y2;
		while( cellay <= ucsocellay ) {
			if( invertalt )
				bekot( cellay, cellax, pv );
			else
				bekot( cellax, cellay, pv );

			cellay++;
		}

		cellax++;
	}
}

void szakaszok::rendez( double maxtav ) {
	// Ellenorzes:
	if( szam <= 0 )
		hiba( "szakaszok::rendez-ben szam <= 0!" );
	if( tomb[0].ptargy )
		hiba( "szakaszok::rendez-ben tomb[0].ptargy igaz!" );
	if( tertomb )
		hiba( "szakaszok::rendez-ban nodetomb != NULL!" );

	#ifdef WATCOM
		cellameret = 1.0;
	#else
		cellameret = 2.0;
	#endif

	// Megkeresi palya kiterjedeset:
	felsorolasresetszak();
	szakasz* psz = getnextszak();
	if( !psz )
		hiba( "Nincs egyetlen szakasz sem definialva!" );
	double minx = psz->vv.r.x;
	double maxx = psz->vv.r.x;
	double miny = psz->vv.r.y;
	double maxy = psz->vv.r.y;
	while( psz ) {
		if( psz->vv.r.x < minx )
			minx = psz->vv.r.x;
		if( psz->vv.r.x > maxx )
			maxx = psz->vv.r.x;
		if( psz->vv.r.y < miny )
			miny = psz->vv.r.y;
		if( psz->vv.r.y > maxy )
			maxy = psz->vv.r.y;

		if( psz->vv.r.x+psz->vv.v.x < minx )
			minx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.x+psz->vv.v.x > maxx )
			maxx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.y+psz->vv.v.y < miny )
			miny = psz->vv.r.y+psz->vv.v.y;
		if( psz->vv.r.y+psz->vv.v.y > maxy )
			maxy = psz->vv.r.y+psz->vv.v.y;

		psz = getnextszak();
	}

	// Beallitja kezdopontot:
	origo = vekt2( minx, miny );
	double xsized = maxx-minx;
	double ysized = maxy-miny;

	// Kiszamolja dimenziot:
	xdim = xsized/cellameret+1;
	ydim = ysized/cellameret+1;
	if( xdim < 0 || ydim < 0 )
		hiba( "xdim < 0 || ydim < 0!" );
	#ifndef WATCOM
		// DOS es DOSX286:
		if( xdim > 100 || ydim > 100 )
			hiba( "xdim > 100 || ydim > 100!" );
	#else
		// WATCOM:
		if( xdim > 200 || ydim > 200 )
			hiba( "xdim > 200 || ydim > 200!" );
	#endif

	// Lefoglalja tombot:
	unsigned tombmeret = xdim * ydim;
	{ tertomb = NEW(pvonalnode*) pvonalnode[tombmeret]; }
	if( !tertomb )
		uzenet( "memory!" );
	int hatar = xdim*ydim;
	for( int i = 0; i < hatar; i++ )
		tertomb[i] = NULL;

	// Most vegigmegy szakaszokon es beilleszti oket:
	felsorolasresetszak();
	psz = getnextszak();
	while( psz ) {
		beindexelvonalat( &psz->vv, maxtav );
		psz = getnextszak();
	}
}

void szakaszok::felsorolasreset( vekt2 r ) {
	if( !tertomb )
		hiba( "szakaszok::felsorolasreset-ban !tertomb!" );
	r = (r-origo)*(1/cellameret);
	int cellax = 0;
	if( r.x > 0 )
		cellax = r.x;
	int cellay = 0;
	if( r.y > 0 )
		cellay = r.y;

	if( cellax > xdim )
		hiba( "szakaszok::felsorolasreset-ben cellax > xdim!" );
	if( cellax == xdim )
		cellax = xdim-1;
	if( cellay > ydim )
		hiba( "szakaszok::felsorolasreset-ben cellay > ydim!" );
	if( cellay == ydim )
		cellay = ydim-1;
	nextnode = tertomb[xdim*cellay+cellax];
}

vonal* szakaszok::getnext( void ) {
	if( !nextnode )
		return NULL;
	#ifdef TEST
		if( !nextnode->pvonal )
			hiba( "szakaszok::getnext-ben !nextnode->pvonal" );
	#endif
	vonal* pret = nextnode->pvonal;
	nextnode = nextnode->pnext;
	return pret;
}

// Szakaszos kereso:
void szakaszok::felsorolasresetszak( void ) {
	szakfuto = 0;
}

szakasz* szakaszok::getnextszak( void ) {
	if( szakfuto >= szam )
		return NULL;
	szakfuto++;
	return &tomb[szakfuto-1];
}


// Visszaadja kajak szamat!:
int szakaszok::setallaktiv( int targytipusszam, targytomb* ptt ) {
	int kajaszam = 0;
	int voltkezdo = 0;
	for( int i = 0; i < szam; i++ ) {
		if( !tomb[i].ptargy )
			hiba( "szakaszok::setallaktiv-ban nem ptargy!" );
		tomb[i].ptargy->aktiv = 1;
		if( tomb[i].ptargy->szam >= targytipusszam )
			hiba( "tomb[i].ptargy->szam >= targytipusszam!" );
		if( ptt->targytipusok[tomb[i].ptargy->szam].tipus == T_KAJA )
			kajaszam++;
		if( ptt->targytipusok[tomb[i].ptargy->szam].tipus == T_KEZDO ) {
			if( voltkezdo )
				hiba( "Palyan ket kezdo is van!" );
			voltkezdo = 1;
			tomb[i].ptargy->aktiv = 0;
			vekt2 diff = tomb[i].vv.r - Kor2.r;
			Kor1.r = Kor1.r + diff;
			Kor2.r = Kor2.r + diff;
			Kor4.r = Kor4.r + diff;
		}
	}
	if( !voltkezdo )
		hiba( "Palyan nincs kezdo!" );
	return kajaszam;
}

// Killereket veszi legelore, utan foodokat, majd vegul exit:
void szakaszok::killerekelore( targytomb* ptt ) {
	if( szam < 2 )
		hiba( "szakaszok::killerekelore-ben szam < 2!" );
	for( int j = 0; j < szam+4; j++ ) {
		for( int i = 0; i < szam-1; i++ ) {
			if( !tomb[i].ptargy )
				hiba( "szakaszok::setallaktiv-ban nem ptargy!" );
			int tipus1 = ptt->targytipusok[tomb[i].ptargy->szam].tipus;

			if( !tomb[i+1].ptargy )
				hiba( "szakaszok::setallaktiv-ban nem ptargy!" );
			int tipus2 = ptt->targytipusok[tomb[i+1].ptargy->szam].tipus;

			// Sullyozzuk tipusokat hogy sorrendiseget lehessen megallapitani:
			int tip1 = 10;
			if( tipus1 == T_HALALOS )
				tip1 = 1;
			if( tipus1 == T_KAJA )
				tip1 = 2;
			if( tipus1 == T_CEL )
				tip1 = 3;

			int tip2 = 10;
			if( tipus2 == T_HALALOS )
				tip2 = 1;
			if( tipus2 == T_KAJA )
				tip2 = 2;
			if( tipus2 == T_CEL )
				tip2 = 3;

			if( tip1 > tip2 ) {
				// Cserelnunk kell:
				szakasz tmp = tomb[i];
				tomb[i] = tomb[i+1];
				tomb[i+1] = tmp;
			}
		}
	}
}

szakasz* szakaszok::getptrszakasz( int sorszam ) {
	if( sorszam >= szam )
		hiba( "szakaszok::getptrszakasz-ban sorszam >= szam!" );
	return &tomb[sorszam];
}

int szakaszok::getsorszam( szakasz* pszakasz ) {
	for( int i = 0; i < szam; i++ ) {
		if( !tomb[i].ptargy )
			hiba( "szakaszok::getsorszam-ban nem ptargy!" );
		if( &tomb[i] == pszakasz )
			return i;
	}
	hiba( "getsorszam-ban nincs meg szakasz!" );
	return -1;
}

void loadszakaszok( char* nev, int kulso, szakaszok** ppszak,
						szakaszok** ppszakt ) {
	if( *ppszak || *ppszakt )
		hiba( "loadszakaszok-ban *ppszak || *ppszakt!" );

	FILE* h = NULL;
	if( kulso )
		h = fopen( nev, "rt" );
	else
		h = qopen( nev, "rt" );
	if( !h )
		hiba( "Nem tudta megnyitni file-t!: ", nev );

	// Eldontese hogy regi vagy uj tipusu file-e:
	int ujtipus = 0;
	char tmp[10] = "AAAAA";
	if( fread( tmp, 1, 4, h ) != 4 )
		hiba( "Nem tudott olvasni txt file-bol!" );
	if( strncmp( tmp, "POT", 3 ) == 0 )
		ujtipus = 1;

	if( kulso )
		fclose( h );
	else
		qclose( h );

	// Beolvasas:
	if( ujtipus ) {
		topol* ptop = new topol( nev, kulso );
		if( ptop->hibas )
			uzenet( "This level file has some topology errors.",
					"If you can load it to the editor you may be able to correct these errors.",
					nev );

		// Most megnezzuk hogy nincs-e tul sok pontja palyanak:
		if( kulso )
			ptop->checklevelforsarvari();

		{ *ppszak = NEW(szakaszok*) szakaszok( ptop, 0 ); }
		{ *ppszakt = NEW(szakaszok*) szakaszok( ptop, 1 ); }
		Ucsoloadszakasz_lezart = ptop->lezart;
		delete ptop;
	}
	else {
		if( kulso )
			hiba( "iopvwoiuvfriuo" );
		h = qopen( nev, "rt" );
		{ *ppszak = NEW(szakaszok*) szakaszok( h, 0 ); }
		{ *ppszakt = NEW(szakaszok*) szakaszok( h, 1 ); }
		qclose( h );
	}
}

void saveszakaszok( char* nev, szakaszok* pszak, szakaszok* pszakt ) {
	char tmp[30] = "..\\files\\";
	strcat( tmp, nev );
	FILE* h = fopen( tmp, "wt" );
	if( !h )
		hiba( "Nem tudta megnyitni file-t!: ", nev );
	pszak->save( h, 0 );
	pszakt->save( h, 1 );
	fclose( h );
}


