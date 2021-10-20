#include	"all.h"

kor Kor1, Kor2, Kor3, Kor4;

szakaszok* Pszak = NULL;
szakaszok* Pszakt = NULL;

int Kajaszam, Kajakell;

int Legkozment = 0; // Ezt 1-be allitjuk ha kell egy snapshot

static double Forgasido = 0.35;
//static double Forgasido = 3;

// 0-meghalt, 1-megnyerte, 2-semmi kulonos:
static int spritefeldolgoz( int sorszam, targytomb* ptt ) {
	szakasz* psz = Pszakt->getptrszakasz( sorszam );
	targy* ptargy = psz->ptargy;
	if( ptt->targytipusok[ptargy->szam].tipus == T_HALALOS )
		return 0;
	if( ptt->targytipusok[ptargy->szam].tipus == T_KAJA ) {
		ptargy->aktiv = 0;
		Kajaszam++;
		startwavegyujto( WAV_EVES, 0.99, -1 );
		return 2;
	}
	if( ptt->targytipusok[ptargy->szam].tipus == T_CEL ) {
		if( Kajaszam >= Kajakell )
			return 1;
	}
	return 2;
}

int Hatra = 0;

/*void kitesz( int igen ) {
	if( igen )
		setcolor( ColorFG );
	else
		setcolor( ColorBG );
	line( 10, 10, 100, 10 );
} */

static int Lenyescape = MK_ESC;
static int Lenygaz = MK_UP;
static int Lenyfek = MK_DOWN;
static int Lenyugras1 = MK_RIGHT;
static int Lenyugras2 = MK_LEFT;
static int Lenyspace = ' ';
static int Lenyview = MK_STATE_V;
static int Lenytimeki = MK_STATE_T;
static int Lenyhang = MK_STATE_S;
static int Lenysnap = MK_STATE_I;

long L = 0;

static int Viewplay = 1;
static int Viewreplay = 0;
static int Timekiplay = 1;
static int Timekireplay = 0;
static int Kellhang = 1;

static double Fordido = 0.5;
static double baljobbszamol( double eltelt ) {
	#ifdef TEST
		if( eltelt < 0 )
			hiba( "baljobbszamol-ban eltelt < 0!" );
	#endif
	if( Hatra ) {
		if( eltelt > Fordido )
			return 0.0;
		return 1.0-eltelt/Fordido;
	}
	else {
		if( eltelt > Fordido )
			return 1.0;
		return eltelt/Fordido;
	}
}

// Akkor igaz, ha regi rec file-t jatszunk le:
// Global replay allitja:
int Kellobjectrendezni = 1;

// idot adja vissza szazadmasodpercben,
// replay eseten termeszetes befejezes eseten 0-t ad vissza:
long lejatszo( palyaleiro* pleiro, st320* pst,
										recorder* prec, int replay ) {
	int palmegnincs = 1;
	setresdd( State->res );

	initadatok();
	// Akkor igaz, ha regi rec file-t jatszunk le:
	// Global replay allitja:
	if( Kellobjectrendezni )
		Pszakt->killerekelore( pleiro->ptt );
	// setallaktiv allitja be motor kezdeti helyzetet is!:
	Kajakell = Pszakt->setallaktiv( pleiro->ptt->targytipusszam, pleiro->ptt );
	Kajaszam = 0;
	Kor12Fejr();


	pst->vxelozo = -11000;
	pst->vyelozo = -11000;

	L = 0;
	double utolsougras = -100.0;
	int utolsougras1volt = 0;
	int idoszerint = 1;
	double eddig = 0;
	hanginfo inf = { 1.0, 0 };
	resetwavegyujto();
	Hatra = 0;
	int hatranyomva = 0;
	int viewnyomva = 0;
	int snapnyomva = 0;
	int timekinyomva = 0;
	int hangnyomva = 0;
	mv_startstopper();
	int elsoalkalom = 1;
	Mute = !Kellhang;
	double ucsoford = -1000.0;
	double ucsoforgas = -1000.0;
	int eddighatra = 0;
	int meghalt = 0;
	long nyertido = 0;
	resetleptet();
	resetkirajzol();
	pleiro->pratesz->reset();
	#ifndef DOS
		setvolume( State->soundvolume );
	#endif
	startmotor();
	while( 1 ) {
		// Kiszamolja mennyit kell leptetni:
		double most = mv_stopperido();
		double cel = most*0.0024;

		// Ha tul sok kimaradt, akkor nem kell annyit behozni:
		// Ezt most elvileg stopper vegzi:
		//if( eddig < cel-1.0 )
		//	eddig = cel-1.0;

		/* Most nincs idoellenorzes:
		#ifdef WATCOM
			if( cel - eddig > 2.0*0.432 && idoszerint ) // Maximum 2 sec a turelem!
				uzenet( "System not fast enough!" );
		#else
			if( cel - eddig > 5.0*0.432 && idoszerint ) // Maximum 5 sec a turelem!
				uzenet( "System not fast enough!" );
		#endif
		*/
		while( !idoszerint || eddig < cel-0.000001 ) {
			mv_check();
			hang();
			double dt = 0.003;
			if( eddig+dt > cel )
				dt = cel-eddig;
			// Ugras elintezese:
			int ugrik1 = 0, ugrik2 = 0;
			if( eddig > utolsougras + Ugroturelem && !replay ) {
				if( ( (mk_getstate( Lenyugras1 ) || mk_getstate( MK_STATE_M )) && Sajatugras) ||
					( (mk_getstate( Lenyugras2 ) || mk_getstate( MK_STATE_N )) && !Sajatugras) ) {
					ugrik1 = 1;
					utolsougras = eddig;
					utolsougras1volt = 1;
					startwavegyujto( WAV_UGRAS1, 0.99, -1 );
				}
				if( ( (mk_getstate( Lenyugras2 ) || mk_getstate( MK_STATE_N )) && Sajatugras) ||
					( (mk_getstate( Lenyugras1 ) || mk_getstate( MK_STATE_M )) && !Sajatugras) ) {
					ugrik2 = 1;
					utolsougras = eddig;
					utolsougras1volt = 0;
					startwavegyujto( WAV_UGRAS2, 0.99, -1 );
				}
			}

			// LEPTET!!!!!!!:
			// 0-meghalt, 1-megnyerte, 2-semmi kulonos
			if( replay ) {
				// recall koordokat es Hatra-t is beallitja:
				if( !prec->recall( eddig, &inf ) ) {
					// Motor hangjat kikapcsolja:
					setmotor( 1.0, 0.0001 );
					setsurlodas( 0 );
					hangosdelay( Vegenvaras*400.0 );
					while( vanmegwav() )
						hang();
					Mute = 1;
					nullazhangot();
					return 0;
				}
				Kor12Fejr();
			}
			else {
				leptet(
					eddig,
					dt,
					mk_getstate( Lenygaz ) || mk_getstate( 'a' ),
					mk_getstate( Lenyfek ) || mk_getstate( 'z' ),
					ugrik1,
					ugrik2 );
				if( prec )
					prec->store( eddig, &inf );
				int eredmeny = vizsgalat( eddig, pleiro->ptt );
				if( eredmeny == 0 && !elsoalkalom ) {
					meghalt = 1;
					eddig += dt;
					break;
				}
			}
			eddig += dt;
			if( !idoszerint )
				break;
		}
		// HANG ELINTEZES:
		if( replay ) {
			setmotor( inf.frekvencia, inf.gaz );
			setsurlodas( inf.surlero );
			char wavindex;
			double hangero;
			int objszam;
			while( prec->gethang( eddig, &wavindex, &hangero, &objszam ) ) {
				if( objszam >= 0 ) {
					spritefeldolgoz( objszam, pleiro->ptt );
				}
				else {
					startwave( wavindex, hangero );
					if( wavindex == WAV_UGRAS1 ) {
						utolsougras1volt = 1;
						utolsougras = eddig;
					}
					if( wavindex == WAV_UGRAS2 ) {
						utolsougras1volt = 0;
						utolsougras = eddig;
					}
				}
			}
		}
		else {
			// Ez itt rendes play (nem replay):
			kiszamolsurlodast( &inf.surlseb, &inf.surlero );
			//kinyom( inf.surlero*320 );
			setsurlodas( inf.surlero );
			double frekvencia = 1.0+fabs(Kor4.omega)*0.025;
			if( mk_getstate( Lenygaz ) )
				setmotor( frekvencia, 1 );
			else
				setmotor( frekvencia, 0 );
			inf.frekvencia = frekvencia;
			inf.gaz = (char)mk_getstate( Lenygaz );
			int wavazonosito;
			double hangero;
			int objszam;
			while( getwavegyujto( &wavazonosito, &hangero, &objszam ) ) {
				if( objszam >= 0 ) {
					int eredmeny = spritefeldolgoz( objszam, pleiro->ptt );
					if( (eredmeny == 0 || eredmeny == 1) && !elsoalkalom ) {
						if( eredmeny == 0 )
							meghalt = 1;
						else
							nyertido = eddig*100.0/(182.0*0.0024);
					}
				}
				else {
					startwave( wavazonosito, hangero );
				}
				if( prec )
					prec->addhang( eddig, (char)wavazonosito, hangero, objszam );
			}
		}
		// Hatra elintezes:
		if( eddighatra != Hatra ) {
			ucsoforgas = eddig;
			double eltelt = eddig-ucsoford;
			if( eltelt < Fordido ) {
				ucsoford = eddig+eltelt-Fordido;
			}
			else {
				ucsoford = eddig;
			}
			eddighatra = Hatra;
			// Hang adas:
			if( !replay )
				startwavegyujto( WAV_FORDULAS, 0.99, -1 );
		}

		// KIRAJZOLAS:
		double ugrasnagysag = 1.0-(eddig-utolsougras)/Ugroturelem;
		if( ugrasnagysag < 0 )
			ugrasnagysag = 0;
		#ifdef TEST
			if( ugrasnagysag > 1.00001 )
				hiba( "ugrasnagysag > 1.00001!" );
		#endif
		for( int i = 0; i < 1+Otszoros*2; i++ ) {
			double forgas = (eddig-ucsoforgas)/Forgasido;
			#ifdef TEST
				if( forgas < 0.0 )
					hiba( "lejatszo-ban forgas < 0.0!" );
			#endif
			if( forgas > 1.0 )
				forgas = 1.0;
			if( replay ) {
				kirajzol320( pst, eddig, pleiro, Viewreplay, Timekireplay,
						baljobbszamol( eddig-ucsoford ), ugrasnagysag,
								utolsougras1volt, forgas );
			}
			else {
				kirajzol320( pst, eddig, pleiro, Viewplay, Timekiplay,
						baljobbszamol( eddig-ucsoford ), ugrasnagysag,
								utolsougras1volt, forgas );
			}
		}
		if( palmegnincs ) {
			palmegnincs = 0;
			pleiro->pal->set();
		}
		if( nyertido || meghalt ) {
			setmotor( 1.0, 0.0001 );
			setsurlodas( 0 );
			if( meghalt )
				startwave( WAV_TORES, 0.999 );
			else
				startwave( WAV_SIKER, 0.999 );
			hangosdelay( Vegenvaras*1000.0 );
			while( vanmegwav() )
				hang();
			Mute = 1;
			nullazhangot();
			if( meghalt )
				return -1;
			else
				return nyertido;
		}

		// Toggle:
		if( !hatranyomva && mk_getstate( Lenyspace ) && !replay )
			Hatra = !Hatra;
		hatranyomva = mk_getstate( Lenyspace );
		if( !viewnyomva && mk_getstate( Lenyview ) ) {
			if( replay )
				Viewreplay = !Viewreplay;
			else
				Viewplay = !Viewplay;
		}
		viewnyomva = mk_getstate( Lenyview );
		// Snap nyomas elintezese (ez csak buherkent nincs kikomentezve):
		if( Snapenabled && !snapnyomva && mk_getstate( Lenysnap ) )
			Legkozment = 1;
		snapnyomva = mk_getstate( Lenysnap );
		// Ido kint legyen vagy ne:
		if( !timekinyomva && mk_getstate( Lenytimeki ) ) {
			if( replay )
				Timekireplay = !Timekireplay;
			else
				Timekiplay = !Timekiplay;
		}
		timekinyomva = mk_getstate( Lenytimeki );
		if( !hangnyomva && mk_getstate( Lenyhang ) ) {
			Kellhang = !Kellhang;
			Mute = !Mute;
		}
		hangnyomva = mk_getstate( Lenyhang );

		// Vizsgalat kilepesre:
		if( mk_getstate( Lenyescape ) ) {
			double eltelido = mv_stopperido();
			// Motor hangjat kikapcsolja:
			Mute = 1;
			nullazhangot();
			// Kiirja framerate-et:
			double tpl = double(L)/eltelido;
			tpl *= 182;
			int framerate = tpl;
			if( access( "f_rate.inf", 0 ) == 0 ) {
				// igyhagyni fopen-t!:
				FILE* h = fopen( "f_rate.inf", "wt" );
				if( !h )
					hiba( "framerate nem nyilik!" );
				fprintf( h, "%d", framerate );
				fclose( h );
			}
			return -1;
		}
		L++;
		elsoalkalom = 0;
	}
}


