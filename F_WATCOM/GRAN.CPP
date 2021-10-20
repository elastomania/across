#include	<stdio.h>
#include	<mem.h>

#include	"pmode.h"

// Azzal ter vissza, hogy sikeres volt-e:
void main( void ) {
	unsigned int	VESABuf_sel;   // Selector for VESABuf (1k size)
	unsigned int	VESABuf_off;   // Offset for VESABuf (1k size)
	unsigned int	VESABuf_rseg;  // Real mode segment of VESABuf
	unsigned int	VESABuf_roff;  // Real mode offset of VESABuf

	PM_allocRealSeg( 1024, &VESABuf_sel,  &VESABuf_off,
						   &VESABuf_rseg, &VESABuf_roff );

	RMSREGS	sr;
	RMREGS r;

	memset( &sr, 0, sizeof( sr ) );
	memset( &r, 0, sizeof( r ) );

	r.x.eax = 0x4f01;
	r.x.ecx = 0x101; // 640x480x8 mod kodja

	sr.es = VESABuf_rseg;
	r.x.edi = VESABuf_roff;

	PM_int86x( 0x10, &r, &r, &sr );

	if( r.w.ax == 0x004F )
		printf( "Mukodik!\n" );
	else
		printf( "Nem mukodik!\n" );
}





