// w_main.h

// M_DD.H-bol nem publikus reszek:
void ddfinish( void );
void ddinit( void );




// M_V.H-bol nem publikus reszek:
extern HWND GhWnd; // Window handler
double mv_getsec( void ); // Elvileg le fog allni, ha elkerul vezerles!


void szovegki( int y, char* szoveg ); // torlendo:




// M_K.H-bol nem publikus reszek:
void mk_init( void );
void mk_wmkeydown( int kod );
void mk_wmkeyup( int kod );
void mk_wmchar( int kod );


