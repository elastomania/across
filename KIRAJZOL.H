
// Annak eldontesere, hogy kirajzolando doboz nem log-e ki:
// peldaul: Hatarx1 = 2.0, Hatarx2 = 318.0 (resetkirajzol tolti ki):
extern double   Hatarx1,
				Hatarx2,
				Hatary1,
				Hatary2;

void resetkirajzol( void );

struct st320 {
	vekt2 origo;
	int vxelozo, vyelozo;
	magassag *pmviz, *pmfugg;
};

class palyaleiro;

void kirajzol640( double t, targytomb* pttt, double ugrasnagysag );

extern double Mo_y, Mo_bal, Mo_dx; // 320-hoz
void kirajzol320( st320* pst, double t, palyaleiro* pleiro, int view,
		int timeki, double baljobb, double ugrasnagysag, int ugras1volt,
		double forgas );

// 640-es uzemmodba valt:
void setgraph( void );
void close640( void );

