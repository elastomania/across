#include	"all.h"

static int gombszakasz( vekt2 r, double sugar, vonal* pv, vekt2* pt ) {
	vekt2 rel = r-pv->r;
	double pozicio = rel*pv->egyseg;
	if( pozicio < 0 ) {
		if( abs( r-pv->r ) < sugar ) {
			*pt = pv->r;
			return 1;
		}
		else {
			return 0;
		}
	}
	if( pozicio > pv->hossz ) {
		if( abs( r-(pv->r+pv->egyseg*pv->hossz) ) < sugar ) {
			*pt = pv->r+pv->egyseg*pv->hossz;
			return 1;
		}
		else {
			return 0;
		}
	}
	vekt2 n = forgatas90fokkal( pv->egyseg );
	double tav = rel*n;
	if( tav < -sugar || tav > sugar )
		return 0;
	*pt = pv->r+pv->egyseg*pozicio;
	return 1;
}

int talppontkereses( vekt2 r, double sugar, vekt2* pt1, vekt2* pt2 ) {
	Pszak->felsorolasreset( r );
	int talppontszam = 0;
	vonal* pv = NULL;
	//int vegignezett = 0; // tesztelesre!
	while( (pv = Pszak->getnext()) != NULL ) {
		//vegignezett++;
		vekt2 t;
		if( gombszakasz( r, sugar, pv, &t ) ) {
			if( talppontszam == 2 ) {
				hiba( "Egyszerre kettonel tobb talppontszam is van" );
			}
			if( talppontszam == 1 ) {
				*pt2 = t;
				talppontszam++;
				// Ellenorzes, hogy nincs-e tul kozel ketpont,
				// ha ket pontrol van szo:
				if( abs( *pt1-*pt2 ) < Talppontegybeolvadasitav ) {
					*pt1 = (*pt1+*pt2)*0.5; // atlagol
					talppontszam = 1;
				}
				else {
					// Kettonel tobb talppont mar ugy sem lehet!
					return talppontszam;
				}
			}
			if( talppontszam == 0 ) {
				*pt1 = t;
				talppontszam++;
			}
		}
	}
	//kinyom( vegignezett );
	return talppontszam;
}




