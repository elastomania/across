#include	"all.h"

int	Enmou, Resolmou;
int	Mostmoux, Mostmouy;
int Xsize, Ysize;

void initmou( int xs, int ys ) {
	union	REGS	inregs, outregs;
	inregs.x.ax = 0;
	int86( 51, &inregs, &outregs );

	Resolmou = 3;
	Enmou = 1;

	Xsize = xs<<Resolmou;
	Ysize = ys<<Resolmou;

	inregs.x.ax = 15;
	inregs.x.cx = 1;
	inregs.x.dx = 1;
	int86( 51, &inregs, &outregs);

	inregs.x.ax=7;
	inregs.x.cx=0;
	inregs.x.dx=10000;
	int86(51,&inregs,&outregs);

	inregs.x.ax=8;
	inregs.x.cx=0;
	inregs.x.dx=10000;
	int86(51,&inregs,&outregs);

	inregs.x.ax=4;
	inregs.x.cx=5000;
	inregs.x.dx=5000;
	int86(51,&inregs,&outregs);
}

int getmou( int* px, int* py ) {
	int	i;
	union	REGS	inregs,outregs;
	inregs.x.ax=3;
	int86(51,&inregs,&outregs);
	if( Enmou ) {
		Mostmoux = Mostmoux + (outregs.x.cx-5000);
		Mostmouy = Mostmouy + (outregs.x.dx-5000);
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

	i=(outregs.x.cx!=5000 || outregs.x.dx!=5000) && Enmou;

	inregs.x.ax=4;
	inregs.x.cx=5000;
	inregs.x.dx=5000;
	int86(51,&inregs,&outregs);
	return i;
}

void setmou( int x, int y ) {
	Mostmoux = x*(1<<Resolmou);
	Mostmouy = y*(1<<Resolmou);
}

int getbutbmou( void ) {
	union	REGS inregs,outregs;
	inregs.x.ax=5;
	inregs.x.bx=0;
	int86(51,&inregs,&outregs);
	return outregs.x.bx>0;
}


int	getbutjmou( void ) {
	union	REGS inregs,outregs;
	inregs.x.ax=5;
	inregs.x.bx=1;
	int86(51,&inregs,&outregs);
	return outregs.x.bx>0;
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
