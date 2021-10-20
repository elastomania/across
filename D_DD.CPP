#include	"all.h"

static void setpal( unsigned char* pal ) {
	#ifndef DOS
		outp( 0x3c8, 0 );
		for( int i = 0; i < 768; i++ )
			outp( 0x3c9, pal[i] );
	#else
		pal++;
		hiba( "Itt nem szabadna lennie!" );
	#endif
}

static int Resdd = 0;

ddpal::ddpal( unsigned char* tombp ) {
	{ tomb = NEW(unsigned char*) unsigned char[768]; }
	if( !tomb )
		hiba( "memory ddpal-ban!" );
	memcpy( tomb, tombp, 768 );
}

ddpal::~ddpal( void ) {
	if( tomb )
		DELETE tomb; }
}

void ddpal::set( void ) {
	setpal( tomb );
}

static pic8* Ppicbuff = NULL;

void init_d_dd( void ) {
	if( Ppicbuff )
		hiba( "init_d_dd ketszer hivva!" );
	{ Ppicbuff = NEW(pic8*) pic8( 320, 200 ); }
	Resdd = 1;
}

unsigned char** lockbackbuffer( int* pxs, int* pys ) {
	if( !Ppicbuff )
		hiba( "lockbackbuffer init_d_dd hivasa nelkul!" );

	*pxs = -1;
	*pys = -1;
	if( Resdd == 1 ) {
		*pxs = 320;
		*pys = 200;
	}
	if( Resdd == 2 ) {
		*pxs = 320;
		*pys = 240;
	}
	if( Resdd == 3 ) {
		*pxs = 640;
		*pys = 400;
	}
	if( Resdd == 4 ) {
		*pxs = 640;
		*pys = 480;
	}
	if( *pxs < 1 )
		hiba( "unlockbackbuffer-ben fizxsize < 1!" );

	return Ppicbuff->sormuttomb;
}

// Ki is teszi
void unlockbackbuffer( void ) {
	if( !Ppicbuff )
		hiba( "unlockbackbuffer init_d_dd hivasa nelkul!" );
	int fizxsize = -1;
	int fizysize = -1;
	if( Resdd == 1 ) {
		fizxsize = 320;
		fizysize = 200;
	}
	if( Resdd == 2 ) {
		fizxsize = 320;
		fizysize = 240;
	}
	if( Resdd == 3 ) {
		fizxsize = 640;
		fizysize = 400;
	}
	if( Resdd == 4 ) {
		fizxsize = 640;
		fizysize = 480;
	}
	if( fizxsize < 1 )
		hiba( "unlockbackbuffer-ben fizxsize < 1!" );

	for( int y = 0; y < fizysize; y++ ) {
		putline8_l( 0, y, Ppicbuff->sormuttomb[fizysize-y-1], fizxsize );
		if( y == 220 )
			hang();
	}

	if( fizysize < 390 )
		return;


	// torlendo: (interlace-elt kepkirakast demostralja):
	/*unsigned char sor[110];
	for( int i = 0; i < 100; i++ ) {
		if( i > 20 )
			sor[i] = 100;
		else
			sor[i] = 0;
	}
	while( 1 ) {
		int dx = 2;
		if( mk_kbhit() ) {
			mk_getextchar();
			hiba( "Vege!" );
		}
		int paros = 1;
		for( int x = 0; x < 500; x += dx ) {
			waitforcathode();
			paros = !paros;
			for( y = 0; y < 100; y += 2 ) {
				if( y > 80 ) {
					if( paros ) {
						putline8_l( x, y+200, sor, 100 );
						putline8_l( x, y+1+200, sor, 100 );
					}
				}
				else {
					if( paros )
						putline8_l( x, y+200, sor, 100 );
					else
						putline8_l( x, y+1+200, sor, 100 );
				}
			}
		}
	}*/
}

// res: 1 -> 320x200, 3 -> 640x400, 4 -> 640x480:
void setresdd( int res ) {
	if( res != 1 && res != 2 && res != 3 && res != 4 )
		hiba( "setres-ben res != 1 && res != 2 && res != 3 && res != 4!" );
	if( Resdd == res )
		return;
	Resdd = res;

	int xsize = -1;
	int ysize = -1;
	if( Resdd == 1 ) {
		xsize = 320;
		ysize = 200;
	}
	if( Resdd == 2 ) {
		xsize = 320;
		ysize = 240;
	}
	if( Resdd == 3 ) {
		xsize = 640;
		ysize = 400;
	}
	if( Resdd == 4 ) {
		xsize = 640;
		ysize = 480;
	}
	if( xsize < 1 )
		hiba( "setresdd-ben xsize < 1!" );

	setmode8_l( Resdd );

	if( !Ppicbuff )
		hiba( "setresdd-ben !Ppicbuffer!" );

	DELETE Ppicbuff; }

	{ Ppicbuff = NEW(pic8*) pic8( xsize, ysize ); }
}

