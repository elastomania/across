#include	"all.h"

int Vonalcsakeditbe = 0; // vonalrajzolashoz

int Savx = 107;
int Savy = 36;

static int Fxsize = 640;
static int Fysize = 480;

//static double Maxxmeret = 150.0;
static double Maxxmeret = 170.0;
static double Ratio = double((Fxsize-Savx-1))/(Fysize-Savy-1);
static double Maxymeret = Maxxmeret/Ratio;

static vekt2 Dw1( -10.0, -10.0 ); // Dw1 a savnelkuli kep bal-felso sarka
static vekt2 Dw2( -10.0+20.0*Ratio, 10.0 );

static double Nagyitas = 1.0; // (Fxsize-Savx-1)/(Dw2.x-Dw1.x), zoom allitja
static double Nagyitas_rec = 1.0; // Reciprok

double konvx_i2d( int x ) {
	return (x-Savx)*Nagyitas_rec+Dw1.x;
}

double konvy_i2d( int y ) {
	return (y-Savy)*Nagyitas_rec+Dw1.y;
}

int konvx_d2i( double x ) {
	double outx = (x-Dw1.x)*Nagyitas;
	return outx+0.5+Savx;
}

int konvy_d2i( double y ) {
	double outy = (y-Dw1.y)*Nagyitas;
	return outy+0.5+Savy;
}

vekt2 konv( int x, int y ) {
	return vekt2( konvx_i2d( x ), konvy_i2d( y ) );
}

int konvx( vekt2 v ) {
	return konvx_d2i( v.x );
}

int konvy( vekt2 v ) {
	return konvy_d2i( v.y );
}

static vekt2 Vonalkozep;
static double Vonalsugar = 1.0; // Egy kicsit nagyobb  tenyleges sugarnal

static double Xsizemin = 0.017;

// Minden zoom-ot ez allit:
void zoom( vekt2 kozep, double xsize ) {
	if( xsize < Xsizemin )
		xsize = Xsizemin;
	if( xsize > Maxxmeret )
		xsize = Maxxmeret;
	Dw1.x = kozep.x-xsize*0.5;
	Dw2.x = kozep.x+xsize*0.5;
	double ysize = xsize/Ratio;
	Dw1.y = kozep.y-ysize*0.5;
	Dw2.y = kozep.y+ysize*0.5;
	Nagyitas = (Fxsize-Savx-1)/(Dw2.x-Dw1.x);
	Nagyitas_rec = 1.0/Nagyitas;
	// Ha palya kilog, kicsit arrebbhelyezzuk:
	double x1, y1, x2, y2;
	Ptop->getminmax( &x1, &y1, &x2, &y2, 1 );
	double dx = 0.0;
	double dy = 0.0;
	if( Dw1.x < x2 - Maxxmeret )
		dx = (x2 - Maxxmeret) - Dw1.x;
	if( Dw2.x > x1 + Maxxmeret )
		dx = - ( Dw2.x - (x1 + Maxxmeret) );
	if( Dw1.y < y2 - Maxymeret )
		dy = (y2 - Maxymeret) - Dw1.y;
	if( Dw2.y > y1 + Maxymeret )
		dy = - ( Dw2.y - (y1 + Maxymeret) );
	Dw1.x += dx;
	Dw1.y += dy;
	Dw2.x += dx;
	Dw2.y += dy;
	// Beallitjuk meg vonalrajzolohoz szukseges paramokat:
	Vonalkozep = (Dw1+Dw2)*0.5;
	// Kicsit nagyobbra vesszuk:
	Vonalsugar = (Dw2.x-Dw1.x)*sqrt(2.0)/2.0 * 1.01;
}

void zoomout( void ) {
	zoom( (Dw1+Dw2)*0.5, fabs(Dw2.x-Dw1.x) * 1.5 );
}

static void zoomin_double( double x1, double y1, double x2, double y2 ) {
	if( x2 < x1 ) {
		double tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if( y2 < y1 ) {
		double tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	if( x2-x1 < 0.0000001 || y2-y1 < 0.0000001 )
		return;
	vekt2 kozep( (x2+x1)*0.5, (y2+y1)*0.5 );
	if( (x2-x1)/(y2-y1) > Ratio ) {
		// Vizszintesen hosszukasabb:
		zoom( kozep, x2-x1 );
	}
	else {
		// Fuggolegesen hosszukasabb:
		zoom( kozep, (y2-y1)*Ratio );
	}
}

void zoomin( int x1, int y1, int x2, int y2 ) {
	zoomin_double( konvx_i2d( x1 ), konvy_i2d( y1 ),
				   konvx_i2d( x2 ), konvy_i2d( y2 ) );
}

void kitolt( void ) {
	double x1, y1, x2, y2;
	Ptop->getminmax( &x1, &y1, &x2, &y2, 1 );
	double x0 = (x1+x2)*0.5;
	double xfels = (x2-x1)*0.5  *1.05;
	double y0 = (y1+y2)*0.5;
	double yfels = (y2-y1)*0.5  *1.05;
	x1 = x0-xfels;
	x2 = x0+xfels;
	y1 = y0-yfels;
	y2 = y0+yfels;
	zoomin_double( x1, y1, x2, y2 );
}

// VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL
// VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL
// VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL
// VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL VONAL


void vonalrajz( vekt2 p1, vekt2 p2, int szaggatott ) {
	if( abs( p2-p1 ) < 0.00000001 )
		return;
	// Eloszor is levagjuk p1-p2 szakaszt, ha kilog Vonalkozep es Vonalsugar
	// altal meghatarozott korbol:
	int p1kilog = 0;
	if( abs( p1-Vonalkozep ) > Vonalsugar * 0.999 )
		p1kilog = 1;
	int p2kilog = 0;
	if( abs( p2-Vonalkozep ) > Vonalsugar * 0.999 )
		p2kilog = 1;
	if( p1kilog && p2kilog ) {
		if( pontszakasztav( Vonalkozep, p1, p2-p1 ) / Vonalsugar > 0.998 )
			return;
	}
	// Most mar biztos hogy legalabb belelog korbe szakasz
	if( p1kilog ) {
		vekt2 ujp1;
		if( !egyeneskormetszes( p1, p2-p1, Vonalkozep, Vonalsugar, &ujp1 ) )
			hiba( "vonal-ban !egyeneskormetszes!" );
		p1 = ujp1;
	}
	if( p2kilog ) {
		vekt2 ujp2;
		if( !egyeneskormetszes( p2, p1-p2, Vonalkozep, Vonalsugar, &ujp2 ) )
			hiba( "vonal-ban !egyeneskormetszes!" );
		p2 = ujp2;
	}
	// Most mar minden pont jo kozel van
	// Most atkonvertalunk int-es koordinata rendszerbe, de
	// floating pointnal maradunk es meg 0.5-ot is csak vegen adjuk hozza:
	// (savot viszont mar le kell takarni):
	double x1 = (p1.x-Dw1.x)*Nagyitas + Savx;
	double y1 = (p1.y-Dw1.y)*Nagyitas + Savy;
	double x2 = (p2.x-Dw1.x)*Nagyitas + Savx;
	double y2 = (p2.y-Dw1.y)*Nagyitas + Savy;
	// Beallitjuk helyes sorrendet:
	/*if( x2 < x1 ) {
		double tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if( y2 < y1 ) {
		double tmp = y1;
		y1 = y2;
		y2 = tmp;
	} */
	if( fabs( x2-x1 ) > fabs( y2-y1 ) ) {
		// Vonal inkabb vizszintes:
		if( x2 == x1 )
			hiba( "vonal-ban x2 == x1!" );

		int forditott = 0;
		if( x2 < x1 ) {
			forditott = 1;
			double tmp = x1;
			x1 = x2;
			x2 = tmp;
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}

		// Egyenes parametereinek meghatarozasa:
		double m = (y2-y1)/(x2-x1);
		double y0 = y1-m*x1;

		int xkezd = x1+0.5;
		int xveg = x2+0.5;
		if( forditott )
			xkezd++;
		else
			xveg--;
		for( int x = xkezd; x <= xveg; x++ ) {
			if( szaggatott && x % 4 )
				continue;
			double yd = y0 + m*x;
			int y = yd+0.5;
			if( x >= Savx && y >= Savy && x < Fxsize && y < Fysize ) {
				unsigned char szin = Editbuffer->gpixel( x, y );
				szin += 128;
				Editbuffer->ppixel( x, y, szin );
				if( !Vonalcsakeditbe )
					Pscr8->ppixel( x, y, szin );
			}
		}
	}
	else {
		// Vonal inkabb fuggoleges:
		if( y2 == y1 )
			hiba( "vonal-ban y2 == y1!" );

		int forditott = 0;
		if( y2 < y1 ) {
			forditott = 1;
			double tmp = x1;
			x1 = x2;
			x2 = tmp;
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}

		// Egyenes parametereinek meghatarozasa:
		double m = (x2-x1)/(y2-y1);
		double x0 = x1-m*y1;

		int ykezd = y1+0.5;
		int yveg = y2+0.5;
		if( forditott )
			ykezd++;
		else
			yveg--;
		for( int y = y1+0.5; y <= yveg; y++ ) {
			if( szaggatott && y % 4 )
				continue;
			double xd = x0 + m*y;
			int x = xd+0.5;
			if( x >= Savx && y >= Savy && x < Fxsize && y < Fysize ) {
				unsigned char szin = Editbuffer->gpixel( x, y );
				szin += 128;
				Editbuffer->ppixel( x, y, szin );
				if( !Vonalcsakeditbe )
					Pscr8->ppixel( x, y, szin );
			}
		}
	}
}

double getzoomvalue( void ) {
	if( fabs(Dw2.x-Dw1.x) == 0.0 )
		hiba( "getzoomvalue-ban 0-val osztas!" );
	return Maxxmeret/fabs(Dw2.x-Dw1.x);
}

// Visszaadja kepernyon tiz pixel tavolsagat double leptekben:
double display_double_kozeltav( void ) {
	return fabs( konvx_i2d( 110 ) - konvx_i2d( 100 ) );
}

// Ezek csak debughoz kellettek (nincsen h-juk):
/*void saveallapot( void ) {
	FILE* h = fopen( "display.sta", "wb" );
	if( !h )
		hiba( "Nem nyilik display.sta!" );

	fwrite( &Dw1.x, 1, sizeof( Dw1.x ), h );
	fwrite( &Dw2.x, 1, sizeof( Dw2.x ), h );
	fwrite( &Dw1.y, 1, sizeof( Dw1.y ), h );
	fwrite( &Dw2.y, 1, sizeof( Dw2.y ), h );

	fwrite( &Nagyitas, 1, sizeof( Nagyitas ), h );
	fwrite( &Nagyitas_rec, 1, sizeof( Nagyitas_rec ), h );
	fwrite( &Vonalkozep, 1, sizeof( Vonalkozep ), h );
	fwrite( &Vonalsugar, 1, sizeof( Vonalsugar ), h );

	fclose( h );
}
void loadallapot( void ) {
	FILE* h = fopen( "display.sta", "rb" );
	if( !h )
		hiba( "Nem nyilik display.sta!" );

	fread( &Dw1.x, 1, sizeof( Dw1.x ), h );
	fread( &Dw2.x, 1, sizeof( Dw2.x ), h );
	fread( &Dw1.y, 1, sizeof( Dw1.y ), h );
	fread( &Dw2.y, 1, sizeof( Dw2.y ), h );

	fread( &Nagyitas, 1, sizeof( Nagyitas ), h );
	fread( &Nagyitas_rec, 1, sizeof( Nagyitas_rec ), h );
	fread( &Vonalkozep, 1, sizeof( Vonalkozep ), h );
	fread( &Vonalsugar, 1, sizeof( Vonalsugar ), h );

	fclose( h );
}*/

