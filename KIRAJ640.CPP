#include	"all.h"

vekt2 Fejr( 0, 0 );

static int Elso640 = 1;
static int Gmode640;

// 640-es modban nincs mit resetalni:
void resetkirajzol( void ) {
}

void setgraph( void ) {
	if( !EGAVGAjo )
		hiba( "set640-ban !EGAVGAjo!" );
	if( Vid640 )
		return;
	if( Vid320 )
		hiba( "setgraph-ban Vid320!" );
	if( Elso640 ) {
		Elso640 = 0;
		int gdriver = VGA;
		Gmode640 = VGAHI;
		initgraph( &gdriver, &Gmode640, "" );
		int errorcode = graphresult();
		if (errorcode != grOk) {
			printf( "Graphics error: %s\n", grapherrormsg( errorcode ) );
			exit( 0 );
		}
		Vid320 = 0; // Ez elvileg nem kell!
		Vid640 = 1;
		return;
	}
	setgraphmode( Gmode640 );
	Vid320 = 0; // Ez elvileg nem kell!
	Vid640 = 1;
}

void close640( void ) {
	restorecrtmode();
}

// v1 iranyabol v2 fele interpolal:
static vekt2 interpolal( vekt2 v1, vekt2 v2, double x ) {
	vekt2 vissza = v1+(v2-v1)*x;
	return vissza;
}

static void vonalki( vekt2 p1, vekt2 p2 ) {
	vonalki( p1.x, p1.y, p2.x, p2.y );
}

static int K1x = 200, K1y = 200, K2x = 200, K2y = 200,
		   K4x = 200, K4y = 200;
static double Alfa1 = 0, Alfa2 = 0, Alfa4 = 0;
static double X0 = 0, Y0 = 0;

static vekt2 P_0, P_1, P_2, P_3, P_4, P_5,
				P_6, P_7, P_8, P_9, P_10, P_11, P_12, P_k1, P_k2;

static double T = 0;

void kirajzol640( double t, targytomb* pttt, double ugrasnagysag ) {
	// Kiszamoljuk kepernyo bal also sarkat:
	vekt2 motorkozep = Kor1.r;
	double x0 = motorkozep.x-4.0;
	double y0 = motorkozep.y-2.4;

	int kelltestis = 0;
	if( kelltestis ) {
		setcolor( ColorBG );
		kiteljeskerek( K1x, K1y, Kor1.sugar*Arany, Alfa1 );
		setcolor( ColorFG );
		K1x = (Kor1.r.x-x0)*Arany;
		K1y = (Kor1.r.y-y0)*Arany;
		Alfa1 = Kor1.alfa;
		kiteljeskerek( K1x, K1y, Kor1.sugar*Arany, Alfa1 );
	}

	setcolor( ColorBG );
	kiteljeskerek( K2x, K2y, Kor2.sugar*Arany, Alfa2 );
	setcolor( ColorFG );
	K2x = (Kor2.r.x-x0)*Arany;
	K2y = (Kor2.r.y-y0)*Arany;
	Alfa2 = Kor2.alfa;
	kiteljeskerek( K2x, K2y, Kor2.sugar*Arany, Alfa2 );

	setcolor( ColorBG );
	kiteljeskerek( K4x, K4y, Kor4.sugar*Arany, Alfa4 );
	setcolor( ColorFG );
	K4x = (Kor4.r.x-x0)*Arany;
	K4y = (Kor4.r.y-y0)*Arany;
	Alfa4 = Kor4.alfa;
	kiteljeskerek( K4x, K4y, Kor4.sugar*Arany, Alfa4 );

	// Most jon motoros kirajzolasa:
	vekt2 sarok = vekt2(x0,y0);
	vekt2 p_k1 = (Kor2.r-sarok)*Arany;
	vekt2 p_k2 = (Kor4.r-sarok)*Arany;

	vekt2 kozep = (Kor1.r-sarok)*Arany;
	vekt2 jobbra = vekt2( cos( Kor1.alfa ), sin( Kor1.alfa ) )*Arany;
	vekt2 fel = forgatas90fokkal( jobbra );
	vekt2 p_1 = P_alap1.x*jobbra+P_alap1.y*fel+kozep;
	vekt2 p_2 = P_alap2.x*jobbra+P_alap2.y*fel+kozep;
	vekt2 p_3 = P_alap3.x*jobbra+P_alap3.y*fel+kozep;
	vekt2 p_4 = P_alap4.x*jobbra+P_alap4.y*fel+kozep;
	vekt2 p_6 = P_alap6.x*jobbra+P_alap6.y*fel+kozep;

	vekt2 p_0 = interpolal( p_2, p_1, 1.4 );
	vekt2 p_7 = interpolal( p_2, p_1, 2.2 );
	vekt2 p_8 = interpolal( p_k1, p_2, 1.3 );
	vekt2 p_9 = interpolal( p_4, p_k2, 0.4 );
	vekt2 p_10 = interpolal( p_2, p_9, 0.5 );
	vekt2 p_11 = interpolal( p_9, p_k2, 0.3 );
	vekt2 p_12 = interpolal( p_7, p_9, 1.08 );

	setcolor( ColorBG ); vonalki( P_12, P_9 );
	setcolor( ColorFG ); vonalki( p_12, p_9 );

	setcolor( ColorBG ); vonalki( P_4, P_k2 );
	setcolor( ColorFG ); vonalki( p_4, p_k2 );

	setcolor( ColorBG ); vonalki( P_2, P_4 );
	setcolor( ColorFG ); vonalki( p_2, p_4 );

	setcolor( ColorBG ); vonalki( P_10, P_9 );
	if( !Hatra )
		setcolor( ColorFG ); vonalki( p_10, p_9 );

	setcolor( ColorBG ); vonalki( P_10, P_1 );
	setcolor( ColorFG ); vonalki( p_10, p_1 );

	setcolor( ColorBG ); vonalki( P_11, P_0 );
	setcolor( ColorFG ); vonalki( p_11, p_0 );

	setcolor( ColorBG ); vonalki( P_k1, P_8 );
	setcolor( ColorFG ); vonalki( p_k1, p_8 );

	setcolor( ColorBG ); vonalki( P_2, P_7 );
	setcolor( ColorFG ); vonalki( p_2, p_7 );

	setcolor( ColorBG ); vonalki( P_1, P_6 );
	setcolor( ColorFG ); vonalki( p_1, p_6 );

	setcolor( ColorBG ); vonalki( P_8, P_3 );
	setcolor( ColorFG ); vonalki( p_8, p_3 );

	setcolor( ColorBG ); vonalki( P_3, P_6 );
	setcolor( ColorFG ); vonalki( p_3, p_6 );

	vekt2 p_5 = (Fejr-sarok)*Arany;

	setcolor( ColorBG ); kiteljeskerek( P_5.x, P_5.y, Fejsugar*Arany );
	setcolor( ColorFG ); kiteljeskerek( p_5.x, p_5.y, Fejsugar*Arany );

	//Fejr = p_5*(1.0/Arany)+sarok;

	// Palya kirajzolasa:
	Pszak->felsorolasresetszak();
	szakasz* pszak;
	while( (pszak=Pszak->getnextszak()) != NULL ) {
		vonal* pv = &pszak->vv;
		setcolor( ColorBG );
		vonalki( (pv->r.x-X0)*Arany, (pv->r.y-Y0)*Arany,
					(pv->r.x+pv->v.x-X0)*Arany,
					(pv->r.y+pv->v.y-Y0)*Arany );
		setcolor( ColorFG );
		vonalki( (pv->r.x-x0)*Arany, (pv->r.y-y0)*Arany,
					(pv->r.x+pv->v.x-x0)*Arany,
					(pv->r.y+pv->v.y-y0)*Arany );
	}
	// Hordok kirajzolasa:
	Phordok->resethordok();
	hordo* phordo = Phordok->getnexthordo();
	while( phordo ) {
		for( int i = 0; i < phordo->vonalszam; i++ ) {
			vonal* pv = &phordo->vonaltomb[i];
			setcolor( ColorBG );
			vonalki( (pv->r.x-X0)*Arany, (pv->r.y-Y0)*Arany,
						(pv->r.x+pv->v.x-X0)*Arany,
						(pv->r.y+pv->v.y-Y0)*Arany );
			setcolor( ColorFG );
			vonalki( (pv->r.x-x0)*Arany, (pv->r.y-y0)*Arany,
						(pv->r.x+pv->v.x-x0)*Arany,
						(pv->r.y+pv->v.y-y0)*Arany );
		}

		phordo = Phordok->getnexthordo();
	}

	// Spriteok kirajzolasa:
	Pszakt->felsorolasresetszak();
	szakasz* psz;
	while( (psz=Pszakt->getnextszak()) != NULL ) {
		targy* pt = psz->ptargy;
		targytipus* ptt = &pttt->targytipusok[pt->szam];
		setcolor( ColorBG );
		vekt2 r = spritehely( psz, pt, T );
		kiteljeskerek( (r.x-X0)*Arany, (r.y-Y0)*Arany, ptt->sugar*Arany );
		setcolor( ColorFG );
		if( !pt->aktiv )
			continue;
		r = spritehely( psz, pt, t );
		kiteljeskerek( (r.x-x0)*Arany, (r.y-y0)*Arany, ptt->sugar*Arany );
	}
	T = t;

	X0 = x0;
	Y0 = y0;
	P_k1 = p_k1;
	P_k2 = p_k2;
	P_0 = p_0;
	P_1 = p_1;
	P_2 = p_2;
	P_3 = p_3;
	P_4 = p_4;
	P_5 = p_5;
	P_6 = p_6;
	P_7 = p_7;
	P_8 = p_8;
	P_9 = p_9;
	P_10 = p_10;
	P_11 = p_11;
	P_12 = p_12;

	int ugrasy = 5;
	int ugrassize = 200;
	setcolor( ColorBG );
	line( 320-ugrassize, ugrasy, 320+ugrassize, ugrasy );
	if( ugrasnagysag > 0.0001 ) {
		setcolor( ColorFG );
		line( 320-ugrassize*ugrasnagysag, ugrasy,
					320+ugrassize*ugrasnagysag, ugrasy );
	}
}


