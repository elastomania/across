#include	<stdio.h>
#include	<stdlib.h>
#include	<io.h>
#include	<dos.h>
#include	<conio.h>
#include	<mem.h>
#include	<string.h>
#include	<alloc.h>
#include	<math.h>
#include	<time.h>

#define DOS

#ifdef DOSX286
	#include	<phapi.h>
#endif

#include	<graphics.h>

#define TEST

#include	"..\main.h"
#include	"..\pic8f.h"
#include	"..\pic8.h"
#include	"..\picl8ves.h"
#include	"..\picmcga.h"

//#include	"..\pal.h"

// EGAVGA-as verziohoz:

//#include	"..\p-sos.h"

#include	"..\mouse.h"

#include	"..\vekt2.h"
#include	"..\kor.h"
#include	"..\beallit.h"
#include	"..\leptet.h"
#include	"..\utkozes.h"
#include	"..\adatok.h"


#include	"..\targy.h"
#include	"..\edtargy.h"
#include	"..\szakasz.h"
#include	"..\utkozes2.h"

#include	"..\editor.h"

#include	"..\magassag.h"
#include	"..\tegla.h"

#include	"..\recorder.h"

#include	"..\egavga.h"

// Kozos:

#include	"..\graph.h"
#include	"..\kirajzol.h"
#include	"..\hatter.h"
//#include	"..\billint.h"

#include	"..\wav.h"
#include	"..\hang.h"
#include	"..\hanghigh.h"
#include	"..\tulminta.h"

#include	"..\config.h"

#include	"..\kiskep.h"
#include	"..\ratesz.h"
#include	"..\leiro.h"

#include	"..\bgi.h"

#include	"..\kozos.h"
#include	"..\teljes.h"

#include	"..\qopen.h"

#include	"..\state.h"

//#include	"..\stopper.h"

#include	"..\hordo.h"

#include	"..\newuj.h"

#include	"..\m_dd.h"
#include	"..\imi_vesa.h"
#include	"..\m_k.h"
#include	"..\m_v.h"
#include	"..\d_nempub.h"
