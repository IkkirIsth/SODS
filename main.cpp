/*
Shores Offline Ship Designer  (by Ikkir, not officially affiliated with Shores of Hazeron)

This is a program I made in order to learn OpenGL (and to a lesser extent SDL 1.3) sometime during 2012 / 2013, when Shores of Hazeron didnt have an offline ship designer and was free to play.
It featured room copy/paste and rotate, as well as floodfill and free-drawing (though the actual Hazeron ship engine didn't support all that it could do at the time, so it was limited in function to comply with game standards)

Notable issues/learned stuff: A lot, really.  Mostly learned a bunch of UI stuff and resolution things, notably that I should deal with multiple resolutions at the start instead of trying to patch it back in later
(would have gone with a virtual resolution for my math stuff)

This designer is no longer supported by actual Shores of Hazeron so is simply presented AS IS, as part of showing the old learning process.  Notable things inside:
point-in-poly and point-on-line stuff are useful in a lot of places
general drawing setup for GL 1.4 (really old version that runs on everything)
various other things in the un-refactored mess.
the start of actual file management (I think thats in this version?), though I didnt get in part saving and re-using, which would have been a next-major-feature.

Requirements: SDL (various parts: SDL and SDL_Image) and GL
*/


//DONE copydeck, consider changing .part deck destinationcopy
//DONE config.txt
//look into vertex continuing crash on laptop
//DONE finishroom/part pastein
//DONE deletefix
//DONE part-edge place
//DONE redfill
//DONE moveroom
//DONE capchair
//<sensors>, hull hp, wormhole/warp power, min-techlevel buttons.
//think of other possible options and find bugs.  build some designs.
//DONE rnum overlay, add option for this
//DONE savestuff
//vehicle placing <need values>
//DONE vertex
//.part safety for adding rooms to it that overlap
//.part saving and loading, and multiple .parts to select from.
//changing window size and fullscreen mode. will make it look a lot better. might be difficult, might not be.
//ui smoothing, at the very least confirm boxes for delete, and a load/save dialog... poss. tooltips
//painting mode for rooms. find a way to make this easy if possible.

//part copy/paste works. need ability to change center on part & UI functionality.
//need to be able to delete rooms out of the part.  may be awhile later due to selectability.
//with the rooms able to overlay each other, need better testlegal stuff.
//also, need to fix testlegal for the bad angles, since it only tests first/last right now.

//special: compare itoa, sprintf, and stringstream for usage and time.  make my own and test against, for science!
//what it does: dont even need display, just a timer (I think).  get time, do, get endtime, fprintf.
/* ref
If you need fast int-to-string, you can write a quick function that's faster than any generic library call;
just keep taking %10 and /10, and fill a static buffer from the back, returning a pointer to the highest digit.
Don't use sprintf, this is a security flaw. – fuzzyTew Sep 6 '11 at 2:44
*/

#include "methods.h"
#define VERSION "Shores of Hazeron Offline Design Studio v0.34"



int main(int argc, char **argv)
{
    mousedrag.Drag_On = false;
    mousemain.xpos = 150;
    mousemain.ypos = 150;
    mousemain.axpos = 150;
    mousemain.aypos = 150;
    mousemain.lkeydown = 0;
    mousemain.rkeydown = 0;
    roomselected = true;
    //currentroom = 0;
    currentdeck = 28;
//    currentequip = 0x00;
    currentmod = T_PULSE;
    currentrot = 0;
    medimod = 0x0000;
    //globalequip = 1;
    sr_cx = 0x00;
    sr_cy = 0x00;
    mode = !mode;
    //brush = 3;

    for (int r = 0; r < 22; r++)
    {
        for (int n = 0; n < 256; n++)
        {
            for (int d = 0; d < 57; d++)
            {
                deckrooms[r][n][d] = NULL;
            }
        }
    }
    ui_main = new UI_WINDOW();
    for (int j = 0; j <numrooms; j++)
    {
       rooms_g[j] = false;
    }
    for (int j = 0; j < 161; j++)
    {
        for (int i = 0; i < 161; i++)
        {
            for (int n = 0; n < 57; n++)
            {
                equipgrid[j][i][n] = NULL; // all initialized to NULL
            }
        }
    }


    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
        SDL_Surface *icon = SDL_LoadBMP("SODS_icon.bmp");
    SDL_WM_SetIcon(icon, NULL);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);


    if ( SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_OPENGL) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        //SDL_Quit();
        exit(2);
    }



    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

    SDL_WM_SetCaption(VERSION, NULL);
    //SDL_WM_SetCaption("Shores of Hazeron Offline Design Studio v0.321", NULL);
    SDL_ShowCursor(SDL_DISABLE);
    InitGL(SCREEN_WIDTH, SCREEN_HEIGHT);
    loadconfigfile();


    bool quit = false;

    while ( quit == false )
    {
        //mainship.pstats();
       DrawGLScene();

       setssf();


        /* This could go in a separate function */

        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
            quit = !handlekeyevent(event);
        }
        if (hangingpart && uimode != X_MOVEROOM_START)
        {
            moveroomend(0, 400, 400, currentdeck);
        }


        dragscroll (mousedrag);
        if (mousemain.lkeydown == true)
        {
            if (uimode == M_OPTIONS)
            {
                GrabOptions(quit);
            }
            else
            if (mousemain.xpos >=804)
            {
                if (hangingpart)
                {
                    moveroomend(0, 400, 400, currentdeck);
                }
                if (uimode == X_UI_DELC)
                {
                    uimode = X_NONE;
                    if (mousemain.xpos >=842 &&
                        mousemain.xpos <=890 &&
                        mousemain.ypos >=42 &&
                        mousemain.ypos <=60)
                        cleardeck(currentdeck);
                    else
                    if (mousemain.xpos >=891 &&
                        mousemain.xpos <=940 &&
                        mousemain.ypos >=42 &&
                        mousemain.ypos <=60)
                        cleardesign();
                }
                else
                if (uimode == X_PART_DELC)
                {
                    uimode = X_NONE;
                    uimode = X_NONE;
                    if (mousemain.xpos >=842 &&
                        mousemain.xpos <=890 &&
                        mousemain.ypos >=622 &&
                        mousemain.ypos <=640)
                        activepart->cleardeck(activepart->c_deck);
                    else
                    if (mousemain.xpos >=891 &&
                        mousemain.xpos <=940 &&
                        mousemain.ypos >=622 &&
                        mousemain.ypos <=640)
                        activepart->clearpart();

                }
                else
                if (mousemain.xpos <= 960 &&
                    mousemain.ypos >= 605 &&
                    mousemain.ypos <= 762)
                {
                    uimode = M_PART_PLACE;

                }
                else //{STAT_DISP_MISC, STAT_DISP_COMBAT, STAT_DISP_SC, STAT_DISP_ENG, STAT_DISP_COST};
                if (mousemain.ypos >= 386 &&
                    mousemain.ypos <= 410)
                {
                    if (mousemain.xpos >= 802 &&
                        mousemain.xpos <= 831)
                    {
                        mainship.statdisplay = STAT_DISP_MISC;

                    }
                    if (mousemain.xpos >= 832 &&
                        mousemain.xpos <= 861)
                    {
                        mainship.statdisplay = STAT_DISP_COMBAT;

                    }
                    if (mousemain.xpos >= 862 &&
                        mousemain.xpos <= 891)
                    {
                        mainship.statdisplay = STAT_DISP_SC;

                    }
                    if (mousemain.xpos >= 892 &&
                        mousemain.xpos <= 921)
                    {
                        mainship.statdisplay = STAT_DISP_ENG;

                    }
                }
                else
                ui_main->getbfunc(mousemain.xpos, mousemain.ypos); // first chance
            }
            else
            {
                if (mousemain.axpos<0 && uimode != M_PART_PLACE && uimode != X_MOVEROOM_START)
                    mousemain.axpos = 0;
                if (mousemain.axpos>800 && uimode != M_PART_PLACE && uimode != X_MOVEROOM_START)
                    mousemain.axpos = 800;
                if (mousemain.aypos<0 && uimode != M_PART_PLACE && uimode != X_MOVEROOM_START)
                    mousemain.aypos=0;
                if (mousemain.aypos>800 && uimode != M_PART_PLACE && uimode != X_MOVEROOM_START)
                    mousemain.aypos=800;

                if (uimode == D_ROOM)
                {
                    if (!a_room)
                    {
                       a_room = makeroom(a_room);
                    }
                    if (a_room->lverts)
                    {
                        VLINK *tempv = a_room->lverts;
                        for (int wtf = 1; wtf < a_room->vnum; wtf++)
                        {
                            tempv = tempv->next;
                        }
                        truesnap(tempv, mousemain.axpos, mousemain.aypos);
                    }
                    AddVLINK(mousemain.axpos, mousemain.aypos, a_room);
                }
                else
                if (uimode == P_EQUIP)
                {
                    //AddEquip(mousemain.axpos, mousemain.aypos);
                    AddEquip2(mousemain.axpos, mousemain.aypos, currentdeck, brush, currentmod);
                    mainship.calcdeck(currentdeck);
                    mainship.calctotal();
                }
                else
                if (uimode == R_PICK ||
                    uimode == M_PART_ADD)
                {
                    // collide with all rooms, make a_room = whatever it collides with
                    for(int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            if (pointInPolygon2(deckrooms[t][r][currentdeck], mousemain.axpos, mousemain.aypos))
                            {
                                finishroom2(a_room);
                                a_room = deckrooms[t][r][currentdeck];
                                //uimode = D_ROOM;
                            }
                        }
                    }
                    if (uimode == R_PICK)
                        uimode = D_ROOM;
                    if (uimode == M_PART_ADD)
                    {

                        if (finishroom2(a_room))
                        {
                            if (a_room->f_legal)
                            {
                                activepart->addroom(a_room);
                            }
                        }

                        else
                            delete a_room;

                        a_room = NULL;

                    }

                }
                else
                if (uimode == V_PICK)
                {
                    if (!a_room)
                    {
                        for(int t = 0; t < 22; t++)
                        {
                            for (int r = 0; r < 256; r++)
                            {
                                if (pointInPolygon2(deckrooms[t][r][currentdeck], mousemain.axpos, mousemain.aypos))
                                {
                                    a_room = deckrooms[t][r][currentdeck];
                                }
                            }
                        }

                    }
                    {
                        vpick(a_room, mousemain.axpos, mousemain.aypos);
                        uimode = D_ROOM;
                        if (/*roomtype == 0x00 ||*/ a_room == NULL)
                            uimode = V_PICK;
                    }
                }
                else
                if (uimode == M_PART_PLACE)
                {
                    activepart->insetatcursor(mousemain.axpos, mousemain.aypos, currentdeck);
                    mainship.calcdeck(currentdeck);
                    mainship.calctotal();
                }
                else
                if (uimode == X_MOVEROOM_START)
                {
                    if (copypart->insetalltest(mousemain.axpos, mousemain.aypos, currentdeck))
                        moveroomend(1, mousemain.axpos, mousemain.aypos, currentdeck);
                    else
                        moveroomend(0, mousemain.axpos, mousemain.aypos, currentdeck);
                    hangingpart = false;

                }
            }
            mousemain.lkeydown = false;
        }
        if (mousemain.lkeypress == true)
        {
            if (mousemain.xpos < 805 && uimode == P_EQUIP)
            {
                // get ticks, if enough place equip?
                //AddEquip(mousemain.axpos, mousemain.aypos);
                AddEquip2(mousemain.axpos, mousemain.aypos, currentdeck, brush, currentmod);
                mainship.calcdeck(currentdeck);
                mainship.calctotal();
            }

        }
        if (mousemain.rkeydown == TRUE)
        {
            //mode = !mode;
            mousemain.rkeydown = false;
            //mousemain.lkeypress
        }
        SDL_Delay(10);
    }
    //savetofile();
    KillFont();
    SDL_Quit();
    return 1;
}

bool vpick(ROOM *setroom, int mousex, int mousey)
{
    if (!setroom)
        return false;
    if (!setroom->lverts)
        return false;
    // we will iterate over all points.  we will then pick the point with the lowest distance to our pick point.
    // we may want to do pythag, we may not. lets try simple first.
    VLINK *nodes[setroom->vnum];
    VLINK *temp = setroom->lverts;
    for (int i = 0; i < setroom->vnum; i++)
    {
        nodes[i]= temp;
        temp = temp->next;
    }
    float dists[setroom->vnum];
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    bool sj = false;
    float j = 0;
    int v = 0;
    int xdist;
    int ydist;
    for (int i = 0; i < setroom->vnum; i++)
    {
        xdist = (px - nodes[i]->cx);
        ydist = (py - nodes[i]->cy);
        if (xdist<0)
            xdist*=-1;
        if (ydist<0)
            ydist*=-1;
        dists[i] = xdist + ydist;
        if (!sj)
        {
            v = i;
            j = dists[i];
            sj = true;
        }
        if (dists[i]<j)
        {
            v = i;
            j = dists[i];
        }
    }
   // fprintf(stdout, "Selected %i", v);

   // int set = v; // v becomes the new last. time for some cases.
    if (!nodes[v]->next) //its already last
        return true;
    nodes[v]->next = NULL;
    setroom->lverts = nodes[v+1];
    temp = setroom->lverts;
    int k = v+2;
    //int l = v+2;
    int p = setroom->vnum;
    int q = 0;

    while (q < p-1)
    {
        if (k>=setroom->vnum)
            k=0;
        temp->next=nodes[k];
        temp = nodes[k];
        q++;
        k++;

    }

    return true;
}

void truesnap(VLINK *node, int &mx, int &my)
{
    int diffx, diffy;
    mx = mx/5;
    mx *=5;
    my = my/5;
    my *=5;
    diffx = mx - node->cx*5;
    diffy = my - (800-node->cy*5);
    //my = 800-node->cy*5+diffx;
    //return;
    if (diffx == 0 || diffy == 0) // no change as its equal anyway
    {
        if ((node->cx*5)%10 == 5)
        {
            //if (diffx <0 || diffy >0)
                my = 800-node->cy*5+diffx;
            //else
            //    my = 800-node->cy*5-diffx;

        }
                if (my > 800)
                    my = 800;
                if (my <0)
                    my = 0;
                if (mx < 0)
                    mx = 0;
                if (mx > 800)
                    mx = 800;




        return;
    }



    float slope = (float)diffy/(float)diffx;
    //if (diffx > 0 && diffy > 0)
    {
        if (slope > 0)
        {
            if ((node->cx*5)%10 == 5)
            {
                my = 800-node->cy*5+diffx;
                if (my < 0)
                {
                    int dmy = 0 - my;
                    my = 0;
                    mx += dmy;
                    return;
                }
                if (my > 800)
                {
                    int dmy = my - 800;
                    my = 800;
                    mx -= dmy;
                    return;
                }
                if (mx < 0)
                {
                    int dmx = 0 - mx;
                    mx = 0;
                    my += dmx;
                    return;
                }
                if (mx > 800)
                {
                    int dmx = mx - 800;
                    mx = 800;
                    my -= dmx;

                }
                return;
            }
            if (slope < .49)
            {
                my = 800-node->cy*5;
                if (mx < 0)
                {
                    mx = 0;
                }
                if (mx > 800)
                {
                    mx = 800;
                }
                return;
            }
            if (slope < 2.50)
            {
                my = 800-node->cy*5+diffx;
                //exit(7);
                if (my < 0)
                {
                    int dmy = 0 - my;
                    my = 0;
                    mx += dmy;
                    return;
                }
                if (my > 800)
                {
                    int dmy = my - 800;
                    my = 800;
                    mx -= dmy;
                    return;
                }
                if (mx < 0)
                {
                    int dmx = 0 - mx;
                    mx = 0;
                    my += dmx;
                    return;
                }
                if (mx > 800)
                {
                    int dmx = mx - 800;
                    mx = 800;
                    my -= dmx;
                    return;
                }

                return;
            }

            {
                mx = node->cx*5;
                if (my > 800)
                    my = 800;
                if (my <0)
                    my = 0;
                if (mx < 0)
                    mx = 0;
                if (mx > 800)
                    mx = 800;

                return;
            }
        }
        else
        {

            if ((node->cx*5)%10 == 5)
            {
                my = 800-node->cy*5-diffx;
                if (my < 0)
                {
                    int dmy = 0 - my;
                    my = 0;
                    mx -= dmy;
                    return;
                }
                if (my > 800)
                {
                    int dmy = my - 800;
                    my = 800;
                    mx += dmy;
                    return;
                }
                if (mx < 0)
                {
                    int dmx = 0 - mx;
                    mx = 0;
                    my -= dmx;
                    return;
                }
                if (mx > 800)
                {
                    int dmx = mx - 800;
                    mx = 800;
                    my += dmx;

                }
                return;
            }
            if (slope > -.49)
            {
                my = 800-node->cy*5;
                if (mx < 0)
                {
                    mx = 0;
                }
                if (mx > 800)
                {
                    mx = 800;
                }
                return;
            }
            if (slope > -2.50)
            {
                my = 800-node->cy*5-diffx;
                //my = 800-my;
                //exit(6);
                //
                if (my < 0)
                {
                    int dmy = 0 - my;
                    my = 0;
                    mx -= dmy;
                }
                if (my > 800)
                {
                    int dmy = my - 800;
                    my = 800;
                    mx += dmy;
                }
                if (mx < 0)
                {
                    int dmx = 0 - mx;
                    mx = 0;
                    my -= dmx;
                }
                if (mx > 800)
                {
                    int dmx = mx - 800;
                    mx = 800;
                    my += dmx;
                }
                return;
            }

            {
                mx = node->cx*5;
                if (my > 800)
                    my = 800;
                if (my <0)
                    my = 0;
                if (mx < 0)
                    mx = 0;
                if (mx > 800)
                    mx = 800;

                return;
            }

        }
    }



}

void removeconsole(int posx, int posy, int deckn);

bool consoletest(int posx, int posy, int deckn);

bool addhatch(int mousex, int mousey, int deckn, int placetype)
{
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);

    bool above = 0;
    bool below = 0;
    bool here = 0;

    //it cannot place on an edge.  test for edges.
    //it also cannot place if there is an edge below it.
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (roomEtest2(deckrooms[t][r][deckn], px, py))
            {
                return false;
            }
            if (deckn > 0)
            {
                if (roomEtest2(deckrooms[t][r][deckn-1], px, py))
                    return false;
            }
        }
    }
    int b_rnum = 0;
    int h_rnum = 0;
    int a_rnum = 0;
    unsigned char locnum = 0;
    unsigned char locnumB = 0;
    //now to test validities.
    for (int t = 0; t<22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (pointInPolygon3(deckrooms[t][r][deckn], px, py))
            {
                here = true;
                h_rnum = deckrooms[t][r][deckn]->globalnum;
                locnum = r;
            }
            if (deckn > 0)
            {
                if (pointInPolygon3(deckrooms[t][r][deckn-1], px, py))
                {
                    below = true;
                    b_rnum = deckrooms[t][r][deckn-1]->globalnum;
                    locnumB = r;
                }
            }
            if (deckn < 56)
            {
                if (pointInPolygon3(deckrooms[t][r][deckn+1], px, py))
                {
                    above = true;
                    a_rnum = deckrooms[t][r][deckn+1]->globalnum;
                }
            }
        }
    }
    //int roomnum = 0;
    int a_equip = equiptype;
    int crot = currentrot;


    if (equiptype == HATCH_H) //can place only if nothing above and here BUT something below, OR nothing below but something here
    {
        if (crot >0)
        {
            a_equip = 0x5B+(crot-1);
        }

        if (!here && below)
        {
            //exit(9);
            if (equipgrid[px][py][deckn])
            {
                delete equipgrid[px][py][deckn];
            }
            equipgrid[px][py][deckn] = new EQUIPUNIT(a_equip, 0, b_rnum, locnumB, 0, crot);
            return true;

        }
        if (here && !below)
        {
            if (equipgrid[px][py][deckn])
            {
                delete equipgrid[px][py][deckn];
            }
            equipgrid[px][py][deckn] = new EQUIPUNIT(a_equip, 0, h_rnum, locnum, 0, crot);
            return true;

        }
    }
    else  //can place only on the inside if a floor is below
    {
        if (crot >0)
        {
            if (a_equip == HATCH_C)
                a_equip = 0x54+(crot-1);
            else
                a_equip = 0x4D+(crot-1);
        }
        if (here && below)
        {
            if (equipgrid[px][py][deckn])
            {
                delete equipgrid[px][py][deckn];
            }
            equipgrid[px][py][deckn] = new EQUIPUNIT(a_equip, 0, b_rnum, locnumB, 0, crot);
            return true;
        }
        return false;

    }

    return false;

}

bool addconsole2(int mousex, int mousey, int deckn)
{
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    if (isconsolenear(px, py, deckn))
        return false;
    //hit rooms: hit only the rooms you care about.if you hit any edges, fail.
    int mainhit, althit;
    mainhit = 0; althit = 0;
    int roomid = 0;
    int roomtype = 0;
    unsigned char locrid = 0;
    //set hits: bridge for everything except tp console, no althits for medc or tp
    if (equiptype == TRA_C)
    {
        mainhit = TP;
    }
    else
    if (equiptype == MED_C ||
        equiptype == MED_U)
    {
        mainhit = MED;
    }
    else
    if (equiptype == POW_C)
    {
        mainhit = ENGINE;
    }
    else
    {
        mainhit = BRIDGE;
        if (equiptype == ENG_C)
            althit = ENGINE;
        if (equiptype == SHI_C)
            althit = SHIELD;
        if (equiptype == SEN_C)
            althit = SENSOR;
        if (equiptype == WEA_C)
            althit = WEAP;
    }
    if (mainhit == 0)
        return false;
    for (int r = 0; r < 256; r++)
    {
        if (roomEtest2(deckrooms[mainhit][r][deckn], px, py))
            return false;
        if (althit != 0)
        {
            if (roomEtest2(deckrooms[althit][r][deckn], px, py))
             return false;
        }
    }

    //if no edges were hit, then we hit p in p. when we hit, we check it for other consoles, and if its good, we place it or if not we remove old console.
    for (int r = 0; r < 256; r++)
    {
        if (pointInPolygon3(deckrooms[mainhit][r][deckn], px, py))
        {
            roomtype = deckrooms[mainhit][r][deckn]->roomtype;
            roomid = deckrooms[mainhit][r][deckn]->globalnum;
            locrid = r;
            break;
        }
        if (althit != 0)
        {
            if (pointInPolygon3(deckrooms[althit][r][deckn], px, py))
            {
                roomtype = deckrooms[althit][r][deckn]->roomtype;
                roomid = deckrooms[althit][r][deckn]->globalnum;
                locrid = r;
                break;
            }
        }
    }
    if (roomtype == 0 || roomid == 0)
        return false;
    //we have found a place thats not on an edge thats in an appropriate room. now we need to remove other consoles of the
    //same type in the room.
    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            if (equipgrid[i][j][deckn])
            {
                if (equipgrid[i][j][deckn]->type == equiptype &&
                    equipgrid[i][j][deckn]->roomid == roomid &&
                    equiptype != CAP_C)
                {
                    delete equipgrid[i][j][deckn];
                    equipgrid[i][j][deckn] = NULL;
                }
            }
        }
    }
    // previous consoles removed, theres no console near it, its of appropriate type, so place it already!
    if (equipgrid[px][py][deckn])
        delete equipgrid[px][py][deckn];
    equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, roomid, locrid, 0, 0);
    for (int i = px -1; i < px +2; i++)
    {
        for (int j = py -1; j < py +2; j++)
        {
            if (i>=0 && i <161 &&
                j>=0 && j <161)
            {
                if (i==px && j==py)
                {


                }
                else
                {
                    if (equipgrid[i][j][deckn])
                    {
                        int colltype = setcollision(equipgrid[i][j][deckn]->type);
                        if (colltype != E_IW_ONLY &&
                            colltype != E_HW_ONLY)
                        {
                            delete equipgrid[i][j][deckn];
                            equipgrid[i][j][deckn] = NULL;
                        }
                    }
                }
            }
        }
    }

    // previous tests should take care of this
    /*
    if (roomtype == ENGINE)
    {
        if (equiptype != ENG_C &&
        equiptype != POW_C )
                return false;
    }
    else
    if (roomtype == SENSOR)
    {
        if (equiptype != SEN_C)
            return false;
    }
    else
    if (roomtype == WEAP)
    {
        if (equiptype != WEA_C)
            return false;
    }
    else
    if (roomtype == SHIELD)
    {
        if (equiptype != SHI_C)
            return false;
    }
    else
    if (roomtype == MED)
    {
        if (equiptype != MED_C &&
            equiptype != MED_U)
            return false;
    }
    else
    if (roomtype == TP)
    {
        if (equiptype != TRA_C)
            return false;
    }
    else
    if (roomtype == BRIDGE)
    {
        if (equiptype == TRA_C ||
            equiptype == MED_C ||
            equiptype == MED_U)
            return false;
    }*/

    /*
    if (roomEtest2(deckrooms[t][r][deckn], px, py))
            {
                roomnum = deckrooms[t][r][deckn]->globalnum;
                roomtype = deckrooms[t][r][deckn]->roomtype;
                trm = deckrooms[t][r][deckn];
                if (roomtype == FUEL)
                    fuelcollide = true;
            }
            else
            if (pointInPolygon3(deckrooms[t][r][deckn], px, py))
    */
    //now we need to get the room its on, test if its on a wall, and test to make sure no consoles are near it.

    return true;
}

bool addturret(int mousex, int mousey, int deckn, int mod)
{
    //two types of turret: 0x15 (side) and 0x16 (top) which get selected.  any others get converted to these before we get in here
    //so loading is easily handled.
    //top gets handled by this: must not be on an edge, and must not be an edge OR a room.
    //bot: must not be an edge and must not be an edge OR a room below it, and must be IN a room.
    //kinda like adding hull hatches, except logic choses if its a bottom or top turret
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    short int r_id = 0;
    if (mod != 0x0106 &&
        !((mod >= 0x00F8) && (mod <= 0x00FB)))
        mod = 0x0106;

    if (equiptype == T_TOP)
    {
        bool here = 0;
        bool below = 0;
        short int hereid = 0;
        short int belowid = 0;
        unsigned char locridh = 0;
        unsigned char locridb = 0;
        for (int t = 0; t < 22; t++)
        {
            for (int r = 0; r < 256; r++)
            {
                if (roomEtest2(deckrooms[t][r][deckn], px, py))
                    return false;
                if (pointInPolygon3(deckrooms[t][r][deckn], px, py))
                {
                    hereid = deckrooms[t][r][deckn]->globalnum;
                    here = true;
                    locridh = r;

                }

                if (deckn > 0)
                {
                    if (roomEtest2(deckrooms[t][r][deckn-1], px, py))
                        return false;
                    if (pointInPolygon3(deckrooms[t][r][deckn-1], px, py))
                    {
                        //exit(9);
                        belowid = deckrooms[t][r][deckn-1]->globalnum;
                        below = true;
                        locridb = r;
                    }

                }

            }
        }
        unsigned char setlid = 0;

        if (here && !below)
        {
            r_id = hereid;
            equiptype = T_BOT;
            setlid = locridh;
        }
        else
        if (!here && below)
        {
            r_id = belowid;
            setlid = locridb;
        }
        else
            return false;

        if (equipgrid[px][py][deckn])
            delete equipgrid[px][py][deckn];


        equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, setlid, mod, 0);
        //(int ttype, long int eqnum, short int ronum, short int modtype, int rot)

    }
    int edgehits = 0;
 //   int inhits = 0;
    if (equiptype == T_SIDE_R)
    {
        bool R = 0;
        bool UR = 0;
        bool U = 0;
        bool UL = 0;
        bool L = 0;
        bool DL = 0;
        bool D = 0;
        bool DR = 0;
        //bool hit = 0;
        bool sides[8] = {DL, L, UL, D, U, DR, R, UR};
        unsigned char locrid = 0;

        int wside = 0;
        ROOM *p_room = NULL;
        for (int t = 0; t < 22; t++)
        {
            for (int r = 0; r < 256; r++)
            {
                wside = 0;
                if (roomEtest2(deckrooms[t][r][deckn], px, py))
                {
                    r_id = deckrooms[t][r][deckn]->globalnum;
                    p_room = deckrooms[t][r][deckn];
                    edgehits++;
                    locrid = r;

                }
                for (int x = px-1; x < px+2; x++)
                {
                    for (int y = py-1; y<py+2; y++)
                    {
                        if (x == px && y == py)
                        {
                            wside--;
                        }
                        else
                        {
                            if (x>=0 && x<161 &&
                                y>=0 && y<161)
                            {
                                if (pointInPolygon2(deckrooms[t][r][deckn], (mousex - (px-x) ), (mousey + (py -y)))) // &&
                                    // !roomEtest2(deckrooms[t][r][deckn], x, y))
                                {
                                    sides[wside] = true;
                                    //inthits++;
                                }
                            }
                        }
                        wside++;
                    }
                }

            }
        }
        if (r_id == 0)
            return false;
        if (edgehits >1)
            return false;
        bool edges[8] = {DL, L, UL, D, U, DR, R, UR};
        wside = 0;
        for (int i = px -1; i < px+2; i++)
        {
            for (int j = py -1; j < py+2; j++)
            {

                if ((i==px)&&(j==py))
                {
                    wside--;

                }
                else
                {
                    if (i>=0 && i<161 &&
                        j>=0 && j<161)
                    {
                        if (roomEtest3(p_room, (mousex - (px-i) ), (mousey + (py -j))))
                        {
                            edges[wside] = true;
                        }
                    }
                }
                wside++;
            }
        }
        //default is right, probably need to test it anyway
            //0   1  2   3  4  5   6  7
//sides[8] = {DL, L, UL, D, U, DR, R, UR};
//apparently you can 'almost' make them play nice like this, but in doing so make certain 'bad' turret placements impossible. ah well
        if (!sides[7] &&  !sides[4] && !sides[6] &&
            sides[0] && //&& sides[3] &&
            edges[2] && edges[5])
        {
            equiptype = T_SIDE_UR;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 7);
            //(int ttype, long int eqnum, short int ronum, short int modtype, int rot)
        }
        else
        if (!sides[2] &&  !sides[4] && !sides[7] &&
            sides[3] &&
            edges[1] && edges[6])
        {
            equiptype = T_SIDE_U;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 0);
        }
        else
        if (!sides[1] &&  !sides[2] && !sides[4] &&
            sides[5] &&
            edges[7] && edges[0])
        {
            equiptype = T_SIDE_UL;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 1);
        }
        else
        if (!sides[2] &&  !sides[0] && !sides[1] &&
            sides[6] &&
            edges[3] && edges[4])
        {
            equiptype = T_SIDE_L;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 2);
        }
        else
        if (!sides[1] &&  !sides[0] && !sides[3] &&
            sides[7] &&
            edges[2] && edges[5])
        {
            equiptype = T_SIDE_DL;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 3);
        }
        else
        if (!sides[0] &&  !sides[3] && !sides[5] &&
            sides[4] &&
            edges[1] && edges[6])
        {
            equiptype = T_SIDE_D;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 4);
        }
        else
        if (!sides[3] &&  !sides[5] && !sides[6] &&
            sides[2] &&
            edges[0] && edges[7])
        {
            equiptype = T_SIDE_DR;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 5);
        }
        else
        if (!sides[7] &&  !sides[6] && !sides[5] &&
            sides[1] &&
            edges[4] && edges[3])
        {
            equiptype = T_SIDE_R;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, locrid, mod, 6);
        }
            //0   1  2   3  4  5   6  7
//sides[8] = {DL, L, UL, D, U, DR, R, UR};
    }
/*
        if (sides[0] && !sides[7] &&
            sides[1] && !sides[6] &&
            sides[3] && !sides[4])
        {
            equiptype = T_SIDE_UR;
            if (roomEtest2(p_room, px-1, py) ||
                roomEtest2(p_room, px, py-1))
                //if (!roomEtest2(p_room, px-1, py+1) &&
                //    !roomEtest2(p_room, px+1, py-1))
                    return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 7);
            //(int ttype, long int eqnum, short int ronum, short int modtype, int rot)
        }
        else
        if (sides[0] && !sides[2] &&
            sides[3] && !sides[4] &&
            sides[5] && !sides[7])
        {
            equiptype = T_SIDE_U;
            if (roomEtest2(p_room, px-1, py-1) ||
                roomEtest2(p_room, px+1, py-1))
               // if (!roomEtest2(p_room, px-1, py) &&
               //     !roomEtest2(p_room, px+1, py))
                    return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 0);
        }
        else
        if (sides[5] && !sides[1] &&
            sides[6] && !sides[2] &&
            sides[3] && !sides[4])
        {
            equiptype = T_SIDE_UL;
            if (roomEtest2(p_room, px+1, py) ||
                roomEtest2(p_room, px, py-1))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 1);
        }
        else
        if (sides[5] && !sides[0] &&
            sides[6] && !sides[1] &&
            sides[7] && !sides[2])
        {
            equiptype = T_SIDE_L;
            if (roomEtest2(p_room, px+1, py+1) ||
                roomEtest2(p_room, px+1, py-1))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 2);
        }
        else
        if (sides[4] && !sides[1] &&
            sides[6] && !sides[0] &&
            sides[7] && !sides[3])
        {
            equiptype = T_SIDE_DL;
            if (roomEtest2(p_room, px, py+1) ||
                roomEtest2(p_room, px+1, py))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 3);
        }
        else
        if (sides[2] && !sides[0] &&
            sides[4] && !sides[3] &&
            sides[7] && !sides[5])
        {
            equiptype = T_SIDE_D;
            if (roomEtest2(p_room, px-1, py+1) ||
                roomEtest2(p_room, px+1, py+1))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 4);
        }
        else
        if (sides[1] && !sides[6] &&
            sides[2] && !sides[5] &&
            sides[4] && !sides[3])
        {
            equiptype = T_SIDE_DR;
            if (roomEtest2(p_room, px-1, py) ||
                roomEtest2(p_room, px, py+1))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 5);
        }
        else
        if (sides[0] && !sides[7] &&
            sides[1] && !sides[6] &&
            sides[2] && !sides[5])
        {
            equiptype = T_SIDE_R;
            if (roomEtest2(p_room, px-1, py+1) ||
                roomEtest2(p_room, px-1, py-1))
                return false;
            if (equipgrid[px][py][deckn])
                delete equipgrid[px][py][deckn];
            equipgrid[px][py][deckn] = new EQUIPUNIT(equiptype, 0, r_id, mod, 6);
        }
            //0   1  2   3  4  5   6  7
//sides[8] = {DL, L, UL, D, U, DR, R, UR};
*/



    return false;
}

bool addberth(int mousex, int mousey, int deckn, int mod);

bool placelegal(int roomtype, int equiptype, int placetype);

bool isberthnear(int tx, int ty, int deckn);

bool addberth(int mousex, int mousey, int deckn, int mod)
{
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    int roomnum = 0;
    int roomtype = 0;
    unsigned char locrid = 0;
    int crot = currentrot;
    int setequip = equiptype;
    //fprintf(stdout, "setequip %i", setequip);
    //fprintf(stdout, "current")
    //exit(9);

    ROOM *trm = NULL;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (pointInPolygon3(deckrooms[t][r][deckn], px, py))
            {
                roomnum = deckrooms[t][r][deckn]->globalnum;
                roomtype = deckrooms[t][r][deckn]->roomtype;
                trm = deckrooms[t][r][deckn];
                locrid = r;
            }
        }
    }
    if (!trm)
        return false;
    if (roomEtest2(trm, px, py))
        return false;
    if (!placelegal(roomtype, equiptype, E_BERTH))
        return false;
    if (isberthnear(px, py, deckn))
        return false;

    if (setequip == BED_U)
    {
        setequip+= crot;
    }
    else
    if (setequip == BED2_U)
    {
        if (crot >0)
        {
            setequip = (0x18+crot);
        }
    }
    bool rcoll = false;
    bool ucoll = false;
    bool lcoll = false;
    bool dcoll = false;

    if (px-1>0)
    {
        if (roomEtest2(trm, px-1, py))
            lcoll = true;
    }
    if (px+1<161)
    {
        if (roomEtest2(trm, px+1, py))
            rcoll = true;
    }
    if (py-1>0)
    {
        if (roomEtest2(trm, px, py-1))
            dcoll = true;
    }
    if (py+1<161)
    {
        if (roomEtest2(trm, px, py+1))
            ucoll = true;
    }
    // cant place next to an existing berth.
    // if R,U,L,D, cant place next to wall. Diagonals dont care.
    if (setequip == BED_U_R ||
        setequip == BED2_U_R)
    {
        if (rcoll)
            return false;
    }
    if (setequip == BED_U_U ||
        setequip == BED2_U_U)
    {
        if (ucoll)
            return false;
    }
    if (setequip == BED_U_L ||
        setequip == BED2_U_L)
    {
        if (lcoll)
            return false;
    }
    if (setequip == BED_U_D ||
        setequip == BED2_U_D)
    {
        if (dcoll)
            return false;
    }
    if (equipgrid[px][py][deckn])
    {
        delete equipgrid[px][py][deckn];
        equipgrid[px][py][deckn] = NULL;
    }
    equipgrid[px][py][deckn] = new EQUIPUNIT(setequip, 0, roomnum, locrid, mod, crot);





    return true;
}

int setcollision(int equiptype)
{
    int placetype = E_MoW;
    if (equiptype == FTL_U ||
        equiptype == LIF_U ||
        equiptype == TRA_U)
        placetype = E_M_ONLY;
    if (equiptype == BED_U ||
        equiptype == BED2_U)
        placetype = E_BERTH;
//placing stuff for hull
    if (equiptype == DOOR_H ||
        equiptype == DOORL_H ||
        equiptype == OPENING_E )
        placetype = E_HW_ONLY;
//internal wall only
    if (equiptype == DOOR_C ||
        equiptype == DOOR_P ||
        equiptype == DOORL_C ||
        equiptype == DOORL_P ||
        equiptype == OPENING_I ||
        equiptype == RAIL)
        placetype = E_IW_ONLY;
    if (equiptype == GUN_T_CAN ||
        equiptype == GUN_S_CAN ||
        equiptype == T_BOT ||
        ((equiptype >= T_SIDE_UR) && (equiptype <= T_SIDE_DR) ))
        placetype = E_TURRET;
    if (equiptype == HATCH_H )
        placetype = E_H_UoO;
    if (equiptype == HATCH_C ||
        equiptype == HATCH_P)
        placetype = E_MoUoO;

    return placetype;
}

bool placelegal(int roomtype, int equiptype, int placetype)
{

    if (roomtype == FUEL)
        return false;
    else
    if (placetype == E_HW_ONLY ||
        placetype == E_IW_ONLY)
        return true;
    else
    if (roomtype == ENGINE)
    {
        if (equiptype != MAN_U && equiptype != POW_U && equiptype != FTL_U && equiptype != LIF_U)
            return false;
        return true;
    }
    else
    if (roomtype == SHIELD)
    {
        if (equiptype != SHI_U)
            return false;
        return true;
    }
    else
    if (roomtype == SENSOR)
    {
        if (equiptype != SEN_U)
            return false;
        return true;
    }
    else
    if (roomtype == WEAP)
    {
        if (equiptype != WEA_U)
            return false;
        return true;
    }
    else
    if (roomtype == BARRACK ||
        roomtype == OFFICER ||
        roomtype == CREW ||
        roomtype == STATE)
    {
        if (equiptype != BED_U &&
            equiptype != BED2_U)
            return false;
        return true;

    }
    else
    if (roomtype == TP)
    {
        if (equiptype != TRA_U)
            return false;
        return true;
    }
    return false;

}

bool isberthnear(int tx, int ty, int deckn)
{
    for (int i = tx-1; i < tx+2; i++)
    {
        for (int j = ty -1; j < ty +2; j++)
        {
            if (i==tx && j == ty)
            {

            }
            else
            {
                if (i>=0 && i<161 &&
                    j>=0 && j<161)
                {
                    if (equipgrid[i][j][deckn])
                    {

                             return true;
                    }
                }
            }

        }
    }

    return false;
}

bool isconsolenear(int tx, int ty, int deckn)
{
    for (int i = tx -1; i < tx +2; i++)
    {
        for (int j = ty -1; j < ty+2; j++)
        {
            if (i == tx && j == ty)
            ;
            else
            if (i>=0 && i<161 &&
                j>=0 && j<161 )
            {
                if (equipgrid[i][j][deckn])
                {
                    if (isconsole(equipgrid[i][j][deckn]->type))
                        return true;
                }

            }
        }
    }
    return false;

}

bool AddEquip2(int mousex, int mousey, int deckn, int bsize, short int cmod)
{
    //first, snap the cursor
    mousex = cursnap(mousex);
    mousey = cursnap(mousey);
    if (isconsole(equiptype))
    {
        return addconsole2(mousex, mousey, deckn);
    }
    if (equiptype >= 0x4D &&
        equiptype <= 0x53)
    {
        currentrot = equiptype - 0x4D + 1;
        equiptype = 0x08;
    }
    if (equiptype >= 0x54 &&
        equiptype <= 0x5A)
    {
        currentrot = equiptype - 0x54 +1;
        equiptype = 0x33;
    }
    if (equiptype >= 0x5B &&
        equiptype <= 0x61)
    {
        currentrot = equiptype - 0x5B +1;
        equiptype = 0x34;
    }
    if (equiptype >= 0x42 &&
        equiptype <= 0x48)
    {
        currentrot = equiptype - 0x42 +1;
        equiptype = 0x41;
    }
    if (equiptype >= 0x19 &&
        equiptype <= 0x1F)
    {
        currentrot = equiptype - 0x19 +1;
        equiptype = 0x02;
    }

    //now convert turrets
    if (equiptype >= T_SIDE_UR &&
        equiptype <= T_SIDE_DR)
    {
        equiptype = T_SIDE_R; //or GUN_S_CAN
    }
    if (equiptype == T_BOT)
        equiptype = T_TOP;
    int placetype = setcollision(equiptype);
    if (placetype == E_MoUoO ||
        placetype == E_H_UoO)
    {
        return addhatch(mousex, mousey, deckn, placetype);
    }
    if (placetype == E_TURRET)
    {
        return addturret(mousex, mousey, deckn, cmod);
    }
    if (placetype == E_BERTH)
    {
        return addberth(mousex, mousey, deckn, 0);
    }
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    if (isconsolenear(px, py, deckn) && ((placetype != E_HW_ONLY) && (placetype != E_IW_ONLY)))
        return false;
    //fprintf(stdout, "px %i py %i", px, py);
    //px and py look good, keep goin
    if (equiptype == 0x00)//deletion
    {
        for (int i = (px - bsize); i < (px+bsize+1); i++)
        {
            for (int j = (py - bsize); j < (py+bsize+1); j++)
            {
                if (i >= 0 && i <161 &&
                    j >= 0 && i <161)
                {
                    if (equipgrid[i][j][deckn])
                    {
                        delete equipgrid[i][j][deckn];
                        equipgrid[i][j][deckn] = NULL;
                    }
                }
            }
        }
        return true;
    }
    //int placetype = setcollision(equiptype);
    unsigned short int roomnum = 0;
    unsigned char roomtype = 0;
    unsigned char locrid = 0;
    int numcollisions = 0;
    //int eflag = 0;
    bool fuelcollide = false;
    int crot = 0;

    /*
    MoW: walls and middle
    E_M_ONLY: only test middle
    E_HW_ONLY: only test walls
    E_IW ONLY: only test walls
    //consoles taken care of in another function.
    need one for hatches too, then need a special one for vehicles. still got the med units too for later.
    */
    //we might need to make the for loop around this, due to lots of points to test!!!
    //take room on cursor, THEN do brush? may be more like ingame that way. probably best.

    ROOM *trm = NULL;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {

            if (roomEtest2(deckrooms[t][r][deckn], px, py))
            {
                roomnum = deckrooms[t][r][deckn]->globalnum;
                roomtype = deckrooms[t][r][deckn]->roomtype;
                trm = deckrooms[t][r][deckn];
                if (roomtype == FUEL)
                    fuelcollide = true;
                locrid = r;
            }
            else
            if (pointInPolygon3(deckrooms[t][r][deckn], px, py))
            {
                roomnum = deckrooms[t][r][deckn]->globalnum;
                roomtype = deckrooms[t][r][deckn]->roomtype;
                trm = deckrooms[t][r][deckn];
                locrid = r;
            }
        }
    }

    if (!trm)
        return false; // no hits
    if (fuelcollide)
        return false; //cant place anything on a fuel cell

    //if () need to set allowable rooms
    if (!placelegal(roomtype, equiptype, placetype))
        return false;


    //exit(9);

    bool canplace = false;
    //needa do somethin bout them corner doors too eventually.
    //doors = E_HW_ONLY && E_IW_ONLY
    //must have opposite edge pairs.
    for (int i = px - bsize; i < px + bsize + 1; i++)
    {
        for (int j = py - bsize; j < py + bsize + 1; j++)
        {
            if (i >=0 && i < 161 &&
                j >=0 && j < 161)
            {
                canplace = true;
                //fuelcollide = false;
                //do your tests at this point to the room
                //if collide with wall in this room, check other collisions too, otherwise you dont care

                if (roomEtest2(trm, i, j))
                {
                    if (placetype == E_M_ONLY)
                        canplace = false;
                    numcollisions = 0;
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            if (roomEtest2(deckrooms[t][r][deckn], i, j))
                            {
                                numcollisions++;
                                if (deckrooms[t][r][deckn]->roomtype == FUEL)
                                    canplace = false;
                            }
                        }
                    }
                    //we now have num collisions, compare and place proper
                    //openings dont care if a console is near, but other equipment does!!!
                    if (placetype == E_HW_ONLY ||
                        placetype == E_IW_ONLY)
                    {
                        int eset = 0;
                        bool edges[8] = {0,0,0,0,0,0,0,0};
                        for (int x = i-1; x < i+2; x++)
                        {
                            for (int y = j-1; y < j+2; y++)
                            {
                                if (x == i && y == j)
                                {
                                    eset--;
                                }
                                else
                                for (int st = 0; st < 22; st++)
                                {
                                    for (int sr = 0; sr < 256; sr++)
                                    {
                                        if (roomEtest3(deckrooms[st][sr][deckn], (x*5)-(x-i), (800-y*5)+(y-j)))
                                        {
                                            edges[eset] = true;
                                        }

                                    }
                                }
                                eset++;
                            }
                        }
                        if ((edges[0] && edges[7]) ||
                            (edges[1] && edges[6]) ||
                            (edges[2] && edges[5]) ||
                            (edges[3] && edges[4]) )
                        {
                            int ehits = 0;
                            for (int et = 0; et < 8; et++)
                            {
                                if (edges[et])
                                    ehits++;
                                if (ehits>2)
                                    canplace = false;

                            }

                        }
                        else
                            canplace = false;
                        //exit(9);


                    }
                    if (numcollisions == 1)
                    {
                        if (placetype == E_IW_ONLY)
                            canplace = false;
                    }
                    else
                    if (numcollisions >= 2)
                    {

                        if (placetype == E_HW_ONLY ||
                            placetype == E_MoW)
                            canplace = false;
                    }
                    if (canplace)
                    {
                        //now we get down to business. if theres nothing there, make a new one. if theres one there, change it.
                        if (equipgrid[i][j][deckn])
                        {
                            delete equipgrid[i][j][deckn];
                            equipgrid[i][j][deckn] = new EQUIPUNIT(equiptype, 0, roomnum, locrid, 0, crot);
                        }
                        else
                        {
                            equipgrid[i][j][deckn] = new EQUIPUNIT(equiptype, 0, roomnum, locrid, 0, crot);
                        }
                    }
                }
                else
                if (pointInPolygon3(trm, i, j))
                {
                    if (placetype == E_HW_ONLY ||
                        placetype == E_IW_ONLY)
                        canplace = false;
                    numcollisions = 0; // may as well collide this shit too
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            if (pointInPolygon3(deckrooms[t][r][deckn], i, j))
                            {
                                numcollisions++;
                            }
                        }
                    }
                    if (numcollisions>1)
                        canplace = false;
                    if (canplace)
                    {
                        //now we get down to business. if theres nothing there, make a new one. if theres one there, change it.
                        if (equipgrid[i][j][deckn])
                        {
                            delete equipgrid[i][j][deckn];
                            equipgrid[i][j][deckn] = new EQUIPUNIT(equiptype, 0, roomnum, locrid,  0, crot);
                        }
                        else
                        {
                            equipgrid[i][j][deckn] = new EQUIPUNIT(equiptype, 0, roomnum, locrid,  0, crot);
                        }
                    }

                }

            }

        }
    }




    return true;

}

void AddEquip(int mousex, int mousey)
{
    if (isconsole(equiptype))
    { // trying to add a console, use that function instead
        addconsole(mousex, mousey);
        return;
    }
    int placetype = E_MoW;
    if (equiptype == FTL_U ||
        equiptype == LIF_U ||
        equiptype == TRA_U ||
        equiptype == BED_U ||
        equiptype == BED2_U)
        placetype = E_M_ONLY;
//placing stuff for hull
    if (equiptype == DOOR_H ||
        equiptype == DOORL_H ||
        equiptype == OPENING_E )
        placetype = E_HW_ONLY;
//internal wall only
    if (equiptype == DOOR_C ||
        equiptype == DOOR_P ||
        equiptype == DOORL_C ||
        equiptype == DOORL_P ||
        equiptype == OPENING_I ||
        equiptype == RAIL)
        placetype = E_IW_ONLY;

/*
{MAN_U = 0x0A, POW_U = 0x0E, SHI_U = 0x12, SEN_U = 0x10, FTL_U = 0x18, WEA_U = 0x17, LIF_U = 0x20, MED_U = 0x0B,
       TRA_U = 0x14, BED_U = 0x41, BED2_U = 0x02, DOOR_H = 0x32, DOOR_C = 0x31, DOOR_P = 0x05, DOORL_H = 0x03, DOORL_C = 0x36,
       DOORL_P = 0x35, HATCH_H = 0x34, HATCH_C = 0x33, HATCH_P = 0x08, OPENING_I = 0x37, OPENING_E = 0x09, RAIL = 0x38,
       PIVOT = 0x46, EMIT_C = 0x62, EMIT_W = 0x63, TABLE_G = 0x00, V_A_T = 0x00, V_A_F1 = 0x00, V_A_F2 = 0x00, V_G_M = 0x00,
       V_G_S = 0x00, V_G_A = 0x00, GUN_T_CAN = 0x00, GUN_S_CAN = 0x00, CAP_C = 0x49, HEL_C = 0x0C, ENG_C = 0x06, SHI_C = 0x11,
       SEN_C = 0x0F, NAV_C = 0x01, WEA_C = 0x07, POW_C = 0x0D, MED_C = 0x04, TRA_C = 0x13, DUMMY = 0xFF };
*/
    //mousex /= 5;
    //mousex *= 5;
    //mousey /=5;
    //mousey *=5;
    float tmx = mousex;
    float tmy = mousey;
    mousex = cvtHEX2(cursnap(mousex));
    mousey = cvtHEX2((800 - cursnap(mousey)));
    // test if its null
    unsigned char roomnum = 0;
    unsigned char roomtype = 0;
    int numcollisions = 0;
    int eflag = EMPTY;

    if (equiptype == 0x00) //delete instead
    {
        if (equipgrid[mousex][mousey][currentdeck])
        {
            if (isconsole(equipgrid[mousex][mousey][currentdeck]->type))
                removeconsole(mousex, mousey, currentdeck);
            else
            if (equipgrid[mousex][mousey][currentdeck]->type != DUMMY)
            {
                delete equipgrid[mousex][mousey][currentdeck];
                equipgrid[mousex][mousey][currentdeck] = NULL;
            }
            return;
        }

    }




    if (numcollisions == 0) // not directly in any rooms, got to do an expensive edge test;
    {
        int csnapx = cursnap(tmx);
        int csnapy = cursnap(tmy);


        for (int i = 0; i < 22; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                if (roomEtest(deckrooms[i][j][currentdeck], csnapx, csnapy))
                {
                    roomnum = deckrooms[i][j][currentdeck]->globalnum;
                    roomtype = deckrooms[i][j][currentdeck]->roomtype;
                    numcollisions++;
                }
            }
        }
    }
    if (numcollisions == 1)
        eflag = EDGE_O;
    if (numcollisions >= 2)
        eflag = EDGE_I;
    int i_coll = 0;
    if (eflag == EMPTY) // not an edge, check the inside
    {
        for (int i = 0; i < 22; i++)
        {
            //if (i != 0 || i != 12)
            {
                for (int j = 0; j < 256; j++)
                {
                    if (pointInPolygon2(deckrooms[i][j][currentdeck], (float)tmx, (float)tmy))
                    //if (pointInPolygon2(&rooms[0], (float)tmx, (float)tmy))
                    {
                        //roomnum = rooms[0].roomnum;
                        //roomtype = rooms[0].roomtype;
                        roomnum = deckrooms[i][j][currentdeck]->globalnum;
                        roomtype = deckrooms[i][j][currentdeck]->roomtype;
                        i_coll++;
                        //numcollisions++;
                        eflag = INSIDE;
                    }
                }
            }
        }
    }

    if (i_coll > 1 || roomnum == 0 || roomtype == 0 || eflag == EMPTY )
    {
        return;
    }
    if (placetype == E_M_ONLY)
    {
        if (eflag == EDGE_I || eflag == EDGE_O)
            return;
    }
    if (placetype == E_HW_ONLY)
    {
        if (eflag != EDGE_O)
            return;
    }
    if (placetype == E_IW_ONLY)
    {
        if (eflag != EDGE_I)
            return;
    }
    if (placetype == E_MoW)
    {
        if (eflag == EDGE_I)
            return;
    }



//put some default stuff here. also, do more room equip research.
//so far, most seem to not care, so theres no mods or anything special. start making the exception list!!!

    //already took care of returning if its an improper edge type, just make the exceptions
    if (placetype == E_IW_ONLY || placetype == E_HW_ONLY)
    {
        //do nothing
    }
    else
    if (roomtype == ENGINE)
    {
        if (equiptype != MAN_U && equiptype != POW_U && equiptype != FTL_U && equiptype != LIF_U)
            return;
    }
    else
    if (roomtype == SHIELD)
    {
        if (equiptype != SHI_U)
            return;
    }
    else
    if (roomtype == SENSOR)
    {
        if (equiptype != SEN_U)
            return;
    }
    else
    if (roomtype == WEAP)
    {
        if (equiptype != WEA_U)
            return;
    }
    else
        return;
    //if (numcollisions !=1)
    //    return;
    EQUIPUNIT *snr = equipgrid[mousex][mousey][currentdeck];
//    if (!equipgrid[mousex][mousey][currentdeck])
//        equipgrid[mousex][mousey][currentdeck] = new EQUIPUNIT(equiptype, 0, roomnum, 0, 0);
//    else
    {
        // if its a console, use delete console first. if its a dummy, leave it, too dangerous now
        if (snr->type == DUMMY)
            return;
        if (snr->type == CAP_C ||
            snr->type == HEL_C ||
            snr->type == ENG_C ||
            snr->type == SHI_C ||
            snr->type == SEN_C ||
            snr->type == NAV_C ||
            snr->type == WEA_C ||
            snr->type == POW_C ||
            snr->type == MED_C ||
            snr->type == TRA_C)
        {
                removeconsole(mousex, mousey, currentdeck);
        }
        else
        {//just delete it
            delete equipgrid[mousex][mousey][currentdeck];
            equipgrid[mousex][mousey][currentdeck] = NULL;
        }
 //       equipgrid[mousex][mousey][currentdeck] = new EQUIPUNIT(equiptype, 0, roomnum, 0, 0);

    }

        //type, globalenum, room num, change these proper later
//you dont care about global equip  unless it goes above a limit: just assign it later in save.
}
//console must be in a room of an appropriate type... there must only be one at a time in a room, and then it has to
//occupy a non-wall and occupy the surrounding spaces: double check this to see how strict it is first.


void addconsole(int mousex, int mousey)
{
    // snap nearest
    int mx = cursnap(mousex);
    int my = cursnap(mousey);
    //test if its on an edge, if it is then abort
    //int numcollisions = 0;
    unsigned char roomtype = 0;
    unsigned char roomnum = 0;
    bool found = false;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (roomEtest(deckrooms[t][r][currentdeck], mx, my))
                return; // on an edge = fail
            if (pointInPolygon2(deckrooms[t][r][currentdeck], mx, my))
            {//then test to see if its inside a proper room of the right type
                roomtype = deckrooms[t][r][currentdeck]->roomtype;
                roomnum = deckrooms[t][r][currentdeck]->globalnum;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    //check type of room
    if (roomtype == BRIDGE)
    {
        if (equiptype != CAP_C &&
        equiptype != HEL_C &&
        equiptype != ENG_C &&
        equiptype != SHI_C &&
        equiptype != SEN_C &&
        equiptype != NAV_C &&
        equiptype != WEA_C )
            return;
    }
    else
    if (roomtype == ENGINE)
    {
        if (equiptype != ENG_C &&
        equiptype != POW_C )
                return;
    }
    else
    if (roomtype == SENSOR)
    {
        if (equiptype != SEN_C)
            return;
    }
    else
    if (roomtype == WEAP)
    {
        if (equiptype != WEA_C)
            return;
    }
    else
    if (roomtype == SHIELD)
    {
        if (equiptype != SHI_C)
            return;
    }
    else
    if (roomtype == MED)
    {
        if (equiptype != MED_C &&
            equiptype != MED_U)
            return;
    }
    else
    if (roomtype == TP)
    {
        if (equiptype != TRA_C)
            return;
    }
    else
        return;
    // its in the room, not on the edge, of the appropriate type: now we need to place it there and turn any surrounding equipment to dummy
    // first though, you need to test if theres any other same consoles in the same room
    // test all deck equipment for the same type as you are placing, then compare roomtypes. if a match,
    //call a remove-console  function to take care of that one and its dummies, then you can continue on.

    mx = cvtHEX2(mx);
    my = cvtHEX2(800-my);
    // first test to see if it places on top of an existing dummy, or a console

    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            if (equipgrid[i][j][currentdeck])
            {
                if (equipgrid[i][j][currentdeck]->type == equiptype) // found a match
                {
                    if (equipgrid[i][j][currentdeck]->roomid == roomnum)
                        removeconsole(i, j, currentdeck);
                    //put func here
                }//remove console this one
            }
        }
    }
    //bool canplace = true;
    // it can place on top of a console.
    if (equipgrid[mx][my][currentdeck]) // something is here
    {
        if (equipgrid[mx][my][currentdeck]->type == DUMMY)
            return; // dont let it place
    }
    else // theres nothing there, test surrounding ones
    {
        for (int i = mx-1; i < mx +2; i++)
        {
            for (int j = my -1; j < my +2; j++)
            {
                if (equipgrid[i][j][currentdeck])
                {
                    if (equipgrid[i][j][currentdeck]->type == DUMMY)
                        return;
                }
            }
        }
    }
    //last comment: this should work good enough for now.


    // it cannot place on top of a dummy.
    // if cannot place in a location where it overlaps a dummy
    for (int i = mx-1; i < mx +2; i++)
    {
        for (int j = my -1; j < my +2; j++)
        {
   //         if (i == mx && j == my)
   //             equipgrid[i][j][currentdeck] = new EQUIPUNIT(equiptype, 0, roomnum, 0, 0);
   //         else
   //             equipgrid[i][j][currentdeck] = new EQUIPUNIT(DUMMY, 0, roomnum, 0, 0);
        }
    }
    //console is removed, now we can safely place


    //then if its placable, replace any equipment near it with DUMMY equipment that doesn't get saved.
    //note that you need to change the save function to do this properly

}

void removeconsole(int posx, int posy, int deckn)
{
    // delete the position and directly surrounding positions, if they exist.
    //insert a test to make sure this is actually a console before continuing.
    for (int i = posx -1; i < posx +2; i++)
    {
        for (int j = posy - 1; j < posy+2; j++)
        {
            if (equipgrid[i][j][deckn])
            {
                delete equipgrid[i][j][deckn];
                equipgrid[i][j][deckn] = NULL;
            }
        }
    }

}

bool isconsole(int type)
{
    if (type == CAP_C ||
        type == HEL_C ||
        type == ENG_C ||
        type == SHI_C ||
        type == SEN_C ||
        type == NAV_C ||
        type == WEA_C ||
        type == POW_C ||
        type == MED_C ||
        type == TRA_C ||
        type == MED_U)
        return true;
    return false;

}

bool consoletest(int posx, int posy, int deckn)
{
    bool flag = false;
    for (int i = posx; i < posx +2; i++)
    {
        for (int j = posy; j < posy +2; j++)
        {
            if (i >= 0 && j >= 0 && i < 161 && j < 161) // make sure to test in bounds
            {
                if (equipgrid[i][j][deckn] && i != posx && j != posy) //dont test itself, only surroundings
                {
                    if (isconsole(equipgrid[i][j][deckn]->type))
                        flag = true;

                }

            }
        }
    }
    /*
    CAP_C = 0x49, HEL_C = 0x0C, ENG_C = 0x06, SHI_C = 0x11,
       SEN_C = 0x0F, NAV_C = 0x01, WEA_C = 0x07, POW_C = 0x0D, MED_C = 0x04, TRA_C = 0x13,
    */

    return flag;
}

void AddVLINK(int curx, int cury, ROOM *setroom)
{
    if (!setroom) // this isn't getting called atm
    {
        //fprintf(stderr,"%i\n", __LINE__);
        if (roomtype == 0x00)
        return;

        setroom = new ROOM;
        setroom->decknum = currentdeck;
        setroom->lverts = NULL;
        setroom->roomtype = roomtype;
        setroom->vnum = 0;
        for (int i = 0; i < 256; i ++)
        {
            if (!deckrooms[setroom->roomtype][i][setroom->decknum])
            {
                setroom->roomnum = i;
                deckrooms[setroom->roomtype][i][setroom->decknum] = setroom;

                break;
            }
        }
        for (int g = 0; g < numrooms; g++)
        {
            if (!rooms_g[g]) // assign first empty globalnum;
            {
                setroom->globalnum = g+1;
                rooms_g[g] = true;
                break;
            }
        }


    }
    VLINK *temp = NULL;
    VLINK *temp2 = NULL;
    temp = setroom->lverts;
    int testcurx = cursnap(curx);
    int testcury = cursnap(cury);

    if ((testcurx % 10 + testcury % 10) == 5)
    {
        if (testcurx % 10 == 5)
        {
            testcurx -= 5;
        }
        if (testcury % 10 == 5)
        {
            testcury -=5;
        } // this should all be good
    }
    //fprintf(stderr,"%i\n", __LINE__);
    testcury = (800 - testcury);
    if (temp == NULL) // case no verts
    {
        setroom->lverts = new VLINK;
        temp = setroom->lverts;

        temp->next = NULL;

        temp->cx = cvtHEX2(testcurx);
        temp->cy = cvtHEX2(testcury);
        temp->cz = 0x00;
        setroom->lverts = temp;
        setroom->vnum = 1;
        return;
    }  //otherwise we have verts
    VLINK *prev = temp;
    //fprintf(stderr,"%i\n", __LINE__);
    temp2 = temp->next;
    while (temp2)
    {
        prev = temp;
        temp = temp2;
        temp2 = temp2->next;
    } // prev = temp, next = temp2, lets do some math.  lets also start using the cx,cy,cz stuff.

    VLINK *quick = new VLINK;
    quick->cx = testcurx / 5;
    quick->cy = (testcury) / 5;
    quick->cz = 0;
    quick->next = NULL;

    if (!testlegal(temp, quick))
    {
        delete quick;
        return;
    }
    if (setroom->vnum >= 2 && roomIsec(setroom, temp->cx, temp->cy, quick->cx, quick->cy))
    {
        delete quick;
        return;
    }
    if (quick->cx == setroom->lverts->cx && quick->cy == setroom->lverts->cy)
    {
        delete quick;
        return;
    }
    if (setroom->vnum >2 && testslope(prev, temp, quick))
    {
        /*
        prev->next = quick;
        delete temp;
        if ((prev->cx == quick->cx) &&
            (prev->cy == quick->cy))
        {
            prev->next = NULL;
            delete quick;
            setroom->vnum--;
            fprintf(stderr,"Going1");
        }
        fprintf(stderr,"Going2");
        return;
        */
        delete prev->next;
        prev->next = quick;
        //delete temp;
        if ((prev->cx == quick->cx) &&
            (prev->cy == quick->cy))
        {
            prev->next = NULL;
            delete quick;
            setroom->vnum--;
        }
        return;
    }
    VLINK *parts[setroom->vnum];
    VLINK *parse = setroom->lverts;
    for (int i = 0; i < setroom->vnum; i++)
    {
        parts[i] = parse;
        parse = parse->next;
    }
    //point on line thingy
    int i, j;
    j = 0;
    for (i = 0; i < setroom->vnum-1; i++)
    {
        j++;
        if (testPsec(parts[i],parts[j], testcurx, 800 - testcury))
        {
            delete quick;
            return;
        }
    }
    //fprintf(stderr,"%i\n", __LINE__);
    delete quick;

    temp2 = new VLINK;

    temp2->cx = cvtHEX2(testcurx);
    temp2->cy = cvtHEX2(testcury);
    temp2->cz = 0x00;


    temp2->next = NULL;
    temp->next = temp2;
    setroom->vnum++;
    //fprintf(stderr,"%i\n", __LINE__);

}

void fillroom(int xpos, int ypos)
{
    if (isconsole(equiptype))
        return;
    ROOM *froom = NULL;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (pointInPolygon2(deckrooms[t][r][currentdeck], xpos, ypos))
            {
                froom = deckrooms[t][r][currentdeck];
                break;
            }
        }
    }
    if (!froom)
        return;
    if (!froom->lverts)
        return;
    VLINK *parts[froom->vnum];
    VLINK *parse = froom->lverts;
    for (int i = 0; i < froom->vnum; i++)
    {
        parts[i]=parse;
        parse = parse->next;
    }
    bool added = false;
    int b, c;
    c = 1;

    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            added = false;
            c = 1;
            if(pointInPolygon2(froom, i*5, 800-j*5))
            {
                //AddEquip(i*5, 800-j*5);
                AddEquip2(i*5, 800-j*5, currentdeck, 0, currentmod);
                added = true;
            }
            if (!added)
            {
                for (b = 0; b < froom->vnum; b++)
                {
                    if (testPsec(parts[b],parts[c], i*5, 800-j*5))
                    {
                        //AddEquip(i*5, 800-j*5);
                        AddEquip2(i*5, 800-j*5, currentdeck, 0, currentmod);
                    }
                    c++;
                    if (c >= froom->vnum)
                    {
                        c = 0;
                    }
                }
            }
        }
    }

    return;
}

bool DVLINK_L(ROOM *setroom)
{
    if (!setroom)
        return false;
    if (!setroom->lverts)
        return false;
    VLINK *temp = setroom->lverts;
    VLINK *last = temp;
    while (temp->next)
    {
        last = temp;
        temp = temp->next;
    }
    last->next = NULL;
    temp->next = NULL;
    delete temp;
    if (temp == last)
    {
        last = NULL;
    }
    setroom->vnum--;
    int rtype = setroom->roomtype;
    int dnum = setroom->decknum;
    int gnum = setroom->globalnum;


    if (last == NULL)
    {
        for(int i = 0; i < 256; i++)
        {
            if (deckrooms[rtype][i][dnum] == setroom) // find it in the list to remove it
            {
                delete deckrooms[rtype][i][dnum];
                deckrooms[rtype][i][dnum] = NULL;
                //setroom = NULL;
            }
        }


        for (int i = 0; i < 161; i++)
        {
            for (int j = 0; j < 161; j++)
            {
                if (equipgrid[i][j][dnum])
                {
                    if (equipgrid[i][j][dnum]->roomid == gnum)
                    {
                        delete equipgrid[i][j][dnum];
                        equipgrid[i][j][dnum] = NULL;
                    }
                }
            }
        }
        return true;

    }

    return false;
}

int cursnap(int cur)
{
    if (cur >= 800)
    {
        cur = 800;
        return cur;
    }
    int snap = cur;
    if (snap % 5 == 0 || snap % 5 == 1 || snap% 5 == 2)
    {
        cur = (cur / 5)*5;
        return cur;
    }
    else
    {
        cur = ((cur /5)*5) +5;
    }
    return cur;

}


unsigned char cvtHEX(int datanum)
{
    datanum = (datanum / 5); // get them to evenzies
    unsigned char testthis = datanum;
    testthis -=80; // this has a chance of working
    return testthis;
}

unsigned char cvtHEX2(int datanum)
{
    datanum = (datanum / 5); // get them to evenzies
    unsigned char testthis = datanum;
    //testthis -=80; // this has a chance of working
    return testthis;

}

//int legalline(VLINK )
bool saveroom(FILE *sfile, ROOM *sroom, int globalnum)
{

    uimode = X_NONE;
    if (!sfile || !sroom)
        return false;
    if (!sroom->lverts || !sroom->f_legal)
        return false;

    unsigned char globalroomnum1 = 0x00;
    unsigned char globalroomnum2 = 0x00;

    globalroomnum1 = (globalnum / 0x100);
    globalroomnum2 = (globalnum - (globalroomnum1 * 0x100));

    //globalroomnum2 += ()
    unsigned decknum = (sroom->decknum - 28);
    unsigned roomtype = sroom->roomtype;
    unsigned char roomnum = sroom->roomnum;
    unsigned char roompolies1 = 0x00;
    unsigned char roompolies2 = 0x00;

    roompolies1 = (sroom->vnum / 0x100);
    roompolies2 = (sroom->vnum - (roompolies1 * 0x100));

    //this needs moved to roomsave
    unsigned char xvals[sroom->vnum];
    unsigned char yvals[sroom->vnum];
    VLINK *temp = sroom->lverts;
    for (int i = 0; i < sroom->vnum; i++)
    {
        xvals[i] = (temp->cx - 80);
        yvals[i] = (temp->cy - 80);
        if (temp->next)
        {
            temp = temp->next;
        }
    }
    fwrite (&shipid, 1, 4, sfile);
    fwrite (&globalroomnum1, 1, 1, sfile);
    fwrite (&globalroomnum2, 1, 1, sfile);
    fwrite (&decknum, 1, 1, sfile);
    fwrite (&roomtype, 1, 1, sfile);
    fwrite (&roomnum, 1, 1, sfile);
    fwrite (&roompolies1, 1, 1, sfile);
    fwrite (&roompolies2, 1, 1, sfile);
    for (int i = 0; i < sroom->vnum; i++)
    {
        fwrite (&xvals[i], 1, 1, sfile);
        fwrite (&yvals[i], 1, 1, sfile);
    }
    return true;


}

bool saveequip(FILE *sfile, EQUIPUNIT *eunit, unsigned char xpos, unsigned char ypos, unsigned char decknum)
{
    if (!sfile || !eunit)
        return false;
    if (eunit->type == DUMMY)
        return false;
    //totaleunits needs writing elsewhere
    unsigned char dsave = decknum-28;
    fwrite (&dsave, 1, 1, sfile);
    //unsigned char test = eunit->type;
    fwrite (&eunit->type, 1, 1, sfile);
    unsigned char fill = 0x00;
    fwrite (&fill, 1, 1, sfile);
    // need to convert next, if it is little endian
    long int g_id = eunit->e_id;
    short int r_id = eunit->roomid;
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
    g_id = (g_id>>24 |
           ((g_id<<8 & 0x00FF0000) |
           ((g_id>>8 & 0x0000FF00) |
           (g_id<<24))));
    r_id = (r_id>>8) |
           (r_id<<8);
    #endif
    unsigned char xpos2 = (xpos - 80);
    unsigned char ypos2 = (ypos - 80);
    fwrite (&g_id, 4, 1, sfile);
    fwrite (&shipid, 4, 1, sfile);
    fwrite (&xpos2, 1, 1, sfile);
    fwrite (&ypos2, 1, 1, sfile);
    fwrite (&r_id, 2, 1, sfile);
    fwrite (&eunit->mod1, 1, 1, sfile);
    fwrite (&eunit->mod2, 1, 1, sfile);

    return true;
}

bool diagonaltest(int oldx, int oldy, int newx, int newy)
{
    int highx;
    int lowx;
    int highy;
    int lowy;
    if (oldx > newx)
    {
        highx = oldx;
        lowx = newx;
    }
    else
    {
        highx = newx;
        lowx = oldx;
    }
    if (oldy > newy)
    {
        highy = oldy;
        lowy = newy;
    }
    else
    {
        highy = newy;
        lowy = oldy;
    }
    int difx = highx - lowx;
    int dify = highy - lowy;
    if (difx != dify) // not diagonal
    {
        return false;
    }
    return true;

}


bool savetofile()
{
    finishroom2(a_room);
    a_room = NULL;

    FILE *shipsave = NULL;
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
    unsigned filler = 0xFFFFFFFF;
    unsigned charid = 0x94C8620A;
    unsigned intro = 0x00000010;
    unsigned modules1 = 0x000301EA;
    unsigned modules2 = 0x00EE00ED;
    unsigned modules3 = 0x01EF00EB;
    unsigned modules4 = 0x00F00007;

    #else
    unsigned filler = 0xFFFFFFFF;
    unsigned charid = 0x0A62C894;
    unsigned intro = 0x10000000;
    unsigned modules1 = 0xEA010300;
    unsigned modules2 = 0xED00EE00;
    unsigned modules3 = 0xEB00EF01;
    unsigned modules4 = 0x0700F000;

    #endif

    unsigned armorl = mainship.armorlevel;
    unsigned streaml = mainship.streamlined;
    unsigned namebytes = 0x08;
    unsigned unknown1 = 0x00000000;
    unsigned unknown2 = 0x0000;
    unsigned char totalroomnum1 = 0x00;
    unsigned char totalroomnum2 = 0x00;

    savedisplay(0,0,FINISHROOM);
    int totalglobal = 0;
    for (int t = 0; t < 22; t++)
    {
        for (int n = 0; n < 256; n++)
        {
            for (int d = 0; d < 57; d++)
            {
                finishroom2(deckrooms[t][n][d]);
                if (deckrooms[t][n][d] && deckrooms[t][n][d]->f_legal)
                {
                    totalglobal++;
                    savedisplay(totalglobal, totalglobal, FINISHROOM);
                    if (totalglobal == 65536)
                        break; // error more rooms than can exist
                }
            }
        }
    }
    if (totalglobal == 0)
    {
        return false;
    }

    totalroomnum1 = (totalglobal / 0x100);
    totalroomnum2 = (totalglobal- (totalroomnum1 * 0x100));

    shipsave=fopen("testSODS.ship", "wb");
    savedisplay(0, 0, PARSEROOM);
    if (shipsave!=NULL)
    {
        fwrite (&shipid, 1, 4, shipsave);
        fwrite (&filler, 1, 4, shipsave);
        fwrite (&charid, 1, 4, shipsave);
        fwrite (&streaml, 1, 1, shipsave);
        fwrite (&armorl, 1, 1, shipsave);
        fwrite (&intro, 1, 4, shipsave);
        fwrite (&namebytes, 1, 1, shipsave);
        fputc ('\0', shipsave);
        fputc ('S', shipsave);
        fputc ('\0', shipsave);
        fputc ('O', shipsave);
        fputc ('\0', shipsave);
        fputc ('D', shipsave);
        fputc ('\0', shipsave);
        fputc ('S', shipsave);
        fwrite (&filler, 1, 4, shipsave);
        fwrite (&unknown1, 1, 4, shipsave);
        fwrite (&unknown2, 1, 2, shipsave);
        fwrite (&modules1, 1, 4, shipsave);
        fwrite (&modules2, 1, 4, shipsave);
        fwrite (&modules3, 1, 4, shipsave);
        fwrite (&modules4, 1, 4, shipsave);
        fwrite (&unknown1, 1, 3, shipsave); // dont really know this either yet, save time

        fwrite (&totalroomnum1, 1, 1, shipsave);
        fwrite (&totalroomnum2, 1, 1, shipsave);

        int cglobalnum = 0;
        for (int t = 0; t < 22; t++)
        {
            for (int n = 0; n < 256; n++)
            {
                for (int d = 0; d < 57; d++)
                {
                    if (deckrooms[t][n][d]) //theres a room here
                    {

                        if (cglobalnum < totalglobal && totalglobal < 65535) // save rooms till theres no more possible to save
                        {
                            savedisplay(cglobalnum, totalglobal, SAVEROOM);
                            saveroom(shipsave, deckrooms[t][n][d], deckrooms[t][n][d]->globalnum); //important check this later
                            cglobalnum++;
                        }
                    }

                }
            }
        }


        int numequipsaved = 0;
        globalequip = 0;
        for (int x = 0; x < 161; x++)
        {
            for (int y = 0; y < 161; y++)
            {
                for (int z = 0; z < 57; z++)
                {
                    if (equipgrid[x][y][z])
                    {

                        if (equipgrid[x][y][z]->type != DUMMY)
                        {
                            savedisplay(globalequip, globalequip, PARSEEQUIP);
                            globalequip ++;
                            equipgrid[x][y][z]->e_id = globalequip;
                        }

                    }

                }
            }
        }

        long int g_ep = globalequip;

        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        g_ep = (g_ep>>24 |
               ((g_ep<<8 & 0x00FF0000) |
               ((g_ep>>8 & 0x0000FF00) |
               (g_ep<<24))));
        #endif


        if (globalequip > 0)
        {
            fwrite (&g_ep, 4, 1, shipsave);
            for (int x = 0; x < 161; x++)
            {
                for (int y = 0; y < 161; y++)
                {
                    for (int z = 0; z < 57; z++)
                    {
                        if (equipgrid[x][y][z])
                        {
                            savedisplay(numequipsaved, globalequip, SAVEEQUIP);
                            numequipsaved += saveequip(shipsave, equipgrid[x][y][z], x, y, z);
                        }
                        if (numequipsaved > globalequip)
                            break;
                    }
                    if (numequipsaved > globalequip)
                            break;
                }
                if (numequipsaved > globalequip)
                            break;
            }
        }


        fclose (shipsave);
    }





    return 1;
}

SDL_Surface *LoadIMGA(char *filename)
{
//    SDL_Surface *image;
    SDL_Surface *temp;


    temp = IMG_Load(filename);
    return(temp);
    /*
    if ( temp == NULL ) {
        fprintf(stderr, "Unable to load %s: %s\n", filename, SDL_GetError());
        return(NULL);
    }

     Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    //image = SDL_CreateRGBSurface(SDL_SWSURFACE, temp->w,temp->h, 32, rmask, gmask, bmask, amask);
    //SDL_BlitSurface(temp, NULL, image, NULL);
    //SDL_FreeSurface(temp);


    return(image); */

}

GLvoid LoadGLTextures()
{
    int numtextures = 2;
    SDL_Surface* tex[2] = {NULL, NULL};
    tex[0] = LoadIMGA((char*)"icons.png");
    tex[1] = LoadIMGA((char*)"Font.png");
    //gen tex 1 using that other thing to test this shit.
    for (int i = 0; i < numtextures; i++)
    {
        if (!tex[i])
        {
            SDL_Quit();
            exit(5);
        }
    }
    glGenTextures(numtextures, &texture[0]);

    for (int i = 0; i < numtextures; i++)
    {
        GLuint texpoint = texture[i];
        glBindTexture(GL_TEXTURE_2D, texpoint);
        glEnable(GL_TEXTURE_2D);


        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex[i]->w, tex[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex[i]->pixels);


    }
    for (int i = 0; i < numtextures; i++)
    {
        SDL_FreeSurface(tex[i]);
    }
}


bool roomIsec(ROOM *droom, float x1, float y1, float x2, float y2)
{

    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    VLINK rverts[droom->vnum];
    VLINK *temp = droom->lverts;
    for (int i = 0; i < droom->vnum; i++)
    {
        rverts[i].cx = temp->cx;
        rverts[i].cy = temp->cy;
        rverts[i].cz = temp->cz;
        temp= temp->next;
    }
    int j = 1;
    for (int i = 0; i < droom->vnum; i++)
    {
        if (j >= droom->vnum)
            j = 0;
        if (testIsec(x1, y1, x2, y2, rverts[i].cx, rverts[i].cy,
                     rverts[j].cx, rverts[j].cy))
            return true;
        j++;

    }

    return false;
}

bool roomIsec2(ROOM *droom, float x1, float y1, float x2, float y2)
{

    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    VLINK rverts[droom->vnum];
    VLINK *temp = droom->lverts;
    for (int i = 0; i < droom->vnum; i++)
    {
        rverts[i].cx = temp->cx;
        rverts[i].cy = temp->cy;
        rverts[i].cz = temp->cz;
        temp= temp->next;
    }
    int j = 1;
    for (int i = 0; i < droom->vnum; i++)
    {
        if (j >= droom->vnum)
            j = 0;
        if (testIsec2(x1, y1, x2, y2, rverts[i].cx, rverts[i].cy,
                     rverts[j].cx, rverts[j].cy))
            return true;
        j++;

    }

    return false;
}

bool testIsec(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    if ((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4))
        return false;
    Vector la1(x1, y1);
    Vector la2(x2, y2);
    Vector lb1(x3, y3);
    Vector lb2(x4, y4);

    return LineITest(la1, la2, lb1, lb2);

    return false;
}

bool testIsec2(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    if ((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4))
        return false;
    Vector la1(x1, y1);
    Vector la2(x2, y2);
    Vector lb1(x3, y3);
    Vector lb2(x4, y4);

    return LineITest2(la1, la2, lb1, lb2);

    return false;
}

bool LineITest(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3)
{
    LineSegment linesegment0(p0, p1);
    LineSegment linesegment1(p2, p3);

    Vector intersection;

    //std::cout << "Line Segment 0: (" << p0.x_ << ", " << p0.y_ << ") to (" << p1.x_ << ", " << p1.y_ << ")\n"
    //          << "Line Segment 1: (" << p2.x_ << ", " << p2.y_ << ") to (" << p3.x_ << ", " << p3.y_ << ")\n";

    switch(linesegment0.Intersect(linesegment1, intersection))
    {
    case LineSegment::PARALLEL:
        return false;
        break;
    case LineSegment::COINCIDENT:
        return false;
        break;
    case LineSegment::NOT_INTERESECTING:
        return false;
        break;
    case LineSegment::INTERESECTING:
        return true;
        break;
    case LineSegment::POINTINTERSECTING:
        return true;
        break;
    }
    return false;
}

bool LineITest2(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3)
{
    LineSegment linesegment0(p0, p1);
    LineSegment linesegment1(p2, p3);

    Vector intersection;

    //std::cout << "Line Segment 0: (" << p0.x_ << ", " << p0.y_ << ") to (" << p1.x_ << ", " << p1.y_ << ")\n"
    //          << "Line Segment 1: (" << p2.x_ << ", " << p2.y_ << ") to (" << p3.x_ << ", " << p3.y_ << ")\n";

    switch(linesegment0.Intersect(linesegment1, intersection))
    {
    case LineSegment::PARALLEL:
        return false;
        break;
    case LineSegment::COINCIDENT:
        return false;
        break;
    case LineSegment::NOT_INTERESECTING:
        return false;
        break;
    case LineSegment::INTERESECTING:
        return true;
        break;
    case LineSegment::POINTINTERSECTING:
        return false;
        break;
    }
    return false;
}

bool testslope(VLINK* prev, VLINK* curr, VLINK* next)
{
    if (!prev || !curr || !next)
        return false;

    float x1, x2, x3, y1, y2, y3;
    float numer1, numer2;
    float denom1, denom2;
    x1 = prev->cx; x2 = curr->cx; x3 = next->cx;
    y1 = prev->cy; y2 = curr->cy; y3 = next->cy;
    denom1 = x2 - x1;
    denom2 = x3 - x2;
    numer1 = y2 - y1;
    numer2 = y3 - y2;
    if (denom1 == 0 || denom2 == 0) // no x change
    {
        if (denom1 == denom2)
            return true;
        else
            return false;
    }
    float slope1, slope2;
    slope1 = numer1 / denom1;
    slope2 = numer2 / denom2;
    if (slope1 == slope2)
        return true;


    return false; // = its good

}

bool pointInPolygon3(ROOM *droom, int testx, int testy)
{
    return pointInPolygon2(droom, testx*5, 800-testy*5);
}

bool pointInPolygon2(ROOM *droom, float testx, float testy)
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    int nvert = droom->vnum;
    float vertx[nvert];
    float verty[nvert];
    VLINK *temp = droom->lverts;
    for (int n = 0; n < nvert; n++)
    {
        vertx[n] = (float)(temp->cx*5);
        verty[n] = (float)(800-(temp->cy*5));
        temp = temp->next;
    }

    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;

}

bool finishroom2(ROOM *droom)
{
    if (!droom)
        return false;
    droom->f_legal = false;
    bool badroom = false;
    int dnum = droom->decknum;
    int gnum = droom->globalnum;

    if (!CWTest(droom))
        CvtClockW(droom);
    if (!CWTest(droom)) // no area still!
    {
        while(droom->lverts)
        {

            if (DVLINK_L(droom))
            {
                badroom = true;
                break;
            }

        }

    }
    //we have a potential bad spot here. need to figure this out.
    if (badroom)
    {

        //just delete any equipment associated and gtfo
        for (int i = 0; i < 161; i++)
        {
            for (int j = 0; j < 161; j++)
            {
                if (equipgrid[i][j][dnum])
                {
                    if (equipgrid[i][j][dnum]->roomid == gnum)
                    {
                    delete equipgrid[i][j][dnum];
                    equipgrid[i][j][dnum] = NULL;
                    }
                }
            }
        }
        if (rooms_g[droom->globalnum -1])
            rooms_g[droom->globalnum -1] = false;

        return false;
    }

    if (!droom->lverts || droom->vnum < 3)
    {
        // need to delete it and the appropriate room
       // while (droom->lverts)
       // {
       //     DVLINK_L(droom);
       // }
        clearroom(droom);
        delete droom;
        //droom->f_legal = false;
        return false;
    }

    droom->f_legal = true;


    VLINK *nodes[droom->vnum];
    VLINK *temp = droom->lverts;
    //bool dflag[droom->vnum];


    for (int i = 0 ; i < droom->vnum; i++)
    {
        nodes[i]=temp;
        //dflag[i] = false;
        temp = temp->next;
    }

    int b;
    int c = 1;
    int d = 2;
    for (b = 0; b < droom->vnum; b++)
    {
        if (d >= droom->vnum)
            d = 0;
        if (c >= droom->vnum)
            c = 0;

        if (!testlegal(nodes[b], nodes[c]))
            droom->f_legal = false;


        c++;
        d++;
    }




    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            if(equipgrid[i][j][droom->decknum])
            {
                int etype = equipgrid[i][j][droom->decknum]->type;
                //fprintf(stdout,"Cequip %i\n", equiptype);
                if (etype == T_BOT)
                    etype = T_TOP;
                if (etype >= T_SIDE_UR && etype <= T_SIDE_DR)
                    etype = T_SIDE_R;
                currentmod = equipgrid[i][j][droom->decknum]->mod1*0x100+equipgrid[i][j][droom->decknum]->mod2;
                equiptype = etype;
                //fprintf(stdout,"Cequip2 %i\n", equiptype);
                currentrot = 0;
                delete equipgrid[i][j][droom->decknum];
                equipgrid[i][j][droom->decknum] = NULL;
                AddEquip2(i*5, 800-j*5, droom->decknum, 0, currentmod);

            }



        }
    }
    int i, j;
    j = 1;
    for (int t = 0; t < 22; t++)
    {
        //j = 1;
        //i = 0;
        for (int r = 0; r < 256; r++)
        {

            //if (i > droom->vnum)
            //    i = 0;
            for (i = 0; i < droom->vnum; i++)
            {
                if (j >= droom->vnum)
                    j = 0;
                if (roomIsec2(deckrooms[t][r][droom->decknum], // yep need a slightly diff function
                             nodes[i]->cx, nodes[i]->cy,
                             nodes[j]->cx, nodes[j]->cy))
                {
                    droom->f_legal = false;
                    //return;
                }
                j++;

            }

            //if (roomIsec(deckrooms[t][r][droom->decknum], ))
        }
    }
    VLINK *prev = droom->lverts;
    temp = prev->next;
    VLINK *next = temp->next;

    while (next)
    {
        if (testslope(prev, temp, next))
        {
            prev->next = next;
            delete temp;
            droom->vnum -=1;
            temp = next;
            next = next->next;
        }
        else
        {
            prev = temp;
            temp = next;
            next = next->next;
        }
    }
    // when we come out, next = NULL, temp = last.
    if (testslope(prev, temp, droom->lverts))
    {
        prev->next = NULL;
        delete temp;
        temp = prev;
        droom->vnum--;
    }

    if (testslope(temp, droom->lverts, droom->lverts->next))
    {
        temp = droom->lverts->next;
        delete droom->lverts;
        droom->lverts = temp;
        droom->vnum--;
    }
    currentmod = 0x0106;

    // test room collisions with other rooms, if collide, set a flag
    // test to see if room has equipment in it, if so, collide each with the new room to see if they are in it, if not, eliminate
    return true;

}

void finishroom(ROOM *droom)
{
/*

            if(equipgrid[i][j][droom->decknum]) // the equipment exists
            {
                // first get the type of the room and sort this shit out proper
                int etype = equipgrid[i][j][droom->decknum]->type;
                int placetype = setcollision(equipgrid[i][j][droom->decknum]->type);
                //int gettype = equipgrid[i][j][droom->decknum]->type;

                numcollisions = 0;
                edgeflag = EMPTY;
                thisroom = false;
                deleteflag = false;
                if(equipgrid[i][j][droom->decknum]->roomid == droom->globalnum) // and its part of this room
                { // test vs all rooms on this floor
                    if (roomEtest(droom, i*5, 800-j*5))
                        thisroom = true;
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            numcollisions+=roomEtest(deckrooms[t][r][droom->decknum], i*5, 800-j*5);
                        }
                    }
                }
                if (numcollisions >1) //inner edge
                    edgeflag = EDGE_I;
                if (numcollisions == 1)
                    edgeflag = EDGE_O;
                if (edgeflag == EMPTY) // only do the inside test if its not on an edge
                {
                    numcollisions+=pointInPolygon2(droom, i*5, 800-j*5);
                    if (numcollisions > 0)
                    {
                        //exit(9);
                        edgeflag = INSIDE;
                        thisroom = true;
                    }
                } //flag is now either empty, on an I, O edge and in/out of this room, or inside
                //now we need to handle equipment delete
                //E_IW_ONLY
                //E_HW_ONLY

                if (thisroom == false || edgeflag == EMPTY) //may want to get the allowable spots on the equipment unit. feckit, later
                    deleteflag = true;
                if (edgeflag == EDGE_I && (placetype != E_IW_ONLY))
                    deleteflag = true;

                if (edgeflag == EDGE_O && (placetype != E_MoW &&
                                           placetype != E_HW_ONLY &&
                                           placetype != E_TURRET))
                    deleteflag = true;
                //bool skip = false;
                if (placetype == E_TURRET)
                {
                    if (etype == T_BOT && edgeflag != INSIDE)
                        deleteflag = true;
                    if (etype == T_TOP &&
                        ((edgeflag == INSIDE) ||
                         (edgeflag == EDGE_I) ||
                         (edgeflag == EDGE_O))) //wont be in THIS room because its on top of its room. so....
                    {
                        delete equipgrid[i][j][droom->decknum];
                        equipgrid[i][j][droom->decknum] = NULL;
                    }
                }




                //if (!skip)
                if (deleteflag && equipgrid[i][j][droom->decknum]->roomid == droom->globalnum) //sloppy but works
                {
                    if (placetype != E_CONSOLE &&
                        placetype != E_OTHER)
                    {
                        delete equipgrid[i][j][droom->decknum];
                        equipgrid[i][j][droom->decknum] = NULL;
                    }
                    if (placetype == E_CONSOLE)
                        removeconsole(i,j,droom->decknum);
                    if (placetype == E_OTHER) // a dummy one, special case. may get more complex later, but remove
                    { //tested: does not care! leave it in
                        //exit(9);
                        //for (int n = i -1; n < i+2; n++)
                        //{
                        //    for (int m = j -1; m < j+2; m++)
                        //    {
                        //        removeconsole(n, m, droom->decknum);
                        //    }
                        //}

                    }

                }
                if (etype == T_SIDE_R ||
                        ((etype >= T_SIDE_UR) && (etype <= T_SIDE_DR))) // now we have cases, lots of fuckin cases.
                    { // no really, lots of cases. just delete it and try adding it again?
                        if (equipgrid[i][j][droom->decknum])
                        {
                            currentmod = equipgrid[i][j][droom->decknum]->mod1*0x100 +
                                                equipgrid[i][j][droom->decknum]->mod2;
                            equiptype = T_SIDE_R;
                            delete equipgrid[i][j][droom->decknum];
                            equipgrid[i][j][droom->decknum] = NULL;
                            AddEquip2(i*5, 800-j*5, droom->decknum, 0, currentmod);

                        }

                    }
            } */

    // here is where we do various tests
    // test room legal (all sides legal & not collide with self), if so, then we link the room

    if (!droom)
        return;
    droom->f_legal = false;
    bool badroom = false;
    int dnum = droom->decknum;
    int gnum = droom->globalnum;

    if (!CWTest(droom))
        CvtClockW(droom);
    if (!CWTest(droom)) // no area still!
    {
        while(droom->lverts)
        {

            if (DVLINK_L(droom))
            {
                badroom = true;
                break;
            }

        }

    }
    //we have a potential bad spot here. need to figure this out.
    if (badroom)
    {

        //just delete any equipment associated and gtfo
        for (int i = 0; i < 161; i++)
        {
            for (int j = 0; j < 161; j++)
            {
                if (equipgrid[i][j][dnum])
                {
                    if (equipgrid[i][j][dnum]->roomid == gnum)
                    {
                    delete equipgrid[i][j][dnum];
                    equipgrid[i][j][dnum] = NULL;
                    }
                }
            }
        }
        //exit(9);
        return;
    }

    if (!droom->lverts || droom->vnum < 3)
    {
        // need to delete it and the appropriate room
        while (droom->lverts)
        {
            DVLINK_L(droom);
        }
        delete droom;
        //droom->f_legal = false;
        return;
    }
    VLINK *first = droom->lverts;
    VLINK *last = droom->lverts;
    VLINK *prev = last;
    while (last->next)
    {
        prev = last;
        last = last->next;
    }

    if (testlegal(first, last) && !roomIsec(droom, first->cx, first->cy, last->cx, last->cy))
        droom->f_legal = true;

    if (testslope(prev, last, first))
    {
        prev->next = NULL;
        delete last;
        droom->vnum--;
    }
    // collide all equipment here

    VLINK *nodes[droom->vnum];
    VLINK *temp = droom->lverts;


    for (int i = 0 ; i < droom->vnum; i++)
    {
        nodes[i]=temp;
        temp = temp->next;
    }


    int c;
    c = 1;
    int edgeflag = EMPTY;
    int numcollisions = 0;
    bool thisroom = false;
    bool deleteflag = false;



    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            if(equipgrid[i][j][droom->decknum]) // the equipment exists
            {
                // first get the type of the room and sort this shit out proper
                int placetype = E_MoW;
                int gettype = equipgrid[i][j][droom->decknum]->type;

                if (gettype == FTL_U ||
                    gettype == LIF_U ||
                    gettype == TRA_U ||
                    gettype == BED_U ||
                    gettype == BED2_U)
                    placetype = E_M_ONLY;

                if (gettype == CAP_C ||
                    gettype == HEL_C ||
                    gettype == ENG_C ||
                    gettype == SHI_C ||
                    gettype == SEN_C ||
                    gettype == NAV_C ||
                    gettype == WEA_C ||
                    gettype == POW_C ||
                    gettype == MED_C ||
                    gettype == MED_U)
                    placetype = E_CONSOLE;

                if (gettype == DOOR_C ||
                    gettype == DOOR_P ||
                    gettype == DOORL_C ||
                    gettype == DOORL_P ||
                    gettype == OPENING_I ||
                    gettype == RAIL)
                    placetype = E_IW_ONLY;

                if (gettype == DOOR_H ||
                    gettype == DOORL_H ||
                    gettype == OPENING_E )
                    placetype = E_HW_ONLY;

                if (gettype == DUMMY)
                    placetype = E_OTHER;


                numcollisions = 0;
                edgeflag = EMPTY;
                thisroom = false;
                deleteflag = false;
                if(equipgrid[i][j][droom->decknum]->roomid == droom->globalnum) // and its part of this room
                { // test vs all rooms on this floor
                    if (roomEtest(droom, i*5, 800-j*5))
                        thisroom = true;
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            numcollisions+=roomEtest(deckrooms[t][r][droom->decknum], i*5, 800-j*5);
                        }
                    }
                }
                if (numcollisions >1) //inner edge
                    edgeflag = EDGE_I;
                if (numcollisions == 1)
                    edgeflag = EDGE_O;
                if (edgeflag == EMPTY) // only do the inside test if its not on an edge
                {
                    numcollisions+=pointInPolygon2(droom, i*5, 800-j*5);
                    if (numcollisions > 0)
                    {
                        //exit(9);
                        edgeflag = INSIDE;
                        thisroom = true;
                    }
                } //flag is now either empty, on an I, O edge and in/out of this room, or inside
                //now we need to handle equipment delete
                //E_IW_ONLY
                //E_HW_ONLY
                if (thisroom == false || edgeflag == EMPTY) //may want to get the allowable spots on the equipment unit. feckit, later
                    deleteflag = true;
                if (edgeflag == EDGE_I && (placetype != E_IW_ONLY))
                    deleteflag = true;
                if (edgeflag == EDGE_O && (placetype != E_MoW &&
                                           placetype != E_HW_ONLY))
                    deleteflag = true;

                if (deleteflag && equipgrid[i][j][droom->decknum]->roomid == droom->globalnum) //sloppy but works
                {
                    if (placetype != E_CONSOLE &&
                        placetype != E_OTHER)
                    {
                        delete equipgrid[i][j][droom->decknum];
                        equipgrid[i][j][droom->decknum] = NULL;
                    }
                    if (placetype == E_CONSOLE)
                        removeconsole(i,j,droom->decknum);
                    if (placetype == E_OTHER) // a dummy one, special case. may get more complex later, but remove
                    { //tested: does not care! leave it in
                        //exit(9);
                        //for (int n = i -1; n < i+2; n++)
                        //{
                        //    for (int m = j -1; m < j+2; m++)
                        //    {
                        //        removeconsole(n, m, droom->decknum);
                        //    }
                        //}

                    }

                }
            }
        }
       // exit(9);

    }
    int i, j;
    j = 1;
    for (int t = 0; t < 22; t++)
    {
        //j = 1;
        //i = 0;
        for (int r = 0; r < 256; r++)
        {

            //if (i > droom->vnum)
            //    i = 0;
            for (i = 0; i < droom->vnum; i++)
            {
                if (j >= droom->vnum)
                    j = 0;
                if (roomIsec2(deckrooms[t][r][droom->decknum], // yep need a slightly diff function
                             nodes[i]->cx, nodes[i]->cy,
                             nodes[j]->cx, nodes[j]->cy))
                {
                    droom->f_legal = false;
                    return;
                }
                j++;

            }

            //if (roomIsec(deckrooms[t][r][droom->decknum], ))
        }
    }

    // test room collisions with other rooms, if collide, set a flag
    // test to see if room has equipment in it, if so, collide each with the new room to see if they are in it, if not, eliminate
    return;
}

bool roomEtest2(ROOM *droom, int xpos, int ypos)
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;

    VLINK *parts[droom->vnum];
    VLINK *parse;
    parse = droom->lverts;
    for (int i = 0; i < droom->vnum; i++)
    {
        parts[i] = parse;
        parse = parse->next;
    }

    int b, c;
    c = 1;
    for (b = 0; b < droom->vnum; b++)
    {
        if (c >= droom->vnum)
            c = 0;
        if (testPsec(parts[b], parts[c], xpos*5, 800-ypos*5))
            return true;
        c++;
    }
    return false;
}

bool roomEtest(ROOM *droom, int xpos, int ypos) //test to see if a point hits any room edge
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;

    VLINK *parts[droom->vnum];
    VLINK *parse;
    parse = droom->lverts;
    for (int i = 0; i < droom->vnum; i++)
    {
        parts[i] = parse;
        parse = parse->next;
    }

    int b, c;
    c = 1;
    for (b = 0; b < droom->vnum; b++)
    {
        if (c >= droom->vnum)
            c = 0;
        if (testPsec(parts[b], parts[c], xpos, ypos))
            return true;
        c++;
    }
    return false;
}

bool roomEtest3(ROOM *droom, int xpos, int ypos) //test to see if a point hits any room edge
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;

    VLINK *parts[droom->vnum];
    VLINK *parse;
    parse = droom->lverts;
    for (int i = 0; i < droom->vnum; i++)
    {
        parts[i] = parse;
        parse = parse->next;
    }

    int b, c;
    c = 1;
    for (b = 0; b < droom->vnum; b++)
    {
        if (c >= droom->vnum)
            c = 0;
        if (testPsec2(parts[b], parts[c], xpos, ypos))
            return true;
        c++;
    }
    return false;
}

bool testPsec(VLINK *line1, VLINK *line2, int xpos, int ypos)
{
    if (!line1 || !line2)
        return false;
    XYZ LineStart, LineEnd, testp;
    float Distance = 0;

    LineStart.X = line1->cx; LineStart.Y = line1->cy; LineStart.Z = 0;
    LineEnd.X = line2->cx; LineEnd.Y = line2->cy; LineEnd.Z = 0;
    testp.X = xpos/5; testp.Y = (800-ypos)/5; testp.Z = 0;

    if( DistancePointLine(&testp, &LineStart, &LineEnd, &Distance))
    {
        if (Distance <= 0.005)
            return true;
    }
    return false;
}

bool testPsec2(VLINK *line1, VLINK *line2, int xpos, int ypos)
{
    if (!line1 || !line2)
        return false;
    XYZ LineStart, LineEnd, testp;
    float Distance = 0;

    LineStart.X = line1->cx*5; LineStart.Y = (800-line1->cy*5); LineStart.Z = 0;
    LineEnd.X = line2->cx*5; LineEnd.Y = (800-line2->cy*5); LineEnd.Z = 0;
    testp.X = xpos; testp.Y = ypos; testp.Z = 0;

    if( DistancePointLine(&testp, &LineStart, &LineEnd, &Distance))
    {
        if (Distance <= 0.005)
            return true;
    }
    return false;
}

float Magnitude( XYZ *Point1, XYZ *Point2 )
{
    XYZ Vector;

    Vector.X = Point2->X - Point1->X;
    Vector.Y = Point2->Y - Point1->Y;
    Vector.Z = Point2->Z - Point1->Z;

    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

int DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )
{
    float LineMag;
    float U;
    XYZ Intersection;

    LineMag = Magnitude( LineEnd, LineStart );

    U = ( ( ( Point->X - LineStart->X ) * ( LineEnd->X - LineStart->X ) ) +
        ( ( Point->Y - LineStart->Y ) * ( LineEnd->Y - LineStart->Y ) ) +
        ( ( Point->Z - LineStart->Z ) * ( LineEnd->Z - LineStart->Z ) ) ) /
        ( LineMag * LineMag );

    if( U < 0.0f || U > 1.0f )
        return 0;   // closest point does not fall within the line segment

    Intersection.X = LineStart->X + U * ( LineEnd->X - LineStart->X );
    Intersection.Y = LineStart->Y + U * ( LineEnd->Y - LineStart->Y );
    Intersection.Z = LineStart->Z + U * ( LineEnd->Z - LineStart->Z );

    *Distance = Magnitude( Point, &Intersection );

    return 1;
}

bool cleardesign() // think its working now
{
    // make sure a_room points to nothing, and anything else.

    finishroom2(a_room);
    a_room = NULL;
    //delete all equipment, then delete all rooms, then set decknum back to 28 and any other defaults
    for (int r = 0; r < 22; r++)
    {
        for (int n = 0; n < 256; n++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (deckrooms[r][n][d])
                {
                    clearroom(deckrooms[r][n][d]);
                    delete deckrooms[r][n][d];
                }
                deckrooms[r][n][d] = NULL;
            }
        }
    }

    for (int j = 0; j < 161; j++)
    {
        for (int i = 0; i < 161; i++)
        {
            for (int n = 0; n < 57; n++)
            {
                if (equipgrid[j][i][n])
                {
                    delete equipgrid[j][i][n];
                }
                equipgrid[j][i][n] = NULL; // all initialized to NULL
            }
        }
    }
    for (int i = 0; i < 65536; i++)
    {
        rooms_g[i] = false;
    }
    currentdeck = 28;
    //currentequip = 0x00;
    //setequip = 0x00;
    uimode = X_NONE;

    return true;

}
long int loadFindID(unsigned char sh_ID[4], FILE *ship, int &err);

struct xyvert
{
    unsigned char x;
    unsigned char y;
};

//errtypes
enum {L_EOF = 1, BADROOM = 2};
xyvert *loadgetroom(unsigned char *info, int &ncorner, int &err, FILE *ship);
ROOM  *loadsetroom(unsigned char info[5], int ncorner, xyvert corners[]);
void loadgetequip(unsigned char &l_x, unsigned char &l_y, unsigned char &l_deck, unsigned char &l_type, int &l_rID, short int &l_emod, long int &l_enum, int &err, FILE *ship);

bool loaddesign() //probably need to add a char string here later, so i can pass in the addy. for now, use default
{
    cleardesign();
    FILE *shipload = NULL;
    shipload=fopen("testSODS.ship", "rb");
    if (!shipload)
        return false;
    //check file length?
    long fsize = 0;
    {
        fseek(shipload, 0, SEEK_END);
        fsize = ftell(shipload);
        if (fsize < 40)
        {
            fclose(shipload);
            return false; // not a valid ship file:: too small
        }
        fseek(shipload, 0, SEEK_SET);
    }
    unsigned char sl_ID[4];
    for (int i = 0; i < 4; i++)
        sl_ID[i] = fgetc(shipload);
    fseek(shipload, 8, SEEK_CUR);
    mainship.streamlined = fgetc(shipload);
    mainship.armorlevel = fgetc(shipload);
    if (mainship.streamlined>1)
        mainship.streamlined = 1;
    if (mainship.armorlevel >10)
        mainship.armorlevel = 10;

    //need to find the characters and get the position: if theres an error, abort
    //func: long int loadFindID(char[4], FILE*, &err)
    //returns position: if eof found, sets error
    int err = 0;
    long int filepos = 0;
    filepos = loadFindID(sl_ID, shipload, err);
    if (err == L_EOF)
    {
        fclose(shipload);
        return false;
    }
    int g_rooms = 0;
    unsigned char f_g1;
    unsigned char f_g2;
    fseek (shipload, filepos - 5, SEEK_SET);
    f_g1 = fgetc (shipload);
    f_g2 = fgetc (shipload);

    //fprintf(stdout, " Pos1 %i Pos2 %i", f_g1, f_g2);
    g_rooms = (f_g1*0x100)+f_g2;
    //fprintf(stdout, "\n TotalRooms %i", g_rooms);

    if (g_rooms == 0)
        return false; //abort, no rooms error
    fseek(shipload, 4, SEEK_CUR); //go past the room id and start reading in rooms.
    xyvert *r_verts;
    unsigned char *r_info = new unsigned char[5];
    //int err;
    int r_corner;

    for (int i = 0; i < g_rooms; i++)
    {
        r_verts = loadgetroom(r_info, r_corner, err, shipload);
        if (err == L_EOF)
            break;
        //fprintf(stderr, "\n %i %i %i %i ", r_info[3], r_info[4], r_info[2], r_corner);
        if (err == 0)
        {
            deckrooms[r_info[3]][r_info[4]][r_info[2]] = loadsetroom(r_info, r_corner, r_verts);
        }
        if (i < g_rooms -1)
            fseek(shipload, 4, SEEK_CUR);
    }
    if (err == L_EOF)
    {
        fclose(shipload);
        return false;
    }

    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                finishroom2(deckrooms[t][r][d]);
            }
        }
    }

    delete[] r_info;
    if (feof(shipload))
    {
        fclose(shipload);
        return true;
    }

    //if we arent done yet, we need to start placing equipment in the proper spots.
    //next 4 = equipunitsnum.  if eof, or 0, fail
    unsigned char sl_eq[4];
    //long int cpos = ftell(shipload);
    //fprintf(stdout, "\n CPOS %li", cpos);
    for (int i = 0; i < 4; i++)
    {
        sl_eq[i] = fgetc(shipload);
    }
    if (feof(shipload))
    {
        fclose(shipload);
        return true;
    }
    long t_eq = (sl_eq[0]*0x1000000)+(sl_eq[1]*0x10000)+(sl_eq[2]*0x100)+sl_eq[3];
    if (t_eq == 0)
    {
        fclose(shipload);
        return true;
    }


    //fprintf(stdout, "\n TEQ %li", t_eq);
    // we have n equipment to load: make a function to start grabbing, then assign if theres no errors.
    // should probably uss the addequip function simply after ripping the proper data.
    unsigned char e_x;
    unsigned char e_y;
    unsigned char e_deck;
    unsigned char e_type;
    int e_rID;
    long int e_num;
    short int modtype;


    for (long int i = 0; i < t_eq && i < 65536; i++)
    {
        loadgetequip(e_x, e_y, e_deck, e_type, e_rID, modtype, e_num, err, shipload);
        if (err == L_EOF)
            break;
        if (err == 0)
        {
            currentdeck = e_deck;
            equiptype = e_type;
            currentrot = 0;
            AddEquip2(e_x*5, 800-e_y*5, currentdeck, 0, modtype);
        }
    }
    currentdeck = 28;
    equiptype = 0x00;
    currentrot = 0;





    fclose(shipload);
    return true;


}

void loadgetequip(unsigned char &l_x, unsigned char &l_y, unsigned char &l_deck, unsigned char &l_type, int &l_rID, short int &l_emod, long int &l_enum, int &err, FILE *ship)
{
    err = 0;
    l_deck = fgetc(ship);
    l_deck += 28;
    l_type = fgetc(ship);
    fseek(ship, 1, SEEK_CUR); //skip next, then more stuff
    unsigned char eqnum[4];
    for (int i = 0; i < 4; i++)
    {
        eqnum[i]= fgetc(ship);
    }
    //fprintf(stdout, "\n %i %i", l_deck, l_type);
    //fprintf(stdout, "\n %i %i %i %i", eqnum[0], eqnum[1], eqnum[2], eqnum[3]);
    l_enum = (eqnum[0]*0x1000000)+(eqnum[1]*0x10000)+(eqnum[2]*0x100)+eqnum[3];
    fseek(ship, 4, SEEK_CUR);
    l_x = fgetc(ship);
    l_y = fgetc(ship);
    l_x += 80;
    l_y += 80;
    unsigned char r1;
    unsigned char r2;
    r1 = fgetc(ship);
    r2 = fgetc(ship);
    l_rID = (r1*0x100)+r2;
    //these next two are mod stuff, i might have to get these later
    unsigned char m1;
    unsigned char m2;
    m1 = fgetc(ship);
    m2 = fgetc(ship);
    short int modtype;
    modtype = m1*0x100 + m2;
    //fseek(ship, 2, SEEK_CUR);
    if (l_deck > 56 || l_x > 160 || l_y > 160)
        err = BADROOM;
    //type tests
    if (l_type != MAN_U &&
        l_type != POW_U &&
        l_type != SHI_U &&
        l_type != SEN_U &&
        l_type != FTL_U &&
        l_type != WEA_U &&
        l_type != LIF_U &&
        l_type != MED_U &&
        l_type != TRA_U &&
        l_type != BED_U &&
        l_type != BED2_U &&
        !(l_type >= BED_U_UR && l_type <= BED_U_DR) &&
        !(l_type >= BED2_U_UR && l_type <= BED2_U_DR) &&
        l_type != DOOR_H &&
        l_type != DOOR_C &&
        l_type != DOOR_P &&
        l_type != DOORL_H &&
        l_type != DOORL_C &&
        l_type != DOORL_P &&
        l_type != HATCH_H &&
        !(l_type >= HATCH_H_UR &&  l_type <= HATCH_H_DR) &&
        l_type != HATCH_C &&
        !(l_type >= HATCH_C_UR &&  l_type <= HATCH_C_DR) &&
        l_type != HATCH_P &&
        !(l_type >= HATCH_P_UR &&  l_type <= HATCH_P_DR) &&
        l_type != OPENING_I &&
        l_type != OPENING_E &&
        l_type != RAIL &&
        l_type != PIVOT &&
        l_type != EMIT_C &&
        l_type != EMIT_W &&
        l_type != TABLE_G &&
        l_type != V_A_T &&
        l_type != V_A_F1 &&
        l_type != V_A_F2 &&
        l_type != V_G_M &&
        l_type != V_G_S &&
        l_type != V_G_A &&
        //l_type != GUN_T_CAN &&
        l_type != T_SIDE_R &&
        !(l_type >= T_SIDE_UR &&  l_type <= T_SIDE_DR) &&
        //l_type != GUN_S_CAN &&
        l_type != T_TOP &&
        l_type != T_BOT &&
        l_type != CAP_C &&
        l_type != HEL_C &&
        l_type != ENG_C &&
        l_type != SHI_C &&
        l_type != SEN_C &&
        l_type != NAV_C &&
        l_type != WEA_C &&
        l_type != POW_C &&
        l_type != MED_C &&
        l_type != TRA_C)
        err = BADROOM;

/*
enum  {MAN_U = 0x0A, POW_U = 0x0E, SHI_U = 0x12, SEN_U = 0x10, FTL_U = 0x18, WEA_U = 0x17, LIF_U = 0x20, MED_U = 0x0B,
       TRA_U = 0x14, BED_U = 0x41, BED2_U = 0x02, DOOR_H = 0x32, DOOR_C = 0x31, DOOR_P = 0x05, DOORL_H = 0x03, DOORL_C = 0x36,
       DOORL_P = 0x35, HATCH_H = 0x34, HATCH_C = 0x33, HATCH_P = 0x08, OPENING_I = 0x37, OPENING_E = 0x09, RAIL = 0x38,
       PIVOT = 0x46, EMIT_C = 0x62, EMIT_W = 0x63, TABLE_G = 0x00, V_A_T = 0x00, V_A_F1 = 0x00, V_A_F2 = 0x00, V_G_M = 0x00,
       V_G_S = 0x00, V_G_A = 0x00, GUN_T_CAN = 0x00, GUN_S_CAN = 0x00, CAP_C = 0x49, HEL_C = 0x0C, ENG_C = 0x06, SHI_C = 0x11,
       SEN_C = 0x0F, NAV_C = 0x01, WEA_C = 0x07, POW_C = 0x0D, MED_C = 0x04, TRA_C = 0x13, DUMMY = 0xFF };
*/
    if (feof(ship))
        err = L_EOF;



    return;
}

ROOM *loadsetroom(unsigned char info[5], int ncorner, xyvert corners[])
{
    if (ncorner == 0 || corners == NULL)
        return NULL;
    //rooms_g[globalnum-1]
    unsigned char g1 = info[0];
    unsigned char g2 = info[1];

    int gnum = (g1*0x100)+g2;
    rooms_g[gnum-1] = true;
    ROOM *nroom = new ROOM;
    nroom->f_legal = 1;
    nroom->decknum = info[2];
    nroom->globalnum = gnum;
    nroom->roomnum = info[4];
    nroom->roomtype = info[3];
    nroom->vnum = 0;
    nroom->lverts = NULL;
    currentdeck = nroom->decknum;

    for (int i = 0; i < ncorner; i++)
    {
        AddVLINK(corners[i].x*5, 800-corners[i].y*5, nroom);
    }
    return nroom;




}


xyvert *loadgetroom(unsigned char *info, int &ncorner, int &err, FILE *ship)
{// we start this at a good id, so just start loading in.
    err = 0;
    if (!ship)
    {
        err = L_EOF;
        return NULL;
    }
    for (int i = 0; i < 5; i++)
    {
        info[i] = fgetc(ship);
    }
    unsigned char c1;
    unsigned char c2;
    c1 = fgetc(ship);
    c2 = fgetc(ship);
    ncorner = (c1*0x100)+c2;

    xyvert *averts;
    averts = new xyvert[ncorner];
    for (int i = 0; i < ncorner; i++)
    {
        averts[i].x = fgetc(ship);
        averts[i].y = fgetc(ship);
        averts[i].x += 80;
        averts[i].y += 80;
    }
    if (feof(ship))
    {
        err = L_EOF;
        delete[] averts;
        return NULL;
    }
    for (int i = 0; i < ncorner; i++)
    {
        if (averts[i].x > 160 || averts[i].y > 160)
        {
            err = BADROOM;
            delete[]averts;
            return NULL;
        }
    }
    //now we do various tests.  first, test info members: only one we are concerned about is roomtype really.
    //but we need to convert the decknumber into one we can use.
    info[2] += 28;
    if (info[3] > 21 ||
        info[3] == 0 ||
        info[3] == 12 ||
        ncorner == 0 ||
        ((info[0]*0x100)+info[1]) == 0)
        {
            err = BADROOM;
            delete[]averts;
            return NULL;
        }


    return averts;
}

long int loadFindID(unsigned char sh_ID[4], FILE *ship, int &err)
{
    long int idpos;
    if (feof(ship))
    {
        err = L_EOF;
        return 0;
    }
    int i = 0;
    unsigned char f_test = sh_ID[i];
    unsigned char f_num = 0x00;
    bool found = false;
    //bool c_test = false;

    while (!feof(ship))
    {
        if (found)
            break;
        f_num= fgetc(ship);
        if (f_num == f_test) //true for this value
        {
            i++;
            if (i >=4) // found
                found = true;
            else
                f_test = sh_ID[i];
        }
        else //not true, then we need to back to square one and keep testing
        {
            i = 0;
            f_test = sh_ID[i];
            if (f_num == f_test)
            {
                i++;
                f_test = sh_ID[i];
            }
        }
    }
    if (feof(ship))
        err = L_EOF;
    idpos = ftell(ship)-1;
    //if (found)
    //    fprintf(stdout, "Found %li", idpos);
    //else
    //    fprintf(stdout, "Error EOF %li", idpos);
    return idpos;
}
