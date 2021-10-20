#include	"all.h"

long Newszam = 0;
void* Mutvoid;

void** newnovel( void ) {
	Newszam++;
	return &Mutvoid;
}

int newnovel2( void ) {
	Newszam++;
	return 0;
}



