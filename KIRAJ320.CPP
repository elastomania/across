#include    "all.h"

vekt2 Fejr( 0, 0 );

double  Hatarx1,
        Hatarx2,
        Hatary1,
        Hatary2;

void resetkirajzol( void ) {
    Hatarx1 = 2.0;
    Hatary1 = 2.0;
    Hatarx2 = Displaysizex-3.0;
    Hatary2 = Displaysizey-3.0;
}

static void forditkepet( pic8* ppic ) {
    for( int y1 = 0; y1 < ppic->getysize()/2; y1++ ) {
        int y2 = ppic->getysize()-1-y1;
        for( int x = 0; x < ppic->getxsize(); x++ ) {
            unsigned char tmp = ppic->gpixel( x, y1 );
            ppic->ppixel( x, y1, ppic->gpixel( x, y2 ) );
            ppic->ppixel( x, y2, tmp );
        }
    }
}

// v1 iranyabol v2 fele interpolal:
static vekt2 interpolal( vekt2 v1, vekt2 v2, double x ) {
    vekt2 vissza = v1+(v2-v1)*x;
    return vissza;
}

// Meg nem affin koordok:
static void kidoboz( vekt2 a, vekt2 b, pic8* ppic8, double szelesseg,
        kiskep* pk, double tulloga, double tullogb, int tukor = 0 ) {
    vekt2 vegys = egys( b-a );
    b = b + vegys*tullogb;
    a = a - vegys*tulloga;
    vekt2 v = b-a;
    vekt2 v2fel;
    if( tukor )
        v2fel = forgatas90fokkal( vegys ) * szelesseg;
    else
        v2fel = forgatasminusz90fokkal( vegys ) * szelesseg;
    a = a-v2fel;

    a.x *= Aranyx;
    a.y *= Aranyy;
    v.x *= Aranyx;
    v.y *= Aranyy;
    v2fel.x *= Aranyx;
    v2fel.y *= Aranyy;
    dobozki( ppic8, pk, v, v2fel*2.0, a );
}

// sugar meg nincs megszorozva:
// Meg nem affin koordok:
static void kidobozkerek( vekt2 r, double sugar, double alfa,
        pic8* ppicbuffer, kiskep* pk, int tukor = 0 ) {
    vekt2 vfel( cos( alfa ) * sugar, sin( alfa ) * sugar );
    if( tukor )
        kidoboz( r+vfel, r-vfel, ppicbuffer, sugar, pk, 0.0, 0.0, tukor );
    else
        kidoboz( r-vfel, r+vfel, ppicbuffer, sugar, pk, 0.0, 0.0, tukor );
}


static double Mx = -1, My = -1;
static vekt2 Mi, Mj, Mr;

static void kitag( pic8* ppic8, kiskep* pk, int x1, int y1, int x2, int y2 ) {
    vekt2 r = Mi*(x1-Mx) + Mj*(My-y1) + Mr;
    vekt2 u = Mi*(x2-x1);
    vekt2 v = Mj*(y1-y2);

    r.x *= Aranyx;
    r.y *= Aranyy;
    u.x *= Aranyx;
    u.y *= Aranyy;
    v.x *= Aranyx;
    v.y *= Aranyy;
    dobozki( ppic8, pk, u, v, r );
}

static void kigyuru( pic8* ppic8, int x, int y, unsigned char szin ) {
    #ifdef TEST
        if( x < 1 || x >= ppic8->getxsize()-1 ||
            y < 1 || y >= ppic8->getysize()-1 )
            hiba( "kigyuru-ben x, vagy y lelog!" );
    #endif
    unsigned char *sor = ppic8->getptr( y-1 );
    sor[x-1] = szin; sor[x] = szin; sor[x+1] = szin;
    sor = ppic8->getptr( y+1 );
    sor[x-1] = szin; sor[x] = szin; sor[x+1] = szin;
    ppic8->ppixel( x-1, y, szin );
    ppic8->ppixel( x+1, y, szin );
}

static void felhokfel( int x, int y, pic8* ppicbuffer, pic8* pfelho,
                            double lassit ) {
    int kerx = -x*lassit;
    int kery = -y*lassit;
    // Vegigmegy kepernyon:
    int kerxsize = pfelho->getxsize();
    kerx %= kerxsize;
    kerx -= kerxsize; // Lehet, hogy enelkul is negativ?
    while( kerx < Displaysizex ) {
        int kerysize = pfelho->getysize();
        kery %= kerysize;
        kery -= kerysize; // Lehet, hogy enelkul is negativ?
        while( kery < Displaysizey ) {
            blt8( ppicbuffer, pfelho, kerx, kery );

            kery += kerysize;
        }
        kerx += kerxsize;
    }
}

double Mo_y, Mo_bal, Mo_dx;

void kirajzol320( st320* pst, double t, palyaleiro* pleiro, int view,
        int timeki, double baljobb, double ugrasnagysag, int ugras1volt,
        double forgas ) {
    int sx = -1, sy = -1;
    unsigned char** ppc = lockbackbuffer( &sx, &sy );
    pic8* ppicbuffer;
    { ppicbuffer = NEW(pic8*) pic8( sx, sy, ppc ); }

    int res = 0;
    if( sy == 200 )
        res = 1;
    if( sy == 240 )
        res = 2;
    if( sy == 400 )
        res = 3;
    if( sy == 480 )
        res = 4;
    if( !res )
        hiba( "kirajzol320-ban !res!" );

    #ifdef TEST
        if( baljobb < 0 || baljobb > 1.0 )
            hiba( "baljobb < 0 || baljobb > 1.0!" );
        if( forgas < 0.0 || forgas > 1.0 )
            hiba( "forgas < 0.0 || forgas > 1.0!" );
    #endif
    // Ha kell affinitassal torodni:
    int vanforgas = 0;
    Affinitas = 0;
    if( forgas < 0.999 ) {
        vanforgas = 1;
        // forgast 0, 1 intervallumbol atkonvertaljuk -1, 1 intervallumba:
        forgas = -cos( forgas*K_pi );
    }


    // Kiszamoljuk kepernyo bal also sarkat:
    vekt2 motorkozep = Kor1.r;

    vekt2 sarok( motorkozep.x-(Mo_bal+baljobb*Mo_dx),
                motorkozep.y-Mo_y );
    /*vekt2 sarok( motorkozep.x-(Motorhelyex0+baljobb*Motorhelyedx),
                motorkozep.y-Motorhelyey );*/
    vekt2 origo = pst->origo;

    int x2 = (sarok-origo).x*Aranyx;
    int y2 = (sarok-origo).y*Aranyy;

    int xucso = x2+Displaysizex-1;
    int yucso = y2+Displaysizey-1;

    // Berakjuk bufferbe hatteret:
    int hatterx = Korny->hatteralapx + Korny->lassitas*x2;
    int hattery = Korny->hatteralapy + Korny->lassitas*y2;
    blt8( ppicbuffer, pleiro->peg, -hatterx, -hattery );

    // Felrakjuk felhoket:
    //felhokfel( x2, y2, pleiro->ppicbuffer, pleiro->pfelho2, 0.3 );
    if( Korny->leiroazonosito == 0 )
        felhokfel( x2, y2, ppicbuffer, pleiro->pfelho, 0.5 );

    // Rarakjuk palyat:
    pleiro->pratesz->teddra( x2, y2, ppicbuffer, Korny->st.pmfugg );

    vekt2 vsarok( 0, 0 );
    vekt2 vmotorhelye;
    // View ablak abszolut leptekben mert elso es utolso koordja, hogy
    // ehhez tudjunk viszonyitani, el kell osztani erteket Configt.zoom-mal:
    int vx1 = -100, vy1 = -100, vx2 = -100, vy2 = -100;

    /*vmotorhelye.x = (Motorhelyex0+baljobb*Motorhelyedx)*
                            Config.zoom*double(Config.viewxsize)/320.0;
    vmotorhelye.y = Motorhelyey*Config.zoom*double(Config.viewysize)/200.0;
    */
    //vekt2 sarok( motorkozep.x-(xdbal+baljobb*xddx),
    //  motorkozep.y-ydsize*0.32 );
    vmotorhelye.x = (Mo_bal+baljobb*Mo_dx)*
                            Config.zoom*double(Config.viewxsize)/sx;
    vmotorhelye.y = Mo_y*Config.zoom*double(Config.viewysize)/sy;
    vsarok = motorkozep-vmotorhelye;

    int x2v = (vsarok.x-origo.x)*Aranyx;
    int y2v = (vsarok.y-origo.y)*Aranyy;

    x2v /= Config.zoom;
    y2v /= Config.zoom;

    vx1 = x2v;
    vy1 = y2v;
    vx2 = vx1+pleiro->pvpic8f->getxsize()-1;
    vy2 = vy1+pleiro->pvpic8f->getysize()-1;

    if( view ) {
        tolpic8f( pst->vxelozo, pst->vyelozo, x2v, y2v, pleiro->pvpic8f,
                            pst->pmviz, pst->pmfugg,
                            pleiro->vieweloter, pleiro->viewhatter );

        pst->vxelozo = x2v;
        pst->vyelozo = y2v;

        // Ranyomatjuk view bufferre view-t:
        int xv0 = pleiro->pvpic8f->x0;
        int yv0 = pleiro->pvpic8f->y0;
        for( int y = 0; y < Config.viewysize; y++ ) {
            unsigned char *pviewsor = pleiro->pvpic8f->getcim( xv0, yv0+y );
            unsigned char *pbuffersor = pleiro->pviewbuffer->getptr( y );
            memcpy( pbuffersor, pviewsor, Config.viewxsize );
        }
    }


    if( view ) {
        // View ablakba motoros berajzolasa:
        int x = (motorkozep.x-origo.x)*Aranyx/Config.zoom;
        int y = (motorkozep.y-origo.y)*Aranyy/Config.zoom;
        kigyuru( pleiro->pviewbuffer, x-vx1, y-vy1,
                                    pleiro->viewmotorindex );
    }

    // Spriteok kirajzolasa, de view-ba csak kesobb kerulnek:
    Pszakt->felsorolasresetszak();
    szakasz* psz;

    while( (psz=Pszakt->getnextszak()) != NULL ) {
        targy* pt = psz->ptargy;
        if( !pt->aktiv )
            continue;
        targytipus* ptt = &pleiro->ptt->targytipusok[pt->szam];
        // Egesz koordinatak kiszamolasa:
        vekt2 rrr = spritehely( psz, pt, t )-origo;
        int xkozep = rrr.x*Aranyx;
        int ykozep = rrr.y*Aranyy;
        // Potty kirajzolasa View ablakba:
        if( view ) {
            int x = xkozep/Config.zoom;
            int y = ykozep/Config.zoom;

            if( x > vx1 && x < vx2 && y > vy1 && y < vy2 ) {
                kigyuru( pleiro->pviewbuffer,
                        x-vx1, y-vy1, ptt->pottyindex );
            }
        }

        // Normalis sprite kirajzolas:
        long l = t*ptt->frameseb;
        int index = int(l%ptt->frameszam);
        pic8* ppic = ptt->spriteok[index];
        int x = xkozep-ptt->dx;
        int y = ykozep-ptt->dy;

        int xs = ppic->getxsize();
        int ys = ppic->getysize();

        if( x+xs >= x2 && y+ys >= y2 && x <= xucso && y <= yucso ) {
            // Van bent belole kepben:
            blt8( ppicbuffer, ppic, x-x2, y-y2 );
        }
    }

    // Most jon motoros kirajzolasa:
    vekt2 p_k1 = (Kor2.r-sarok);
    vekt2 p_k2 = (Kor4.r-sarok);

    int elsokerekrogton = 1;
    int hatsokerekrogton = 1;

    if( vanforgas ) {
        if( (forgas > 0.0 && !Hatra) || (forgas <= 0.0 && Hatra) )
            elsokerekrogton = 0;
        else
            hatsokerekrogton = 0;
    }

    // Kerekek:
    if( elsokerekrogton )
        kidobozkerek( p_k1, Kor2.sugar, Kor2.alfa, ppicbuffer, pleiro->pkiskerek );
    if( hatsokerekrogton )
        kidobozkerek( p_k2, Kor4.sugar, Kor4.alfa, ppicbuffer, pleiro->pkiskerek );

    vekt2 kozep = Kor1.r-sarok;
    vekt2 jobbra = vekt2( cos( Kor1.alfa ), sin( Kor1.alfa ) );
    vekt2 fel = forgatas90fokkal( jobbra );

    if( vanforgas ) {
        //if( forgas > 0.2 )
        //  semmi();
        Affinitas = 1;
        setaffinitas( kozep, jobbra, forgas, Aranyx, Aranyy );
    }

    if( Hatra ) {
        jobbra = Vekt2null - jobbra;
        vekt2 tmpv = p_k1;
        p_k1 = p_k2;
        p_k2 = tmpv;
    }

    Mx = 390, My = 420;
    double malfa = 0.62;
    double mmeret = 0.0045;
    Mi = jobbra*(mmeret*cos(malfa))+fel*(mmeret*sin(malfa));
    Mj = forgatas90fokkal( Mi );
    if( Hatra )
        Mj = Vekt2null - Mj;
    Mr = kozep;

    // Elso felfuggesztes:
    //vekt2 p_fogantyu = Mi*(330-Mx) + Mj*(My-335) + Mr;
    vekt2 p_fogantyu = Mi*(336-Mx) + Mj*(My-335) + Mr;
    p_fogantyu = interpolal( p_k1, p_fogantyu, 1.235 );
    kidoboz( p_k1, p_fogantyu, ppicbuffer, 0.06,
        pleiro->pkiselsorud, 0.05, 0.03 );

    // Hatso felfuggesztes:
    vekt2 p_darab2 = Mi*(370-Mx) + Mj*(My-520) + Mr;
    kidoboz( p_darab2, p_k2, ppicbuffer, 0.06,
        pleiro->pkishatsorud, 0.0, 0.1 );

    // Motor teste:
    kitag( ppicbuffer, pleiro->pkisa, 272, 275, 347, 334 );
    kitag( ppicbuffer, pleiro->pkisb, 287, 334, 404, 558 );
    kitag( ppicbuffer, pleiro->pkisc, 404, 369, 451, 530 );
    kitag( ppicbuffer, pleiro->pkisd, 451, 289, 568, 538 );
    kitag( ppicbuffer, pleiro->pkise, 568, 474, 626, 529 );

    // Fej:
    double fejalfa = 0.20; // 0.0;
    if( Hatra )
        fejalfa = 0 - fejalfa;
    if( Hatra )
        kidobozkerek( Fejr-sarok, Fejsugar, Kor1.alfa+fejalfa,
                ppicbuffer, pleiro->pkisfej, 1 );
    else
        kidobozkerek( Fejr-sarok, Fejsugar, Kor1.alfa+fejalfa,
                ppicbuffer, pleiro->pkisfej );

    // Kar ket reszletben:
    //vekt2 p_vall = Mi*(540-Mx) + Mj*(My-324) + Mr;
    vekt2 p_vall = Mi*(532-Mx) + Mj*(My-320) + Mr;
    vekt2 p_kezfej = p_fogantyu;

    // Ugras belekombinalasa kar kirajzolasaba:
    // p_kezfej-et mozditjuk el szukseges mertekben:
    if( ugrasnagysag > 0.0001 ) {
        ugrasnagysag = 1.0-ugrasnagysag;
        int fellendit = 1; // kar kiteresenek iranya
        if( (ugras1volt && !Hatra) || (!ugras1volt && Hatra) )  // XOR
            fellendit = 0;
        // alfa es hosszit kiszamolasa:
        double alfa = 0.0;
        double hosszit = 0.1;
        if( fellendit ) {
            // KAR FELFELE LENDUL KI:
            double hatar = 0.25;
            double maxalfa = 2.7;
            double maxhosszit = -0.3;
            if( ugrasnagysag < hatar ) {
                // Kilendit:
                alfa = maxalfa*ugrasnagysag/hatar;
                hosszit = maxhosszit*ugrasnagysag/hatar+1.0;
            }
            else {
                // Visszaenged:
                double mertek = 1.0-(ugrasnagysag-hatar)/(1.0-hatar);
                alfa = maxalfa*mertek;
                hosszit = maxhosszit*mertek+1.0;
            }
        }
        else {
            // KAR LEFELE LENDUL KI:
            double hatar = 0.2;
            double maxalfa = -1.6;
            double maxhosszit = 0.15;
            if( ugrasnagysag < hatar ) {
                // Kilendit:
                alfa = maxalfa*ugrasnagysag/hatar;
                hosszit = maxhosszit*ugrasnagysag/hatar+1.0;
            }
            else {
                // Visszaenged:
                double mertek = 1.0-(ugrasnagysag-hatar)/(1.0-hatar);
                alfa = maxalfa*mertek;
                hosszit = maxhosszit*mertek+1.0;
            }
        }
        // alfa es hosszit alapjan p_kezfej forgatasa es nyujtasa vall korul:
        vekt2 kar = p_kezfej-p_vall;
        if( !Hatra )
            kar.forgatas( -alfa );
        else
            kar.forgatas( alfa );
        kar = kar*hosszit;
        p_kezfej = p_vall + kar;
    }

    double alkarhossz = 0.385;
    //double felkarhossz = 0.45;
    double felkarhossz = 0.41;
    vekt2 p_konyok;
    if( Hatra )
        p_konyok = ketkormetszete( p_kezfej, p_vall,
                                        alkarhossz, felkarhossz );
    else
        p_konyok = ketkormetszete( p_vall, p_kezfej,
                                        felkarhossz, alkarhossz );

    if( p_konyok.x < -10000000.0 && p_konyok.x > -10000002.0 )
        p_konyok = (p_kezfej + p_vall) * 0.5;
    else {
        #ifdef TEST
        double alhossz = abs( p_konyok-p_kezfej );
        if( fabs( alhossz - alkarhossz ) > 0.0001 )
            hiba( "alhossz - alkarhossz ) > 0.0001!" );
        #endif
    }


    //kidoboz( p_konyok, p_vall, ppicbuffer, 0.14,
    //  pleiro->pkisfelkar, 0.08, 0.1, !Hatra );
    //kidoboz( p_kezfej, p_konyok, ppicbuffer, 0.086,
    //  pleiro->pkisalkar, 0.08, 0.1, Hatra );

    kidoboz( p_konyok, p_vall, ppicbuffer, 0.11,
        pleiro->pkisfelkar, 0.08, 0.1, !Hatra );
    kidoboz( p_kezfej, p_konyok, ppicbuffer, 0.076,
        pleiro->pkisalkar, 0.08, 0.1, Hatra );

    Affinitas = 0;

    // Kerekek:
    if( !elsokerekrogton || !hatsokerekrogton ) {
        if( Hatra ) {
            // Visszacserelem kerekek koordjait:
            vekt2 tmpv = p_k1;
            p_k1 = p_k2;
            p_k2 = tmpv;
        }
        if( !elsokerekrogton )
            kidobozkerek( p_k1, Kor2.sugar, Kor2.alfa, ppicbuffer, pleiro->pkiskerek );
        if( !hatsokerekrogton )
            kidobozkerek( p_k2, Kor4.sugar, Kor4.alfa, ppicbuffer, pleiro->pkiskerek );
    }

    // torlendo:
    // Be kell meg allitani affinitast!:
    /*vekt2 r_p( 141.564, 94.6185 );
    vekt2 u_p( -8.5157, 12.1406 );
    vekt2 v_p( -11.4608, -5.58251 );
    Affinitas = 1;
    vekt2 irany = vekt2( 45.6169, -17.0599 );
    vekt2 tmpkozep = vekt2( 100, 100 );
    setaffinitas( tmpkozep, irany, 0.204 );
    dobozki( pleiro->ppicbuffer, pleiro->pkisa, u_p, v_p, r_p );
    Affinitas = 0;
    */

    // Idokiiras:
    if( timeki ) {
        kidigit( Korny->legjobbido, t, pleiro->idoszin1, pleiro->idoszin2,
                                    ppicbuffer, res );
    }

    if( view ) {
        // Ranyomatjuk view bufferre view-t:
        unsigned char keret = Korny->pleiro->keretindex;
        int viewxorig = Config.viewxorig + Config.viewxtolas*baljobb;
        int viewyorig = 1;
        for( int y = 0; y < Config.viewysize; y++ ) {
            unsigned char *pviewsor = pleiro->pviewbuffer->getptr( y );
            unsigned char *pbuffersor = ppicbuffer->getptr( y+viewyorig );
            pbuffersor += viewxorig;
            memcpy( pbuffersor, pviewsor, Config.viewxsize );
            // Keret oldalsok:
            *(pbuffersor-1) = keret;
            *(pbuffersor+Config.viewxsize) = keret;
        }
        // Keret also:
        unsigned char *pbuffersor = ppicbuffer->getptr( viewyorig-1 );
        pbuffersor += viewxorig-1;
        memset( pbuffersor, keret, Config.viewxsize+2 );
        // Keret felso:
        pbuffersor = ppicbuffer->getptr( viewyorig+Config.viewysize );
        pbuffersor += viewxorig-1;
        memset( pbuffersor, keret, Config.viewxsize+2 );
    }

    hang();

    //waitforcathode();
    unlockbackbuffer();

    // Mentes lemezre:
    extern int Legkozment;
    if( Legkozment ) {
        Legkozment = 0;
        int i = 0;
        while( 1 ) {
            char filenev[20];
            if( i < 10 )
                sprintf( filenev, "snap0%d.pcx", i );
            else
                sprintf( filenev, "snap%d.pcx", i );
            if( access( filenev, 0 ) != 0 ) {
                forditkepet( ppicbuffer );
                ppicbuffer->save( filenev, pleiro->pal->tomb );
                forditkepet( ppicbuffer );
                break;
            }
            i++;
        }
    }

    DELETE ppicbuffer; }
    ppicbuffer = NULL;
}

