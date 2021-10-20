#include	"all.h"

double Elszakadasisebhat, Belsosav, G;
double  Talppontegybeolvadasitav;

double Ugrassebesseg1, Ugrassebesseg2;
double Ugroturelem;
double Vegenvaras;

double Magassag = 2, Szelesseg = 0.1;

double Drsugar, Drtang, Sr;

vekt2 P_alap1, P_alap2, P_alap3, P_alap4, P_alap6;
double Fejsugar;

double Ketmaxsugar, Spritemaxsugar;

double Fekegyutthato;
double Kord2x, Kord2y, Kord4x, Kord4y;

// MCGA kirajzolasos:


// Aranyok mindig be vannak allitva play.cpp-ben:
double Arany = 100.0, Aranyx = Arany*320.0/640.0, Aranyy = Arany*200.0/480.0;

void initadatok( void ) {
	Fekegyutthato = 100.0;
	Kord2x = -0.1, Kord2y = -0.5, Kord4x = 0.5, Kord4y = -0.5;

	Elszakadasisebhat = 0.01;
	Belsosav = 0.02;
	G = 10.0;      // nehezsegi gyorsulas: a [m/s^2]
	Talppontegybeolvadasitav = 0.1;

	Ugrassebesseg1 = 5;
	Ugrassebesseg2 = 5;
	// Ugroturelem = 0.2;
	Ugroturelem = 0.4;
	Vegenvaras = 1.0;

	Drsugar = 10000.0; // rudban fuggoleges modulus: F/l [N/m]
	Drtang = 10000.0; // rudban vizszintes modulus: F/l [N/m]
	Sr = 1000.0;  // rudban mozgasi ellenallas: F/v [N*s/m]

	// Kor1:
	strcpy( Kor1.nev, "Test elso" );
	Kor1.alfa = 0.0;
	Kor1.omega = 0.0;
	Kor1.sugar = 0.3;
	Kor1.m = 200; // 100;
	// theta = m * r*r;
	Kor1.theta = 200.0*0.55*0.55;
	Kor1.r = vekt2( 2.75, 3.6 );
	Kor1.v = vekt2( 0, 0 );
	//Kor1.utoljaraleert = 0;

	// Kor2:
	strcpy( Kor2.nev, "Kerek elso" );
	Kor2.alfa = 0.0;
	Kor2.omega = 0.0;
	Kor2.sugar = 0.4;
	Kor2.m = 10;
	Kor2.theta = 0.32;
	Kor2.r = vekt2( 1.9, 3.0 );
	Kor2.v = vekt2( 0, 0 );
	//Kor2.utoljaraleert = 0;

	// Kor4:
	strcpy( Kor4.nev, "Kerek hatso" );
	Kor4.alfa = 0.0;
	Kor4.omega = 0.0;
	Kor4.sugar = 0.4;
	Kor4.m = 10;
	Kor4.theta = 0.32;
	Kor4.r = vekt2( 3.6, 3.0 );
	Kor4.v = vekt2( 0, 0 );
	//Kor4.utoljaraleert = 0;

	// MCGA kirajzolasos:
	P_alap1 = vekt2( 0.2, 0.05 );
	P_alap2 = vekt2( -0.6, -0.04 );
	P_alap3 = vekt2( -0.05, 0.4 );
	P_alap4 = vekt2( -0.3, -0.5 );
	P_alap6 = vekt2( 0.0, 0.6 );
	Fejsugar = 0.238; // 0.26;


	Spritemaxsugar = 0.5;
	Ketmaxsugar = Spritemaxsugar + Kor4.sugar;

	vekt2 vtmp = Kor2.r-Kor1.r;
	Kord2x = vtmp.x;
	Kord2y = vtmp.y;
	vtmp = Kor4.r-Kor1.r;
	Kord4x = vtmp.x;
	Kord4y = vtmp.y;

	Kor12Fejr();
}


/*static double atmeretez( void ) {
	FILE* h = fopen( "size.inf", "rt" );
	if( !h )
		return 1.0;
	long meret = 1000;
	if( fscanf( h, "%ld", &meret ) != 1 ) {
		fclose( h );
		return 1.0;
	}
	fclose( h );
	if( meret < 100 )
		meret = 100;
	if( meret > 10000 )
		meret = 10000;
	return (double)meret/1000.0;
} */


