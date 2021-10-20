
// 0-nem kell hang, 1 8bit mono, 2 16bit mono:
int starthang( unsigned buffsize );
void stophang( void );
void hang( void );
void nullazhangot( void );

// 0.0 - 1.0 kozott:
void setvolume( double hangero ); // Ez h_.cpp-ben van kozvetlenul!

// Ezt meg kell irni hozza:
void callbackhang( short* sbuff, unsigned char* cbuff );
