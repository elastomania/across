#include	"w_all.h"

int Win_95 = 1;

static LRESULT CALLBACK _export WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

static HINSTANCE ghInstance = 0;
HWND GhWnd = 0;

static char szAppName[100] = "sz_nev";
static char szCaption[100] = "sz_cap";

static int GameLoop = 0;

static void* Dummypvoid = NULL;

void w_dosbeallitasok( void );

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				LPSTR lpszCmdLine, int CmdShow ) {

	WNDCLASS wndclass;

   /*for( int hh = 0; hh < 1; hh++ )
   	;
	for( int hh = 0; hh < 1; hh++ )
   	; */

	if( !hPrevInstance ) {
		// Register parent window class:
		wndclass.style				= CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
		wndclass.lpfnWndProc 	= (WNDPROC)WndProc;
		wndclass.cbClsExtra		= 0;
		wndclass.cbWndExtra		= 0;
		wndclass.hInstance		= hInstance;
		wndclass.hIcon				= LoadIcon( hInstance, szAppName );
		wndclass.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wndclass.hbrBackground	= (HBRUSH)GetStockObject( GRAY_BRUSH );
		wndclass.lpszMenuName	= (LPSTR)NULL;
		wndclass.lpszClassName	= szAppName;

		if( !RegisterClass( &wndclass ) )
			return 0;
	}

	ghInstance = hInstance;

	//GhWnd = CreateWindowEx( WS_EX_TOPMOST, szAppName, szCaption, WS_POPUP,
	//						0, 0, 600, 400,
	//						(HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL );

	// DirectDraw-hoz ez volt:
	GhWnd = CreateWindowEx( WS_EX_TOPMOST, szAppName,
   						szCaption, WS_POPUP,
							0, 0, 320, 200,
							(HWND)NULL, (HMENU)NULL,
                     hInstance, (LPVOID)NULL );

	/*ghWnd    = CreateWindowEx (WS_EX_TOPMOST, szAppName,
                              szCaption, WS_POPUP,
                              0, 0, 640, 480,
                              (HWND)NULL, (HMENU)NULL,
                              hInstance, (LPVOID)NULL) ;*/

	if ( !GhWnd )
		return( FALSE );

	ShowWindow( GhWnd, CmdShow );
	UpdateWindow( GhWnd );

   mk_init();

	GameLoop = 1;

   mv_check();
   ddinit();

   w_dosbeallitasok(); // initqopen is itt van
	mv_main();

	// Ide mar nem szabad jonnie, kene valami uzenet!
   hiba( "WinMain vegere erkezett!" );

   Dummypvoid = &lpszCmdLine;

	return 0;
}

void mv_check( void ) {
	int elso = 1; // Hogy meg !Gameloop eseten is bejojjon:
	while( elso || !GameLoop ) {
		elso = 0;

		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if( !GetMessage( &msg, NULL, 0, 0 ) ) {
				exit( 0 );
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
}

// Ez itt torlendo:
void szovegki( int y, char* szoveg ) {
	HDC hdc = GetDC( GhWnd );
	TextOut( hdc, 30, y, szoveg, strlen( szoveg ) );
	ReleaseDC( GhWnd, hdc );
}

static LRESULT CALLBACK _export WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_CREATE:
			PostMessage( hWnd, WM_USER+1000, 0, 0 );
			break;
		case WM_ACTIVATEAPP:
			//GameLoop = wParam;
			break;
		case WM_USER+1000:
			//ShowCursor( FALSE );
			break;
		case WM_DESTROY:
			GameLoop = 0;
			//ShowCursor( TRUE );
			PostQuitMessage( 0 );
			break;
		// Billentyuzet uzenetek:
		case WM_KEYDOWN:
			mk_wmkeydown( wParam );
			break;
		case WM_KEYUP:
			mk_wmkeyup( wParam );
			break;
		case WM_CHAR:
			mk_wmchar( wParam );
			break;

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

void mv_exit( void ) {
	//PostQuitMessage( 0 );
	//mv_check();
   ddfinish();
	SendMessage( GhWnd, WM_CLOSE, 0, 0 );
	exit( 0 );
}


static double Startido = 0;

// 182*sec-et adja vissza idot tortresszel egyutt!
// d_stoppe.cpp-ben vannak megvalositva:
double mv_stopperido( void ) {
	return timeGetTime()*0.182 - Startido;
}

void mv_startstopper( void ) {
	Startido = timeGetTime()*0.182;
}

void hiba( char* text1, char* text2, char* text3 ) {
	FILE* h = fopen( "error.txt", "wt" );
	if( h ) {
		fprintf( h, "%s\n", text1 );
		if( text2 )
			fprintf( h, "%s\n", text2 );
		if( text3 )
			fprintf( h, "%s\n", text3 );

		fclose( h );
	}
	

	char text[200];
	strcpy( text, text1 );
	if( text2 ) {
		strcat( text, "   " );
		strcat( text, text2 );
	}
	if( text3 ) {
		strcat( text, "   " );
		strcat( text, text3 );
	}
	//MessageBox( NULL, text, "Message", MB_OK | MB_ICONINFORMATION);
	mv_exit();
}

void uzenet( char* text1, char* text2, char* text3 ) {
	hiba( text1, text2, text3 );
}

