#include	"all.h"
#include	"pmode.h"

/*static unsigned int	VESABuf_sel;   // Selector for VESABuf (1k size)
static unsigned int	VESABuf_off;   // Offset for VESABuf (1k size)
static unsigned int	VESABuf_rseg;  // Real mode segment of VESABuf
static unsigned int	VESABuf_roff;  // Real mode offset of VESABuf

// Azzal ter vissza, hogy sikeres volt-e:
static int pmhivas( void *buffer, int size ) {
	RMSREGS	sr;
	RMREGS r;

	memset( &sr, 0, sizeof( sr ) );
	memset( &r, 0, sizeof( r ) );

	r.x.eax = 0x4f01;
	r.x.ecx = 0x101;

	sr.es = VESABuf_rseg;
	r.x.edi = VESABuf_roff;

	PM_int86x( 0x10, &r, &r, &sr );
	PM_memcpynf(buffer, VESABuf_sel, VESABuf_off, size);

	if( r.w.ax == 0x004F )
		return 1;
	else
		return 0;
} */

static int Elso = 1;
static int Granula = 64;

// Sokszor lehet hivni, de csak egyszer kiserli meg granularitas beolvasasat:
int getgranularitas( void ) {
	if( !Elso )
		return Granula;
	Elso = 0;

	// Eloszor megnezzuk, hogy nincs-e SETUP.VID file-ban beallitva
	// granularitas:
	if( access( "setup.vid", 0 ) == 0 ) {
		// Letezik SETUP.VID file:
		FILE* h = fopen( "setup.vid", "rt" );
		if( !h )
			return Granula;
		int ujgranula = 0;
		if( fscanf( h, "%d", &ujgranula ) != 1 )
			uzenet( "Can't read granularity from file SETUP.VID!", "Run setup.exe or delete this file!" );
		if( ujgranula != 1 && ujgranula != 2 && ujgranula != 4 &&
			ujgranula != 8 && ujgranula != 16 && ujgranula != 32 &&
			ujgranula != 64 )
			uzenet( "Number in file SETUP.VID must be either 1, 2, 4, 8, 16, 32 or 64!",
					"Run setup.exe, or delete the file SETUP.VID!" );
		Granula = ujgranula;
		return Granula;
	}

	/*if( !PM_allocRealSeg( 1024, &VESABuf_sel,  &VESABuf_off,
								&VESABuf_rseg, &VESABuf_roff ) ) {
		FILE* h = fopen( "warning.txt", "wt" );
		if( h ) {
			fprintf( h, "Real segment allocation failed!" );
			fclose( h );
		}
		return Granula;
	}

	struct {
		int kitolt;
		unsigned short gran, b, c;
	} gs;

	if( !pmhivas( &gs, 8 ) ) {
		FILE* h = fopen( "warning.txt", "wt" );
		if( h ) {
			fprintf( h, "VESA 0x4f01 interrupt failed!" );
			fclose( h );
		}
		return Granula;
	}
	// Jo volt hivas:
	//FILE* h = fopen( "warning2.txt", "wt" );
	//if( h ) {
	//	fprintf( h, "VESA 0x4f01 interrupt jo volt!" );
	//	fclose( h );
	//}
	Granula = gs.gran;
	PM_freeRealSeg( VESABuf_sel, VESABuf_off ); */

	return Granula;
}


