#include	"all.h"

void initmode320x200_drv( void ) {
	#ifdef WATCOM
		union	REGS	inregs, outregs;
		inregs.x.eax = 0x13;
		inregs.x.ebx = 0;
		int386( 16, &inregs, &outregs );
	#else
		asm {
			push ax; push bx; push cx; push dx; push di; push si;
		};
		_AX = 0x13;
		_BX = 0;
		asm int 16;
		asm {
			pop si; pop di; pop dx; pop cx; pop bx; pop ax;
		};
	#endif
	return;
}

