
// #define TEST eseten plusz teszteket vegez!

class vekt2 {
public:
	double x, y;
	vekt2( void );
	vekt2( double, double );
	vekt2 operator + ( vekt2 );
	vekt2 operator - ( vekt2 );
	double operator * ( vekt2 );
	double operator % ( vekt2 ); // Vektorialis szorzas.
	void normal( void );
	void forgatas( double alfa );
};

vekt2 operator * ( double, vekt2 );
vekt2 operator * ( vekt2, double );
double abs( vekt2 );
vekt2 egys( vekt2 );
vekt2 forgatas90fokkal( vekt2 );
vekt2 forgatasminusz90fokkal( vekt2 );

// Sajat negyzetgyok:
double gyok( double x );

// KULONBOZO SZAMITASOK:

// Az r1-bol indulo v1 vektor altal es az r2-bol indulo v2 altal
// meghatarozott ket egyenes metszespontjanak visszaadasa:
// Parhuzamosok eseten hiba( char* ); fv.-t hivja meg.
vekt2 crossingpoint( vekt2 r1, vekt2 v1, vekt2 r2, vekt2 v2 );
// Pont szakasztol valo tavolsagat szamolja ki:
// (Szakasz hossza vsz hossza).
double pontszakasztav( vekt2 rpont, vekt2 rsz, vekt2 vsz );
// Pont egyenestol valo tavolsagat szamolja ki:
double pontegyenestav( vekt2 rpont, vekt2 rsz, vekt2 vsz );
// Visszaadja ket kor metszetet, r1-bol r2-be bal oldalon (Ganyolt):
vekt2 ketkormetszete( vekt2 r1, vekt2 r2, double l1, double l2 );

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
int vektormetszes( vekt2 r1, vekt2 v1, vekt2 r2, vekt2 v2 );
// Ez csak akkor tekinti metszesnek, ha nem csak eppen erintik egymast:
int vektormetszes2( vekt2 r1, vekt2 v1, vekt2 r2, vekt2 v2 );
// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
int vektormetszes3( vekt2 r1, vekt2 v1, vekt2 r2, vekt2 v2 );

// Igazzal ter vissza, ha metszi kort egyenes:
// Ket metszespont kozul a v iranyaba haladva az elsot adja vissza:
int egyeneskormetszes( vekt2 r, vekt2 v, vekt2 r_kor, double sugar, vekt2* pmetszet );

extern vekt2 Vekt2i, Vekt2j, Vekt2k, Vekt2null;
extern double Pi;

// Ezt meg kell irni hozza:
// Uzenetet kuld peldaul 0-val valo osztas eseten (inverz matrix).
//void hiba( char* message, char* text2 = NULL );
