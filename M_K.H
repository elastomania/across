// m_k.h

int mk_getextchar( void );
void mk_emptychar( void );
int mk_kbhit( void );
int mk_getstate( int kod );

#ifdef W_95

#define MK_ESC    (27)
#define MK_ENTER  (13)
#define MK_UP     (38)
#define MK_DOWN   (40)
#define MK_LEFT   (37)
#define MK_RIGHT  (39)
#define MK_PGUP   (33)
#define MK_PGDOWN (34)
#define MK_BACKSPACE (8)

#define MK_STATE_S (83)
#define MK_STATE_T (84)
#define MK_STATE_V (86)
#define MK_STATE_M (87) // Ezt meg nem irtam be (Snapshothoz)

#else

#define MK_ESC    (1)
#define MK_ENTER  (28)
#define MK_UP     (22) // (72) // ez nem 40-el kisebb, mivel az SPACE lenne
#define MK_DOWN   (40) // (80)
#define MK_LEFT   (35) // (75)
#define MK_RIGHT  (37) // (77)
#define MK_PGUP   (33) // (73)
#define MK_PGDOWN (41) // (81)
#define MK_DEL	  (43) // (83)
#define MK_BACKSPACE (14)

#define MK_STATE_S ('s')
#define MK_STATE_T ('t')
#define MK_STATE_V ('v')
#define MK_STATE_M ('m')
#define MK_STATE_N ('n')
#define MK_STATE_I ('i')

#endif

