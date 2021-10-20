#include	"all.h"

int	Enmou, Resolmou;
int	Mostmoux, Mostmouy;
int Xsize, Ysize;

void initmou( int xs, int ys ) {
	union	REGS	inregs, outregs;
	inregs.x.eax = 0;
	int386( 51, &inregs, &outregs );

	Resolmou = 3;
	Enmou = 1;

	Xsize = xs<<Resolmou;
	Ysize = ys<<Resolmou;

	inregs.x.eax = 15;
	inregs.x.ecx = 1;
	inregs.x.edx = 1;
	int386( 51, &inregs, &outregs);

	inregs.x.eax=7;
	inregs.x.ecx=0;
	inregs.x.edx=10000;
	int386(51,&inregs,&outregs);

	inregs.x.eax=8;
	inregs.x.ecx=0;
	inregs.x.edx=10000;
	int386(51,&inregs,&outregs);

	inregs.x.eax=4;
	inregs.x.ecx=5000;
	inregs.x.edx=5000;
	int386(51,&inregs,&outregs);
}

int getmou( int* px, int* py ) {
	int	i;
	union	REGS	inregs,outregs;
	inregs.x.eax=3;
	int386(51,&inregs,&outregs);
	if( Enmou ) {
		Mostmoux = Mostmoux + (((unsigned short)(outregs.x.ecx))-5000);
		Mostmouy = Mostmouy + (((unsigned short)(outregs.x.edx))-5000);
	}
	if( Mostmoux < 0 )
		Mostmoux = 0;
	if( Mostmoux >= Xsize )
		Mostmoux = Xsize-1;
	if( Mostmouy < 0 )
		Mostmouy = 0;
	if( Mostmouy >= Ysize )
		Mostmouy = Ysize-1;

	*px = Mostmoux>>Resolmou;
	*py = Mostmouy>>Resolmou;

	i=(outregs.x.ecx!=5000 || outregs.x.edx!=5000) && Enmou;

	inregs.x.eax=4;
	inregs.x.ecx=5000;
	inregs.x.edx=5000;
	int386(51,&inregs,&outregs);
	return i;
}

void setmou( int x, int y ) {
	Mostmoux = x*(1<<Resolmou);
	Mostmouy = y*(1<<Resolmou);
}

int getbutbmou( void ) {
	union	REGS inregs,outregs;
	inregs.x.eax=5;
	inregs.x.ebx=0;
	int386(51,&inregs,&outregs);
	return outregs.x.ebx>0;
}


int	getbutjmou( void ) {
	union	REGS inregs,outregs;
	inregs.x.eax=5;
	inregs.x.ebx=1;
	int386(51,&inregs,&outregs);
	return outregs.x.ebx>0;
}


void setresolmou( int k ) {
	double	valt;
	int i = k-Resolmou;
	if(i>0)
		valt=1<<i;
	  else
		valt=1/((double)((int) 1 << (-i)));
	Mostmoux = Mostmoux*valt;
	Mostmouy = Mostmouy*valt;
	Resolmou = k;
}
