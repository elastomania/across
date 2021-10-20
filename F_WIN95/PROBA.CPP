#include	"w_all.h"

void w_initvaltozok( void ) {
}

//static double Elteltelozo = 0.0;

// Idohuzasra:
void semmi( void ) {
}

void mv_main( void ) {
	mv_startstopper();
	while( 1 ) {
		mv_check();

		char tmp[10];
		long l = mv_stopperido()*1000.0/182.0;
		ltoa( l, tmp, 10 );
		szovegki( 100, tmp );

		/*char tmp[20];
		double eltelt = mv_getsec();
		if( Elteltelozo == 0.0 )
			Elteltelozo = eltelt;
		sprintf( tmp, "%f", (float)(eltelt-Elteltelozo) );
		strcat( tmp, "          " );
		szovegki( 100, tmp );
		Elteltelozo = eltelt;

		for( long l = 0; l < 100000l; )
			l++;
		*/

		//if( szam == 100000l )
		//	hiba( "Szazezzer!" );

		if( mk_kbhit() ) {
			char szoveg[10];
			int c = mk_getextchar();
			if( c == 'q' )
				mv_exit();
			if( c == 'd' )
				hiba( "d lenyomva!" );
			itoa( c, szoveg, 10 );
			szovegki( 20, szoveg );
			szoveg[0] = (char)c;
			szoveg[1] = 0;
			szovegki( 40, szoveg );
		}

		for( int i = 0; i < 100; i++ ) {
			if( mk_getstate( MK_ESC ) ) {
				szovegki( 60, "ESC               " );
			}
			if( mk_getstate( MK_ENTER ) ) {
				szovegki( 60, "ENTER               " );
            //ddinit();
			   // Betoltunk valamit back bufferbe:
            unsigned char buff1[640];
            unsigned char buff2[640];
            for( int i = 0; i < 640; i++ ) {
            	if( i%10 >= 5 )
               	buff1[i] = 0;
					else
                  buff1[i] = 1;
					if( i%20 >= 10 )
               	buff2[i] = 0;
					else
                  buff2[i] = 1;
            }

			   for( long l = 0; l < 640; l++ ) {
            	int paros = 0;
            	if( l%2 )
                 	paros = 1;
	           	mv_check();
        	      int xs,ys;
				   unsigned char** ppc = lockbackbuffer( &xs, &ys );
				   for( int y = 0; y < ys; y++ ) {
					  	unsigned char* sor = ppc[y];
               	if( paros )
							//memcpy( sor+1, buff1, 639 );
                     memcpy( sor, buff1, 640 );
	               else
                     //memcpy( sor+1, buff2, 639 );
                     memcpy( sor, buff2, 640 );
		  		   }
		     		unsigned char* sor = ppc[180];
               memset( sor, 50, l );
		  		   unlockbackbuffer();
            }
			   //ddfinish();
			   hiba( "Befejezte dd probat!" );
			}
			if( mk_getstate( MK_UP ) ) {
				szovegki( 60, "UP               " );
			}
			if( mk_getstate( MK_DOWN ) ) {
				szovegki( 60, "DOWN               " );
			}
			if( mk_getstate( MK_RIGHT ) ) {
				szovegki( 60, "RIGHT               " );
			}
			if( mk_getstate( MK_LEFT ) ) {
				szovegki( 60, "LEFT               " );
			}
         if( mk_getstate( MK_PGUP ) ) {
				szovegki( 60, "PGUP               " );
			}
			if( mk_getstate( MK_PGDOWN ) ) {
				szovegki( 60, "PGDOWN               " );
			}                           
         if( mk_getstate( MK_BACKSPACE ) ) {
				szovegki( 60, "BACKSPACE               " );
			}
		}
	}
}



