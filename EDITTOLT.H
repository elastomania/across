

void tolt_open( void );
// Ha sikeresen mentettek, akkor igazak:
int tolt_save_as( int lokkolhato );
int tolt_save( int lokkolhato );
void tolt_new( void );

extern char Level_name[100];
void change_level_name( void );

// Editfilenev alapjan megprobal josolni, majd oda teszi eredmenyt:
void kivalasztkezdetit( void );

