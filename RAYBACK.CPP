#include	"all.h"


/*void waitforcathode( void ) {
	asm	push	ds

	asm	mov		dx, 3DAh
ret1:
	asm	in		al, dx
	asm	and		al, 8
	asm	jnz		ret1
ret2:
	asm	in		al, dx
	asm	and		al, 8
	asm	jz		ret2

	asm	pop		ds
} */

void waitforcathode( void ) {
	while( inp( 0x3da ) & 8 )
		;
	while( !(inp( 0x3da ) & 8) )
		;
}
