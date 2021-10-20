#include	"all.h"

// Visszaadja, hogy volt-e hiba:
int check_topology( int kelldialog ) {
	dialog( "Checking Topology, please wait!",
					"GOMBOK", "VISSZATER" );

	// Eloszor egymason fekvo szomszedos pontokat valasztjuk szet:
	for( int i = 0; i < MAXGYURU; i++ ) {
		gyuru* pgy = Ptop->ptomb[i];
		if( pgy )
			pgy->szetvalaszt();
	}

	// Most megszuntetjuk hegyes szogeket:
	for( i = 0; i < MAXGYURU; i++ ) {
		gyuru* pgy = Ptop->ptomb[i];
		if( pgy )
			pgy->pozitivkorjaras(); // Ez automatikusan megcsinalja:
	}

	// Most megnezzuk, hogy nincs-e tul sok pont osszesen:
	int pontszam = 0;
	for( i = 0; i < MAXGYURU; i++ ) {
		gyuru* pgy = Ptop->ptomb[i];
		if( pgy )
			pontszam += pgy->pontszam;
	}
	if( pontszam > 5000 ) {
		if( kelldialog ) {
			char tmp[100];
			sprintf( tmp, "There are %d vertices now in the level.", pontszam );
			dialog( "Error: The number of vertices must be less than 5000!",
					tmp,
					"Please delete some vertices or polygons from this level!" );
			invalidateegesz();
			return 1;
		}
		else {
			return 1;
		}
	}



	// Most megnezzuk, hogy nincsenek-e egymast metszo vonalak:
	for( i = 0; i < MAXGYURU; i++ ) {
		gyuru* pgy = Ptop->ptomb[i];
		if( pgy ) {
			for( int j = 0; j < pgy->pontszam; j++ ) {
				vekt2 r = pgy->ponttomb[j];
				vekt2 v;
				if( j == pgy->pontszam-1 )
					v = pgy->ponttomb[0] - r;
				else
					v = pgy->ponttomb[j+1] - r;
				// Megvan r, v szakasz, amirol el kell donteni, hogy van-e
				// metszespontja akarmi massal:
				for( int k = 0; k < MAXGYURU; k++ ) {
					gyuru* pgy2 = Ptop->ptomb[k];
					if( pgy2 ) {
						int vanmetszes = 0;
						vekt2 m;
						if( pgy == pgy2 )
							vanmetszes = pgy2->metszeshely( r, v, j, &m );
						else
							vanmetszes = pgy2->metszeshely( r, v, -1, &m );
						if( vanmetszes ) {
							if( kelldialog ) {
								zoom( m, 0.0000001 );
								dialog( "Error: Two lines are crossing each others!",
										"After this dialog you will see the crossing point.",
										"Use Zoomout to see where it is located!" );
								invalidateegesz();
								return 1;
							}
							else {
								return 1;
							}
						}
					}
				}
			}
		}
	}

	// Most megnezzuk, hogy nincs-e kintlevo objektum:
	double x1, y1, x2, y2;
	Ptop->getminmax( &x1, &y1, &x2, &y2, 0 );
	x1 -= 1.0;
	y1 -= 1.0;
	x2 += 1.0;
	y2 += 1.0;
	for( i = 0; i < MAXKEREK; i++ ) {
		kerek* pker = Ptop->kerektomb[i];
		if( pker ) {
			if( pker->r.x < x1 || pker->r.x > x2 ||
				pker->r.y < y1 || pker->r.y > y2 ) {
				if( kelldialog ) {
					zoom( pker->r, 1.5 );
					dialog( "Error: An object is outside the level borders!",
							"After this dialog you will see the object.",
							"Use Zoomout to see where it is located!" );
					invalidateegesz();
					return 1;
				}
				else {
					return 1;
				}
			}
		}
	}

	// Most megnezzuk, hogy bal kerek nincs-e kint:
	for( i = 0; i < MAXKEREK; i++ ) {
		kerek* pker = Ptop->kerektomb[i];
		if( pker && pker->tipus == T_KEZDO ) {
			if( !Ptop->levegoben( NULL, &pker->r ) ) {
				if( kelldialog ) {
					dialog( "Error: The start object, which determines the place of the left wheel when you start to",
							"play on a particular level, is inside ground!",
							"If you cannot resolve this error message, you should try to move this object inside",
							"ground, because it is possible that you have mixed up which is the ground and",
							"which is the air.",
							"After this dialog you will see the object.",
							"Use Zoomout to see where it is located!" );
					zoom( pker->r, 1.5 );
				}
				return 1;
			}
		}
	}

	if( kelldialog )
		dialog( "Everything seems to be all right." );
	return 0;
}



