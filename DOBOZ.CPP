#include	"all.h"

// Affinitassal kapcsolatos dolgok:
int Affinitas = 0;
static double Affinnagysag = 1.0;
static vekt2 Affinkozep = Vekt2i;
static vekt2 Affinirany = Vekt2i;
static double Affinaranyx = 1.0;
static double Affinaranyy = 1.0;

static double Tulvizszintes = 0.005;

void egysor( unsigned char lyuk, short szam, unsigned char* sor,
		unsigned char* tomb, long futox, long futoy, long dx, long dy );

#ifdef WATCOM

#pragma aux	egysor = "push ebp" \
					 "mov ebp, esp" \
					 "sub esp, 100" \
					 "add ebp, 36" \
					 "xor eax, eax" \
					 "mov di, [ebp-20]" \
					 "mov si, [ebp-16]" \
					 "mov ch, [ebp-32]" \
					 "mov edx, [[ebp-28]" \
					 "mov al, [ebp-18]" \
					 "mov ah, [ebp-14]" \
					 "jmp kezd" \
					 "ciklus2:" \
					 "dec ch" \
					 "jz vege" \
					 "ciklus:" \
					 "inc edx" \
					 "add di, [ebp-12]" \
					 "adc al, [ebp-10]" \
					 "add si, [ebp-8]" \
					 "adc ah, [ebp-6]" \
					 "kezd:" \
					 "mov ebx, [[ebp-24]" \
					 "add ebx, eax" \
					 "mov bl, [ebx]" \
					 "cmp bl, cl" \
					 "jz ciklus2" \
					 "mov [edx], bl" \
					 "dec ch" \
					 "jnz ciklus" \
					 "vege:" \
					 "sub ebp, 36" \
					 "mov esp, ebp" \
					 "pop ebp" \
					 "add esp, 28" \
				  parm [cl] \
				  modify [eax ebx ecx edx esi edi]

//void proba( void ) {
//	unsigned char* pc = (unsigned char*)0x0feeeeeee;
//	egysor( 1, 2, pc, pc+1, 3, 4, 5, 6 );
//}

#else

void egysor( unsigned char lyuk, short szam, unsigned char* sor,
		unsigned char* tomb, long futox, long futoy, long dx, long dy ) {
	short* pf_futox = (short*)(&futox);
	pf_futox++;
	short* pf_futoy = (short*)(&futoy);
	pf_futoy++;
	for( short x = 0; x < szam; x++ ) {
		unsigned short fx = *pf_futox;
		unsigned short fy = *pf_futoy;
		fy = (fy << 8) + fx;

		unsigned char c = tomb[fy];
		if( c != lyuk )
			sor[x] = c;

		futox += dx;
		futoy += dy;
	}
}

#endif

// Ez a fuggveny kilogas eseten hivodik meg, plussz parametere xhossz:
void egysor( unsigned char lyuk, short szam, unsigned char* sor,
		unsigned char* tomb, long futox, long futoy, long dx, long dy,
		int xhossz ) {
	short* pf_futox = (short*)(&futox);
	pf_futox++;
	short* pf_futoy = (short*)(&futoy);
	pf_futoy++;
	for( short x = 0; x < szam; x++ ) {
		if( x >= xhossz )
			return;
		unsigned short fx = *pf_futox;
		unsigned short fy = *pf_futoy;
		fy = (fy << 8) + fx;

		unsigned char c = tomb[fy];
		if( c != lyuk )
			sor[x] = c;

		futox += dx;
		futoy += dy;
	}
}

void setaffinitas( vekt2 kozep, vekt2 irany, double nagysag,
			double aranyx, double aranyy ) {
	Affinkozep = kozep;
	irany.normal();
	Affinirany = irany;
	Affinnagysag = nagysag;
	Affinaranyx = aranyx;
	Affinaranyy = aranyy;
}

//static unsigned char Egysor[400];

void dobozki( pic8* ppic8, kiskep* pkiskep, vekt2 u, vekt2 v, vekt2 r ) {
	unsigned char lyuk = pkiskep->lyuk;

	// Affinitas:
	if( Affinitas ) {
		// Kicsit atalakitja vektorokat:
		// Visszaalakitjuk 1:1-es aspect-ratio-ra r, u, v-t:
		r.x /= Affinaranyx;
		r.y /= Affinaranyy;
		u.x /= Affinaranyx;
		u.y /= Affinaranyy;
		v.x /= Affinaranyx;
		v.y /= Affinaranyy;

		// szamitas:
		double tav = (r-Affinkozep)*Affinirany;
		vekt2 kivonando = (tav*(1.0-Affinnagysag))*Affinirany;
		r = r - kivonando;

		tav = u*Affinirany;
		kivonando = (tav*(1.0-Affinnagysag))*Affinirany;
		u = u - kivonando;

		tav = v*Affinirany;
		kivonando = (tav*(1.0-Affinnagysag))*Affinirany;
		v = v - kivonando;

		// Visszaalakitjuk aranyx:aranyy-os aspect-ratio-ra r, u, v-t:
		r.x *= Affinaranyx;
		r.y *= Affinaranyy;
		u.x *= Affinaranyx;
		u.y *= Affinaranyy;
		v.x *= Affinaranyx;
		v.y *= Affinaranyy;
	}

	// Megnezi, hogy nem tul vizszintes-e:
	int vizszintes = 0;
	int pozitivan = 0;
	if( u.x == 0.0 ) {
		vizszintes = 1;
	}
	else {
		if( fabs( u.y / u.x ) < Tulvizszintes ) {
			vizszintes = 1;
			if( u.y / u.x > 0 )
				pozitivan = 1;
		}
	}

	if( v.x == 0.0 ) {
		vizszintes = 1;
	}
	else {
		if( fabs( v.y / v.x ) < Tulvizszintes ) {
			vizszintes = 1;
			if( v.y / v.x > 0 )
				pozitivan = 1;
		}
	}

	if( vizszintes ) {
		if( pozitivan ) {
			u.forgatas( Tulvizszintes );
			v.forgatas( Tulvizszintes );
		}
		else {
			u.forgatas( -Tulvizszintes );
			v.forgatas( -Tulvizszintes );
		}
	}
	// Most mar biztos, hogy egy kicsit ferde az egesz!
	// Kiveve Affin esetet, amikor azert ez lehetseges:
	if( Affinitas ) {
		vizszintes = 1;
		while( vizszintes ) {
			vizszintes = 0;
			if( u.x == 0 ) vizszintes = 1;
			if( fabs( u.y / u.x ) < Tulvizszintes ) vizszintes = 1;
			if( u.y == 0 ) vizszintes = 1;
			if( fabs( u.x / u.y ) < Tulvizszintes ) vizszintes = 1;
			if( v.x == 0 ) vizszintes = 1;
			if( fabs( v.y / v.x ) < Tulvizszintes ) vizszintes = 1;
			if( v.y == 0 ) vizszintes = 1;
			if( fabs( v.x / v.y ) < Tulvizszintes ) vizszintes = 1;
			if( vizszintes ) {
				u.forgatas( Tulvizszintes );
				v.forgatas( Tulvizszintes );
			}
		}
	}

	// Inverz matrix kiszamitasa:
	vekt2 uu = u*(1.0/(pkiskep->xsize-1));
	vekt2 vv = v*(1.0/(pkiskep->ysize-1));
	double recdet = 1.0/(uu.x*vv.y-vv.x*uu.y);
	vekt2 invi( vv.y*recdet, -uu.y*recdet );
	vekt2 invj( -vv.x*recdet, uu.x*recdet );

	// Most megnezzuk, hogy nem log-e ki a negy csucs kozul valamelyik
	// kepernyorol (negy csucs: r, r+u, r+v, r+u+v

	// X irany;
	double maxert = 0.0;
	double minert = 0.0;
	if( u.x > 0 ) {
		if( v.x > 0 ) {
			// u poz, v poz:
			maxert = r.x+u.x+v.x;
			minert = r.x;
		}
		else {
			// u poz, v neg:
			maxert = r.x+u.x;
			minert = r.x+v.x;
		}
	}
	else {
		if( v.x > 0 ) {
			// u neg, v poz:
			maxert = r.x+v.x;
			minert = r.x+u.x;
		}
		else {
			// u neg, v neg:
			maxert = r.x;
			minert = r.x+u.x+v.x;
		}
	}
	int kilog = 0;
	if( maxert > Hatarx2 )
		kilog = 1;
	if( minert < Hatarx1 )
		kilog = 1;

	// A legmagasabban fekvo pontot kell erre valasztani:
	vekt2 csucs;

	// Y irany;
	if( u.y > 0 ) {
		if( v.y > 0 ) {
			// u poz, v poz:
			csucs = r+u+v;
			minert = r.y;
		}
		else {
			// u poz, v neg:
			csucs = r+u;
			minert = r.y+v.y;
		}
	}
	else {
		if( v.y > 0 ) {
			// u neg, v poz:
			csucs = r+v;
			minert = r.y+u.y;
		}
		else {
			// u neg, v neg:
			csucs = r;
			minert = r.y+u.y+v.y;
		}
	}
	maxert = csucs.y;
	if( maxert > Hatary2 )
		kilog = 1;
	if( minert < Hatary1 )
		kilog = 1;

	//if( kilog )
	//	return;

	// Kivalasztjuk egesz kezdopontot:
	int xbal = csucs.x;
	int ybal = csucs.y;
	double szint = ybal;

	// Kiszamolja hatarolo egyenesek parametereit:
	long hatbal1, hatjobb1;
	double m1d = u.x/u.y;
	long hatbal2, hatjobb2;
	double m2d = v.x/v.y;
	if( Affinitas ) {
		// u es v nem biztos, hogy merolegesek egymasra, ezert szamitas
		// bonyolultab:
		// Most u-ra meroleges eleket intezzuk el:
		// megkeressuk r magassagaban nem r-en atmeno u x koordjat:
		double xmetszet = r.x+v.x-(u.x/u.y)*v.y;
		if( r.x < xmetszet ) {
			// u a ball es u+v a jobb!:
			hatbal1 = (r.x+(szint-r.y)*m1d)*65536.0;
			hatjobb1 = (r.x+v.x+(szint-r.y-v.y)*m1d)*65536.0;
		}
		else {
			hatjobb1 = (r.x+(szint-r.y)*m1d)*65536.0;
			hatbal1 = (r.x+v.x+(szint-r.y-v.y)*m1d)*65536.0;
		}
		// Most v-ra meroleges eleket intezzuk el:
		// megkeressuk r magassagaban nem r-en atmeno v x koordjat:
		xmetszet = r.x+u.x-(v.x/v.y)*u.y;
		if( r.x < xmetszet ) {
			// v a ball es v+u a jobb!:
			hatbal2 = (r.x+(szint-r.y)*m2d)*65536.0;
			hatjobb2 = (r.x+u.x+(szint-r.y-u.y)*m2d)*65536.0;
		}
		else {
			hatjobb2 = (r.x+(szint-r.y)*m2d)*65536.0;
			hatbal2 = (r.x+u.x+(szint-r.y-u.y)*m2d)*65536.0;
		}
	}
	else {
		// u es v merolegesek egymasra, ezert szamitas gyors lehet:
		// Most u-ra meroleges eleket intezzuk el:
		if( v.x > 0 ) {
			// u a ball es u+v a jobb!:
			hatbal1 = (r.x+(szint-r.y)*m1d)*65536.0;
			hatjobb1 = (r.x+v.x+(szint-r.y-v.y)*m1d)*65536.0;
		}
		else {
			hatjobb1 = (r.x+(szint-r.y)*m1d)*65536.0;
			hatbal1 = (r.x+v.x+(szint-r.y-v.y)*m1d)*65536.0;
		}
		// Most v-ra meroleges eleket intezzuk el:
		if( u.x > 0 ) {
			// v a ball es v+u a jobb!:
			hatbal2 = (r.x+(szint-r.y)*m2d)*65536.0;
			hatjobb2 = (r.x+u.x+(szint-r.y-u.y)*m2d)*65536.0;
		}
		else {
			hatjobb2 = (r.x+(szint-r.y)*m2d)*65536.0;
			hatbal2 = (r.x+u.x+(szint-r.y-u.y)*m2d)*65536.0;
		}
	}

	long m1 = m1d*65536.0;
	long m2 = m2d*65536.0;

	// Most kiszamoljuk kis kepen futok elejet:
	vekt2 diff = vekt2( xbal, ybal ) - r;
	vekt2 kiskeporigo = vekt2( 0.5, 0.5 ) + diff.x*invi + diff.y*invj;
	long xkiskep = kiskeporigo.x*65536.0;
	long ykiskep = kiskeporigo.y*65536.0;

	long invix = invi.x*65536.0;
	long inviy = invi.y*65536.0;
	long invjx = invj.x*65536.0;
	long invjy = invj.y*65536.0;

	short* pf_hatbal1 = (short*)(&hatbal1);
	pf_hatbal1++;
	short* pf_hatjobb1 = (short*)(&hatjobb1);
	pf_hatjobb1++;
	short* pf_hatbal2 = (short*)(&hatbal2);
	pf_hatbal2++;
	short* pf_hatjobb2 = (short*)(&hatjobb2);
	pf_hatjobb2++;

	if( kilog ) {
		// Nagy ciklus kezdetet veszi:
		while( 1 ) {
			// Kiszamoljuk bal es jobb hatarokat ebben a sorban!
			int x1 = *pf_hatbal1;
			int xb = *pf_hatbal2;
			if( xb > x1 )
				x1 = xb;
			x1++;
			int x2 = *pf_hatjobb1;
			xb = *pf_hatjobb2;
			if( xb < x2 )
				x2 = xb;
			// x1 elso, x2 utolso hely, amit meg ki kell rakni!
			if( x1 <= x2 && ybal < Displaysizey ) { // VALT
				// Most raallitjuk xbal-t x1-re:
				while( xbal > x1 && xbal > 0 ) {    // VALT
					xbal--;
					xkiskep -= invix;
					ykiskep -= inviy;
				}
				while( xbal < x1 || xbal < 0 ) {
					xbal++;
					xkiskep += invix;
					ykiskep += inviy;
				}
				unsigned char* cim = ppic8->getptr( ybal );
				cim += xbal;
				int xhossz = Displaysizex-xbal;
				egysor( lyuk, short(x2-xbal+1), cim, pkiskep->tomb,
								xkiskep, ykiskep, invix, inviy, xhossz );
			}
			else {
				if( x1 > x2+1 )
					return;
			}
			// Egy sorral lejjebb kell lepni!:
			ybal--;
			if( ybal < 0 )                          // VALT
				return;                             // VALT
			xkiskep -= invjx;
			ykiskep -= invjy;
			hatbal1 -= m1;
			hatjobb1 -= m1;
			hatbal2 -= m2;
			hatjobb2 -= m2;
		}
	}
	else {
		// Nagy ciklus kezdetet veszi:
		while( 1 ) {
			// Kiszamoljuk bal es jobb hatarokat ebben a sorban!
			int x1 = *pf_hatbal1;
			int xb = *pf_hatbal2;
			if( xb > x1 )
				x1 = xb;
			x1++;
			int x2 = *pf_hatjobb1;
			xb = *pf_hatjobb2;
			if( xb < x2 )
				x2 = xb;
			// x1 elso, x2 utolso hely, amit meg ki kell rakni!
			if( x1 <= x2 ) {
				// Most raallitjuk xbal-t x1-re:
				while( xbal > x1 ) {
					xbal--;
					xkiskep -= invix;
					ykiskep -= inviy;
				}
				while( xbal < x1 ) {
					xbal++;
					xkiskep += invix;
					ykiskep += inviy;
				}
				unsigned char* cim = ppic8->getptr( ybal );
				cim += xbal;
				egysor( lyuk, short(x2-x1+1), cim, pkiskep->tomb,
								xkiskep, ykiskep, invix, inviy );
			}
			else {
				if( x1 > x2+1 )
					return;
			}
			// Egy sorral lejjebb kell lepni!:
			ybal--;
			xkiskep -= invjx;
			ykiskep -= invjy;
			hatbal1 -= m1;
			hatjobb1 -= m1;
			hatbal2 -= m2;
			hatjobb2 -= m2;
		}
	}
}

