
// Visszaadja, hogy hanyadik gombot nyomtak:
int dialog( char* text1, char* text2 = NULL, char* text3 = NULL,
			 char* text4 = NULL, char* text5 = NULL, char* text6 = NULL,
			 char* text7 = NULL, char* text8 = NULL, char* text9 = NULL,
			 char* text10 = NULL, char* text11 = NULL, char* text12 = NULL,
			 char* text13 = NULL, char* text14 = NULL, char* text15 = NULL,
			 char* text16 = NULL, char* text17 = NULL, char* text18 = NULL,
			 char* text19 = NULL, char* text20 = NULL, char* text21 = NULL,
			 char* text22 = NULL, char* text23 = NULL, char* text24 = NULL );

extern unsigned char Dialogszin;
extern unsigned char Dialogszelszin;
extern unsigned char Buttonszin;

struct box {
	int x1, y1, x2, y2;
};

int boxban( int x, int y, box boxa );

void kirajzbox( pic8* ppic, int x1, int y1, int x2, int y2,
						unsigned char felulet, unsigned char szele );
void kirajzbox( pic8* ppic, box boxa, unsigned char felulet, unsigned char szele );
