#include	"all.h"

static void kitolt( unsigned i, int szorzo, signed short e1, signed short e2,
							wav* pd ) {
	double m = e1+0.5;
	double a = (double(e2)-double(e1))/double(szorzo);
	for( int j = 0; j < szorzo; j++ ) {
		pd->tomb[i*szorzo+j] = m+a*j;
	}
}

wav* tulminta( wav* pw, int szorzo, signed short ucsoertek ) {
	long nsize = pw->size*szorzo;
	if( nsize > 64000u )
		hiba( "tulminta-ban nsize > 64000!" );
	wav* pn = NEW wav( (unsigned short)nsize );
	for( unsigned i = 0; i < pw->size-1; i++ )
		kitolt( i, szorzo, pw->tomb[i], pw->tomb[i+1], pn );
	kitolt( pw->size-1, szorzo, pw->tomb[pw->size-1], ucsoertek, pn );

	return pn;
}


void vegtelenit( wav* pw ) {
	if( pw->size % 2 )
		hiba( "vegtelenitben wav hossza nem paros!" );
	int siz = pw->size/2;
	for( int i = 0; i < siz; i++ ) {
		pw->tomb[i] = pw->tomb[i]*(double(i)/(siz-1)) +
					  pw->tomb[i+siz]*double(siz-1-i)/(siz-1);
		pw->tomb[i+siz] = pw->tomb[i+siz]*double(siz-1-i)/(siz-1) +
						  pw->tomb[i]*(double(i)/(siz-1));
	}
}


