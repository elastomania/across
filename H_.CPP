#include	"all.h"

#define RECORD           0
#define PLAY             1

#define DSP_RESET        (SbIOaddr+0x06)
#define DSP_READ_DATA    (SbIOaddr+0x0a)
#define DSP_WRITE_DATA   (SbIOaddr+0x0c)
#define DSP_WRITE_STATUS (SbIOaddr+0x0c)
#define DSP_DATA_AVAIL   (SbIOaddr+0x0e)

#define MIXER_WRITE_INDEX   (SbIOaddr+0x4)
#define MIXER_WRITE_DATA 	(SbIOaddr+0x5)

#define TIME_CONSTANT    0x40
#define SPEAKER_ON       0xd1
#define DMA_8_BIT_DAC    0x14

#define DMA_MASK         0x0a
#define DMA_MODE         0x0b
#define BYTE_FLIPFLOP    0x0c

#define READ_CMD         0x48
#define SET_CMD          0x00
#define RESET_CMD        0x04

struct cimstrukt {
	unsigned char LSB, MSB, Page;
};

void DSP_Reset(void);
void DSP_Command(long);
void DMA_Set( cimstrukt* pcim);
void Alloc_Sound_Mem(unsigned buffsize);
void Init_Blaster( int dmapar, int intnum, int ioaddresspar );

#ifdef __cplusplus
	#define __CPPARGS ...
#else
	#define __CPPARGS
#endif


#ifndef WATCOM
	static void _interrupt _far (*Oldhandler)(__CPPARGS);
	void _interrupt _far handler(__CPPARGS);
#else
	static void (__interrupt __far *Oldhandler)();
	void __interrupt handler();
#endif

static long  SbIOaddr;
static long  DMA_Channel;
static long  Interrupt_Num;
static long  Interrupt_Controller;
static long  Interrupt_Mask;
static long  Interrupt_Vector;

static long  Page_Table[8] = {0x87, 0x83, 0x81, 0x82, -1, 0x8b, 0x89, 0x8a};

static long  DMAC_Addr;
static long  DMAC_Count;
static long  DMAC_Page;
static long  DMAC_Mode;
static long  DMAC_Mask;

static unsigned char LSB_Len, MSB_Len;
static unsigned char Mode;
static unsigned char Set_Mask, Reset_Mask;

//long  snd_mem;
static int Fejezdbe = 0, Befejeztem = 0, Hangmegy = 0;

cimstrukt Cim1, Cim2;

static unsigned char *Buffer = NULL, *Buffer1 = NULL, *Buffer2 = NULL;
static long Fizikai1 = 0, Fizikai2 = 0; // DOSEXT eseten fizikai cim
										// kitoltcimstrukthoz
static short* Shortbuffer = NULL;

static int ic( long l ) {
	if( l < -32000 || l > 32000 ) {
		printf( "Out of range!" );
		exit( 0 );
	}
	return (int)l;
}

static void setupinterrupt( void ) {
	_disable();
	Oldhandler = _dos_getvect( ic( Interrupt_Vector ) );
	_dos_setvect( ic(Interrupt_Vector) , handler);
	outp(ic(Interrupt_Controller+1), ic((inp(ic(Interrupt_Controller+1)) & ~Interrupt_Mask)));
	_enable();
}

/*static void ment( void ) {
	FILE* h = fopen( "irott.wav", "wb" );
	if( !h )
		hiba( "Nem nyitja wavot!" );

	// elso:
	for( int i = 0; i < 25000; i++ )
		Buffer[i] = 0;
	if( fwrite( Buffer, 1, 25000, h ) != 25000 )
		hiba( "Nem irta wavot!" );

	// kozep:
	for( i = 0; i < 10000; i++ ) {
		double alfa = double(i)/double(SND_BUFFER_LEN-2)*2.0*M_PI*100.0;
		Buffer[i] = 128+120*sin( alfa );
	}
	if( fwrite( Buffer, 1, 10000, h ) != 10000 )
		hiba( "Nem irta wavot!" );

	// vege:
	for( i = 0; i < 25000; i++ )
		Buffer[i] = 0;
	if( fwrite( Buffer, 1, 25000, h ) != 25000 )
		hiba( "Nem irta wavot!" );

	fclose( h );
}
*/

static void kitoltcimstrukt( cimstrukt* pcim, unsigned char* mut ) {
	unsigned long startaddr;

	#ifdef DOSX286
		if( mut != Buffer1 && mut != Buffer2 )
			hiba( "kitoltcimstrukt-ban mut != Buffer1 && mut != Buffer2!" );
		long fizikai = 0;
		if( mut == Buffer1 )
			fizikai = Fizikai1;
		else
			fizikai = Fizikai2;
		if( fizikai <= 0 )
			hiba( "kitoltcimstrukt-ban fizikai <= 0!" );
		startaddr = fizikai;
	#endif

	#ifdef DOS
		unsigned long startseg, startoff;
		startseg = (unsigned long) ((unsigned long)mut & 0xffff0000l) >> 16;
		startoff = (unsigned long) mut & 0x0000ffff;
		startaddr = startseg*16 + startoff;
	#endif

	#ifdef WATCOM
		startaddr = (long)mut;
	#endif

	pcim->LSB = (unsigned char)(startaddr & 0xffL);
	pcim->MSB = (unsigned char)((startaddr>>8) & 0xffL);
	pcim->Page = (unsigned char)((startaddr>>16) & 0xffL);
}

// 0, ha setup szerint nincs hang:
static int getparams( int* pcim, int* pirq, int* pdma ) {
	FILE* h = fopen( "setup.snd", "rt" );
	if( !h )
		uzenet( "Missing file!: ", "setup.snd" );
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		hiba( "Nem tudott olvasni targyat!" );
	fclose( h );

	char tipusnev[20] = "";
	if( sscanf( sor, "%s", tipusnev ) != 1 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	if( strcmpi( tipusnev, "none" ) == 0 ) {
		Hangenabled = 0;
		return 0;
	}
	if( strcmpi( tipusnev, "sb" ) != 0 )
		hiba( "Ismeretlen tipus setup.snd-ben!" );

	*pcim = 0;
	*pirq = 0;
	*pdma = 0;
	int szam = 0;
	if( (szam=sscanf( sor, "%s %i %d %d", tipusnev, pcim, pirq, pdma )) != 4 )
		hiba( "Targyban nem tudta feldolgozni sort!: ", sor );
	szam++;
	return 1;
}

static unsigned Buffsize = 0;

int starthang( unsigned buffsize ) {
	if( Hangmegy )
		hiba( "starthangban hangmegy!" );

	Buffsize = buffsize;

	int cim, irqszam, dmaszam;
	if( !getparams( &cim, &irqszam, &dmaszam ) ) {
		Hangenabled = 0;
		return 1;
	}

	Befejeztem = 0;
	/* Get the Sound Memory */
	Alloc_Sound_Mem( buffsize );

	kitoltcimstrukt( &Cim1, Buffer1 );
	kitoltcimstrukt( &Cim2, Buffer2 );

	callbackhang( Shortbuffer, Buffer2 );
	callbackhang( Shortbuffer, Buffer1 );

	/* Initialize All the Variables Associated with the SoundBlaster */
	Init_Blaster( cim, irqszam, dmaszam );

	/* Calculate Length Bytes */
	LSB_Len = (unsigned char)(buffsize-1);
	MSB_Len = (unsigned char)((buffsize-1)>>8);

	// Reset Mixer:
	outp(ic(MIXER_WRITE_INDEX), 0 );
	outp(ic(MIXER_WRITE_DATA), 0 );

	setupinterrupt();

	/* Reset the Digital Sound Processor */
	DSP_Reset();

	/* Prevent DMA from Happening yet */
	outp(ic(DMAC_Mask), Reset_Mask);

	/* Initialize DMA Registers */
	DMA_Set( &Cim2 );

	/* Start Playing the Sound */
	DSP_Command(TIME_CONSTANT);
	DSP_Command(164);
	DSP_Command(SPEAKER_ON);
	DSP_Command(DMA_8_BIT_DAC);
	DSP_Command(LSB_Len);
	DSP_Command(MSB_Len);
	outp(ic(DMAC_Mask), Set_Mask);    /* Start_DMA */
	Hangmegy = 1;
	return 1;
}

void stophang( void ) {
	if( !Hangmegy )
		return;
	Hangmegy = 0;
	Fejezdbe = 1;
	while( !Befejeztem )
		;

	/* Stop Any DMA Activity */
	outp(ic(DMAC_Mask), Reset_Mask);

	/* Remove the Interrupt Handler */
	_disable();
	outp(ic(Interrupt_Controller+1), (inp(ic(Interrupt_Controller+1)) | Interrupt_Mask));
	_dos_setvect(ic(Interrupt_Vector), Oldhandler);
	_enable();
}

static unsigned char* Tmp;
static unsigned char Tmppage, Tmpmsb, Tmplsb;

static int Lehetirni = 0;

//static Hatravan = 3;
#ifndef WATCOM
	void _interrupt _far handler(__CPPARGS) {
#else
	void __interrupt handler() {
#endif
	inp(ic(DSP_DATA_AVAIL));  /* Acknowledge Interrupt */

	if (Interrupt_Num>8) {
		outp(0x20,0x20);   /* Send EOI command to Master Interrupt Controller */
	}
	outp(ic( Interrupt_Controller), 0x20 ); /* Send EOI command to Interrupt Controller */

	if( Fejezdbe ) {
		Befejeztem = 1;
		return;
	}

	outp(ic(DMAC_Mask), Reset_Mask );
	outp(ic(DMAC_Page), Cim1.Page );   /* Output the Address Page */
	outp(ic(BYTE_FLIPFLOP), 0 );       /* Reset Flip Flop */
	outp(ic(DMAC_Addr), Cim1.LSB );    /* Output the Sound Buffer Address */
	outp(ic(DMAC_Addr), Cim1.MSB );
	outp(ic(DMAC_Count), LSB_Len );    /* Output the Sound Buffer Length */
	outp(ic(DMAC_Count), MSB_Len );
	outp(ic(DMAC_Mask), Set_Mask );    /* Start_DMA */


	while (inp(ic(DSP_WRITE_STATUS)) & 0x80); /* Restart the Digital Sound Processor */
	outp(ic(DSP_WRITE_DATA), DMA_8_BIT_DAC);

	while (inp(ic(DSP_WRITE_STATUS)) & 0x80);
	outp(ic(DSP_WRITE_DATA), LSB_Len);

	while (inp(ic(DSP_WRITE_STATUS)) & 0x80);
	outp(ic(DSP_WRITE_DATA), MSB_Len);

	Tmp = Buffer1;
	Buffer1 = Buffer2;
	Buffer2 = Tmp;

	Tmplsb = Cim1.LSB;
	Tmpmsb = Cim1.MSB;
	Tmppage = Cim1.Page;

	Cim1.LSB = Cim2.LSB;
	Cim1.MSB = Cim2.MSB;
	Cim1.Page = Cim2.Page;

	Cim2.LSB = Tmplsb;
	Cim2.MSB = Tmpmsb;
	Cim2.Page = Tmppage;

	Lehetirni = 1;

	//callbackhang( Shortbuffer, Buffer1 );
}

void hang( void ) {
	if( !Hangenabled || !Lehetirni )
		return;
	Lehetirni = 0;
	callbackhang( Shortbuffer, Buffer1 );
}

void nullazhangot( void ) {
	if( !Hangenabled )
		return;
	for( int i = 0; i < Buffsize; i++ ) {
		Buffer1[i] = 128;
		Buffer2[i] = 128;
	}
}

/****************************************************************************
*                            DSP_Command()                                  *
*                                                                           *
* Sends a command to the DSP port.                                          *
*                                                                           *
* Inputs: command - the command byte to send to port                        *
*                                                                           *
****************************************************************************/
void DSP_Command(long command)
{
   while (inp(ic(DSP_WRITE_STATUS)) & 0x80);

   outp(ic(DSP_WRITE_DATA), command);

   return;
}

/****************************************************************************
*                               DMA_Set()                                   *
*                                                                           *
* Sets up the address and length to start playing the sound buffer.         *
*                                                                           *
* Inputs: channel - the DMA channel Number                                  *
*         buffer  - pointer to the sound data                               *
*         length  - the length of the sound buffer                          *
*                                                                           *
****************************************************************************/
void DMA_Set( cimstrukt* pcim ) {
   _disable();

   outp(ic(DMAC_Page), pcim->Page );  /* Output the Address Page */

   outp(ic(BYTE_FLIPFLOP), 0);       /* Reset Flip Flop */

   outp(ic(DMAC_Addr), pcim->LSB );   /* Output the Sound Buffer Address */
   outp(ic(DMAC_Addr), pcim->MSB );

   outp(ic(DMAC_Count), LSB_Len);    /* Output the Sound Buffer Length */
   outp(ic(DMAC_Count), MSB_Len);

   outp(ic(DMAC_Mode), Mode);        /* Output the Mode */

// outp(DMAC_Mask, Set_Mask);    /* Output the Mask - to Start DMA */

   _enable();
}

/****************************************************************************
*                              DSP_Reset()                                  *
*                                                                           *
* Resets the Digital Sound Processor.                                       *
*                                                                           *
* Returns: -1=Could Not Reset                                               *
*           0=Reset Okay                                                    *
*                                                                           *
****************************************************************************/
void DSP_Reset(void)
{
   long i;

   outp(ic(DSP_RESET), 1);

   inp(ic(DSP_RESET));      /* Delay */
   inp(ic(DSP_RESET));
   inp(ic(DSP_RESET));
   inp(ic(DSP_RESET));

   outp(ic(DSP_RESET), 0);

   for (i=0; i<100; i++) {
	  if (inp(ic(DSP_DATA_AVAIL)) & 0x80) {
		 if (inp(ic(DSP_READ_DATA))==0xaa) {
			return;
		 }
	  }
   }
   uzenet( "Sound Blaster DSP did not reset succesfully!" );
}

/****************************************************************************
*                             Init_Blaster()                                *
*                                                                           *
* Sets up SoundBlaster Port Address, DMA address, Interrupt Settings, etc.  *
*                                                                           *
* Returns: -1=Okay                                                          *
*           0=Error                                                         *
*                                                                           *
****************************************************************************/
void Init_Blaster( int ioaddresspar, int intnum, int dmapar ) {
	SbIOaddr = ioaddresspar;

	DMA_Channel = dmapar;
	if( DMA_Channel<0 || DMA_Channel>7)
		hiba( "Init_Blaster-ben int dmapar out of range!" );

	Interrupt_Num = intnum;

	if (Interrupt_Num < 8) {
		Interrupt_Controller = 0x20;
		Interrupt_Mask = (1<<Interrupt_Num);
		Interrupt_Vector = Interrupt_Num + 0x08;
	}
	else {
		Interrupt_Controller = 0xa0;
		Interrupt_Mask = (1<<(Interrupt_Num-8));
		Interrupt_Vector = (Interrupt_Num-8) + 0x70;
   }

   long dMAC_Port;
   long dMAC2_Flag;
   if (DMA_Channel>=4) {                 /* Calculate Base Port */
	  dMAC_Port = DMA_Channel-4;
	  dMAC2_Flag = -1;
   } else {
	  dMAC_Port = DMA_Channel;
	  dMAC2_Flag = 0;
   }

   DMAC_Mask = DMA_MASK;                 /* Calculate Mask Port */
   if (dMAC2_Flag) {
	  DMAC_Mask <<= 1;
	  DMAC_Mask += 0xc0;
   }

   DMAC_Page = Page_Table[ic(DMA_Channel)];  /* Lookup Address Page Port */

   DMAC_Mode = DMA_MODE;                 /* Calculate Mode Port */
   if (dMAC2_Flag) {
	  DMAC_Mode <<= 1;
	  DMAC_Mode += 0xc0;
   }

   DMAC_Addr = dMAC_Port*2;              /* Calculate Address Port */
   if (dMAC2_Flag) {
	  DMAC_Addr <<= 1;
	  DMAC_Addr += 0xc0;
   }

   DMAC_Count = (dMAC_Port*2)+1;         /* Calculate Counter Port */
   if (dMAC2_Flag) {
	  DMAC_Count <<= 1;
	  DMAC_Count += 0xc0;
   }

   /* Calculate Reset Mask Value */
   Reset_Mask = (unsigned char)(RESET_CMD+dMAC_Port);

   /* Calculate Mode Setting */
   Mode = (unsigned char)(READ_CMD+dMAC_Port);

   /* Calculate the Mask Setting */
   Set_Mask = (unsigned char)(SET_CMD+dMAC_Port);
}

static unsigned char pagebyte( void* mut ) {
	unsigned char* pc = (unsigned char*)mut;
	return pc[2];
}

#ifdef DOS

static int egypage( void* p1, void* p2 ) {
	long offszet = FP_OFF( p1 );
	long szegmens = FP_SEG( p1 );
	long osszeg = (szegmens<<4)+offszet;
	int szeg1 = pagebyte( &osszeg );

	offszet = FP_OFF( p2 );
	szegmens = FP_SEG( p2 );
	osszeg = (szegmens<<4)+offszet;
	int szeg2 = pagebyte( &osszeg );

	if( szeg1 == szeg2 )
		return 1;
	else
		return 0;
}

void Alloc_Sound_Mem( unsigned buffsize ) {
	if( Buffer )
		hiba( "Alloc_Sound_Mem-ben Buffer != NULL!" );
	{ Buffer = NEW(unsigned char*) unsigned char[buffsize*3]; }
	if( !Buffer )
		uzenet( "memory starthang-ban!" );
	Buffer1 = Buffer;
	int eltolva = 0;
	if( !egypage( Buffer1, Buffer1+buffsize-1 ) ) {
		Buffer1 += buffsize;
		eltolva = 1;
		if( !egypage( Buffer1, Buffer1+buffsize-1 ) )
			hiba( "starthang-ban 2x eltolas!" );
	}
	Buffer2 = Buffer1 + buffsize;
	if( !egypage( Buffer2, Buffer2+buffsize-1 ) ) {
		if( eltolva )
			hiba( "starthang-ban 2x eltolas!" );
		Buffer2 += buffsize;
		if( !egypage( Buffer2, Buffer2+buffsize-1 ) )
			hiba( "starthang-ban 2x eltolas!" );
	}
	{ Shortbuffer = NEW(short*) short[buffsize]; }
	if( !Shortbuffer )
		uzenet( "memory!" );
}

#endif


#ifdef DOSX286

/*void Alloc_Sound_Mem( unsigned buffsize ) {
	unsigned short paragraf = 0;
	unsigned short selector = 0;
	unsigned short ret = DosAllocRealSeg( 1000, &paragraf, &selector );
	unsigned char* logmut = (unsigned char*)MK_FP( selector, 0 );
	logmut[0] = 0;
	logmut[1] = 1;
	logmut[2] = 2;
	logmut[3] = 3;
	logmut[4] = 4;
	logmut[5] = 5;

	unsigned short selector2 = NULL;
	DosMapRealSeg( paragraf, 1000, &selector2 );
	unsigned char* fizmut = (unsigned char*)MK_FP( selector2, 0 );


	ret++;
} */

static int egypagefizikai( long a, long b ) {
	a = a & 0xffffl;
	b = b & 0xffffl;
	if( a == b )
		return 1;
	else
		return 0;
}

static void Alloc_Sound_Mem( unsigned buffsize ) {
	if( buffsize > 15000 )
		hiba( "Alloc_Sound_Mem-ben buffsize > 15000!" );
	unsigned short paragraf = 0;
	unsigned short selector = 0;
	DosAllocRealSeg( 4*buffsize, &paragraf, &selector );
	Buffer = (unsigned char*)MK_FP( selector, 0 );
	long fizikai = ((long)paragraf)*16;
	if( !egypagefizikai( fizikai, fizikai+2*buffsize-1 ) ) {
		fizikai += 2*buffsize;
		Buffer += 2*buffsize;
	}
	Buffer1 = Buffer;
	Buffer2 = Buffer+buffsize;
	Fizikai1 = fizikai;
	Fizikai2 = fizikai+buffsize;
	{ Shortbuffer = NEW(short*) short[buffsize]; }
	if( !Shortbuffer )
		uzenet( "memory!" );
}


#endif

#ifdef WATCOM

static int egypage( void* p1, void* p2 ) {
	// (WATCOMOS)
	p1 = (void*)(((long)p1)&0x0ffff0000);
	p2 = (void*)(((long)p2)&0x0ffff0000);

	if( p1 == p2 )
		return 1;
	else
		return 0;
}

/****************************************************************************
*                           Alloc_Sound_Mem()                               *
*                                                                           *
* Allocates Memory in Lower Meg Using DOS Protected Mode Interface (DPMI)   *
* It Makes sure it is all contained within the same Page.                   *
*                                                                           *
* Returns: -1=Okay                                                          *
*           0=Error                                                         *
*                                                                           *
****************************************************************************/
void Alloc_Sound_Mem( unsigned buffsize ) {
	union    REGS regs;

	/* Allocate Memory in Lower Meg Using DOS Protected Mode Interface (DPMI) Command */
	regs.w.ax = 0x100;           /* Allocate Memory */
	regs.w.bx = short(buffsize*3/16+5);  /* Number of Paragraphs of Memory */
	int386(0x31, &regs, &regs);
	if (regs.x.cflag)
		uzenet( "Could Not Allocate DOS Memory for Sound Buffer" );
	long parag = regs.w.ax;
	Buffer = (unsigned char*)(parag<<4);

	if( !Buffer )
		uzenet( "memory starthang-ban!" );
	Buffer1 = Buffer;
	int eltolva = 0;
	if( !egypage( Buffer1, Buffer1+buffsize-1 ) ) {
		Buffer1 += buffsize;
		eltolva = 1;
		if( !egypage( Buffer1, Buffer1+buffsize-1 ) )
			hiba( "starthang-ban 2x eltolas!" );
	}
	Buffer2 = Buffer1 + buffsize;
	if( !egypage( Buffer2, Buffer2+buffsize-1 ) ) {
		if( eltolva )
			hiba( "starthang-ban 2x eltolas!" );
		Buffer2 += buffsize;
		if( !egypage( Buffer2, Buffer2+buffsize-1 ) )
			hiba( "starthang-ban 2x eltolas!" );
	}
	if( Shortbuffer )
		hiba( "Alloc_Sound_Mem-ben Shortbuffer != NULL!" );
	{ Shortbuffer = NEW(short*) short[buffsize]; }
	if( !Shortbuffer )
		uzenet( "memory!" );
}

#endif

// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,
// MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER, MIXER,

// 0.0 - 1.0 kozott
void setvolume( double hangero ) {
	if( !Hangenabled )
		return;
	if( hangero < 0.0 || hangero > 1.0 )
		hiba( "setvolume-ban hangero < 0.0 || hangero > 1.0!" );
	outp(ic(MIXER_WRITE_INDEX), 4 );
	unsigned char adat = 1.5 + hangero*14.0;
	adat += adat*16;
	outp(ic(MIXER_WRITE_DATA), adat );
}



