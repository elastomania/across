#include	"all.h"

// H_, H_HIGH, H_WAV, H_TULMIN

int Mute = 1;

int Hangenabled = 0;

void setvolume( double volume ) {
	volume++;
}

void starthanghigh( void ) {
}

void stophanghigh( void ) {
}

void stophang( void ) {
}

void startmotor( void ) {
}

void setsurlodas( double a ) {
	a++;
}

void setmotor( double frekvencia, int hangero ) {
	frekvencia++;
	hangero++;
}

void startwave( int wavindex, double hangero ) {
	wavindex++;
	hangero++;
}

int vanmegwav( void ) {
	return 0;
}

void hang( void ) {
}

void nullazhangot( void ) {
}

void hangosdelay( long t ) {
	// 182*sec-et adja vissza idot tortresszel egyutt!
	double kezdo = mv_stopperido();
   while( mv_stopperido()/182.0 < kezdo/182.0+t/1000.0 )
			mv_check();
}


