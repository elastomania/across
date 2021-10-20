#include	"all.h"

vekt2 spritehely( szakasz* psz, targy* pt, double t ) {
	if( !pt->mozgo )
		return psz->vv.r+psz->vv.v;
	vekt2 r = psz->vv.r;
	vekt2 v = psz->vv.v;
	// Csere:
	r = r+v;
	v = Vekt2null-v;
	double fazis = t*pt->sebesseg+pt->fazis;
	// Kepezzuk fazis tort reszet:
	long l = fazis;
	fazis -= l;
	#ifdef TEST
		if( fazis > 1.01 || fazis < -0.01 )
			hiba( "spritehely-ben fazis out of range!" );
	#endif
	if( pt->odavissza ) {
		if( l%2 )
			return r+v*(1.0-fazis);
		else
			return r+v*fazis;
	}
	else {
		return r+v*fazis;
	}
}

int utkozikesprite( vekt2 r, double sugar, int fej, double t,
							targytomb* pttt ) {
	Pszakt->felsorolasresetszak();
	szakasz* psz = NULL;
	while( (psz = Pszakt->getnextszak()) != NULL ) {
		targy* pt = psz->ptargy;
		targytipus* ptt = &(pttt->targytipusok[psz->ptargy->szam]);
		#ifdef TEST
			if( !pt )
				hiba( "utkozikesprite-ban !psz->ptargy!" );
		#endif

		if( !pt->aktiv || (fej&&!ptt->fejerz) || (!fej&&!ptt->kerekerz) )
			continue;
		vekt2 diff = r-spritehely( psz, psz->ptargy, t );
		double maxtav = sugar+ptt->sugar;
		if( diff.x*diff.x+diff.y*diff.y < maxtav*maxtav )
			return Pszakt->getsorszam( psz );
	}
	return -1;
}


