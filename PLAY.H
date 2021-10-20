
void play( void );
void replaykorny( void );
// A jelenleg beolvasott palyan jatszik vissza:
// Csak afterplay menu replay-ebol szabad hasznalni:
void replayjelenlegi( void );

// Vagy level == -1 es belyeg != 0 es filenev == NULL,
// vagy level >= 0 < 100 es belyeg = 0 es filenev == NULL,
// vagy level == -1 es belyeg = 0 es filenev ervenyes,
// Hamis eseten nem sikerult betolteni palyat (rec filehoz nincs lev):
int palyabe( int level, unsigned long belyeg = 0, char* filenev = NULL );

// Legkozelebbi palyabe mindenkeppen kell hogy olvasson uj topolt:
void ervenytlenitpalya( void );

// belyeg == 0 eseten internal level file:
void saveplay( long belyeg );

