#include    "all.h"

int Win_95 = 0;

void    billstart( void );
void    billstop( void );

/*static void settext( void ) {
    if( Vid640 ) {
        close640();
        Vid640 = 0;
    }
    if( Vid320 ) {
        settextmode();
        Vid320 = 0;
    }
} */

/*static void vegeszoveg( void ) {
    printf( "Thank you for playing SuperCross.\n" );
    printf( "\n" );
    //printf( "Evaluation version for Epic MegaGames.\n" );
    //printf( "Evaluation version for Broderbund Software, Inc.\n" );
    printf( "Evaluation version.\n" );
    printf( "Do not distribute!\n" );
    printf( "Copyright 1996 Bal zs R¢zsa\n" );
    printf( "\n" );
    //printf( "Original idea, Programming, Sound, Levels: Balazs Rozsa\n" );
    //printf( "Art: Barczi Imre, Balazs Rozsa\n" );
    //printf( "Testers: Barczi Imre, Csaba Rozsa, Andras Torok, Csaba Fekete,\n" );
    //printf( "           Tamas Kristyan, Akos Nemeth Buhin\n" );
    //printf( "Special thanks to Gabor Gerenyi and Tamas Kristyan for some sound files.\n" );
} */



static void vegeszoveg( void ) {
    #ifdef SARVARI
		printf( "Action SuperCross shareware version 1.2 Copyright (C) 1997, 1998 Bal zs R¢zsa  \n" );
        printf( "                                                                               \n" );
        printf( "You are welcome to distribute this program freely, as long as you don't        \n" );
        printf( "change it in any way. If you like Action SuperCross and continue to use it,    \n" );
        printf( "I ask you to register. I depend on your registrations.                         \n" );
        printf( "                                                                               \n" );
        printf( "When you register Action SuperCross for $29.95, you will get the registered    \n" );
        printf( "version, which has 26 more new levels and the full editor. Please add $4 for   \n" );
        printf( "shipping. Registered users of previous versions can upgrade to version 1.2     \n" );
        printf( "for free, read order.txt.                                                      \n" );
        printf( "                                                                               \n" );
        printf( "To register, send a check or money order payable to: Pik A Program (tm)        \n" );
        printf( "                                                     13 Saint Marks Place      \n" );
        printf( "                                                     NY, NY 10003              \n" );
        printf( "                                                     USA                       \n" );
        printf( "For Credit Card orders, call TOLL FREE (ORDERS ONLY) 1-800-867-3447.           \n" );
        printf( "For Online Compuserve orders, GO SWREG (product ID.: 12817).                   \n" );
        printf( "You can register and immediately download Action SuperCross on the WEB at      \n" );
        printf( "                                                http://www.pik.com             \n" );
        printf( "                                                                               \n" );
        printf( "If you have any question about Action SuperCross, visit                        \n" );
        printf( "     http://ourworld.compuserve.com/homepages/balazs/across.htm                \n" );
        printf( "or send an e-mail to:     100324.3131@compuserve.com                           \n" );
        printf( "Press a key!\n" );
        //getche();
        //exit( 0 );
    #else
        for( int i = 0; i < 6; i++ )
            printf( "\n" );
        printf( "Action SuperCross registered version 1.2 Copyright (C) 1997, 1998 Bal zs R¢zsa \n" );
        printf( "                                                                               \n" );
        printf( "                          Do not distribute!                                   \n" );
        printf( "                                                                               \n" );
        printf( "               Thank you for registering Action SuperCross.                    \n" );
        printf( "If you encounter problems running the game, please refer to the file           \n" );
        printf( "tech_inf.txt. In this file you can find information on how to get technical    \n" );
        printf( "support also.                                                                  \n" );
        printf( "                                                                               \n" );
        printf( "You can find the homepage of Action SuperCross at:                            \n" );
        printf( "  http://ourworld.compuserve.com/homepages/balazs/across.htm                \n" );
        printf( "Here you can download some more levels to play.                                  \n" );
        printf( "\n" );
        printf( "Press a key!\n" );
    #endif
}



// Ezekbol nem johet vissza:
static void directhiba( char* text1, char* text2, char* text3, char* text4 ) {

    // torlendo:
	//mk_getextchar();

	settextmode();
	if( strcmp( text1, "Kiir szoveg!" ) == 0 ) {
		vegeszoveg();
		stophang();
		mk_emptychar();
		while( !mk_kbhit() )
			;
		if( Billint )
			billstop();
		// Visszaall sor elejere:
		#ifdef WATCOM
			_settextposition( 24, 1 );
			_outtext( "                                      " );
			_settextposition( 23, 60 );
			_outtext( "  " );
		#else
			gotoxy( 1, 23 );
			printf( "                                      " );
			gotoxy( 46, 21 );
		#endif
		exit( 0 );
		/*mv_startstopper();
		while( 1 ) {
			if( kbhit() || mv_stopperido() > 2*182.0 ) {
				//if( kbhit() )
				//  getch();
				exit( 0 );
			}
		} */
	}

	printf( "%s\n", text1 );
	if( text2 )
		printf( "%s\n", text2 );
	if( text3 )
		printf( "%s\n", text3 );
	if( text4 )
		printf( "%s\n", text4 );
	printf( "Press a key!\n" );

	mk_emptychar();
	while( !mk_kbhit() )
		;
	if( Billint )
		billstop();
	//getch();
	exit( 0 );
}

static int Tovabb;

void hiba( char* text1, char* text2, char* text3 ) {
    Tovabb = 0;
    if( Tovabb )
        return;
    if( strcmp( text1, "Kiir szoveg!" ) == 0 )
        directhiba( text1, text2, text3, NULL );
    if( access( "message.inf", 0 ) == 0 )
        directhiba( "message.inf accessed\n", text1, text2, text3 );
    directhiba( "Sorry, internal error.", NULL, NULL, NULL );
}

void uzenet( char* text1, char* text2, char* text3 ) {
    // about azert kell, hogy kezdo memchecknel ne agazzon el:
    if( strstr( text1, "memory" ) && !strstr( text1, "about" ) ) {
        // Ha memory szerepel text1-ben:
        if( access( "message.inf", 0 ) == 0 ) // Most mem-et is csak inf-re adja:
            directhiba( "message.inf accessed\n", text1, text2, text3 );
        directhiba( "Sorry, out of memory!", NULL, NULL, NULL );
    }
    directhiba( text1, text2, text3, NULL );
}

// Ugyanaz mint random, de az nincs WATCOM-ban:
int s_random( int tartomany ) {
    return rand() % tartomany;
}

// Normalisra allitja orat:
static void setseb( void ) {
    unsigned short sebesseg = 0xffff;
    outp( 0x43, 0x34 );
    outp( 0x40, (unsigned char)(sebesseg & 0x0ff) );
    outp( 0x40, (unsigned char)(sebesseg >> 8) );
}

int Billint = 1;

void main( int argc, const char* argv[] ) {
    /*mv_startstopper();
    int elozoido = 0;
    while( 1 ) {
        int ido = mv_stopperido();
        if( ido != elozoido )
            printf( "ido: %d\n", ido );
        elozoido = ido;
        if( kbhit() )
            exit( 0 );
    } */

    //void midnightteszt( void );
    //midnightteszt(); // Ez ki is lep

    Hangenabled = 0;
    #ifdef WATCOM
        Hangenabled = 1;
    #endif

    #ifdef DOSX286
        Hangenabled = 0;
    #endif

    setseb();

    initqopen();

    // Ezt itt vissza kell allitani:
    starthanghigh();

    // ITT ALLITJUK BE MODOT:
    setmode8_l( 1 );
    init_d_dd();

    void kulonexe_abcmake( void );
    //kulonexe_abcmake();

    mk_init();
    if( Billint )
        billstart();

    // Ezt vegleges verzioba bele kell tenni:
	//check_kommand( argv[0] );

    mv_main();

    hiba( "main vegere nem szabadna jonnie" );
}

void mv_check( void ) {
};

