#include	"all.h"

static void surlodasverseny( vekt2 fgumi, vekt2 sebesseg );

static double Maxsurlodas = 0;

static void szogigazit( double* pd ) {
	if( *pd < -K_pi )
		*pd += 2*K_pi;
	if( *pd > K_pi )
		*pd -= 2*K_pi;
}

/*static double Elmozdhatar1 = 10.4;
static double Elmozdhatar2 = 10.5;
static double Atmenet = Elmozdhatar2 - Elmozdhatar1;
static double Szorzo = 2.0;

static pic8* Ppic8 = NULL;
*/

// Ez a fv. visszaadja az erot elmozdulas fuggvenyeben:
static double fv_ero( double l ) {
	return l*Drsugar;
	/*if( !Ppic8 ) {
		{ Ppic8 = NEW(pic8*) pic8( 320, 2 ); }
		for( int i = 0; i < 320; i++ ) {
			Ppic8->ppixel( i, 0, 0 );
			Ppic8->ppixel( i, 1, 0 );
		}
	}
	if( l < Elmozdhatar1 )
		return l*Drsugar;

	if( l > Elmozdhatar2 ) {
		// Legnagyobb ero:
		int hatar = 100;
		blt8( Pscr8, Ppic8, hatar, 100 );
		return l*Drsugar;
	}
	int hatar = 200;
	blt8( Pscr8, Ppic8, hatar, 100 );
	return (l+(Szorzo-1.0)*(l-Elmozdhatar1)/(Atmenet))*Drsugar;
	*/
}

static void erokszamitasa( kor* pkor, vekt2 i1, vekt2 j1,
			double kordx, double kordy,
			vekt2* pFkerek, vekt2* pFtest, double* pMtest,
			double* pMkerek ) {
	vekt2 gumis = i1*kordx+j1*kordy;
	vekt2 gumisabsz = gumis + Kor1.r;

	// Gumi huzoero kiszamolasa:
	vekt2 gumi = gumisabsz-pkor->r;
	double Fsugar = 0;
	double Ftang = 0;
	if( gumi.x < -0.0001 || gumi.x > 0.0001 ||
				gumi.y < -0.0001 || gumi.y > 0.0001 ) {
		// Gumieronek szetbontasa ket komponensre:
		double rudhossz = abs( gumis );
		vekt2 rudegys = gumis*(1.0/rudhossz);
		vekt2 rudegysmer = forgatas90fokkal( rudegys );

		Fsugar = fv_ero(gumi*rudegys);   // eredetileg: *Drsugar;
		Ftang = fv_ero(gumi*rudegysmer); // eredetileg: *Drtang;

		*pFkerek = Fsugar*rudegys+Ftang*rudegysmer;

		*pFtest = Vekt2null - *pFkerek;
		*pMtest = -Ftang*rudhossz;
	}
	else {
		*pFkerek = vekt2();
		*pFtest = vekt2();
		*pMtest = 0;
	}
	// Most jon surlodas szamitasa:
	vekt2 koto = pkor->r-Kor1.r;
	double kotol = abs( koto );
	double reckotol = 1.0/kotol;
	vekt2 kotoe = koto*reckotol;
	vekt2 kotomer = forgatas90fokkal( koto );
	vekt2 kotoemer = forgatas90fokkal( kotoe );
	vekt2 korongrelv = (kotomer*Kor1.omega+Kor1.v)-pkor->v;
	double vlong = korongrelv*kotoe;
	double vtang = korongrelv*kotoemer;
	vekt2 Fklong = (vlong*Sr)*kotoe;
	vekt2 Fktang = (vtang*Sr)*kotoemer;

	// Most jon kerek forgatonyomateka:
	vekt2 Ftestnyom = kotoemer*(*pMkerek*reckotol);

	*pFkerek = *pFkerek + Fklong + Fktang - Ftestnyom;
	*pMtest += -(Fktang*kotomer);
	*pFtest = *pFtest - Fklong - Fktang + Ftestnyom;

	surlodasverseny( gumi, korongrelv );
	//surlodasverseny( Ftang, vtang );
}

static Voltfek = 0;
static double Dfek2 = 0, Dfek4 = 0;
//static double Loket = 6.0;
static double Loket = 12.0;
static double Omegavalt = 3.0;

static int Ugrasban1 = 1, Ugrasban2 = 1; // Ezeket nullazni kell minden kezdesnel!
static double Ugras1kezd = -1.0, Ugras2kezd = -1.0;
static double Kezdoomega1 = -1.0, Kezdoomega2 = -1.0;

void resetleptet( void ) {
	Ugrasban1 = 0;
	Ugrasban2 = 0;
}

void leptet( double most, double dt, int gaz, int fek,
									int ugrik1, int ugrik2 ) {
	Maxsurlodas = 0;

	vekt2 i1( cos( Kor1.alfa ), sin( Kor1.alfa ) );
	vekt2 j1 = forgatas90fokkal( i1 );

	// Fek, gaz:
	if( !Voltfek && fek ) {
		Dfek2 = Kor2.alfa-Kor1.alfa;
		Dfek4 = Kor4.alfa-Kor1.alfa;
	}
	Voltfek = fek;
	double Mkerek2 = 0;
	double Mkerek4 = 0;
	if( gaz ) {
		double tulporgesomega = 110.0;
		double gaznyomatek = 600.0;
		if( Hatra ) {
			if( Kor2.omega > -tulporgesomega )
				Mkerek2 = -gaznyomatek;
		}
		else {
			if( Kor4.omega < tulporgesomega )
				Mkerek4 = gaznyomatek;
		}
	}
	if( fek ) {
		double fekero = 1000.0;
		double surlodas = 100.0;

		double dalfa = Kor2.alfa-(Kor1.alfa+Dfek2);
		double domega = Kor2.omega-Kor1.omega;
		Mkerek2 = -fekero*dalfa-surlodas*domega;

		dalfa = Kor4.alfa-(Kor1.alfa+Dfek4);
		domega = Kor4.omega-Kor1.omega;
		Mkerek4 = -fekero*dalfa-surlodas*domega;
	}
	else {
		szogigazit( &Kor2.alfa );
		szogigazit( &Kor4.alfa );
	}

	int crcido = most*1000;

	vekt2 Fkerek2;
	vekt2 Ftest2;
	double Mtest2;
	erokszamitasa( &Kor2, i1, j1, Kord2x, Kord2y, &Fkerek2,
					&Ftest2, &Mtest2, &Mkerek2 );

	vekt2 Fkerek4;
	vekt2 Ftest4;
	double Mtest4;
	erokszamitasa( &Kor4, i1, j1, Kord4x, Kord4y, &Fkerek4,
					&Ftest4, &Mtest4, &Mkerek4 );

	// Ugras elintezese:
	// Eloszor ugras befejezese, ha kell:
	if( Ugrasban1 && (ugrik1 || ugrik2 ||
							most > Ugras1kezd + Ugroturelem*0.25) ) {
		Kor1.omega += Loket;
		if( Kor1.omega > Kezdoomega1 )
			Kor1.omega = Kezdoomega1;
		// Meg marado szogsebesseg valtozast is okozunk, ha nem forgatjuk tul:
		if( Kor1.omega > 0.0 ) {
			Kor1.omega -= Omegavalt;
			if( Kor1.omega < 0.0 )
				Kor1.omega = 0.0;
		}
		Ugrasban1 = 0;
		Kezdoomega1 = -1.0;
		Ugras1kezd = -1.0;
	}
	if( Ugrasban2 && (ugrik1 || ugrik2 ||
							most > Ugras2kezd + Ugroturelem*0.25) ) {
		Kor1.omega -= Loket;
		if( Kor1.omega < Kezdoomega2 )
			Kor1.omega = Kezdoomega2;
		// Meg marado szogsebesseg valtozast is okozunk, ha nem forgatjuk tul:
		if( Kor1.omega < 0.0 ) {
			Kor1.omega += Omegavalt;
			if( Kor1.omega > 0.0 )
				Kor1.omega = 0.0;
		}
		Ugrasban2 = 0;
		Kezdoomega2 = -1.0;
		Ugras2kezd = -1.0;
	}

	if( crcido % 511 == 103 )
		crccheck2();

	// Most ugras kezdes, ha kell:
	if( ugrik1 ) {
		// Csak azert van kikomentezve, mert egyszer elojott:
		//if( Ugrasban1 || Ugrasban2 )
		//	hiba( "Ugrasban!" );
		Ugrasban1 = 1;
		Kezdoomega1 = Kor1.omega;
		Ugras1kezd = most;
		Kor1.omega -= Loket;
	}
	if( ugrik2 ) {
		//if( Ugrasban1 || Ugrasban2 )
		//	hiba( "Ugrasban!" );
		Ugrasban2 = 1;
		Kezdoomega2 = Kor1.omega;
		Ugras2kezd = most;
		Kor1.omega += Loket;
	}

	beallit( &Kor1, Ftest2+Ftest4-Vekt2j*Kor1.m*G,
					Mtest2+Mtest4, dt, 0 );
	beallit( &Kor2, Fkerek2-Vekt2j*Kor2.m*G, Mkerek2, 	   dt, 1 );
	beallit( &Kor4, Fkerek4-Vekt2j*Kor4.m*G, Mkerek4, 	   dt, 1 );
	Kor12Fejr();
}

// Kor1-bol eloallitja Fejr-et!:
void Kor12Fejr( void ) {
	double tav = 1.02; // 1.05;
	double fejhatra = -0.1;
	if( Hatra )
		fejhatra = 0-fejhatra;
	Fejr = Kor1.r + vekt2( tav*cos( Kor1.alfa+K_pip2+fejhatra ),
						   tav*sin( Kor1.alfa+K_pip2+fejhatra ) );
}

// 0-meghalt, 2-semmi kulonos
int vizsgalat( double t, targytomb* ptt ) {
	vekt2 t1, t2;
	if( talppontkereses( Fejr, Fejsugar, &t1, &t2 ) )
		return 0;

	// Spriteokkal utkozesvizsgalat:
	int voltkaja = 1;
	while( voltkaja ) { // Ha nem volt kaja kilepunk, kulonben vegtelen ciklus
		voltkaja = 0;
		int sorszamtomb[3];
		sorszamtomb[0] = utkozikesprite( Kor2.r, Kor2.sugar, 0, t, ptt );
		sorszamtomb[1] = utkozikesprite( Kor4.r, Kor4.sugar, 0, t, ptt );
		sorszamtomb[2] = utkozikesprite( Fejr, Fejsugar, 1, t, ptt );
		for( int i = 0; i < 3; i++ ) {
			if( sorszamtomb[i] >= 0 ) {
				startwavegyujto( 0, 0.0, sorszamtomb[i] );
				szakasz* psz = Pszakt->getptrszakasz( sorszamtomb[i] );
				targy* ptargy = psz->ptargy;
				if( ptt->targytipusok[ptargy->szam].tipus == T_KAJA ) {
					ptargy->aktiv = 0;
					voltkaja = 1; // Hatha van meg tobb kaja is
				}
			}
		}
	}

	return 2;
}


static double Oszto = 1.0/1.0;

static void surlodasverseny( vekt2 fgumi_v, vekt2 sebesseg_v ) {
	vekt2 joirany( cos( Kor1.alfa-K_pip2 ), sin( Kor1.alfa-K_pip2 ) );
	double fgumi = joirany*fgumi_v;
	double sebesseg = joirany*sebesseg_v;
	if( fgumi <= 0 || sebesseg <= 0 )
		return;
	double ertek = fgumi*sebesseg*Oszto;
	if( ertek > Maxsurlodas ) {
		Maxsurlodas = ertek;
	}
}

void kiszamolsurlodast( double* pseb, double* pero ) {
	*pseb = Maxsurlodas;
	*pero = Maxsurlodas;
}




