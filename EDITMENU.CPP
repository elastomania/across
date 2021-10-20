#include	"all.h"

// igaz, ha kilep
int menu_exit( void ) {
	invalidateegesz();
	if( Valtozott ) {
		if( dialog( "There are unsaved changes in the level file.",
					"If you exit now, you will loose these changes.",
					"Do you still want to exit?",
					"GOMBOK", "Yes", "No" ) == 1 )
		return 0;
	}
	delete Ptop;
	Ptop = NULL;
	return 1;
}

void menu_zoomout( void ) {
	zoomout();
	invalidate();
}

