


#ifndef METHODS_H
#define METHODS_H


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>	// Header File For The OpenGL32 Library
#include <OpenGL/glu.h>	// Header File For The GLu32 Library
#else
#include <gl/gl.h>	// Header File For The OpenGL32 Library
#include <gl/glu.h>	// Header File For The GLu32 Library
#endif
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <fstream>
#include <iostream>
#include <math.h>




void InitGL(int Width, int Height);
void DrawGLScene();
void DrawOptions();
void DrawRow(int row);
void DrawColumn(int column);
void DrawCursor(int curx, int cury);
void MouseUpdate(int curx, int cury);
bool cleardesign();
unsigned char currentdeck;
short int currentmod;
int currentrot;
int buttonsize = 8;
int brush = 0;
float angle = 0;
//struct SHIPSTATS;
//SHIPSTATS mainship;


class Vector
{
public:
    float x_, y_;

    Vector(float f = 0.0f)
        : x_(f), y_(f) {}

    Vector(float x, float y)
        : x_(x), y_(y) {}
};

typedef struct tagXYZ
{
    float X, Y, Z;
}
XYZ;

int DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance );
float Magnitude( XYZ *Point1, XYZ *Point2 );

class LineSegment
{
public:
    Vector begin_;
    Vector end_;

    LineSegment(const Vector& begin, const Vector& end)
        : begin_(begin), end_(end) {}

    enum IntersectResult { PARALLEL, COINCIDENT, NOT_INTERESECTING, INTERESECTING, POINTINTERSECTING };

    IntersectResult Intersect(const LineSegment& other_line, Vector& intersection)
    {
        float denom = ((other_line.end_.y_ - other_line.begin_.y_)*(end_.x_ - begin_.x_)) -
                      ((other_line.end_.x_ - other_line.begin_.x_)*(end_.y_ - begin_.y_));

        float nume_a = ((other_line.end_.x_ - other_line.begin_.x_)*(begin_.y_ - other_line.begin_.y_)) -
                       ((other_line.end_.y_ - other_line.begin_.y_)*(begin_.x_ - other_line.begin_.x_));

        float nume_b = ((end_.x_ - begin_.x_)*(begin_.y_ - other_line.begin_.y_)) -
                       ((end_.y_ - begin_.y_)*(begin_.x_ - other_line.begin_.x_));

        if(denom == 0.0f)
        {
            if(nume_a == 0.0f && nume_b == 0.0f)
            {
                return COINCIDENT;
            }
            return PARALLEL;
        }

        float ua = nume_a / denom;
        float ub = nume_b / denom;

        if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
        {

            // Get the intersection point.
            intersection.x_ = begin_.x_ + ua*(end_.x_ - begin_.x_);
            intersection.y_ = begin_.y_ + ua*(end_.y_ - begin_.y_);
            if (ua == 0.0f || ua == 1.0f || ub == 0.0f || ub ==1.0f)
                return POINTINTERSECTING;

            return INTERESECTING;
        }

        return NOT_INTERESECTING;
    }
};
bool LineITest(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3);

bool savetofile();
bool loaddesign();
bool diagonaltest(int oldx, int oldy, int newx, int newy);
long int globalequip;
int roomtype = 0x00;
int equiptype = 0x00;

enum {EQUIPMENT_B = 1, ROOM_B, EDITOR_B};

enum  {NIL, BARRACK, BRIDGE, BRIG, OFFICER, HOLD, HALL, CREW, ENGINE, FUEL, GALLEY, GRAV, NIL2, VEHICLE, LOUNGE,
       OPEN, SENSOR, SHIELD, MED, STATE, TP, WEAP};

enum  {MAN_U = 0x0A, POW_U = 0x0E, SHI_U = 0x12, SEN_U = 0x10, FTL_U = 0x18, WEA_U = 0x17, LIF_U = 0x20, MED_U = 0x0B,
       TRA_U = 0x14, BED_U = 0x41, BED2_U = 0x02, DOOR_H = 0x32, DOOR_C = 0x31, DOOR_P = 0x05, DOORL_H = 0x03, DOORL_C = 0x36,
       DOORL_P = 0x35, HATCH_H = 0x34, HATCH_C = 0x33, HATCH_P = 0x08, OPENING_I = 0x37, OPENING_E = 0x09, RAIL = 0x38,
       PIVOT = 0x4C, EMIT_C = 0x62, EMIT_W = 0x63, TABLE_G = 0x00, V_A_T = 0x00, V_A_F1 = 0x00, V_A_F2 = 0x00, V_G_M = 0x00,
       V_G_S = 0x00, V_G_A = 0x00, GUN_T_CAN = 0x16, GUN_S_CAN = 0x15, CAP_C = 0x49, HEL_C = 0x0C, ENG_C = 0x06, SHI_C = 0x11,
       SEN_C = 0x0F, NAV_C = 0x01, WEA_C = 0x07, POW_C = 0x0D, MED_C = 0x04, TRA_C = 0x13, DUMMY = 0xFF };

enum {UI_A_UP = 0xB0, UI_A_DOWN, UI_E_DEL, UI_E_MDEL, UI_BARRACK, UI_BRIDGE, UI_BRIG, UI_CREW, UI_ENGINEERING, UI_VEHICLE,
      UI_FUEL, UI_GALLEY, UI_GRAVITY, UI_HALL, UI_HOLD, UI_OFFICER, UI_OPEN, UI_LOUNGE, UI_SENSOR, UI_SHIELD, UI_MEDICAL,
      UI_STATEROOM, UI_TRANSPORTER, UI_WEAPON, UI_SAVE, UI_LOAD, UI_R_MINUS, UI_R_PLUS, UI_L_MINUS, UI_L_PLUS, UI_R_VSEL, UI_R_PICK,
      UI_B_MINUS, UI_B_PLUS, UI_STREAMLINE, UI_PART_PLACE, UI_PART_CLEAR, UI_PART_UP, UI_PART_DOWN, UI_PART_ADD, UI_CLEAR_DESIGN,
      UI_FLIP_LR, UI_FLIP_UD, UI_ROT90, UI_PART_FLIP_LR, UI_PART_FLIP_UD, UI_PART_ROT90};

enum {T_SIDE_R = 0x15, T_SIDE_UR = 0x22, T_SIDE_U, T_SIDE_UL, T_SIDE_L, T_SIDE_DL, T_SIDE_D, T_SIDE_DR};
enum {T_TOP = 0x16, T_BOT = 0x21};

//T_TOP = 0x16, T_BOT = 0x21
//opt 1: enum the mod types
//opt 2: just slap em in there in the funcs.
enum {T_CANNON = 0x0106, T_PULSE = 0x00F8, T_BEAM, T_ROCKET, T_MISSILE};
enum {MED_SUR = 0x00F3, MED_GEN, MED_CYB};

enum {X_NONE, D_ROOM, E_ROOM, P_EQUIP, R_PICK, V_PICK, D_SAVE_T, D_SAVE_F, D_SAVE_P, M_PART_PLACE,
      M_PART_ADD, M_ROOM_MOVE, M_OPTIONS, X_UI_DELC, X_PART_DELC, X_MOVEROOM_START, X_MOVEROOM_END};

enum {EMPTY, INSIDE, EDGE_O, EDGE_I};

enum {E_HW_ONLY, E_IW_ONLY, E_M_ONLY, E_MoW, E_MoUoO, E_H_UoO, E_TURRET, E_OTHER, E_CONSOLE, E_BERTH};

enum {HATCH_P_UR = 0x4D, HATCH_P_U, HATCH_P_UL, HATCH_P_L, HATCH_P_DL, HATCH_P_D, HATCH_P_DR, //0x4D to 0x53
      HATCH_C_UR, HATCH_C_U, HATCH_C_UL, HATCH_C_L, HATCH_C_DL, HATCH_C_D, HATCH_C_DR, //0x54 to 0x5A
      HATCH_H_UR, HATCH_H_U, HATCH_H_UL, HATCH_H_L, HATCH_H_DL, HATCH_H_D, HATCH_H_DR}; //0x5B to 0x61

enum {BED_U_R = 0x41, BED_U_UR, BED_U_U, BED_U_UL, BED_U_L, BED_U_DL, BED_U_D, BED_U_DR};
enum {BED2_U_R = 0x02, BED2_U_UR = 0x19, BED2_U_U, BED2_U_UL, BED2_U_L, BED2_U_DL, BED2_U_D, BED2_U_DR};

enum {DRAW_CDECK, DRAW_CnABOVE, DRAW_CnBELOW, DRAW_CnABOVEnBELOW, DRAW_ALLDECKS};


void gettxcoord(int PART_ID, float &x1, float &x2, float &y1, float &y2);
int uimode = X_NONE;
int drawdeckmode = DRAW_CDECK;
struct mousepos
{
    int xpos;
    int ypos;
    int axpos;
    int aypos;
    bool lkeydown;
    bool rkeydown;
    bool lkeypress;
    bool rkeypress;
} mousemain;

struct VERTEX
{
    GLfloat x, y, z;
    //GLfloat tx, ty; // tex position, tex position, xyz
};

struct VLINK
{
    //GLfloat x, y, z;
    unsigned char cx, cy, cz;

    struct VLINK *next;
};


struct ROOM
{

    //VERTEX *overts;
    VLINK *lverts;
    int roomtype;
    unsigned short int vnum;
    unsigned short int roomnum;
    unsigned short int globalnum;
    unsigned char decknum;
    bool f_legal;
};

struct EQUIPUNIT
{
    //unsigned char decknum;
    unsigned char type;
    unsigned char fill;
    unsigned char loc_roomid;
    long int e_id;
    //long int s_id;
    //unsigned char xpos;
    //unsigned char ypos;
    int rotation;
    short int roomid;
    unsigned char mod1;
    unsigned char mod2;
    float texx1, texx2, texy1, texy2;
    EQUIPUNIT();
    EQUIPUNIT(int ttype, long int eqnum, short int ronum, unsigned char lo_ronum, short int modtype, int rot);
    bool setequip(int TYPE);
};

EQUIPUNIT::EQUIPUNIT()
{
    type = 0x00;
    fill = 0x00;
    e_id = 0;
    roomid = 0;
    mod1 = 0;
    mod2 = 0;
}

EQUIPUNIT::EQUIPUNIT(int ttype, long int eqnum, short int ronum, unsigned char lo_ronum, short int modtype, int rot)
{
    type = ttype;
    fill = 0x00;
    e_id = eqnum;
    roomid = ronum;
    loc_roomid = lo_ronum;
    mod1 = modtype / 0x100;
    mod2 = modtype - (mod1*0x100);
    rotation = rot;
    //rotation = currentrot;

    gettxcoord(type, texx1, texx2, texy1, texy2);

}

bool EQUIPUNIT::setequip(int TYPE)
{
    type = TYPE;
    gettxcoord(type, texx1, texx2, texy1, texy2);
    return true;

}

enum {STAT_DISP_MISC, STAT_DISP_COMBAT, STAT_DISP_ENG, STAT_DISP_SC, STAT_DISP_COST};

struct SHIPSTATS
{
    long unsigned int deckweight[57];
    long unsigned int totalweight;
    long unsigned int fuelweights[57];
    long unsigned int equipweights[57];
    long unsigned int numgravunits[57];
    long unsigned int numshieldunits[57];
    long unsigned int numwep[3][57];
    long unsigned int numpower[57];
    long unsigned int VHP[57];
    long unsigned int SHP[57];
    long unsigned int FHP[57];
    long unsigned int HHP[57];  //should I make finishroom calculate this? probably, so you dont get bajillion instances. do that later
    long unsigned int numsensors[57];
    unsigned short int numwep_ids[3];
    long unsigned int totalgravunits;
    long unsigned int totalshieldunits;
    long unsigned int totalwep[3];
    long unsigned int totalpower;
    long unsigned int totalsensors;
    long unsigned int totalVHP;
    long unsigned int totalSHP;
    long unsigned int totalFHP;
    long unsigned int totalHHP;
    //long unsigned int actualweight;
    int m_s_s;
    unsigned int shieldpower;
    long unsigned int wep_range[3];
    long unsigned int wep_power[3];
    float wep_shots[3];
    long unsigned int cap_power;
    long unsigned int sensorrange;

    int statdisplay;
    unsigned char armorlevel;
    unsigned char techlevel;
    bool streamlined;
    char shipname[256];
    char shipweight[16]; //long int has max of 10
    char alevel[6];
    char tlevel[6];
    char accel[12];
    char slevel[19];
    char wep_rc[3][20];
    char wep_pc[3][20];
    char wep_sc[3][20];
    char powerc[20];
    char sensorc[20];


    SHIPSTATS();
    bool calcdeck(int dnum);
    bool calctotal();
    double grabarea(ROOM *droom);
    long unsigned int grabequip(EQUIPUNIT *dunit);
    bool pstats(); //work on this later to make it better than just deeerrrp
    int calspeed(); // goes thru all equipment for grav units, then multiplies by gweight, then takes
    //% of weight times (475+TL*25)
};

SHIPSTATS mainship;
enum {FLIP_LR, FLIP_UD};
enum {ROT_90, ROT_180, ROT_270};

struct PART // a part has a bunch of rooms and a bunch of equipment. its basically another whole designer.
{
    ROOM *rooms[22][256][57];
    EQUIPUNIT *equiplist[161][161][57];
    //EQUIPUNIT *derplist[161][161][57];
    short int lglobal;
    bool g_rooms[65536];
    int c_deck; // this is the 'center', which is displayed.
    //int wid;
    //int hei;

    PART();
    bool addroom(ROOM *setroom);
    ROOM* roomcopy(ROOM *setroom);
    bool equipcopy(EQUIPUNIT *se, int x, int y, int z, int g_id);
    bool displaypart();
    bool displayroom(ROOM *sr);
    bool displayequip(EQUIPUNIT *se, int xpos, int ypos);
    bool displayatcursor(int mx, int my);
    bool displayroomac(ROOM *sr, int mx, int my);
    bool displayeqac(EQUIPUNIT *se, int xpos, int ypos, int mx, int my);
    bool clearpart();
    bool clearroom(ROOM *dr);
    bool cleardeck(int cdeck);
    bool insetatcursor(int mx, int my, int md);
    bool insettest(ROOM *droom, int ox, int oy, int od);
    bool insettest2(ROOM *droom, int ox, int oy, int od);
    bool insetalltest(int ox, int oy, int od);
    bool setproom(ROOM *droom, int type, int roomnum, int decknum, int ox, int oy, int od);
    bool setpeg(EQUIPUNIT *seq, int x, int y, int d, int ox, int oy, int od);
    bool leveldown();
    bool levelup();
    bool flip(int flip);
    bool rotate(int rotate);
};

PART testpart;
PART *activepart = &testpart;
PART *copypart = new PART();
bool hangingpart = false;
unsigned char hangingpartdeck = 0;
bool rotatedesign();
bool flipdesign(int flip);

struct UI_BUTTON
{
    float texx1, texx2, texy1, texy2;
    int pos_x, pos_y;
    int B_TYPE;
    int SUB_TYPE;
    bool PRESSED;
    UI_BUTTON();
    UI_BUTTON(int type, int subtype, int xpos, int ypos);
    void setup_B(int type, int subtype, int xpos, int ypos);
    bool draw_f();
    bool b_press(int xpos, int ypos);
};

struct UI_WINDOW
{
    bool active;
    int numbuttons;
    int pos_x, pos_y;
    UI_BUTTON *buttons;
    UI_WINDOW();
    void drawall();
    void getpress(int &type, int &subtype, int xpos, int ypos); //goes thru all buttons, find the one that is pressed, then return that value.
    //we now have returning type and subtype: if subtype is room, we change the room id and allow room placement (toggle).  if its
    //an equipment type we change the equip to place value and then start placing equipment.  Editor = do something special.
    //this next function will be a 'do it all' function for now, interacting with global vars
    void getbfunc(int xpos, int ypos);  //this will call getpress, look at types returned, then change appropriate global values outside
};

UI_BUTTON::UI_BUTTON()
{
    pos_x = 0;
    pos_y = 0;
    B_TYPE = 0x00;
    SUB_TYPE = EDITOR_B;
    PRESSED = false;
    texx1 =0, texx2 = 0, texy1 = 0, texy2 = 0;
}

UI_BUTTON::UI_BUTTON(int type, int subtype, int xpos, int ypos)
{
    B_TYPE = type;
    SUB_TYPE = subtype;
    pos_x = xpos;
    pos_y = ypos;
    PRESSED = false;
    texx1 = 0;
    texx2 = 0;
    texy1 = 0;
    texy2 = 0;
    gettxcoord(B_TYPE, texx1, texx2, texy1, texy2);
}

void UI_WINDOW::getpress(int &type, int &subtype, int xpos, int ypos)
{
    //type = 0;
    //subtype = 0;
    for (int i = 0; i < numbuttons; i++)
    {
        buttons[i].PRESSED = false;
        if (buttons[i].b_press(xpos, ypos))
        {
            type = buttons[i].B_TYPE;
            subtype = buttons[i].SUB_TYPE;
            buttons[i].PRESSED = true;
        }

    }
    return;
}

void UI_WINDOW::getbfunc(int xpos, int ypos)
{
    int type =0;
    int subtype = 0;
    //this is going to assume you pressed something, so it may as well feed in coords too
    getpress(type, subtype, xpos, ypos);
    if (subtype == 0) // if it fails or gets an uninitialized button, gtfo
        return;
    if (type == 0)
        return;

/*
enum  {NIL, BARRACK, BRIDGE, BRIG, OFFICER, HOLD, HALL, CREW, ENGINE, FUEL, GALLEY, GRAV, NIL2, VEHICLE, LOUNGE,
       OPEN, SENSOR, SHIELD, MED, STATE, TP, WEAP};

*/
    //int b_list1[2] = {UI_SAVE, UI_LOAD};
    //int b_list2[6] = {UI_A_DOWN, UI_A_UP, UI_R_MINUS, UI_R_PLUS, UI_L_MINUS, UI_L_PLUS};
    int b_listr[22] = {0, UI_BARRACK, UI_BRIDGE, UI_BRIG, UI_OFFICER, UI_HOLD, UI_HALL,
      UI_CREW, UI_ENGINEERING, UI_FUEL, UI_GALLEY, UI_GRAVITY, 0, UI_VEHICLE, UI_LOUNGE, UI_OPEN, UI_SENSOR, UI_SHIELD, UI_MEDICAL,
      UI_STATEROOM, UI_TRANSPORTER, UI_WEAPON};
    //int b_liste[45] = {MAN_U, POW_U, SHI_U, SEN_U, FTL_U, WEA_U, LIF_U, MED_U, TRA_U, BED_U, BED2_U, DOOR_H,
     //               DOOR_C, DOOR_P, DOORL_H, DOORL_C, DOORL_P, HATCH_H, HATCH_C, HATCH_P, OPENING_I, OPENING_E, RAIL,
     //               PIVOT, EMIT_C, EMIT_W, TABLE_G, V_A_T, V_A_F1, V_A_F2, V_G_M, V_G_S, V_G_A, GUN_T_CAN, GUN_S_CAN,
     //               CAP_C, HEL_C, ENG_C, SHI_C, SEN_C, NAV_C, WEA_C, POW_C, MED_C, TRA_C};
    if (subtype == ROOM_B)
    {
        //first, we change the edit type to placing rooms
        uimode = D_ROOM;
        //then, we change the global room type to the appropriate one.  since different, got to do manually.
        for (int i = 0; i < 22; i++)
        {
            if (i == 0 || i == 12)
                ;
            else
            {
                if (type == b_listr[i])  // button type: UI_BARRACK at [1], if yes, then we set to 1, which is BARRACK
                {
                    roomtype = i;
                    break;//we are done
                }
            }
        }
    } // we have set the drawmode and set the type, we are good.
    if (subtype == EQUIPMENT_B) // this should be a bit easier
    {
        // set mode to draw equipment mode
        uimode = P_EQUIP;
        equiptype = type;
        //account for rotation for hatches // or not, may be simpler to just do in the adding function!!!
        /*
        if (type == HATCH_H)
        {
            if (currentrot >0)
            {
                equiptype = 0x5B + (currentrot-1);
            }
        }
        if (type == HATCH_C)
        {
            if (currentrot >0)
            {
                equiptype = 0x54 + (currentrot-1);
            }

        }
        if (type == HATCH_P)
        {

        } */
        //HATCH_H = 0x34, HATCH_C = 0x33, HATCH_P = 0x08

    } // done and ready to place equipment
    if (subtype == EDITOR_B)
    { // most important one is save, do that first
        uimode = X_NONE;
        if (type == UI_SAVE)
        {
            uimode = D_SAVE_T;
            if (!savetofile())
                uimode = D_SAVE_F;


        }
        if (type == UI_A_DOWN)
        {
            if (mainship.armorlevel > 0)
            {
                mainship.armorlevel--;
                mainship.calctotal();
            }
        }
        if (type == UI_A_UP)
        {
           if (mainship.armorlevel < 10)
           {
               mainship.armorlevel++;
               mainship.calctotal();
           }

        }
        if (type == UI_STREAMLINE)
        {
            mainship.streamlined = !mainship.streamlined;
            mainship.calctotal();
        }
        //UI_A_DOWN, UI_A_UP, UI_R_MINUS, UI_R_PLUS, UI_L_MINUS, UI_L_PLUS}
        if (type == UI_R_PICK)
        {
            uimode = R_PICK;
        }
        if (type == UI_L_MINUS)
        {
            if (currentdeck >0)
                currentdeck --;

        }
        if (type == UI_L_PLUS)
        {
            if (currentdeck <56)
                currentdeck ++;

        }
        if (type == UI_LOAD)
        {
            loaddesign();
            for (int d = 0; d < 57; d++)
                mainship.calcdeck(d);
            mainship.calctotal();
        }
        if (type == UI_E_DEL)
        {
            uimode = P_EQUIP;
            equiptype = 0;
        }
        if (type == UI_E_MDEL)
        {
            uimode = P_EQUIP;
            equiptype = 0;
        }
        if (type == UI_B_MINUS)
        {
            if (brush > 0)
                brush--;
        }
        if (type == UI_B_PLUS)
        {
            if (brush < 10)
                brush++;
        }
        if (type == UI_R_VSEL)
        {
            uimode = V_PICK;

        }
        if (type == UI_PART_PLACE)
        {
            uimode = M_PART_PLACE;
        }
        if (type == UI_PART_CLEAR)
        {
            uimode = X_PART_DELC;
            //activepart->clearpart();
        }
        if (type == UI_PART_DOWN)
        {
            activepart->leveldown();
        }
        if (type == UI_PART_UP)
        {
            activepart->levelup();
        }
        if (type == UI_PART_ADD)
        {
            uimode = M_PART_ADD;
        }
        if (type == UI_CLEAR_DESIGN)
        {
            uimode = X_UI_DELC;
            //cleardesign();
        }
        if (type == UI_FLIP_LR)
        {
            flipdesign(FLIP_LR);
        }
        if (type == UI_FLIP_UD)
        {
            flipdesign(FLIP_UD);
        }
        if (type == UI_ROT90)
        {
            rotatedesign();
        }
        if (type == UI_PART_FLIP_LR)
        {
            activepart->flip(FLIP_LR);
        }
        if (type == UI_PART_FLIP_UD)
        {
            activepart->flip(FLIP_UD);
        }
        if (type == UI_PART_ROT90)
        {
            activepart->rotate(ROT_90);
        }
        //UI_FLIP_LR, UI_FLIP_UD, UI_ROT90, UI_PART_FLIP_LR, UI_PART_FLIP_UD, UI_PART_ROT90
//UI_PART_PLACE, UI_PART_CLEAR, UI_PART_UP, UI_PART_DOWN

        //filler for save

    }
}

bool UI_BUTTON::b_press(int xpos, int ypos)
{
    if (xpos >= (pos_x-buttonsize) && xpos <= (pos_x+buttonsize) && ypos >= (pos_y-buttonsize) && ypos <= (pos_y+buttonsize))
    {
        PRESSED = true;
        return true;
    }
    PRESSED = false;
    return false;
}

bool UI_BUTTON::draw_f()
{
    // ui buttons are easy, they always go in the same place.  at least they do now
    // make a 'quad' at 'around' the area, static size for now.  Textures will be pre-bound and loaded so dont need to do that here
    if (PRESSED)
    {

        glDisable(GL_TEXTURE_2D);
        glColor4f(0.4,0.4,0.4, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(pos_x - (buttonsize+2) , pos_y - (buttonsize+2), 0);
        glVertex3f(pos_x + (buttonsize+2) , pos_y - (buttonsize+2), 0);
        glVertex3f(pos_x + (buttonsize+2) , pos_y + (buttonsize+2), 0);
        glVertex3f(pos_x - (buttonsize+2) , pos_y + (buttonsize+2), 0);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0,1.0,1.0);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(pos_x -buttonsize , pos_y -buttonsize, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(pos_x +buttonsize , pos_y -buttonsize, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(pos_x +buttonsize , pos_y +buttonsize, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(pos_x -buttonsize , pos_y +buttonsize, 0);
    glEnd();
    if (PRESSED)
    {

        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0, 1.0, 0.3);
        glBegin(GL_QUADS);
        glVertex3f(pos_x - (buttonsize) , pos_y - (buttonsize), 0);
        glVertex3f(pos_x + (buttonsize) , pos_y - (buttonsize), 0);
        glVertex3f(pos_x + (buttonsize) , pos_y + (buttonsize), 0);
        glVertex3f(pos_x - (buttonsize) , pos_y + (buttonsize), 0);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0,1.0,1.0);
    }


    return true;
}

void UI_BUTTON::setup_B(int type, int subtype, int xpos, int ypos)
{
    //UI_BUTTON(type, xpos, ypos);
    B_TYPE = type;
    SUB_TYPE = subtype;
    pos_x = xpos;
    pos_y = ypos;
    gettxcoord(B_TYPE, texx1, texx2, texy1, texy2);
}

void gettxcoord(int PART_ID, float &x1, float &x2, float &y1, float &y2)
{
    int multix =0;
    int multiy =0;

    {
        if (PART_ID == DUMMY)
        {
            multix = 15;
            multiy = 15;
        }
        if (PART_ID == MAN_U)
        {
            multix = 0;
            multiy = 0;
        }
        if (PART_ID == POW_U)
        {
            multix = 1;
            multiy = 0;
        }
        if (PART_ID == SHI_U)
        {
            multix = 2;
            multiy = 0;
        }
        if (PART_ID == SEN_U)
        {
            multix = 3;
            multiy = 0;
        }
        if (PART_ID == FTL_U)
        {
            multix = 4;
            multiy = 0;
        }
        if (PART_ID == WEA_U)
        {
            multix = 5;
            multiy = 0;
        }
        if (PART_ID == LIF_U)
        {
            multix = 6;
            multiy = 0;
        }
        if (PART_ID == MED_U)
        {
            multix = 7;
            multiy = 0;
        }
        if (PART_ID == TRA_U)
        {
            multix = 8;
            multiy = 0;
        }
        if (PART_ID == BED_U ||
            (PART_ID >= BED_U_UR && PART_ID <= BED_U_DR))
        {
            multix = 9;
            multiy = 0;
        }
        if (PART_ID == BED2_U ||
            (PART_ID >= BED2_U_UR && PART_ID <= BED2_U_DR))
        {
            multix = 10;
            multiy = 0;
        }
        if (PART_ID == DOOR_H)
        {
            multix = 11;
            multiy = 0;
        }
        if (PART_ID == DOOR_C)
        {
            multix = 12;
            multiy = 0;
        }
        if (PART_ID == DOOR_P)
        {
            multix = 13;
            multiy = 0;
        }
        if (PART_ID == DOORL_H)
        {
            multix = 14;
            multiy = 0;
        }
        if (PART_ID == DOORL_C)
        {
            multix = 15;
            multiy = 0;
        }
        if (PART_ID == DOORL_P)
        {
            multix = 0;
            multiy = 1;
        }
        if ((PART_ID == HATCH_H) ||
            ((PART_ID >= HATCH_H_UR) && (PART_ID <= HATCH_H_DR)))
        {
            multix = 1;
            multiy = 1;
        }
        if ((PART_ID == HATCH_C) ||
            ((PART_ID >= HATCH_C_UR) && (PART_ID <= HATCH_C_DR)))
        {
            multix = 2;
            multiy = 1;
        }
        if ((PART_ID == HATCH_P) ||
            ((PART_ID >= HATCH_P_UR) && (PART_ID <= HATCH_P_DR)))
        {
            multix = 3;
            multiy = 1;
        }
        if (PART_ID == OPENING_I)
        {
            multix = 4;
            multiy = 1;
        }
        if (PART_ID == OPENING_E)
        {
            multix = 5;
            multiy = 1;
        }
        if (PART_ID == RAIL)
        {
            multix = 6;
            multiy = 1;
        }
        if (PART_ID == PIVOT)
        {
            multix = 7;
            multiy = 1;
        }
        if (PART_ID == EMIT_C)
        {
            multix = 8;
            multiy = 1;
        }
        if (PART_ID == EMIT_W)
        {
            multix = 9;
            multiy = 1;
        }
        if (PART_ID == TABLE_G)
        {
            multix = 10;
            multiy = 1;
        }
        if (PART_ID == V_A_T)
        {
            multix = 11;
            multiy = 1;
        }
        if (PART_ID == V_A_F1)
        {
            multix = 12;
            multiy = 1;
        }
        if (PART_ID == V_A_F2)
        {
            multix = 13;
            multiy = 1;
        }
        if (PART_ID == V_G_M)
        {
            multix = 14;
            multiy = 1;
        }
        if (PART_ID == V_G_S)
        {
            multix = 15;
            multiy = 1;
        }
        if (PART_ID == V_G_A)
        {
            multix = 10;
            multiy = 4;
        }
        if (PART_ID == UI_A_UP)
        {
            multix = 0;
            multiy = 2;
        }
        if (PART_ID == UI_A_DOWN)
        {
            multix = 1;
            multiy = 2;
        }
        if (PART_ID == GUN_T_CAN ||
            PART_ID == T_BOT)
        {
            multix = 2;
            multiy = 2;
        }
        if (PART_ID == GUN_S_CAN ||
            PART_ID == T_SIDE_UR ||
            PART_ID == T_SIDE_U ||
            PART_ID == T_SIDE_UL ||
            PART_ID == T_SIDE_L ||
            PART_ID == T_SIDE_DL ||
            PART_ID == T_SIDE_D ||
            PART_ID == T_SIDE_DR)
        {
            multix = 3;
            multiy = 2;
        }
        if (PART_ID == UI_E_DEL)
        {
            multix = 4;
            multiy = 2;
        }
        if (PART_ID == UI_E_MDEL)
        {
            multix = 5;
            multiy = 2;
        }
        if (PART_ID == CAP_C)
        {
            multix = 6;
            multiy = 2;
        }
        if (PART_ID == HEL_C)
        {
            multix = 7;
            multiy = 2;
        }
        if (PART_ID == ENG_C)
        {
            multix = 8;
            multiy = 2;
        }
        if (PART_ID == SHI_C)
        {
            multix = 9;
            multiy = 2;
        }
        if (PART_ID == SEN_C)
        {
            multix = 10;
            multiy = 2;
        }
        if (PART_ID == NAV_C)
        {
            multix = 11;
            multiy = 2;
        }
        if (PART_ID == WEA_C)
        {
            multix = 12;
            multiy = 2;
        }
        if (PART_ID == POW_C)
        {
            multix = 13;
            multiy = 2;
        }
        if (PART_ID == MED_C)
        {
            multix = 14;
            multiy = 2;
        }
        if (PART_ID == TRA_C)
        {
            multix = 15;
            multiy = 2;
        }
        if (PART_ID == UI_BARRACK)
        {
            multix = 0;
            multiy = 3;
        }
        if (PART_ID == UI_BRIDGE)
        {
            multix = 1;
            multiy = 3;
        }
        if (PART_ID == UI_BRIG)
        {
            multix = 2;
            multiy = 3;
        }
        if (PART_ID == UI_CREW)
        {
            multix = 3;
            multiy = 3;
        }
        if (PART_ID == UI_ENGINEERING)
        {
            multix = 4;
            multiy = 3;
        }
        if (PART_ID == UI_VEHICLE)
        {
            multix = 5;
            multiy = 3;
        }
        if (PART_ID == UI_FUEL)
        {
            multix = 6;
            multiy = 3;
        }
        if (PART_ID == UI_GALLEY)
        {
            multix = 7;
            multiy = 3;
        }
        if (PART_ID == UI_GRAVITY)
        {
            multix = 8;
            multiy = 3;
        }
        if (PART_ID == UI_HALL)
        {
            multix = 9;
            multiy = 3;
        }
        if (PART_ID == UI_HOLD)
        {
            multix = 10;
            multiy = 3;
        }
        if (PART_ID == UI_OFFICER)
        {
            multix = 11;
            multiy = 3;
        }
        if (PART_ID == UI_OPEN)
        {
            multix = 12;
            multiy = 3;
        }
        if (PART_ID == UI_LOUNGE)
        {
            multix = 13;
            multiy = 3;
        }
        if (PART_ID == UI_SENSOR)
        {
            multix = 14;
            multiy = 3;
        }
        if (PART_ID == UI_SHIELD)
        {
            multix = 15;
            multiy = 3;
        }
        if (PART_ID == UI_MEDICAL)
        {
            multix = 0;
            multiy = 4;
        }
        if (PART_ID == UI_STATEROOM)
        {
            multix = 1;
            multiy = 4;
        }
        if (PART_ID == UI_TRANSPORTER)
        {
            multix = 2;
            multiy = 4;
        }
        if (PART_ID == UI_WEAPON)
        {
            multix = 3;
            multiy = 4;
        }
        if (PART_ID == UI_SAVE)
        {
            multix = 4;
            multiy = 4;
        }
        if (PART_ID == UI_LOAD)
        {
            multix = 5;
            multiy = 4;
        }
        if (PART_ID == UI_R_MINUS)
        {
            multix = 6;
            multiy = 4;
        }
        if (PART_ID == UI_R_PLUS)
        {
            multix = 7;
            multiy = 4;
        }
        if (PART_ID == UI_L_MINUS)
        {
            multix = 8;
            multiy = 4;
        }
        if (PART_ID == UI_L_PLUS)
        {
            multix = 9;
            multiy = 4;
        }
        if (PART_ID == UI_R_VSEL)
        {
            multix = 11;
            multiy = 4;
        }
        if (PART_ID == UI_R_PICK)
        {
            multix = 12;
            multiy = 4;
        }
        if (PART_ID == UI_B_MINUS)
        {
            multix = 13;
            multiy = 4;
        }
        if (PART_ID == UI_B_PLUS)
        {
            multix = 14;
            multiy = 4;
        }
        if (PART_ID == UI_STREAMLINE)
        {
            multix = 15;
            multiy = 4;
        }
        if (PART_ID == UI_PART_PLACE)
        {
            multix = 0;
            multiy = 5;
        }
        if (PART_ID == UI_PART_CLEAR)
        {
            multix = 1;
            multiy = 5;
        }
        if (PART_ID == UI_PART_DOWN)
        {
            multix = 8;
            multiy = 4;
        }
        if (PART_ID == UI_PART_UP)
        {
            multix= 9;
            multiy= 4;
        }
        if (PART_ID == UI_PART_ADD)
        {
            multix = 2;
            multiy = 5;
        }
        if (PART_ID == UI_CLEAR_DESIGN)
        {
            multix = 1;
            multiy = 5;
        }
        if (PART_ID == UI_FLIP_LR ||
            PART_ID == UI_PART_FLIP_LR)
        {
            multix = 3;
            multiy = 5;
        }
        if (PART_ID == UI_FLIP_UD ||
            PART_ID == UI_PART_FLIP_UD)
        {
            multix = 5;
            multiy = 5;
        }
        if (PART_ID == UI_ROT90 ||
            PART_ID == UI_PART_ROT90)
        {
            multix = 4;
            multiy = 5;
        }
        //UI_PART_PLACE, UI_PART_CLEAR, UI_PART_UP, UI_PART_DOWN
        //UI_FLIP_LR, UI_FLIP_UD, UI_ROT90, UI_PART_FLIP_LR, UI_PART_FLIP_UD, UI_PART_ROT90
//3 4
    }
    x1 = (float)(multix*64)/(float)1024;
    x2 = (float)(multix*64 + 64)/(float)1024;
    y1 = (float)(multiy*64)/(float)1024;
    y2 = (float)(multiy*64 + 64)/(float)1024;

}

UI_WINDOW::UI_WINDOW()
{
    numbuttons = 92;
    buttons = new UI_BUTTON[numbuttons]();
    pos_x = 820;
    pos_y = 20;
    int setbutton = 0;
    int b_list1[2] = {UI_SAVE, UI_LOAD};
    int b_list2[8] = {UI_A_DOWN, UI_A_UP, UI_R_MINUS, UI_R_PLUS, UI_L_MINUS, UI_L_PLUS, UI_STREAMLINE, UI_CLEAR_DESIGN};
    int b_listr[20] = {UI_BARRACK, UI_BRIDGE, UI_BRIG, UI_CREW, UI_ENGINEERING, UI_VEHICLE,
      UI_FUEL, UI_GALLEY, UI_GRAVITY, UI_HALL, UI_HOLD, UI_OFFICER, UI_OPEN, UI_LOUNGE, UI_SENSOR, UI_SHIELD, UI_MEDICAL,
      UI_STATEROOM, UI_TRANSPORTER, UI_WEAPON};
    int b_liste[45] = {MAN_U, POW_U, SHI_U, SEN_U, FTL_U, WEA_U, LIF_U, MED_U, TRA_U, BED_U, BED2_U, DOOR_H,
                    DOOR_C, DOOR_P, DOORL_H, DOORL_C, DOORL_P, HATCH_H, HATCH_C, HATCH_P, OPENING_I, OPENING_E, RAIL,
                    PIVOT, EMIT_C, EMIT_W, TABLE_G, V_A_T, V_A_F1, V_A_F2, V_G_M, V_G_S, V_G_A, GUN_T_CAN, GUN_S_CAN,
                    CAP_C, HEL_C, ENG_C, SHI_C, SEN_C, NAV_C, WEA_C, POW_C, MED_C, TRA_C};
    int b_listc[6] = {UI_R_VSEL, UI_R_PICK, UI_E_DEL, UI_E_MDEL, UI_B_MINUS, UI_B_PLUS};
    for (int i = 0; i < 2; i++)
    {
        buttons[setbutton].setup_B(b_list1[i], EDITOR_B, pos_x+(2*(buttonsize+2)*i), pos_y);
        setbutton++;
    }
    //buttons[2].setup_B(UI_A_UP, 920, 50);
    for (int i = 0; i < 8; i++)
    {
        buttons[setbutton].setup_B(b_list2[i], EDITOR_B, pos_x+(2*(buttonsize+2)*i), pos_y+3*buttonsize);
        setbutton++;
    }
    int setx = pos_x;
    int sety = pos_y + (6*buttonsize);
    int incrm = 0;
    for (int i = 0; i < 20; i++)
    {
        buttons[setbutton].setup_B(b_listr[i], ROOM_B, setx, sety);
        setbutton++;
        incrm++;
        setx += (3*buttonsize);
        if (incrm>7)
        {
            incrm = 0;
            sety += 3*buttonsize;
            setx = pos_x;
        }
    }
    setx = pos_x;
    sety = pos_y + (15*buttonsize);
    incrm = 0;
    for (int i = 0; i < 45; i++)
    {
        buttons[setbutton].setup_B(b_liste[i], EQUIPMENT_B, setx, sety);
        setbutton++;
        incrm++;
        setx += (3*buttonsize);
        if (incrm>7)
        {
            incrm = 0;
            sety += 3*buttonsize;
            setx = pos_x;
        }
    }
    setx = pos_x;
    sety = pos_y + (35*buttonsize);
    for (int i = 0; i < 6; i++)
    {
        buttons[setbutton].setup_B(b_listc[i], EDITOR_B, setx, sety);
        setbutton++;
        setx += (3* buttonsize);
    }

    setx = 982;
    sety = 616;
    buttons[setbutton].setup_B(UI_PART_CLEAR, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 638;
    buttons[setbutton].setup_B(UI_PART_PLACE, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 660;
    buttons[setbutton].setup_B(UI_PART_UP, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 682;
    buttons[setbutton].setup_B(UI_PART_DOWN, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 702;
    buttons[setbutton].setup_B(UI_PART_ADD, EDITOR_B, setx, sety);
    setbutton++;
    setx = 882;
    sety = 20;
    buttons[setbutton].setup_B(UI_FLIP_LR, EDITOR_B, setx, sety);
    setbutton++;
    setx = 902;
    sety = 20;
    buttons[setbutton].setup_B(UI_FLIP_UD, EDITOR_B, setx, sety);
    setbutton++;
    setx = 922;
    sety = 20;
    buttons[setbutton].setup_B(UI_ROT90, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 722;
    buttons[setbutton].setup_B(UI_PART_FLIP_LR, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 742;
    buttons[setbutton].setup_B(UI_PART_FLIP_UD, EDITOR_B, setx, sety);
    setbutton++;
    setx = 982;
    sety = 762;
    buttons[setbutton].setup_B(UI_PART_ROT90, EDITOR_B, setx, sety);

    //UI_PART_PLACE, UI_PART_CLEAR, UI_PART_UP, UI_PART_DOWN
    //UI_FLIP_LR, UI_FLIP_UD, UI_ROT90, UI_PART_FLIP_LR, UI_PART_FLIP_UD, UI_PART_ROT90


}

void UI_WINDOW::drawall()
{
    for (int i = 0; i<numbuttons; i++)
    {
        if(buttons[i].B_TYPE != 0x00)
            buttons[i].draw_f();
    }

}



bool saveroom(FILE *sfile, ROOM *sroom, int globalnum);
bool saveequip(FILE *sfile, EQUIPUNIT *eunit, unsigned char xpos, unsigned char ypos, unsigned char decknum);

bool testlegal(VLINK *one, VLINK *two);
void M_Line(int curx, int cury, ROOM *setroom);
void AddVLINK(int curx, int cury, ROOM *setroom);
void c_legal(int &curx, int &cury, ROOM *t_room);
void DrawRoom(ROOM *droom);
void DrawRoom2(ROOM *droom, float alpha);
void DrawDeck(unsigned char deck);
void DrawDeck2(unsigned char deck, float alpha);
void rsetcolor(int rtype);
void rsetcolor2(int rtype, float alpha);
bool roomselected;
void AddEquip(int mousex, int mousey);
//int currentroom;
//unsigned char currentequip;

unsigned char sr_cx;
unsigned char sr_cy;
ROOM *a_room = NULL;
void drawequip(int xpos, int ypos, int deck);
void drawequip2(int xpos, int ypos, int deck, float alpha);
void DrawPolies(ROOM *droom);
void CvtClockW(ROOM *droom);
double CXProduct(int xm1, int x, int xp1, int ym1, int y, int yp1);
bool CWTest(ROOM *droom);
bool pointInPolygon2(ROOM *droom, float x, float y);
bool DVLINK_L(ROOM *setroom);
void finishroom(ROOM *droom);
ROOM *makeroom(ROOM *droom);
bool testIsec(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
bool roomIsec(ROOM *droom, float x1, float y1, float x2, float y2);
bool testslope(VLINK* prev, VLINK* curr, VLINK* next);
bool roomIsec2(ROOM *droom, float x1, float y1, float x2, float y2);
bool testIsec2(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
bool LineITest2(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3);
void fillroom(int xpos, int ypos);
bool testPsec(VLINK *line1, VLINK *line2, int xpos, int ypos);
bool roomEtest(ROOM *droom, int xpos, int ypos);
void addconsole(int mousex, int mousey);
void zoomin();
void zoomout();
void setssf();
int ssf = 5;

    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
    unsigned shipid = 0xF9A71100;
    #else
    unsigned shipid = 0x0011A7F9;
    #endif

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int SCROLLX = 0;
int SCROLLY = 0;
float SCALEMOD = 1;

int cursnap(int cur);
unsigned char cvtHEX(int datanum);
unsigned char cvtHEX2(int datanum);

const int numrooms = 65536;
int activerooms = 0;

GLuint texture[3];
GLvoid LoadGLTextures();

int mode = 0; // replace with mousemode later

//enum {DRAW, D_DRAW, EQUIP, D_EQUIP};
//int drawmode = DRAW;
//ROOM rooms[numrooms];
EQUIPUNIT *equipgrid[161][161][57];
bool rooms_g[numrooms];
SDL_Surface *LoadIMGA(char *filename);
//UI_BUTTON *tbutton = NULL;
UI_WINDOW *ui_main = NULL;
//int buttonpressed = -1;
ROOM *deckrooms[22][256][57]; // 22 room types (2 null), 256 possibilities in each deck (57 decks). They point to rooms.
bool isconsole(int type);
bool roomEtest2(ROOM *droom, int xpos, int ypos);
bool pointInPolygon3(ROOM *droom, int testx, int testy);
bool dopolyfill = 1;
bool AddEquip2(int mousex, int mousey, int deckn, int bsize, short int cmod);
void DrawParseDecks();

void KillFont();
void glPrint(GLint x, GLint y, char *string, int set, float scale);
void BuildFont();
bool drawmousepos(int px, int py);
bool moveroomstart(ROOM *droom, int cdeck);

GLuint	base;

void BuildFont()								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	base=glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture[0]);			// Select Our Font Texture
	for (int loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=(float)(loop%16)/16.0f;						// X Position Of Current Character
		cy=(float)(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,cy);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,cy);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,cy+0.0625f);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,cy+0.0625f);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(10,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
		//its flipped, so just flip this back.
	}													// Loop Until All 256 Are Built
}

void KillFont()									// Delete The Font From Memory
{
	glDeleteLists(base,256);							// Delete All 256 Display Lists
}

void glPrint(GLint x, GLint y, char *string, int set, float scale)	// Where The Printing Happens
{
	if (set>1)
	{
		set=1;
	}
	if (scale <= 0)
        scale = 1;
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);			// Select Our Font Texture
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	//glTranslated(-x, -y, 0);
	glScalef(scale, scale, 0);
	//glTranslated(x, y, 0);
	//glOrtho(0,1000,0,800,-1,1);							// Set Up An Ortho Screen
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	//glPushMatrix();										// Store The Modelview Matrix
	//glLoadIdentity();									// Reset The Modelview Matrix
	glTranslated(x/scale,y/scale,0);								// Position The Text (0,0 - Bottom Left)
	glListBase(base-32+(128*set));						// Choose The Font Set (0 or 1)
	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glPopMatrix();										// Restore The Old Projection Matrix
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);							// Enables Depth Testing
}

void glPrintST(GLint x, GLint y, char *string, int set, float scale, float alpha)	// Where The Printing Happens
{
	if (set>1)
	{
		set=1;
	}
	if (scale <= 0)
        scale = 1;
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1, alpha);
	glBindTexture(GL_TEXTURE_2D, texture[1]);			// Select Our Font Texture
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	//glLoadIdentity();									// Reset The Projection Matrix
	//glTranslated(-x, -y, 0);
	glScalef(scale, scale, 0);
	//glTranslated(x, y, 0);
	//glOrtho(0,1000,0,800,-1,1);							// Set Up An Ortho Screen
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	//glPushMatrix();										// Store The Modelview Matrix
	//glLoadIdentity();									// Reset The Modelview Matrix
	glTranslated(x/scale,y/scale,0);								// Position The Text (0,0 - Bottom Left)
	glListBase(base-32+(128*set));						// Choose The Font Set (0 or 1)
	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glPopMatrix();										// Restore The Old Projection Matrix
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);							// Enables Depth Testing
}

struct MOUSEDRAG
{
    bool Drag_On;
    int startx;
    int starty;
    int newx;
    int newy;
    int diffx;
    int diffy;

};

MOUSEDRAG mousedrag;
//mousedrag.Drag_On = false;

void dragscroll(MOUSEDRAG drag)
{
    if (!drag.Drag_On)
        return;
    drag.startx = 400;//middle of graph window
    drag.starty = 400;
    drag.newx = mousemain.xpos;
    drag.newy = mousemain.ypos;
    drag.diffx = drag.newx - drag.startx;
    drag.diffy = drag.newy - drag.starty;
    SDL_WarpMouse(drag.startx, drag.starty);
    SCROLLX -= drag.diffx;
    SCROLLY -= drag.diffy;

}


void zoomin()
{
    if (SCALEMOD == 5)
    {
        SCALEMOD = 10;
        SCROLLX -= 40;
        SCROLLY -= 40;
    }
    if (SCALEMOD == 2)
    {
        SCALEMOD = 5;
        SCROLLX -= 120;
        SCROLLY -= 120;
    }
    if (SCALEMOD == 1)
    {
        SCALEMOD = 2;
        SCROLLX -= 200;
        SCROLLY -= 200;
    }

}
void zoomout()
{
    if (SCALEMOD == 1)
    {
        SCROLLX = 0;
        SCROLLY = 0;
    }
    if (SCALEMOD == 2)
    {
        SCALEMOD = 1;
        SCROLLX +=200;
        SCROLLY +=200;
    }
    if (SCALEMOD == 5)
    {
        SCALEMOD = 2;
        SCROLLX += 120;
        SCROLLY += 120;
    }
    if (SCALEMOD == 10)
    {
        SCALEMOD = 5;
        SCROLLX += 40;
        SCROLLY += 40;
    }
}
void DrawUI();

void setssf() //use this for now
{
    if (SCALEMOD == 1)
        ssf = 100;
    if (SCALEMOD == 2)
        ssf = 50;
    if (SCALEMOD == 5)
        ssf = 25;
    if (SCALEMOD == 10)
        ssf = 10;

}


void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// This Will Clear The Background Color To Black
    glClearStencil(0);
    glClearDepth(1000.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    //glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearStencil(0x0);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
    //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);

    //glAlphaFunc(GL_GREATER,0.1f);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_CULL_FACE);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -200, 100 );



    //gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    LoadGLTextures();
    BuildFont();
    atexit(KillFont);
}

void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.4,0.4,0.4, 1.0);
    glLoadIdentity();				// Reset The View
    //options screen draw?
    if (uimode == M_OPTIONS)
    {
        DrawOptions();

        return;
    }
    glScalef(SCALEMOD, SCALEMOD, SCALEMOD);
    glTranslatef(SCROLLX,SCROLLY,0);
    //glRotatef(angle, 1, 0, 0);

    glLineWidth(0.5);
    for (int i = 0; i < 81; i++)
    {
        if ((i == 0) || i % 10 == 0)
        glLineWidth(1.5);
        else
        glLineWidth(0.5);
        DrawColumn(i);
        DrawRow(i);
    }
        if (uimode == D_SAVE_T || uimode == D_SAVE_F || uimode == D_SAVE_P)
    {
        if (uimode == D_SAVE_T)
            glColor3f(0.2, 1.0, 0.0);
        else
        if (uimode == D_SAVE_P)
            glColor3f(0.5, 0.5, 0.0);
        else
            glColor3f(1.0, 0.0, 0.2);
        glBegin(GL_QUADS);
        {
            glVertex3f(805, 05, 0);
            glVertex3f(825, 05, 0);
            glVertex3f(825, 25, 0);
            glVertex3f(805, 25, 0);
        }
        glEnd();

    }


    DrawParseDecks();


    M_Line(mousemain.axpos, mousemain.aypos, a_room);

    DrawRoom(a_room);
    if (uimode == M_PART_PLACE)
        activepart->displayatcursor(mousemain.axpos, mousemain.aypos);
    if (uimode == X_MOVEROOM_START)
        copypart->displayatcursor(mousemain.axpos, mousemain.aypos);


    glLoadIdentity();
    testpart.displaypart();
    DrawUI();
    //drawmousepos(10, 10);
    if (uimode == X_UI_DELC)
    {
        glColor4f(.2, .2, .2, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(840, 20, 0);
        glVertex3f(940, 20, 0);
        glVertex3f(940, 60, 0);
        glVertex3f(840, 60, 0);
        glColor3f(0, 0, 0);
        glVertex3f(842, 22, 0);
        glVertex3f(938, 22, 0);
        glVertex3f(938, 58, 0);
        glVertex3f(842, 58, 0);

        glEnd();
        glPushMatrix();
        glPrint(850, 20, (char*)"Delete?", 1, 1);
        glPrint(840, 40, (char*)"Deck", 1, 1);
        glPrint(900, 40, (char*)"All", 1, 1);
        glPopMatrix();
    }
    if (uimode == X_PART_DELC)
    {
        glColor4f(.2, .2, .2, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(840, 600, 0);
        glVertex3f(940, 600, 0);
        glVertex3f(940, 640, 0);
        glVertex3f(840, 640, 0);
        glColor3f(0, 0, 0);
        glVertex3f(842, 602, 0);
        glVertex3f(938, 602, 0);
        glVertex3f(938, 638, 0);
        glVertex3f(842, 638, 0);

        glEnd();
        glPushMatrix();
        glPrint(850, 600, (char*)"Delete?", 1, 1);
        glPrint(840, 620, (char*)"Deck", 1, 1);
        glPrint(900, 620, (char*)"All", 1, 1);
        glPopMatrix();
    }

    DrawCursor(mousemain.xpos, mousemain.ypos);

    //glPushMatrix();
    char sts[15] = "CurrentDeck";
    char stb[13] = "Brushsize";
    stb[9] = ' ';
    stb[10] = (brush+1)/10+48;
    stb[11] = (brush+1)%10+48;
    stb[12] = '\0';
    sts[11] = ' ';
    sts[12] = (currentdeck+1)/10+48;
    sts[13] = (currentdeck+1)%10+48;
    sts[14] = '\0';
    char str[12] = "Hatch:";
    str[5] = ' ';
    str[6] = ' ';
    str[7] = ' ';
    if (currentrot == 0)
        str[7] = 'R';
    if (currentrot == 1)
    {
        str[6] = 'U';
        str[7] = 'R';
    }
    if (currentrot == 2)
    {
        str[6] = 'U';
    }
    if (currentrot == 3)
    {
        str[6] = 'U';
        str[7] = 'L';
    }
    if (currentrot == 4)
    {
        str[7] = 'L';
    }
    if (currentrot == 5)
    {
        str[6] = 'D';
        str[7] = 'L';
    }
    if (currentrot == 6)
    {
        str[6] = 'D';
    }
    if (currentrot == 7)
    {
        str[6] = 'D';
        str[7] = 'R';
    }
    str[11] = '\0';
    //str[1] = currentrot+48;
    //str[2] = 'V';
    //str[3] = '\0';
    char stw[20] = "Turret:";
    stw[7]= ' ';
    stw[8]= ' ';
    if (currentmod == T_CANNON)
    {
        stw[9] = 'C';
        stw[10] = 'a';
        stw[11] = 'n';
        stw[12] = 'n';
        stw[13] = 'o';
        stw[14] = 'n';
    }
    if (currentmod == T_PULSE)
    {
        stw[9] = 'P';
        stw[10] = 'u';
        stw[11] = 'l';
        stw[12] = 's';
        stw[13] = 'e';
        //stw[14] = 'n';
    }
    if (currentmod == T_BEAM)
    {
        stw[9] = 'B';
        stw[10] = 'e';
        stw[11] = 'a';
        stw[12] = 'm';
        //stw[13] = 'e';
        //stw[14] = 'n';
    }
    if (currentmod == T_ROCKET)
    {
        stw[9] = 'R';
        stw[10] = 'o';
        stw[11] = 'c';
        stw[12] = 'k';
        stw[13] = 'e';
        stw[14] = 't';
    }
    if (currentmod == T_MISSILE)
    {
        stw[9] = 'M';
        stw[10] = 'i';
        stw[11] = 's';
        stw[12] = 's';
        stw[13] = 'i';
        stw[14] = 'l';
        stw[15] = 'e';
    }
    stw[19]= '\0';
/*
    char msx[4];
    char msy[4];
    msx[0] = mousemain.axpos/100 +48;
    msx[1] = mousemain.axpos%100 / 10 +48;
    msx[2] = mousemain.axpos%10 +48;
    msx[3] = '\0';
    msy[0] = mousemain.aypos/100 +48;
    msy[1] = mousemain.aypos%100 / 10 +48;
    msy[2] = mousemain.aypos%10 +48;
    msy[3] = '\0';
*/
    glColor3f(0.3, 0.5, 0.1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex3f(802, 318, 0);
    glVertex3f(999, 318, 0);
    glVertex3f(999, 386, 0);
    glVertex3f(802, 386, 0);
    glEnd();

    glColor3f(0.1, 0.2, 0.1);
    //glLineWidth(2);
    glBegin(GL_QUADS);
    glVertex3f(802, 318, 0);
    glVertex3f(999, 318, 0);
    glVertex3f(999, 386, 0);
    glVertex3f(802, 386, 0);
    glEnd();

    glPrint(800, 320, stb, 0, 1);
    glPrint(800, 336, sts, 0, 1);
    glPrint(800, 352, str, 0, 1);
    glPrint(800, 368, stw, 0, 1);

  //  glPrint(800, 470, msx, 0);
   // glPrint(800, 490, msy, 0);
    mainship.pstats();




    //glPrint(800, 380, sts, 0);
    //glPrint(860, 390, bsize, 0);





    glDisable(GL_TEXTURE_2D);






    glFlush();
    SDL_GL_SwapBuffers();


}

bool drawmousepos(int px, int py)
{
    char msx[4];
    char msy[4];
    msx[0] = mousemain.xpos/100 +48;
    msx[1] = mousemain.xpos%100 / 10 +48;
    msx[2] = mousemain.xpos%10 +48;
    msx[3] = '\0';
    msy[0] = mousemain.ypos/100 +48;
    msy[1] = mousemain.ypos%100 / 10 +48;
    msy[2] = mousemain.ypos%10 +48;
    msy[3] = '\0';
    glPrint(px, py, msx, 0, 1);
    glPrint(px, py+20, msy, 0, 1);
    return true;
}

struct V_POINT {
double x,y;
};

ROOM *makeroom (ROOM *droom)
{
    droom = new ROOM;
    droom->decknum = currentdeck;
    droom->lverts = NULL;
    droom->roomtype = roomtype;
    droom->vnum = 0;
    droom->f_legal = 1;
    droom->roomnum = 0;
    for (int i = 0; i < 256; i ++)
    {
        if (!deckrooms[droom->roomtype][i][droom->decknum])
        {
            droom->roomnum = i;
            deckrooms[droom->roomtype][i][droom->decknum] = droom;
            break;
        }
    }
    for (int g = 0; g < numrooms; g++)
    {
        if (!rooms_g[g]) // assign first empty globalnum;
        {
            droom->globalnum = g+1;
            rooms_g[g] = true;
            break;
        }
    }
    return droom;

}

bool CWTest(ROOM *droom) //return true if CW
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    VLINK *temp = droom->lverts;

    int i,j;
    double area = 0;
    V_POINT points[droom->vnum];  //# of verts to points
    for (int n = 0; n < droom->vnum; n++) // assign to the point structure
    {
        points[n].x = temp->cx*5;
        points[n].y = (800 - temp->cy*5);
        temp = temp->next;
    }
    for (i = 0; i <droom->vnum; i++)
    {
        j = (i + 1) % droom->vnum;
        area += points[i].x * points[j].y;
        area -= points[i].y * points[j].x;
    }

    if (area >0) // its clockwise, remember, y is flipped
        return true;

    return false;
}

void CvtClockW(ROOM *droom) //call this after a room is finished drawing, and only if its CCW
{
    VLINK *temp = droom->lverts;
    //VLINK *rsort = droom->lverts;

    VLINK *vlist[droom->vnum];
    for (int i = 0; i < droom->vnum; i++)
    {
        vlist[i] = temp;
        temp = temp->next;
    }
    droom->lverts = vlist[droom->vnum-1];
    for (int j = droom->vnum-1; j > 0; j--)
    {
        vlist[j]->next = vlist[j-1];
    }
    vlist[0]->next = NULL;


}

void DrawRow(int row)
{
    glBegin(GL_LINES);
    glVertex3f(000, row*10, 0.0f);
    glVertex3f(800, row*10, 0.0f);
    glEnd();
    // line from left to right
}

void DrawColumn(int column)
{
    glBegin(GL_LINES);
    glVertex3f(10*column, 000, 0.0f);
    glVertex3f(10*column, 800, 0.0f);
    glEnd();
    // line from top to bottom
}

void DrawCursor(int curx, int cury)
{
    glColor3f(0.5, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(curx-2, cury-2, 0.1f);
    glVertex3f(curx-2, cury+2, 0.1f);
    glVertex3f(curx+2, cury+2, 0.1f);
    glVertex3f(curx+2, cury-2, 0.1f);
    glEnd();


}

void MouseUpdate(int curx, int cury)
{
    mousemain.xpos = curx;
    mousemain.ypos = cury;
    //now make the actual positions
    //amt you are over is figured out upon zoomin.  do that for now till you figure it out.
    mousemain.axpos = (curx/SCALEMOD)-SCROLLX;
    mousemain.aypos = (cury/SCALEMOD)-SCROLLY;
}

struct DPVERT
{
    float vx;
    float vy;
    DPVERT *next;
};

double CXProduct(int xm1, int x, int xp1, int ym1, int y, int yp1)
{
    double result = 0;
    result = (((x-xm1)*(yp1-y))-((y-ym1)*(xp1-x)));
    return result;
}

void DrawPolies2(ROOM *droom, float alpha)
{
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    VLINK *temp = droom->lverts;
    if (!temp)
        return;
    if (!temp->next)
        return;
    if (!temp->next->next)
        return; //ensure we have at least 3 verts, otherwise we just dont care
    if (dopolyfill == 0)
        return;


     VLINK *first;
    VLINK *last;
    first = temp;
    last = temp;


    glEnable(GL_STENCIL_TEST);          // enable stencil test

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // disable writing to color buffer
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        glVertex3f(temp->cx*5, (800- temp->cy*5), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    temp = droom->lverts;


    // PASS 2: draw color buffer
    // Draw again the exact same polygon to color buffer where the stencil
    // value is only odd number(1). The even(0) area will be descarded.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    // enable writing to color buffer
    rsetcolor2(droom->roomtype, alpha);
    if (!droom->f_legal)
        rsetcolor2(0, .21);

    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    //glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        //glTexCoord2f(temp->cx, 160-temp->cy);
        glVertex3f(temp->cx*5, (800- temp->cy*5), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    //glDisable(GL_TEXTURE_2D);

    glDisable(GL_STENCIL_TEST);

}

void DrawPolies(ROOM *droom)
{
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    VLINK *temp = droom->lverts;
    if (!temp)
        return;
    if (!temp->next)
        return;
    if (!temp->next->next)
        return; //ensure we have at least 3 verts, otherwise we just dont care
    if (dopolyfill == 0)
        return;


     VLINK *first;
    VLINK *last;
    first = temp;
    last = temp;


    glEnable(GL_STENCIL_TEST);          // enable stencil test

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // disable writing to color buffer
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        glVertex3f(temp->cx*5, (800- temp->cy*5), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    temp = droom->lverts;


    // PASS 2: draw color buffer
    // Draw again the exact same polygon to color buffer where the stencil
    // value is only odd number(1). The even(0) area will be descarded.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    // enable writing to color buffer
    rsetcolor(droom->roomtype);
    if (!droom->f_legal)
        rsetcolor(0);

    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    //glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        //glTexCoord2f(temp->cx, 160-temp->cy);
        glVertex3f(temp->cx*5, (800- temp->cy*5), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    //glDisable(GL_TEXTURE_2D);

    glDisable(GL_STENCIL_TEST);

}

void DrawDeck2(unsigned char deck, float alpha)
{
    if (deck <0 || deck > 56)
        return;
    for (int i = 0; i < 22; i++)
    {
        if (i != 0 && i != 12)
        {
            for (int j = 0; j < 256; j++)
                {
                    DrawRoom2(deckrooms[i][j][deck], alpha);
                    DrawPolies2(deckrooms[i][j][deck], alpha);
                }
        }
    }

    glPointSize(3.0);
    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            drawequip2(i, j, deck, alpha);
        }
    }


}

void DrawDeck(unsigned char deck)
{
    for (int i = 0; i < 22; i++)
    {
        if (i != 0 && i != 12)
        {
            for (int j = 0; j < 256; j++)
                {
                    DrawRoom(deckrooms[i][j][deck]);
                    DrawPolies(deckrooms[i][j][deck]);
                }
        }
    }

    glPointSize(3.0);
    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            drawequip(i, j, deck);
        }
    }


}

void DrawUI()
{
    //first draw a black wall behind it
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(800, 0, 0);
        glVertex3f(1000, 0, 0);
        glVertex3f(1000, 400, 0);
        glVertex3f(800, 400, 0);
    }
    glEnd();
    glLineWidth(2);
    glColor4f(0.1, 0.2, 0.4, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(802, 388, 0.0);
    glVertex3f(999, 388, 0.0);
    glVertex3f(999, 520, 0.0);
    glVertex3f(802, 520, 0.0);
    glEnd();


    glColor4f(0.1, 0.1, 0.2, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(802, 388, 0.0);
    glVertex3f(999, 388, 0.0);
    glVertex3f(999, 520, 0.0);
    glVertex3f(802, 520, 0.0);
    glEnd();


//shipstats displaymode highlight
//first, draw all of them. then find the displaymode and drawcolor the proper one.
    {

    glColor4f(.1, .2, .3, 0.4);
    //glColor4f(0.1, 0.2, 0.3, 1.0);
    glBegin(GL_QUADS);
    if (mainship.statdisplay == STAT_DISP_MISC)
        glColor4f(0.1, 0.2, 0.3, 1.0);
    glVertex3f(802, 388, 0.0);
    glVertex3f(832, 388, 0.0);
    glVertex3f(832, 412, 0.0);
    glVertex3f(802, 412, 0.0);
    glColor4f(.1, .2, .3, 0.4);

    if (mainship.statdisplay == STAT_DISP_COMBAT)
        glColor4f(0.1, 0.2, 0.3, 1.0);
    glVertex3f(834, 388, 0.0);
    glVertex3f(864, 388, 0.0);
    glVertex3f(864, 412, 0.0);
    glVertex3f(834, 412, 0.0);
    glColor4f(.1, .2, .3, 0.4);

    if (mainship.statdisplay == STAT_DISP_SC)
        glColor4f(0.1, 0.2, 0.3, 1.0);
    glVertex3f(864, 388, 0.0);
    glVertex3f(894, 388, 0.0);
    glVertex3f(894, 412, 0.0);
    glVertex3f(864, 412, 0.0);
    glColor4f(.1, .2, .3, 0.4);

    if (mainship.statdisplay == STAT_DISP_ENG)
        glColor4f(0.1, 0.2, 0.3, 1.0);
    glVertex3f(894, 388, 0.0);
    glVertex3f(924, 388, 0.0);
    glVertex3f(924, 412, 0.0);
    glVertex3f(894, 412, 0.0);

    glEnd();
    //{STAT_DISP_MISC, STAT_DISP_COMBAT, STAT_DISP_SC, STAT_DISP_ENG, STAT_DISP_COST};


    glColor4f(0.3, 0.3, 0.4, 1.0);
    glBegin(GL_LINES);
    glVertex3f(802, 412, 0.0);
    glVertex3f(999, 412, 0.0);

    glVertex3f(832, 412, 0.0);
    glVertex3f(832, 388, 0.0);

    //glVertex3f(832, 412, 0.0);
    //glVertex3f(832, 388, 0.0);

    glVertex3f(862, 412, 0.0);
    glVertex3f(862, 388, 0.0);

    glVertex3f(892, 412, 0.0);
    glVertex3f(892, 388, 0.0);

    glVertex3f(922, 412, 0.0);
    glVertex3f(922, 388, 0.0);

    glEnd();
    }




    glPushMatrix();
    char ph[] = "Part: Placeholder";
    glPrint(802, 522, ph, 0, 1);
    glPopMatrix();


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    //tbutton->draw_f();
    //ui_main->buttons[0].draw_f();
    ui_main->drawall();
    glDisable(GL_TEXTURE_2D);

}

void DrawRoom2(ROOM *droom, float alpha)
{
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    VLINK *temp;
    VLINK *next;
    VLINK *first;
    VLINK *last;
    temp = droom->lverts;
    VLINK *links[droom->vnum];
    for (int i = 0; i < droom->vnum; i++)
    {
        links[i] = temp;
        temp = temp->next;
    }
    temp = droom->lverts;
    first = temp;
    next = temp->next;
    last = temp;

    if (!next)
        return;
    //last = temp;
    rsetcolor2(droom->roomtype, alpha);

    glBegin(GL_LINES);
    while (temp)
    {
        if (!next)
            break;

        if (!testlegal (temp, next))
            rsetcolor2(0, .21);


        glVertex3f(temp->cx * 5, 800 - (temp->cy * 5), temp->cz);
        glVertex3f(next->cx * 5, 800 - (next->cy *5), next->cz);
        last = next;
        temp = next;
        next = temp->next;
        rsetcolor2(droom->roomtype, alpha);

    }
    glEnd();
    glBegin(GL_LINES);

    if (!testlegal (first, last))
        rsetcolor2(0, .21);

    glVertex3f(last->cx * 5, 800 - (last->cy *5), last->cz);
    glVertex3f(first->cx * 5, 800 - (first->cy *5), first->cz);
    glEnd();
    //char RN[4] = {' ',' ',' ','\0'};
    //int rna = droom->roomnum+1;
    //for (int j = 3; j >=0; j--)
    //{
    //    RN[j] = rna %10 +48;
    //    rna /= 10;
    //}
    //for (int c = 0; c < 3; c++)
    //{
    //    if (RN[c] == '0')
    //        RN[c] -= 16;
    //    else
    //    if (RN[c] == 32)
    //    ;
    //    else
    //        break;
    //}
    char RN[4];

    itoa(droom->roomnum+1, RN, 10);
    glPrintST(first->cx*5, 800-first->cy*5, RN,0, 1/(SCALEMOD+1), alpha);




}

void DrawRoom(ROOM *droom)
{
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    VLINK *temp;
    VLINK *next;
    VLINK *first;
    VLINK *last;
    temp = droom->lverts;
    VLINK *links[droom->vnum];
    for (int i = 0; i < droom->vnum; i++)
    {
        links[i] = temp;
        temp = temp->next;
    }
    temp = droom->lverts;
    first = temp;
    next = temp->next;
    last = temp;

    if (!next)
        return;
    //last = temp;
    rsetcolor(droom->roomtype);

    glBegin(GL_LINES);
    while (temp)
    {
        if (!next)
            break;

        if (!testlegal (temp, next))
            rsetcolor(0);


        glVertex3f(temp->cx * 5, 800 - (temp->cy * 5), temp->cz);
        glVertex3f(next->cx * 5, 800 - (next->cy *5), next->cz);
        last = next;
        temp = next;
        next = temp->next;
        rsetcolor(droom->roomtype);

    }
    glEnd();
    glBegin(GL_LINES);
    if (!testlegal (first, last))
        rsetcolor(0);
    glVertex3f(last->cx * 5, 800 - (last->cy *5), last->cz);
    glVertex3f(first->cx * 5, 800 - (first->cy *5), first->cz);
    glEnd();


}

void drawequip2(int xpos, int ypos, int deck, float alpha)
{
    if (equipgrid[xpos][ypos][deck] == 0)
        return;


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor4f(1.0, 1.0, 1.0, alpha);
    GLfloat texx1 = equipgrid[xpos][ypos][deck]->texx1;
    GLfloat texx2 = equipgrid[xpos][ypos][deck]->texx2;
    GLfloat texy1 = equipgrid[xpos][ypos][deck]->texy1;
    GLfloat texy2 = equipgrid[xpos][ypos][deck]->texy2;
    //dib = 0.0625;
    //glLoadIdentity();
    glPushMatrix();
    //glTranslatef(-(xpos*5+0), -(800-ypos*5+222), 0);
    glTranslated(xpos*5, (800-ypos*5), 0);
    //glTranslated(-xpos*5, -(800-ypos*5), 0);
    glRotated(-equipgrid[xpos][ypos][deck]->rotation*45, 0, 0, 1);
    //glTranslated(xpos*5, (800-ypos*5), 0);
    //glTranslated(-5, -5, 0);

    //glRotated(-30, 0, 0, 1);
    //glRotated(30, 0, 0, 1);
    //glTranslated(5, 5, 0);
    //glRotated(-30, 0, 0, 1);
    //glRotate(30);
    //glTranslatef(xpos*5+0, 800-ypos*5+222,  0);

    /*
    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(xpos*5 -2 , (800-ypos*5) -3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(xpos*5 +3, (800-ypos*5) -3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(xpos*5 + 3, (800-ypos*5)+2, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(xpos*5 -2, (800-ypos*5)+2, 0);
    glEnd();
    */
    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(-2 , -3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(3, -3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(3, 2, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(-2, 2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


}

void drawequip(int xpos, int ypos, int deck)
{
    if (equipgrid[xpos][ypos][deck] == 0)
        return;


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    GLfloat texx1 = equipgrid[xpos][ypos][deck]->texx1;
    GLfloat texx2 = equipgrid[xpos][ypos][deck]->texx2;
    GLfloat texy1 = equipgrid[xpos][ypos][deck]->texy1;
    GLfloat texy2 = equipgrid[xpos][ypos][deck]->texy2;
    //dib = 0.0625;
    //glLoadIdentity();
    glPushMatrix();
    //glTranslatef(-(xpos*5+0), -(800-ypos*5+222), 0);
    glTranslated(xpos*5, (800-ypos*5), 0);
    //glTranslated(-xpos*5, -(800-ypos*5), 0);
    glRotated(-equipgrid[xpos][ypos][deck]->rotation*45, 0, 0, 1);
    //glTranslated(xpos*5, (800-ypos*5), 0);
    //glTranslated(-5, -5, 0);

    //glRotated(-30, 0, 0, 1);
    //glRotated(30, 0, 0, 1);
    //glTranslated(5, 5, 0);
    //glRotated(-30, 0, 0, 1);
    //glRotate(30);
    //glTranslatef(xpos*5+0, 800-ypos*5+222,  0);

    /*
    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(xpos*5 -2 , (800-ypos*5) -3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(xpos*5 +3, (800-ypos*5) -3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(xpos*5 + 3, (800-ypos*5)+2, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(xpos*5 -2, (800-ypos*5)+2, 0);
    glEnd();
    */
    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(-2 , -3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(3, -3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(3, 2, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(-2, 2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


}

void rsetcolor2(int rtype, float alpha)
{
    if (rtype == 0)
        glColor4f(1.0, 0.0, 0.0, alpha);
    if (rtype == -1)
        glColor4f(0.5, 1.0, 0.0, alpha);
    if (rtype == -2) //bad origin
        glColor4f(0.2, 0.2, 1.0, alpha);
    if (rtype == -3) //good origin
        glColor4f(0.4, 0.8, 0.0, alpha);
    if (rtype == BARRACK)
        glColor4f(0.59, 0.59, 0.2, alpha);
    if (rtype == BRIDGE)
        glColor4f(1.0, 0.85, 0.2, alpha);
    if (rtype == BRIG)
        glColor4f(0.49, 0.49, 0.49, alpha);
    if (rtype == CREW)
        glColor4f(0.8, 0.7, 0.6, alpha);
    if (rtype == ENGINE)
        glColor4f(0.89, 0.89, 0.89, alpha);
    if (rtype == VEHICLE)
        glColor4f(1.0, .68, 0.2, alpha);
    if (rtype == FUEL)
        glColor4f(1.0, 1.0, 1.0, alpha);
    if (rtype == GALLEY)
        glColor4f(1.0, 1.0, 0.8, alpha);
    if (rtype == GRAV)
        glColor4f(0.80, 1.0, 0.65, alpha);
    if (rtype == HALL)
        glColor4f(0.49, 0.89, 0.49, alpha);
    if (rtype == HOLD)
        glColor4f(0.80, 0.68, 0.54, alpha);
    if (rtype == OFFICER)
        glColor4f(0.11, 0.11, 0.8, alpha);
    if (rtype == OPEN)
        glColor4f(0.54, 0.54, 0.65, alpha);
    if (rtype == LOUNGE)
        glColor4f(0.74, 0.24, 0.25, alpha);
    if (rtype == SENSOR)
        glColor4f(0.74, 0.0, 0.75, alpha);
    if (rtype == SHIELD)
        glColor4f(0.0, 0.94, 0.75, alpha);
    if (rtype == MED)
        glColor4f(0.44, 0.84, 0.65, alpha);
    if (rtype == STATE)
        glColor4f(0.64, 0.0, 0.45, alpha);
    if (rtype == TP)
        glColor4f(0.80, 0.0, 0.75, alpha);
    if (rtype == WEAP)
        glColor4f(0.4, 0.4, 1.0, alpha);

}

void rsetcolor(int rtype)
{
    if (rtype == 0)
        glColor4f(1.0, 0.0, 0.0, 0.3);
    if (rtype == -1)
        glColor4f(0.5, 1.0, 0.0, 1.0);
    if (rtype == -2) //bad origin
        glColor4f(0.2, 0.2, 1.0, 0.5);
    if (rtype == -3) //good origin
        glColor4f(0.4, 0.8, 0.0, 1.0);
    if (rtype == BARRACK)
        glColor4f(0.59, 0.59, 0.2, .6);
    if (rtype == BRIDGE)
        glColor4f(1.0, 0.85, 0.2, .6);
    if (rtype == BRIG)
        glColor4f(0.49, 0.49, 0.49, .6);
    if (rtype == CREW)
        glColor4f(0.8, 0.7, 0.6, .6);
    if (rtype == ENGINE)
        glColor4f(0.89, 0.89, 0.89, .6);
    if (rtype == VEHICLE)
        glColor4f(1.0, .68, 0.2, .6);
    if (rtype == FUEL)
        glColor4f(1.0, 1.0, 1.0, .6);
    if (rtype == GALLEY)
        glColor4f(1.0, 1.0, 0.8, .6);
    if (rtype == GRAV)
        glColor4f(0.80, 1.0, 0.65, .6);
    if (rtype == HALL)
        glColor4f(0.49, 0.89, 0.49, .6);
    if (rtype == HOLD)
        glColor4f(0.80, 0.68, 0.54, .6);
    if (rtype == OFFICER)
        glColor4f(0.11, 0.11, 0.8, .6);
    if (rtype == OPEN)
        glColor4f(0.54, 0.54, 0.65, .6);
    if (rtype == LOUNGE)
        glColor4f(0.74, 0.24, 0.25, .6);
    if (rtype == SENSOR)
        glColor4f(0.74, 0.0, 0.75, .6);
    if (rtype == SHIELD)
        glColor4f(0.0, 0.94, 0.75, .6);
    if (rtype == MED)
        glColor4f(0.44, 0.84, 0.65, .6);
    if (rtype == STATE)
        glColor4f(0.64, 0.0, 0.45, .6);
    if (rtype == TP)
        glColor4f(0.80, 0.0, 0.75, .6);
    if (rtype == WEAP)
        glColor4f(0.4, 0.4, 1.0, .6);
           /* enum  {NIL, BARRACK, BRIDGE, BRIG, OFFICER, HOLD, HALL, CREW, ENGINE, FUEL, GALLEY, GRAV, NIL2, VEHICLE, LOUNGE,
       OPEN, SENSOR, SHIELD, MED, STATE, TP, WEAP}; */

}

bool testlegal(VLINK *one, VLINK *two)
{
    //return true;
    if (!one || !two)
    return false;
    //fprintf(stderr,"onex %i oney %i twox %i twoy %i\n", one->cx, one->cy, two->cx, two->cy);
    int onex = one->cx * 5;
    int oney = one->cy * 5;
    int twox = two->cx * 5;
    int twoy = two->cy * 5;
    bool xequal = (one->cx == two->cx);
    bool yequal = (one->cy == two->cy);
    bool stateox = (onex % 10 == 5);
    bool stateoy = (oney % 10 == 5);
    bool statetx = (twox % 10 == 5);
    bool statety = (twoy % 10 == 5);
    if ((stateox && stateoy) || (statetx && statety))
    {
        if (!diagonaltest(onex, oney, twox, twoy))
        return false;
    }
    else
    {
        // if one of the planes is equal, you are fine, otherwise diagonal test
        if (!xequal && !yequal)
        {
            if (!diagonaltest(onex, oney, twox, twoy))
            return false;
        }
    }
    if (xequal && yequal)
    {
        return false;
    }
    return true;
}

void truesnap(VLINK *node, int &mx, int &my);

void M_Line(int curx, int cury, ROOM *setroom)
{
    //glColor4f(0.5, 1.0, 0.0, 1.0)
    rsetcolor(-1);

    VLINK *temp;
    VLINK *first;
    VLINK *last;
    if (!setroom)
        return;
    temp = setroom->lverts;
    //temp = rooms[0].lverts;
    first = temp;
    glLineWidth(2.0);
    int x = curx / 5;
    int y = cury / 5;
    x *= 5;
    y *= 5;
    glBegin(GL_LINE_STRIP);
    while (temp)
    {
       // glVertex3f(temp->x, temp->y, temp->z);
       glVertex3f(temp->cx * 5, 800 - (temp->cy * 5), temp->cz);
       last = temp;
       temp = temp->next;
    }
    glEnd();
    if (!first || !last)
    {
        return;
    }
    if ((!mode && first) || (uimode != D_ROOM && first))
    {

        if (!testlegal(last, first))
            rsetcolor(-2);

        if (roomIsec(setroom, first->cx, first->cy, last->cx, last->cy))
            rsetcolor(-2);
        glBegin(GL_LINES);
        glVertex3f(first->cx * 5, 800 - (first->cy *5), first->cz);
        glVertex3f(last->cx * 5, 800 - (last->cy *5), last->cz);
        glEnd();
    }
    else if (uimode == D_ROOM)
    {
        int qx = curx;
        int qy = cury;
        truesnap(last, qx, qy);


        VLINK *quick = new VLINK;
        quick->cx = cursnap(curx) / 5;
        quick->cy = (800 - cursnap(cury)) / 5;

        if (!testlegal(quick, last))
            rsetcolor(0);

        //if (pointInPolygon2(setroom, (float)curx, (float)cury))
           // rsetcolor(0);
        if (roomIsec(setroom, last->cx, last->cy, quick->cx, quick->cy))
            rsetcolor(0);
        //glColor3f(0.8, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(last->cx * 5, 800 - (last->cy *5), last->cz);
        //glVertex3f(curx, cury, 0.0);
        glVertex3f(qx, qy, 0.0);
        //glColor3f(0.5, 1.0, 0.0);
        rsetcolor(-3);

        if (!testlegal(quick, first))
            rsetcolor(-2);

        if (roomIsec(setroom, first->cx, first->cy, quick->cx, quick->cy))
            rsetcolor(-2);

        //glColor3f(0.8, 0.0, 0.0);
        delete quick;
        glBegin(GL_LINES);
        glVertex3f(first->cx * 5, 800 - (first->cy *5), first->cz);
        //glVertex3f(curx, cury, 0.0);
        glVertex3f(qx, qy, 0.0);
        glEnd();

    }

}


bool isconsolenear(int tx, int ty, int deckn);
int setcollision(int equiptype);
bool finishroom2(ROOM *droom);
bool testPsec2(VLINK *line1, VLINK *line2, int xpos, int ypos);
bool roomEtest3(ROOM *droom, int xpos, int ypos);
int medimod;
bool vpick(ROOM *setroom, int mousex, int mousey);



PART::PART()
{
    c_deck = 28;
    lglobal =0;
    for (int g = 0; g < 65536; g++)
        g_rooms[g] = false;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                rooms[t][r][d] = NULL;
            }
        }
    }
    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            for (int d = 0; d < 57; d++)
            {
                equiplist[i][j][d] = NULL;
            }
        }
    }

}

bool PART::addroom(ROOM *setroom)
{
    if (!setroom)
        return false;
    if (!setroom->lverts)
        return false;
    //exit(5);
    bool grf = false;
    int gr = 0;
    for (int g = 0; g < 65536; g++)
    {
        if (!g_rooms[g])
        {
            gr = g;
            grf = true;
            break;
        }
    }
    if (!grf)
        return false;
    //exit(5);

    //i call this in from the main program to copy a room into it.  so, here we are and it shows the room exists.
//need to make    setroom->decknum; // this gets the level the room is on. we need to copy the room to the appropriate level.
//will add by default    setroom->roomtype; //we dont care about the global number because it will have to 'set' when we place it. or do we? grab it anyway just in case
//will add by default    setroom->globalnum;
//setroom->roomnum; // dont need to make and in fact shouldn't use: assign it to the first one available.
    //when we do this for real, we need to genericize it so that deck29 is the 'center' deck always. This will be a
    //part only operation, since we will add rooms/equip to it room by room later.
    //we have [type][n][deck], assign to first n available.
    int type = setroom->roomtype;
    //int g_id = setroom->globalnum;
    int deck = setroom->decknum;
    int rnum;
    for (rnum = 0; rnum < 256; rnum++)
    {
        if (!rooms[type][rnum][deck])
            break;
    }
    if (rnum >= 256)
        return false;

    //we now assign the room and copy
    rooms[type][rnum][deck] = roomcopy(setroom);
    //now we set the other things.

    if (!rooms[type][rnum][deck])
        return false;
    rooms[type][rnum][deck]->roomnum = rnum;
    rooms[type][rnum][deck]->globalnum = gr;
    if (lglobal < 1)
        c_deck = deck;
    lglobal++;

    //now add all equipment thats the same global id
    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            for (int z = 0; z < 57; z++)
            {
                equipcopy(equipgrid[x][y][z], x, y, z, setroom->globalnum);
            }
        }
    }
    return true;
}

ROOM* PART::roomcopy(ROOM *setroom) // looks like I can use this anywhere
{
    if (!setroom)
        return NULL;
    if (!setroom->lverts)
        return NULL;
    ROOM *droom = new ROOM;
    int iter = setroom->vnum;
    droom->decknum = setroom->decknum;
    droom->f_legal = setroom->f_legal;
    droom->globalnum = 0; //its global is set later
    droom->roomnum = 0; //its set up above, because it cant just copy
    droom->roomtype = setroom->roomtype;
    droom->vnum = iter;
    VLINK *nodes[iter];
    VLINK *temp = setroom->lverts;
    //exit(9);
    for (int i = 0; i < iter; i++)
    {
        nodes[i] = new VLINK;
        nodes[i]->cx = temp->cx;
        nodes[i]->cy = temp->cy;
        nodes[i]->cz = temp->cz;
        nodes[i]->next = NULL;
        temp = temp->next;
    } // they are all set up but unlinked.
    droom->lverts = nodes[0];
    temp = droom->lverts;
    for (int i = 0; i < iter-1; i++)
    {

        //temp->next = nodes[i+1];
        //temp = temp->next;
        nodes[i]->next = nodes[i+1];

    }

    return droom;


}

bool PART::equipcopy(EQUIPUNIT *se, int x, int y, int z, int g_id)
{
    if (!se)
        return false;
    if (se->roomid != g_id)
        return false;
    if (equiplist[x][y][z])
        delete equiplist[x][y][z];
    equiplist[x][y][z] = new EQUIPUNIT(se->type, 0, g_id, se->loc_roomid, se->mod1*0x100 + se->mod2, se->rotation);
   // fprintf(stdout, "made a %i", equiplist[x][y][z]->type);
//EQUIPUNIT(int ttype, long int eqnum, short int ronum, short int modtype, int rot);


    return true;
}

bool PART::displaypart()
{
    /*
    Note: Need to change this so that it displays the 'current deck' selected.  Then, change the addroom function to snap
    to the first room created.  When I add delete, I need to take care of this too.

    */
    // this draws a very scrunched up version of the part (factor of 5).
    // since only one level can display at once, we need to figure this stuff out.
    // may want to run the part stabilizer first: goes up and down and sets the part to the 'middle' deck when you are done
    // adding stuff to it.
    // fuck it. for now, lets just go to the first level it finds.
    // note: middle deck = 28
    // I also need a location to display it, just display it at 800, 520 right now.
    // take first vertex as the translate point and translate it and all other points backwards.
    /*
    int dlevel = -1;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (rooms[t][r][d])
                    dlevel = d;
            }
        }
    }
    if (dlevel == -1)
        return false;
    */
    //exit(7);
    // now what we do is display each 'room' in this deck / whichever deck
    //displayroom(someroom)
    //int ox =
    //glColor4f(0.6, 0.4, 0.6, 1.0);
    //glBegin(GL_QUADS);
    //glVertex3f(801, 600, 0.0);
    //glVertex3f(999, 600, 0.0);
    //glVertex3f(999, 799, 0.0);
    //glVertex3f(801, 799, 0.0);
    //glEnd();

    glLineWidth(2);

    glColor4f(0.6, 0.4, 0.6, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(801, 600, 0.0);
    glVertex3f(999, 600, 0.0);
    glVertex3f(999, 799, 0.0);
    glVertex3f(801, 799, 0.0);
    glEnd();
    glColor4f(0.1, 0.1, 0.3, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(802, 600, 0.0);
    glVertex3f(997, 600, 0.0);
    glVertex3f(997, 799, 0.0);
    glVertex3f(802, 799, 0.0);
    glEnd();

    glColor4f(0.2, 0.2, 0.2, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(802, 600, 0.0);
    glVertex3f(964, 600, 0.0);
    glVertex3f(964, 764, 0.0);
    glVertex3f(802, 764, 0.0);
    glEnd();

    glColor4f(0.6, 0.5, 0.6, 1.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(965, 601, 0.0);
    glVertex3f(965, 765, 0.0);
    glVertex3f(802, 765, 0.0);

    glEnd();

        //part selection area

    glColor4f(0.15, 0.1, 0.15, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(802, 522, 0.0);
    glVertex3f(999, 522, 0.0);
    glVertex3f(999, 600, 0.0);
    glVertex3f(802, 600, 0.0);
    //darker sec
    glColor4f(0.08, 0.05, 0.08, 1.0);
    glVertex3f(802, 586, 0.0);
    glVertex3f(999, 586, 0.0);
    glVertex3f(999, 600, 0.0);
    glVertex3f(802, 600, 0.0);
    //lighter sec, do by selection later
    glColor4f(0.18, 0.15, 0.18, 1.0);
    glVertex3f(802, 586, 0.0);
    glVertex3f(832, 586, 0.0);
    glVertex3f(832, 600, 0.0);
    glVertex3f(802, 600, 0.0);


    glEnd();


    glColor4f(0.5, 0.3, 0.4, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(802, 522, 0.0);
    glVertex3f(999, 522, 0.0);
    glVertex3f(999, 600, 0.0);
    glVertex3f(802, 600, 0.0);

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(802, 586, 0.0);
    glVertex3f(999, 586, 0.0);
    glVertex3f(832, 586, 0.0);
    glVertex3f(832, 600, 0.0);
    glVertex3f(862, 586, 0.0);
    glVertex3f(862, 600, 0.0);
    glVertex3f(892, 586, 0.0);
    glVertex3f(892, 600, 0.0);
    glVertex3f(922, 586, 0.0);
    glVertex3f(922, 600, 0.0);
    glVertex3f(952, 586, 0.0);
    glVertex3f(952, 600, 0.0);
    glVertex3f(982, 586, 0.0);
    glVertex3f(982, 600, 0.0);


    glEnd();

    glPushMatrix();
    char cl[] = "Level:";
    char cl2[3];
    int cpr = c_deck+1;
    cl2[0] = cpr / 10 + 48;
    if (cl2[0] == '0')
        cl2[0] = ' ';

    cl2[1] = cpr % 10 + 48;
    cl2[2] = '\0';
    glPrint(802, 568, cl, 0, 1);
    glPrint(862, 568, cl2, 0, 1);



    glPopMatrix();






    glTranslated(802, 602, 0);
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            //displayroom(rooms[t][r][dlevel]);
            displayroom(rooms[t][r][c_deck]);

        }
    }

    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            //displayequip(equiplist[x][y][dlevel], x, y);
            displayequip(equiplist[x][y][c_deck], x, y);
        }
    }
    glTranslated(-802, -602, 0);

    //exit(9);
    return true;

}

bool PART::displayequip(EQUIPUNIT *se, int xpos, int ypos)
{
    if (!se)
        return false;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    GLfloat texx1 = se->texx1;
    GLfloat texx2 = se->texx2;
    GLfloat texy1 = se->texy1;
    GLfloat texy2 = se->texy2;

    glPushMatrix();
    glTranslated(xpos, (160-ypos), 0);
    glRotated(-se->rotation*45, 0, 0, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(-2/3 , -3/3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(3/3, -3/3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(3/3, 2/3, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(-2/3, 2/3, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    return true;
}

bool PART::displayroom(ROOM *sr)
{
    if (!sr)
        return false;
    if (!sr->lverts)
        return false;
    if (sr->vnum<=2)
        return false;

    // we use the stencil thing here too, remember.
    /*
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    */
    //what I will need to do is find the room center and top left for displaying.  Might have to do some float displaying.
    //first we need to do translations: translate temp->cx, 160- temp->cy backwards, THEN translate to our
    //display position. We actually need to do this up above because otherwise you will draw all the rooms on top of each other!!!
    //note: to do this properly, we have to get the bounding box of ALL the rooms!!! Otherwise, we just assume the topleft bound is the proper one. lets do that for now.

    VLINK *temp = sr->lverts;
    //glTranslated(-temp->cx, -(160-temp->cy), 0);
    //glTranslated(805, 540, 0);

    VLINK *first;
    VLINK *last;
    first = temp;
    last = temp;


    glEnable(GL_STENCIL_TEST);          // enable stencil test

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // disable writing to color buffer
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        glVertex3f(temp->cx, (160 - temp->cy), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    temp = sr->lverts;


    // PASS 2: draw color buffer
    // Draw again the exact same polygon to color buffer where the stencil
    // value is only odd number(1). The even(0) area will be descarded.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    // enable writing to color buffer
    rsetcolor(sr->roomtype);
    if (!sr->f_legal)
        rsetcolor(0);

    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    //glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        //glTexCoord2f(temp->cx, 160-temp->cy);
        glVertex3f(temp->cx, (160- temp->cy), 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    //glDisable(GL_TEXTURE_2D);

    glDisable(GL_STENCIL_TEST);
    //*/



    return true;

}

bool PART::displayatcursor(int mx, int my)
{ //sort this out, its all copypasta atm

    /*
    int dlevel = -1;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (rooms[t][r][d])
                    dlevel = d;
            }
        }
    }
    if (dlevel == -1)
        return false;
        */
// we need to figure out the 'origin' for this thingy. remember, we are going D/R amount equal to mouse, and that should
// = the origin point
// we need to sort all rooms and figure it out probably, otherwise we will just have to guess this shit.
//perhaps simply make the center point an origin point and use the difference by mouse loc. good compromise at first
//i suppose to get it 'working'
//800, 800: 400, 400 is center.  therefore, if mousex and mousey = 400, translate 0 in either direction.
    glTranslated(mx-400, my-400, 0);
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            //displayroomac(rooms[t][r][dlevel], mx, my);  // do i even need these? if not, just remove the mx, my later.
            displayroomac(rooms[t][r][c_deck], mx, my);
        }
    }

    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            //displayeqac(equiplist[x][y][dlevel], x, y, mx, my);
            displayeqac(equiplist[x][y][c_deck], x, y, mx, my);
        }
    }
    glTranslated(-(mx+400), -(my+400), 0);

    //exit(9);
    return true;


}

bool PART::displayroomac(ROOM *sr, int mx, int my)
{ //also copypasta
    if (!sr)
        return false;
    if (!sr->lverts)
        return false;
    if (sr->vnum<=2)
        return false;

    // we use the stencil thing here too, remember.
    /*
    if (!droom)
        return;
    if (!droom->lverts)
        return;
    */
    //what I will need to do is find the room center and top left for displaying.  Might have to do some float displaying.
    //first we need to do translations: translate temp->cx, 160- temp->cy backwards, THEN translate to our
    //display position. We actually need to do this up above because otherwise you will draw all the rooms on top of each other!!!
    //note: to do this properly, we have to get the bounding box of ALL the rooms!!! Otherwise, we just assume the topleft bound is the proper one. lets do that for now.

    VLINK *temp = sr->lverts;
    //glTranslated(-temp->cx, -(160-temp->cy), 0);
    //glTranslated(805, 540, 0);

    VLINK *first;
    VLINK *last;
    first = temp;
    last = temp;


    glEnable(GL_STENCIL_TEST);          // enable stencil test

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // disable writing to color buffer
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);

    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        glVertex3f(temp->cx*5, (160 - temp->cy)*5, 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    temp = sr->lverts;


    // PASS 2: draw color buffer
    // Draw again the exact same polygon to color buffer where the stencil
    // value is only odd number(1). The even(0) area will be descarded.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);    // enable writing to color buffer
    rsetcolor(sr->roomtype);
    if (!sr->f_legal)
        rsetcolor(0);

    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    //glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    while (temp)
    {
        //glTexCoord2f(temp->cx, 160-temp->cy);
        glVertex3f(temp->cx*5, (160- temp->cy)*5, 0.0);
        last = temp;
        temp = temp->next;
    }
    glEnd();
    //glDisable(GL_TEXTURE_2D);

    glDisable(GL_STENCIL_TEST);
    //*/



    return true;

}

bool PART::displayeqac(EQUIPUNIT *se, int xpos, int ypos, int mx, int my)
{ // also copypasta
    if (!se)
        return false;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    GLfloat texx1 = se->texx1;
    GLfloat texx2 = se->texx2;
    GLfloat texy1 = se->texy1;
    GLfloat texy2 = se->texy2;

    glPushMatrix();
    glTranslated(xpos*5, (160-ypos)*5, 0);
    glRotated(-se->rotation*45, 0, 0, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(texx1, texy1); glVertex3f(-2 , -3, 0);
    glTexCoord2f(texx2, texy1); glVertex3f(3, -3, 0);
    glTexCoord2f(texx2, texy2); glVertex3f(3, 2, 0);
    glTexCoord2f(texx1, texy2); glVertex3f(-2, 2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    return true;

}

bool PART::clearpart()
{
    // delete all rooms, delete all equipment.
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (rooms[t][r][d])
                {
                    clearroom (rooms[t][r][d]);
                    delete rooms[t][r][d];
                    rooms[t][r][d] = NULL;
                }
            }
        }
    }
    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (equiplist[x][y][d])
                {
                    delete equiplist[x][y][d];
                    equiplist[x][y][d] = NULL;
                }

            }
        }
    }


    lglobal = 0;
    c_deck = 28;

    return true;
}

bool PART::clearroom(ROOM *dr)
{
    int vnum = dr->vnum;
    VLINK *nodes[vnum];
    VLINK *temp = dr->lverts;
    for (int i = 0; i < vnum; i++)
    {
        nodes[i] = temp;
        temp = temp->next;
        delete nodes[i];
    }
    dr->lverts = NULL;
    return true;
}

bool PART::insettest(ROOM *droom, int ox, int oy, int od) // may want to take care of the snap outside here
{
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    VLINK *nodes[droom->vnum];
    VLINK *temp = droom->lverts;
    for (int i = 0; i < droom->vnum; i++) // do i need this? if not, just do while, test
    {
        nodes[i] = temp;
        temp = temp->next;
    }
    int zt = droom->decknum + od;
    if (zt < 0 || zt > 57)
        return false;
    //+/- mx based on 400 i suppose. We also snap it to the nearest 10, not 5.
    //we should also 'snap' it based on closest. otherwise, its always to lesser than.
    for (int j = 0; j < droom->vnum; j++)
    {
        if (nodes[j]->cx+ox < 0 || nodes[j]->cx+ox > 160 ||
            nodes[j]->cy+oy < 0 || nodes[j]->cy+oy > 160)
            return false;
    }
    return true;
}

bool PART::insettest2(ROOM *droom, int ox, int oy, int od) // may want to take care of the snap outside here
{
    if (!droom)
        return true;
    if (!droom->lverts)
        return true;
    VLINK *nodes[droom->vnum];
    VLINK *temp = droom->lverts;
    for (int i = 0; i < droom->vnum; i++) // do i need this? if not, just do while, test
    {
        nodes[i] = temp;
        temp = temp->next;
    }
    int zt = droom->decknum + od;
    if (zt < 0 || zt > 57)
        return false;
    //+/- mx based on 400 i suppose. We also snap it to the nearest 10, not 5.
    //we should also 'snap' it based on closest. otherwise, its always to lesser than.
    for (int j = 0; j < droom->vnum; j++)
    {
        if (nodes[j]->cx+ox < 0 || nodes[j]->cx+ox > 160 ||
            nodes[j]->cy+oy < 0 || nodes[j]->cy+oy > 160)
            return false;
    }
    return true;
}

bool PART::insetalltest(int ox, int oy, int od)
{
    int px = (ox/10)*10;
    int py = (oy/10)*10;
    if (ox % 10 >= 5)
        px += 10;
    if (oy % 10 >= 5)
        py += 10;
    px = (px-400)/5;
    py = ((800-py)-400)/5;
    //md = the deck you are putting it on. to work, this has to be very specific: it has to take the center,
    //which is where the display is, and compare.  we need to look over the display/other stuff again to
    //make sure we have this right.
    int pd = od - c_deck;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (!insettest2(rooms[t][r][d], px, py, pd))
                    return false;
            }
        }
    }
    return true;

}

bool PART::setproom(ROOM *droom, int type, int roomnum, int decknum, int ox, int oy, int od)
{
// this will take in a room and attempt to add it to the deckrooms.
    if (!droom)
        return false;
    if (!droom->lverts)
        return false;
    //first, we sort thru global rooms and see if theres room to add it.
    bool gadd = false;
    int gadd_i = 0;
    for (int i = 0; i < 65536; i++)
    {
        if (!rooms_g[i])
        {
            gadd = true;
            gadd_i = i;
            break;
        }
    }
    if (!gadd)
        return false;
    //now we know theres little enough global rooms, now we need to test decktypes
    bool radd = false;
    int radd_i = 0;
    if (!deckrooms[type][roomnum][decknum+od])
    {
        radd = true;
        radd_i = roomnum;
    }
    else
    {
        for (int r = 0; r < 256; r++)
        {
            if (!deckrooms[type][r][decknum+od])
            {
                radd = true;
                radd_i = r;
                break;
            }
        }
    }
    if (!radd)
        return false;
    //Enough rooms and theres a room thats open: we make it that room number and set the available global number
    deckrooms[type][radd_i][decknum + od] = roomcopy(droom);
    deckrooms[type][radd_i][decknum + od]->globalnum = gadd_i+1;
    rooms_g[gadd_i] = true;
    deckrooms[type][radd_i][decknum + od]->roomnum = radd_i;
    deckrooms[type][radd_i][decknum + od]->decknum += od;

    // now we need to sort out all the links and move them by the amount passed in with ox, oy
    VLINK *temp = deckrooms[type][radd_i][decknum + od]->lverts;
    while (temp)
    {
        temp->cx += ox;
        temp->cy += oy;
        temp = temp->next;
    }

    return true;
}

bool PART::setpeg(EQUIPUNIT *seq, int x, int y, int d, int ox, int oy, int od)
{
    if (!seq)
        return false;
    //we must make sure any equipment we try to place is in the range proper: even though it SHOULD be taken care of, we dont want to risk shit.

    int setx = x + ox;
    int sety = y + oy;
    if (setx < 0 || setx > 161 ||
        sety < 0 || sety > 161)
        return false;

    int td = d+od;
    if (td < 0 || td > 57)
        return false;




    //now we are safe to place: if theres one there, just delete it.
    if (equipgrid[setx][sety][td])
        delete (equipgrid[setx][sety][td]);
    equipgrid[setx][sety][td] = NULL;
    currentrot = 0;
    equiptype = seq->type;
    //fprintf(stderr,"EQTYPE %i\n", equiptype);
    AddEquip2(setx*5, (160-sety)*5, td, 0, seq->mod1*0x10 + seq->mod2);
        //exit(9);
    //yep, need to actually 'place' it in there with the other function, since theres no easy 'hit' for global id
//bool AddEquip2(int mousex, int mousey, int deckn, int bsize, short int cmod)
    //equipgrid[setx][sety][d+od] = new EQUIPUNIT(seq->type, 0, )
    //equiplist[x][y][z] = new EQUIPUNIT(se->type, 0, g_id, se->mod1*0x100 + se->mod2, se->rotation);

    return true;
}

bool PART::insetatcursor(int mx, int my, int md)
{
    //bool nofail = true;
    int px = (mx/10)*10;
    int py = (my/10)*10;
    if (mx % 10 >= 5)
        px += 10;
    if (my % 10 >= 5)
        py += 10;
    px = (px-400)/5;
    py = ((800-py)-400)/5;
    //md = the deck you are putting it on. to work, this has to be very specific: it has to take the center,
    //which is where the display is, and compare.  we need to look over the display/other stuff again to
    //make sure we have this right.
    int pd = md - c_deck;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                if (insettest(rooms[t][r][d], px, py, pd))
                { // you want to go to a function that tests anything, then sets the room in.
                    //this may test for other collisions as well.  maybe.
                    setproom(rooms[t][r][d], t, r, d, px, py, pd);
                    //roomcopy(rooms[t][r][d]);
                }
            }
        }
    }
    // now attempt to place each equipment unit;
    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            for (int d = 0; d < 57; d++)
            {
                setpeg(equiplist[x][y][d], x, y, d, px, py, pd);
            }
        }
    }
    return true;
}

bool PART::leveldown()
{
    if (c_deck > 0)
        c_deck--;
    return true;
}

bool PART::levelup()
{
    if (c_deck < 56)
        c_deck++;
    return true;
}
/*
enum {FLIP_LR, FLIP_UD};
enum {ROT_90, ROT_180, ROT_270};
    ROOM *rooms[22][256][57];
    EQUIPUNIT *equiplist[161][161][57];
*/
bool PART::flip(int flip)
{

    EQUIPUNIT *changelist[161][161];

    if (flip == FLIP_LR) // 0 to 160 unsigned char, signed = 128 cap. need an int to hold this
    {//4 cvt: needs to be 156. 4 - 160 * -1 = 156. 10 to 150: 10-160 = -150.  X changes, Y constant.
        //for equipment: everything is a pointer. make new pointer array and point to everything, then flip
        //what the main array points to: no need to copypasta anything. 0-79, 80-160.
        //first, flip rooms
        for (int t = 0; t < 22; t++)
        {
            for (int r = 0; r < 256; r++)
            {
                for (int d = 0; d < 57; d++)
                {
                    if (rooms[t][r][d])
                    {
                        if (rooms[t][r][d]->lverts)
                        {
                            VLINK* temp = rooms[t][r][d]->lverts;
                            while (temp)
                            {
                                short int flx = temp->cx;
                                flx -= 160;
                                flx *= -1;
                                temp->cx = flx;
                                temp = temp->next;
                            }
                        }
                    }
                }
            }
        }
        for (int d = 0; d < 57; d++)
        {
            for (int bx = 0; bx < 161; bx++)
            {
                for (int by = 0; by < 161; by++)
                {
                    changelist[bx][by] = equiplist[bx][by][d];
                }
            }
            for (int x = 0; x < 161; x++)
            {
                for (int y = 0; y < 161; y++)
                {

                        equiplist[x][y][d] = changelist[160-x][y];

                }
            }
        }


    }
    else
    {
        for (int t = 0; t < 22; t++)
        {
            for (int r = 0; r < 256; r++)
            {
                for (int d = 0; d < 57; d++)
                {
                    if (rooms[t][r][d])
                    {
                        if (rooms[t][r][d]->lverts)
                        {
                            VLINK* temp = rooms[t][r][d]->lverts;
                            while (temp)
                            {
                                short int fly = temp->cy;
                                fly -= 160;
                                fly *= -1;
                                temp->cy = fly;
                                temp = temp->next;
                            }
                        }
                    }
                }
            }
        }
        for (int d = 0; d < 57; d++)
        {
            for (int bx = 0; bx < 161; bx++)
            {
                for (int by = 0; by < 161; by++)
                {
                    changelist[bx][by] = equiplist[bx][by][d];
                }
            }
            for (int x = 0; x < 161; x++)
            {
                for (int y = 0; y < 161; y++)
                {

                        equiplist[x][y][d] = changelist[x][160-y];

                }
            }
        }

    }
    return true;

}

bool PART::rotate(int rotate)
{
    // just do rotate 90 for now.
    EQUIPUNIT *changelist[161][161];
    //first, rotate the rooms.
    if (rotate == ROT_90)
    {
        for (int t = 0; t < 22; t++)
        {
            for (int r = 0; r < 256; r++)
            {
                for (int d = 0; d < 57; d++)
                {
                    if (rooms[t][r][d])
                    {
                        if (rooms[t][r][d]->lverts)
                        {
                            VLINK* temp = rooms[t][r][d]->lverts;
                            while (temp)
                            {
                                short int fly = temp->cy;
                                short int flx = temp->cx;
                                fly -= 80;
                                flx -= 80;


                                //temp->cx = -fly;
                                //temp->cx += 80;
                                temp->cx = fly +80;
                                //temp->cy = flx;
                                //temp->cy += 80;
                                temp->cy = -flx+80;
                                temp = temp->next;
                            }
                        }
                    }
                }
            }
        }
        for (int d = 0; d < 57; d++)
        {
            for (int bx = 0; bx < 161; bx++)
            {
                for (int by = 0; by < 161; by++)
                {
                    changelist[bx][by] = equiplist[bx][by][d];
                }
            }
            for (int x = 0; x < 161; x++)
            {
                for (int y = 0; y < 161; y++)
                {
                    int subx = x-80;
                    int suby = y-80;


                        equiplist[x][y][d] = changelist[-suby+80][subx+80];

                }
            }
        }



    }


    return true;

}

bool PART::cleardeck(int cdeck)
{
    if (cdeck < 0 ||
        cdeck > 57)
        return false;
   for (int t = 0; t < 22; t++)
   {
       for (int r = 0; r < 256; r++)
       {
           if (rooms[t][r][cdeck])
           {
               g_rooms[rooms[t][r][cdeck]->globalnum-1] = false;
               clearroom(rooms[t][r][cdeck]);
               rooms[t][r][cdeck] = NULL;
           }
       }
   }
   return true;
}

SHIPSTATS::SHIPSTATS()
{
    //{STAT_DISP_MISC, STAT_DISP_COMBAT, STAT_DISP_SC};

    statdisplay = STAT_DISP_MISC;
    for (int i = 0; i < 57; i++)
    {
        deckweight[i] = 0;
        equipweights[i] = 0;
        fuelweights[i] = 0;
        numgravunits[i] = 0;
        numshieldunits[i] = 0;
        numpower[i] = 0;
        numsensors[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            numwep[j][i] = 0;
        }
    }
    for (int j = 0; j < 3; j++)
    {
        numwep_ids[j]= 0;
        wep_range[j] = 0;
        wep_power[j] = 0;
        wep_shots[j] = 0;
        totalwep[j] = 0;
    }

    totalweight = 0;
    totalgravunits = 0;
    totalshieldunits = 0;
    totalpower = 0;
    totalsensors = 0;
    cap_power = 0;
    armorlevel = 0;
    streamlined = false;
    m_s_s = 0;
    sensorrange = 0;
    techlevel = 1;
    for (int j = 0; j < 13; j++)
        shipweight[j] = 48;
    shipweight[13] = 'k';
    shipweight[14] = 'g';
    shipweight[15] = '\0';

    for (int c = 0; c < 12; c++)
    {
        if (shipweight[c] == '0')
            shipweight[c] -= 16;
        else
            break;
    }
    alevel[0] = 'A';
    alevel[1] = 'L';
    alevel[2] = 32;
    alevel[3] = '0';
    alevel[3] -= 16;
    alevel[4] = '0';
    alevel[5] = '\0';
    tlevel[0] = 'T';
    tlevel[1] = 'L';
    tlevel[2] = '0';
    tlevel[2] -= 16;
    tlevel[3] = '0';
    tlevel[3] -= 16;
    tlevel[4] = '1';
    tlevel[5] = '\0';
    accel[0] = ' ';
    accel[1] = ' ';
    accel[2] = ' ';
    accel[3] = ' ';
    accel[4] = '0';
    accel[5] = ' ';
    accel[6] = 'M';
    accel[7] = '/';
    accel[8] = 'S';
    accel[9] = '^';
    accel[10] = '2';
    accel[11] = '\0';
    slevel[0] = 'A';
    slevel[1] = 'b';
    slevel[2] = 's';
    slevel[3] = ' ';
    slevel[4] = ' ';
    slevel[5] = ' ';
    slevel[6] = ' ';
    slevel[7] = ' ';
    slevel[8] = ' ';
    slevel[9] = ' ';
    slevel[10] = ' ';
    slevel[11] = ' ';
    slevel[12] = ' ';
    slevel[13] = ' ';
    slevel[14] = ' ';
    slevel[15] = ' ';
    slevel[16] = ' ';
    slevel[17] = '0';
    slevel[18] = '\0';

    powerc[0] = 'P';
    powerc[1] = 'o';
    powerc[2] = 'w';
    powerc[3] = 'e';
    powerc[4] = 'r';
    powerc[5] = ' ';
    powerc[6] = ' ';
    powerc[7] = ' ';
    powerc[8] = ' ';
    powerc[9] = ' ';
    powerc[10] = ' ';
    powerc[11] = ' ';
    powerc[12] = ' ';
    powerc[13] = ' ';
    powerc[14] = ' ';
    powerc[15] = ' ';
    powerc[16] = ' ';
    powerc[17] = ' ';
    powerc[18] = '0';
    powerc[19] = '\0';

    //sensorc = {'S','e','n','s','o','r',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','0','\0'};
    //sensorc = ((char[20])"Sensor           0\0");
    strcpy(sensorc, "Sensor            0\0");


    /*
    char wep_rc[3][14];
    char wep_pc[3][14];
    char wep_sc[3][14];
    */
    for (int j = 0; j < 20; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            wep_rc[i][j] = ' ';
            wep_pc[i][j] = ' ';
            wep_sc[i][j] = ' ';
        }
    }
    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][19] = '\0';
        wep_pc[j][19] = '\0';
        wep_sc[j][19] = '\0';
    }

    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][18] = '0';
        wep_pc[j][18] = '0';
        wep_sc[j][18] = '0';
    }

    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][0] = 'R';
        wep_pc[j][0] = 'D';
        wep_sc[j][0] = 'S';

    }

    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][1] = 'a';
        wep_pc[j][1] = 'm';
        wep_sc[j][1] = 'h';
    }

    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][2] = 'n';
        wep_pc[j][2] = 'a';
        wep_sc[j][2] = 'o';
    }

    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][3] = 'g';
        wep_pc[j][3] = 'g';
        wep_sc[j][3] = 't';
    }
    for (int j = 0; j < 3; j++)
    {
        wep_rc[j][4] = 'e';
        wep_pc[j][4] = 'e';
        wep_sc[j][4] = 's';
    }










}

bool SHIPSTATS::calcdeck(int dnum)
{
    deckweight[dnum] = 0;
    equipweights[dnum] = 0;
    numgravunits[dnum] = 0;
    numpower[dnum] = 0;
    numshieldunits[dnum] = 0;
    numsensors[dnum] = 0;
    for (int j = 0; j < 3; j++)
        numwep[j][dnum] = 0;
    //this.
    double area = 0;
    double thisarea = 0;
    long int fueladd = 0;
    for (int t = 0; t<22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            thisarea = grabarea(deckrooms[t][r][dnum]);
            if (deckrooms[t][r][dnum])
            {
                if (deckrooms[t][r][dnum]->roomtype == FUEL)
                    fueladd += 127*(thisarea/50);
            }
            area += thisarea;
        }
    }
    deckweight[dnum] = area;
    fuelweights[dnum] = fueladd;
    long int eqwt = 0;
    for (int i = 0; i < 161; i++)
    {
        for (int j = 0; j < 161; j++)
        {
            eqwt+=grabequip(equipgrid[i][j][dnum]);
            if (equipgrid[i][j][dnum])
            {
                if (equipgrid[i][j][dnum]->type == MAN_U)
                {
                    numgravunits[dnum]++;
                }
                if (equipgrid[i][j][dnum]->type == SHI_U)
                {
                    numshieldunits[dnum]++;
                }
                if (equipgrid[i][j][dnum]->type == WEA_U)
                {
                    if (equipgrid[i][j][dnum]->loc_roomid < 3)
                        numwep[equipgrid[i][j][dnum]->loc_roomid][dnum]++;
                }
                if (equipgrid[i][j][dnum]->type == POW_U)
                {
                    numpower[dnum]++;
                }
                if (equipgrid[i][j][dnum]->type == SEN_U)
                {
                    numsensors[dnum]++;
                }
            }
        }
    }
    deckweight[dnum] *= .9981;
    equipweights[dnum] += eqwt;
    //
    //if (streamlined)
    //    deckweight[dnum] *= .09;

    return true;
}

bool SHIPSTATS::calctotal()
{
    //structure hp: 10 per equipment, 10 per full tile area (2.5 each) actually seems to roll high, 10 = 110
    //techlevel effects it, perhaps TL1 is too?
    //fuel cell, hold, vehicle bay, hull.  minimum 10 incrm? maybe i should just fake it...
    //all the random equipment, consoles = command and control and shield = those too.
    //armor increases structure hp only.
    //seems to be an odd formula for armor. compare notes, find a best fit line, and get it close enough for snuff.
    /*
    long unsigned int totalVHP;
    long unsigned int totalSHP;
    long unsigned int totalFHP;
    long unsigned int totalHHP;
    */
    totalweight = 0;
    totalgravunits = 0;
    totalshieldunits = 0;
    totalpower = 0;
    totalsensors = 0;

    for (int i = 0; i < 3; i++)
    {
        totalwep[i] = 0;
    }
    for (int i = 0; i < 57; i++)
    {
        totalweight += deckweight[i];
        totalgravunits += numgravunits[i];
        totalshieldunits += numshieldunits[i];
        totalpower += numpower[i];
        totalsensors += numsensors[i];
        for (int j = 0; j < 3; j++)
        {
            totalwep[j] += numwep[j][i];
        }
    }
    totalweight = totalweight*(armorlevel+1)/2;
    if (!streamlined)
        totalweight *= .909;
    for (int d = 0; d < 57; d++)
    {
        totalweight += fuelweights[d];
        totalweight += equipweights[d];
    }
    float pmult;
    techlevel = totalweight / 350000 + 1;
    if (techlevel < 2 &&  (totalwep[0] >=1 || totalwep[1] >= 1 || totalwep[2] >=1))
        techlevel = 2;
    if (techlevel <4 && totalshieldunits >= 1)
        techlevel = 4;
    if (totalweight != 0)
    {
        pmult = totalgravunits*2250/(float)totalweight;
        m_s_s = pmult*(475+25*techlevel);
    }
    else
        m_s_s = 0;

    //capacitor
    cap_power = totalpower*(1+(techlevel-1)*.05);
    long unsigned int capparse = cap_power;
    for (int i = 18; i >= 6; i--)
    {
        if (i != 15 && i != 11 && i != 7)
        {
            powerc[i] = capparse %10 +48;
            capparse /=10;

        }
        else
        {
            powerc[i] = ',';
        }
    }

    //shielding: total shields
    shieldpower = totalshieldunits*10*(1+(techlevel-1)*.05);
//exit(7);
    //weapons: 1 thru 3
    //wep_range[3]; 316.5*pow(x, .05)*(1+TL-1*.05)
    //wep_power[3]; long int: 10 per unit + tl bonus
    //wep_shots[3];
    for (int i = 0; i < 3; i++)
    {
        wep_range[i] = 316.5*pow(totalwep[i], .5)*(1+(techlevel-1)*.05);
        wep_power[i] = 10*totalwep[i]*(1+(techlevel-1)*.05);
        if (totalwep[i] != 0)
            wep_shots[i] = ((float)cap_power)/(float)totalwep[i];
        else
            wep_shots[i] = 0;
    }

    long int wep_rsets[3] = {wep_range[0], wep_range[1], wep_range[2]};

    long int wep_ssets[3] = {wep_shots[0]*10, wep_shots[1]*10, wep_shots[2]*10};
  //note that : shots 1 cap = 1 wep unit shot therefore shots = x/cap
  // cap = 1 per pp * tech bonus
    for (int i = 0; i < 3; i++)
    {
        for (int j = 18; j >= 6; j--)
        {
            if (j != 17 && j != 13 && j != 9)
            {
                wep_sc[i][j] = wep_ssets[i] % 10 + 48;
                wep_ssets[i] /= 10;
            }
            else
            {
                if (j == 17)
                    wep_sc[i][j] = '.';
                else
                    wep_sc[i][j] = ',';
            }
        }
    }


    for (int i = 0; i < 3; i++)
    {
        for (int j = 18; j >= 6; j--)
        {
            if (j != 15 && j!= 11 && j!= 7)
            {
                wep_rc[i][j] = wep_rsets[i] %10 +48;
                wep_rsets[i] /= 10;
            }
            else
            {
                wep_rc[i][j] = ',';
            }

        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int c = 6; c < 18; c++)
        {
            if (wep_rc[i][c] == '0')
                wep_rc[i][c] -= 16;
            else
            if (wep_rc[i][c] == 32)
            ;
            else
            if (wep_rc[i][c] == ',')
                wep_rc[i][c] = 32;
            else
                break;

        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int c = 6; c < 16; c++)
        {
            if (wep_sc[i][c] == '0')
                wep_sc[i][c] -= 16;
            else
            if (wep_sc[i][c] == 32)
            ;
            else
            if (wep_sc[i][c] == ',')
                wep_sc[i][c] = 32;
            else
                break;

        }
    }

    for (int c = 6; c < 18; c++)
    {
        if (powerc[c] == '0')
            powerc[c] -= 16;
        else
        if (powerc[c] == 32)
        ;
        else
        if (powerc[c] == ',')
            powerc[c] = 32;
        else
            break;

    }
    long int wep_psets[3] = {wep_power[0], wep_power[1], wep_power[2]};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 18; j >= 6; j--)
        {
            if (j != 15 && j!= 11 && j!= 7)
            {
                wep_pc[i][j] = wep_psets[i] %10 +48;
                wep_psets[i] /= 10;
            }
            else
            {
                wep_pc[i][j] = ',';
            }

        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int c = 6; c < 18; c++)
        {
            if (wep_pc[i][c] == '0')
                wep_pc[i][c] -= 16;
            else
            if (wep_pc[i][c] == 32)
            ;
            else
            if (wep_pc[i][c] == ',')
                wep_pc[i][c] = 32;
            else
                break;

        }
    }


    //sensors: 6477 each, linear, normal tech bonus.
    sensorrange = totalsensors*6477*(1+(techlevel-1)*.05);
    long int sensorparse = sensorrange;
    for (int j = 18; j > 7; j--)
    {
        if (j != 15 && j != 11)
        {
            sensorc[j] = sensorparse %10 + 48;
            sensorparse /= 10;
        }
        else
        {
            sensorc[j] = ',';
        }

    }
    for (int c = 7; c < 18; c++)
    {
        if (sensorc[c] == '0')
            sensorc[c] -= 16;
        else
        if (sensorc[c] == 32)
        ;
        else
        if (sensorc[c] == ',')
            sensorc[c] = 32;
        else
            break;
    }
    //now, set the charstring;
    //acceleration
    int t_accel = m_s_s;
    for (int j = 4; j >=0; j--)
    {
        if (j != 1)
        {
            accel[j] = t_accel %10 + 48;
            t_accel /= 10;
        }
        else
        {
            accel[j] = ',';
        }
    }
    for (int c = 0; c < 4; c++)
    {
        if (accel[c] == '0')
            accel[c] -= 16;
        else
        if (accel[c] == 32)
        ;
        else
        if (accel[c] == ',')
            accel[c] = 32;
        else
            break;
    }
    //shielding char slevel[22];  start at 8
    unsigned int sparse = shieldpower;
    for (int j = 17; j > 4; j--)
    {
        if (j != 14 && j!= 10 && j!= 6)
        {
            slevel[j] = (sparse%10)+48;
            sparse/=10;

        }
        else
        {
            slevel[j] = ',';
        }
    }

    //weights
    long unsigned int setchar = totalweight;
    for (int j = 12; j >=0; j--)
    {
        if (j != 9 && j != 5 && j!= 1)
        {
            shipweight[j] = (setchar%10)+48;
            setchar /= 10;
        }
        else
        {
            shipweight[j] = ',';
        }

    }
    for (int c = 0; c < 12; c++)
    {
        if (shipweight[c] == '0')
            shipweight[c] -= 16;
        else
        if (shipweight[c] == 32)
        ;
        else
        if (shipweight[c] == ',')
            shipweight[c] = 32;
        else
            break;
    }
    for (int c = 5; c < 17; c++)
    {
        if (slevel[c] == '0')
            slevel[c] -= 16;
        else
        if (slevel[c] == 32)
        ;
        else
        if (slevel[c] == ',')
            slevel[c] = 32;
        else
            break;

    }
    //AL 00_
    alevel[3] = armorlevel/10 + 48;
    alevel[4] = armorlevel%10 + 48;
    if (alevel[3] == '0')
        alevel[3] -= 16;
    tlevel[3] = techlevel/10 +48;
    tlevel[4] = techlevel%10 +48;
    if (tlevel[3] == '0')
        tlevel[3] -= 16;

    return true;
}

bool SHIPSTATS::pstats()
{
    //this may get complex later:
    //print AL, TL, Strl, Weight
    //Print Weapons, Shields, power
    //scanning, cargo, speed
    //crew, stations
    //setting 1
    //{STAT_DISP_MISC, STAT_DISP_COMBAT, STAT_DISP_SC, STAT_DISP_ENG, STAT_DISP_COST};
    if(statdisplay == STAT_DISP_MISC)
    {
        glPrint(800, 416, shipweight, 0, 1);
        glPrint(800, 432, alevel, 0, 1);
        if (streamlined)
        {
            char sl[] = "Streamlined";
            glPrint(800, 448, sl, 0, 1);
        }
        else
        {
            char sl[] = "Not Streamlined";
            glPrint(800, 448, sl, 0, 1);
        }
        glPrint(800, 464, tlevel, 0, 1);
        glPrint(800, 484, accel, 0, 1);

    }
    if(statdisplay == STAT_DISP_COMBAT)
    {
        //print weapons (how many?)4?, shields
        glPrint(800, 410, wep_rc[0], 0, 1);
        glPrint(800, 422, wep_pc[0], 0, 1);
        glPrint(800, 434, wep_sc[0], 0, 1);
        glPrint(800, 446, wep_rc[1], 0, 1);
        glPrint(800, 458, wep_pc[1], 0, 1);
        glPrint(800, 470, wep_sc[1], 0, 1);
        glPrint(800, 482, wep_rc[2], 0, 1);
        glPrint(800, 494, wep_pc[2], 0, 1);
        glPrint(800, 506, wep_sc[2], 0, 1);


    }
    if(statdisplay == STAT_DISP_SC)
    {
        glPrint(800, 504, slevel, 0, 1);
        glPrint(800, 410, powerc, 0, 1);
        glPrint(800, 430, sensorc, 0, 1);

        // wormhole power, sensors, power, shields?
    }
    if(statdisplay == STAT_DISP_ENG)
    {

    }
    if(statdisplay == STAT_DISP_COST)
    {

    }


    return true;
}

double SHIPSTATS::grabarea(ROOM *droom)
{
    if (!droom)
        return 0;
    if (!droom->lverts)
        return 0;

    VLINK *temp = droom->lverts;

    int i,j;
    double area = 0;
    V_POINT points[droom->vnum];  //# of verts to points
    for (int n = 0; n < droom->vnum; n++) // assign to the point structure
    {
        points[n].x = temp->cx*5;
        points[n].y = (800 - temp->cy*5);
        temp = temp->next;
    }
    for (i = 0; i <droom->vnum; i++)
    {
        j = (i + 1) % droom->vnum;
        area += points[i].x * points[j].y;
        area -= points[i].y * points[j].x;
    }

    if (area >0) // its clockwise, remember, y is flipped
        return area;

    return 0;

}

long unsigned int SHIPSTATS::grabequip(EQUIPUNIT *dunit)
{
    if (!dunit)
        return 0;
    int t = dunit->type;
    if (t == CAP_C)  //consoles
        return 40;
    if (t == HEL_C || t == ENG_C || t == SHI_C || t == SEN_C || t == NAV_C || t == WEA_C || t == POW_C || t == MED_C || t == TRA_C)
        return 80;
    // turrets
    if (t == T_SIDE_R || t == T_SIDE_UR || t == T_SIDE_U || t == T_SIDE_UL || t== T_SIDE_L || t == T_SIDE_DL || t == T_SIDE_D || t == T_SIDE_DR || t == T_TOP || t == T_BOT)
        return 1000;
    // equipment
    if (t == MAN_U)
        return 2250;
    if (t == POW_U)
        return 800;
    if (t == SHI_U)
        return 1350;
    if (t == SEN_U)
        return 450;
    if (t == FTL_U)
        return 700;
    if (t == WEA_U)
        return 1800;
    if (t == LIF_U)
        return 250;
    if (t == MED_U)
        return 200;
    if (t == TRA_U)
        return 20;
    if (t >= BED_U_R && t <= BED_U_DR)
        return 70;
    if (t == BED2_U_R ||
        (t >= BED2_U_UR && t <= BED2_U_DR))
        return 120;
    if (t == DOOR_H)
        return 30;
    if (t == DOOR_C)
        return 20;
    if (t == DOOR_P)
        return 10;
    if (t == DOORL_H)
        return 120;
    if (t == DOORL_C)
        return 90;
    if (t == DOORL_P)
        return 70;
    if (t == HATCH_H ||
        (t >= HATCH_H_UR && t <= HATCH_H_DR))
        return 60;
    if (t == HATCH_C ||
        (t >= HATCH_C_UR && t <= HATCH_C_DR))
        return 50;
    if (t == HATCH_P ||
        (t >= HATCH_P_UR && t <= HATCH_P_DR))
        return 40;

    //gametable, vehicles, etc




    /*

    enum {HATCH_P_UR = 0x4D, HATCH_P_U, HATCH_P_UL, HATCH_P_L, HATCH_P_DL, HATCH_P_D, HATCH_P_DR, //0x4D to 0x53
      HATCH_C_UR, HATCH_C_U, HATCH_C_UL, HATCH_C_L, HATCH_C_DL, HATCH_C_D, HATCH_C_DR, //0x54 to 0x5A
      HATCH_H_UR, HATCH_H_U, HATCH_H_UL, HATCH_H_L, HATCH_H_DL, HATCH_H_D, HATCH_H_DR}; //0x5B to 0x61
    enum  {MAN_U = 0x0A, POW_U = 0x0E, SHI_U = 0x12, SEN_U = 0x10, FTL_U = 0x18, WEA_U = 0x17, LIF_U = 0x20, MED_U = 0x0B,
       TRA_U = 0x14, BED_U = 0x41, BED2_U = 0x02, DOOR_H = 0x32, DOOR_C = 0x31, DOOR_P = 0x05, DOORL_H = 0x03, DOORL_C = 0x36,
       DOORL_P = 0x35, HATCH_H = 0x34, HATCH_C = 0x33, HATCH_P = 0x08, OPENING_I = 0x37, OPENING_E = 0x09, RAIL = 0x38,
       PIVOT = 0x4C, EMIT_C = 0x62, EMIT_W = 0x63, TABLE_G = 0x00, V_A_T = 0x00, V_A_F1 = 0x00, V_A_F2 = 0x00, V_G_M = 0x00,
       V_G_S = 0x00, V_G_A = 0x00, GUN_T_CAN = 0x16, GUN_S_CAN = 0x15, CAP_C = 0x49, HEL_C = 0x0C, ENG_C = 0x06, SHI_C = 0x11,
       SEN_C = 0x0F, NAV_C = 0x01, WEA_C = 0x07, POW_C = 0x0D, MED_C = 0x04, TRA_C = 0x13, DUMMY = 0xFF };
    enum {T_SIDE_R = 0x15, T_SIDE_UR = 0x22, T_SIDE_U, T_SIDE_UL, T_SIDE_L, T_SIDE_DL, T_SIDE_D, T_SIDE_DR};
enum {T_TOP = 0x16, T_BOT = 0x21};

enum {BED_U_R = 0x41, BED_U_UR, BED_U_U, BED_U_UL, BED_U_L, BED_U_DL, BED_U_D, BED_U_DR};
enum {BED2_U_R = 0x02, BED2_U_UR = 0x19, BED2_U_U, BED2_U_UL, BED2_U_L, BED2_U_DL, BED2_U_D, BED2_U_DR};

    */

    return 0;
}


void DrawOptions()
{
    //glPushMatrix();
    //glScalef(20, 0, 0);
    char op[] = "OPTIONS";
    char q[] = "QUIT";
    char oset[] = "DRAWMODE:";
    char dmode[16];
    dmode[15] = '\0';
    if (drawdeckmode == DRAW_CDECK)
    {
        dmode[0] = 'C';
        dmode[1] = 'U';
        dmode[2] = 'R';
        dmode[3] = 'R';
        dmode[4] = 'E';
        dmode[5] = 'N';
        dmode[6] = 'T';
        dmode[7] = '\0';
    }
    if (drawdeckmode == DRAW_CnABOVE)
    {
        dmode[0] = 'C';
        dmode[1] = 'U';
        dmode[2] = 'R';
        dmode[3] = 'R';
        dmode[4] = 'E';
        dmode[5] = 'N';
        dmode[6] = 'T';
        dmode[7] = '+';
        dmode[8] = 'A';
        dmode[9] = 'B';
        dmode[10] = 'O';
        dmode[11] = 'V';
        dmode[12] = 'E';
        dmode[13] = '\0';
    }
    if (drawdeckmode == DRAW_CnBELOW)
    {
        dmode[0] = 'C';
        dmode[1] = 'U';
        dmode[2] = 'R';
        dmode[3] = 'R';
        dmode[4] = 'E';
        dmode[5] = 'N';
        dmode[6] = 'T';
        dmode[7] = '+';
        dmode[8] = 'B';
        dmode[9] = 'E';
        dmode[10] = 'L';
        dmode[11] = 'O';
        dmode[12] = 'W';
        dmode[13] = '\0';
    }
    if (drawdeckmode == DRAW_CnABOVEnBELOW)
    {
        dmode[0] = 'C';
        dmode[1] = 'U';
        dmode[2] = 'R';
        dmode[3] = 'R';
        dmode[4] = '+';
        dmode[5] = 'A';
        dmode[6] = '+';
        dmode[7] = 'B';
        dmode[8] = '\0';
    }
    if (drawdeckmode == DRAW_ALLDECKS)
    {
        dmode[0] = 'A';
        dmode[1] = 'L';
        dmode[2] = 'L';
        dmode[3] = '\0';
    }

    //enum {DRAW_CDECK, DRAW_CnABOVE, DRAW_CnBELOW, DRAW_CnABOVEnBELOW, DRAW_ALLDECKS};
    glPrint(400, 200, op, 0, 2);
    glPrint(400, 600, q, 0, 2);
    glPrint(585, 400, dmode, 0, 2);
    glPrint(400, 400, oset, 0, 2);
    //drawmousepos(10, 10);
    DrawCursor(mousemain.xpos, mousemain.ypos);
    glFlush();
    SDL_GL_SwapBuffers();

}

void GrabOptions(bool &quit)
{
    if (mousemain.xpos >= 400 && mousemain.xpos <=500 &&
        mousemain.ypos >= 600 && mousemain.ypos <=640)
        quit = !quit;
    if (mousemain.xpos >= 400 && mousemain.xpos <=740 &&
        mousemain.ypos >= 400 && mousemain.ypos <=430)
    {
        drawdeckmode++;
        if (drawdeckmode > DRAW_ALLDECKS)
            drawdeckmode = DRAW_CDECK;


    }

    //other options: draw above/below decks?

}

void DrawParseDecks()
{
    //enum {DRAW_CDECK, DRAW_CnABOVE, DRAW_CnBELOW, DRAW_CnABOVEnBELOW, DRAW_ALLDECKS};

    if (drawdeckmode == DRAW_CDECK)
    {
        glLineWidth(4);
        DrawDeck2(currentdeck, .7); // replace other stuff
        return;
    }
    if (drawdeckmode == DRAW_CnABOVE)
    {
        glLineWidth(4);
        DrawDeck2(currentdeck, .7);
        glLineWidth(20);
        DrawDeck2(currentdeck+1, .2);
        return;

    }
    if (drawdeckmode == DRAW_CnBELOW)
    {
        glTranslatef(0, 0, -10);
        glLineWidth(2);
        DrawDeck2(currentdeck-1, .15);
        glTranslatef(0, 0, +10);
        glLineWidth(4);
        DrawDeck2(currentdeck, .7);
        return;
    }
    if (drawdeckmode == DRAW_CnABOVEnBELOW)
    {
        glLineWidth(2);
        DrawDeck2(currentdeck-1, .15);
        glLineWidth(4);
        DrawDeck2(currentdeck, .7);
        glLineWidth(20);
        DrawDeck2(currentdeck+1, .2);
        return;
    }
    if (drawdeckmode == DRAW_ALLDECKS)
    {
        glLineWidth(2);
        for (int i = 0; i < currentdeck; i++)
        {
            DrawDeck2(i, .15);
        }
        glLineWidth(4);
        DrawDeck2(currentdeck, .4);
        glLineWidth(20);
        for (int i = currentdeck+1; i < 56; i++)
        {
            DrawDeck2(i, .2);
        }
        return;

    }
    glLineWidth(2);
    DrawDeck2(currentdeck-1, .15);
    glLineWidth(4);
    DrawDeck2(currentdeck, .4);
    glLineWidth(20);
    DrawDeck2(currentdeck+1, .2);

}

bool copytopart(PART *somepart)
{
    somepart->clearpart();
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            for (int d = 0; d < 57; d++)
            {
                somepart->addroom(deckrooms[t][r][d]);
            }
        }
    }
    return true;
}

bool copydecktopart(PART *somepart, int cdeck)
{
    if (cdeck <0 ||
        cdeck > 57)
        return false;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            somepart->addroom(deckrooms[t][r][cdeck]);
        }
    }
    return true;

}



bool rotatedesign()
{
    currentdeck = 28;
    copytopart(copypart);
    cleardesign();
    copypart->rotate(ROT_90);
    copypart->c_deck = 28;
    copypart->insetatcursor(400, 400, 28);
    copypart->clearpart();
    return true;
}

bool flipdesign(int flip)
{
    currentdeck = 28;
    copytopart(copypart);
    cleardesign();
    copypart->flip(flip);
    copypart->c_deck = 28;
    copypart->insetatcursor(400, 400, 28);
    copypart->clearpart();
    return true;
}

bool clearroom(ROOM *dr)
{
    int vnum = dr->vnum;
    VLINK *nodes[vnum];
    VLINK *temp = dr->lverts;
    for (int i = 0; i < vnum; i++)
    {
        nodes[i] = temp;
        temp = temp->next;
        delete nodes[i];
    }
    dr->lverts = NULL;
    return true;
}

bool cleardeck(int cdeck)
{
    if (cdeck < 0 ||
        cdeck > 57)
        return false;
   for (int t = 0; t < 22; t++)
   {
       for (int r = 0; r < 256; r++)
       {
           if (deckrooms[t][r][cdeck])
           {
               for (int x = 0; x < 161; x++)
               {
                   for (int y = 0; y < 161; y++)
                   {
                       if (equipgrid[x][y][cdeck])
                       {
                           if (equipgrid[x][y][cdeck]->roomid == deckrooms[t][r][cdeck]->globalnum)
                           {
                               delete equipgrid[x][y][cdeck];
                               equipgrid[x][y][cdeck] = NULL;
                           }
                       }
                   }
               }
               rooms_g[deckrooms[t][r][cdeck]->globalnum-1] = false;
               clearroom(deckrooms[t][r][cdeck]);
               deckrooms[t][r][cdeck] = NULL;
           }
       }
   }
   return true;
}

int HK_SDL_DECKUP, HK_SDL_DECKDOWN, HK_SDL_OPTIONS, HK_SDL_BRUSHDOWN, HK_SDL_BRUSHUP, HK_SDL_ROTRIGHT, HK_SDL_ROTLEFT,
    HK_SDL_TTYPELEFT, HK_SDL_TTYPERIGHT, HK_SDL_FINISH, HK_SDL_VDEL, HK_SDL_RDEL, HK_SDL_REFILL,
    HK_SDL_SCROLLL, HK_SDL_SCROLLR, HK_SDL_SCROLLU, HK_SDL_SCROLLD, HK_SDL_ZOOMIN, HK_SDL_ZOOMOUT,
    HK_SDL_COPYDECK, HK_SDL_MOVEROOM;

bool loadconfigdefaults()
{
    HK_SDL_DECKUP = SDLK_KP_PLUS;
    HK_SDL_DECKDOWN = SDLK_KP_MINUS;
    HK_SDL_OPTIONS = SDLK_ESCAPE;
    HK_SDL_BRUSHDOWN = SDLK_LSHIFT;
    HK_SDL_BRUSHUP = SDLK_RSHIFT;
    HK_SDL_ROTRIGHT = SDLK_RALT;
    HK_SDL_ROTLEFT = SDLK_LALT;
    HK_SDL_TTYPELEFT = SDLK_LCTRL;
    HK_SDL_TTYPERIGHT = SDLK_RCTRL;
    HK_SDL_FINISH = SDLK_SPACE;
    HK_SDL_VDEL = SDLK_BACKSPACE;
    HK_SDL_RDEL = SDLK_DELETE;
    HK_SDL_REFILL = SDLK_f;
    HK_SDL_SCROLLL = SDLK_LEFT;
    HK_SDL_SCROLLR = SDLK_RIGHT;
    HK_SDL_SCROLLU = SDLK_UP;
    HK_SDL_SCROLLD = SDLK_DOWN;
    HK_SDL_ZOOMIN = SDLK_PAGEUP;
    HK_SDL_ZOOMOUT = SDLK_PAGEDOWN;
    HK_SDL_COPYDECK = SDLK_c;
    HK_SDL_MOVEROOM = SDLK_m;
    return true;
}

void readstr(FILE *f, char *string)
{
    do {
	fgets(string, 255, f); // read the line
    } while ((string[0] == '/') || (string[0] == '\n') || (string[0] == '#'));
    return;
}

bool loadconfigfile()
{
    loadconfigdefaults();
    FILE *configload = NULL;
    configload = fopen("Config.txt", "rt");
    if (!configload)
    {
        fprintf(stderr,"Cannot find Config.txt\n");
        return false;
    }
    long fsize = 0;
    {
        fseek(configload, 0, SEEK_END);
        fsize = ftell(configload);
        if (fsize < 100)
        {
            fclose(configload);
            return false; // not a valid ship file:: too small
        }
        fseek(configload, 0, SEEK_SET);
    }

    char intest = ' ';
    while (intest != '\n') //skip the first
    {
        intest = fgetc(configload);
       // fprintf(stderr, "%c", intest);
        if (feof(configload))
            break;
    }


    char configc[255];
    readstr(configload, configc);
    sscanf(configc, "OPTIONS: %i\n", &HK_SDL_OPTIONS);
    readstr(configload, configc);
    sscanf(configc, "DECKUP: %i\n", &HK_SDL_DECKUP);
    readstr(configload, configc);
    sscanf(configc, "DECKDOWN: %i\n", &HK_SDL_DECKDOWN);
    readstr(configload, configc);
    sscanf(configc, "BRUSHDOWN: %i\n", &HK_SDL_BRUSHDOWN);
    readstr(configload, configc);
    sscanf(configc, "BRUSHUP: %i\n", &HK_SDL_BRUSHUP);
    readstr(configload, configc);
    sscanf(configc, "ROTLEFT: %i\n", &HK_SDL_ROTLEFT);
    readstr(configload, configc);
    sscanf(configc, "ROTRIGHT: %i\n", &HK_SDL_ROTRIGHT);
    readstr(configload, configc);
    sscanf(configc, "TURRETTYPELEFT: %i\n", &HK_SDL_TTYPELEFT);
    readstr(configload, configc);
    sscanf(configc, "TURRETTYPERIGHT: %i\n", &HK_SDL_TTYPERIGHT);
    readstr(configload, configc);
    sscanf(configc, "FINISH: %i\n", &HK_SDL_FINISH);
    readstr(configload, configc);
    sscanf(configc, "VERTEXDEL: %i\n", &HK_SDL_VDEL);
    readstr(configload, configc);
    sscanf(configc, "ROOMDEL: %i\n", &HK_SDL_RDEL);
    readstr(configload, configc);
    sscanf(configc, "FILLROOM: %i\n", &HK_SDL_REFILL);
    readstr(configload, configc);
    sscanf(configc, "SCROLLLEFT: %i\n", &HK_SDL_SCROLLL);
    readstr(configload, configc);
    sscanf(configc, "SCROLLRIGHT: %i\n", &HK_SDL_SCROLLR);
    readstr(configload, configc);
    sscanf(configc, "SCROLLUP: %i\n", &HK_SDL_SCROLLU);
    readstr(configload, configc);
    sscanf(configc, "SCROLLDOWN: %i\n", &HK_SDL_SCROLLD);
    readstr(configload, configc);
    sscanf(configc, "ZOOMIN: %i\n", &HK_SDL_ZOOMIN);
    readstr(configload, configc);
    sscanf(configc, "ZOOMOUT: %i\n", &HK_SDL_ZOOMOUT);








    fclose(configload);
    return true;

}

bool handlekeyevent(SDL_Event &event)
{
    if ( event.type == SDL_QUIT )
    {
        return false;
    }
    if ( event.type == SDL_KEYDOWN )
    {
        if (event.key.keysym.sym == HK_SDL_OPTIONS )
        {
            if (uimode != M_OPTIONS)
                uimode = M_OPTIONS;
            else
                uimode = X_NONE;
        }
        if (event.key.keysym.sym == HK_SDL_BRUSHDOWN)
        {
            if (brush > 0)
                brush--;
        }
        if (event.key.keysym.sym == HK_SDL_BRUSHUP)
        {
            if (brush < 10)
                brush++;
        }
        if (event.key.keysym.sym == HK_SDL_ROTLEFT)
        {
            if (currentrot < 7)
                currentrot++;
            else
                currentrot = 0;
        }
        if (event.key.keysym.sym == HK_SDL_ROTRIGHT)
        {
            if (currentrot > 0)
                currentrot--;
            else
                currentrot = 7;

        }
        if (event.key.keysym.sym == HK_SDL_TTYPERIGHT)
        {
            if (currentmod == T_CANNON)
                currentmod = T_PULSE;
            else
            if (currentmod == T_MISSILE)
                currentmod = T_CANNON;
            else
                currentmod++;
        }
        if (event.key.keysym.sym == HK_SDL_TTYPELEFT)
        {
            if (currentmod == T_CANNON)
                currentmod = T_MISSILE;
            else
            if (currentmod == T_PULSE)
                currentmod = T_CANNON;
            else
                currentmod--;
        }
        if (event.key.keysym.sym == HK_SDL_FINISH)
        {

            // if drawing a room, finish it (current room)
            if (a_room)
            {
                int grabdeck = a_room->decknum;


                if(!finishroom2(a_room))
                {
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            for (int d = 0; d < 57; d++)
                            {
                                if (deckrooms[t][r][d] == a_room)
                                {
                                    deckrooms[t][r][d] = NULL;
                                }
                            }
                        }
                    }
                    delete a_room;
                }

                mainship.calcdeck(grabdeck);
                mainship.calctotal();

                a_room = NULL;
                //uimode = X_NONE;
                //roomtype = 0;
               // currentroom = 0;
            }
            if (uimode == M_PART_PLACE)
                uimode = X_NONE;
        }
       // if (event.key.keysym.sym == SDLK_HOME)
       //     angle++;
        //{
        //    if (a_room)
        //        testpart.addroom(a_room);
        //}
        //if (event.key.keysym.sym == SDLK_END)
        //{

        //    testpart.insetatcursor(mousemain.axpos, mousemain.aypos, currentdeck);
        //}
        if (event.key.keysym.sym == HK_SDL_VDEL)
        {
            if (a_room)
            {
                int gid = a_room->globalnum;
                if (DVLINK_L(a_room))
                {
                    rooms_g[gid-1] = false;
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            if (deckrooms[t][r][currentdeck]==a_room)
                            {
                                deckrooms[t][r][currentdeck] = NULL;
                            }
                        }
                    }

                    a_room = NULL;
                }
            }
        }
        if (event.key.keysym.sym == HK_SDL_RDEL)
        {

            if (a_room)
            {
                int gid = a_room->globalnum;
                for (int x = 0; x < 161; x++)
                {
                    for (int y = 0; y < 161; y++)
                    {
                        if(equipgrid[x][y][currentdeck])
                            if (equipgrid[x][y][currentdeck]->roomid == a_room->globalnum)
                            {
                                delete equipgrid[x][y][currentdeck];
                                equipgrid[x][y][currentdeck] = NULL;
                            }
                    }
                }
                clearroom(a_room);
                rooms_g[gid-1] = false;
                for (int t = 0; t < 22; t++)
                {
                    for (int r = 0; r < 256; r++)
                    {
                        if (deckrooms[t][r][currentdeck] == a_room)
                        {
                            deckrooms[t][r][currentdeck] = NULL;
                        }
                    }
                }
                a_room = NULL;
                mainship.calcdeck(currentdeck);
                mainship.calctotal();
            }
        }
        if (event.key.keysym.sym == HK_SDL_REFILL)
        {
            if (uimode == P_EQUIP)
            {
                //pick a room then fill it with equipment
                //ROOM *rfill = NULL;
                fillroom(mousemain.axpos, mousemain.aypos);
                mainship.calcdeck(currentdeck);
                mainship.calctotal();
            }

        }
        //if (event.key.keysym.sym == SDLK_p)
        //    dopolyfill = !dopolyfill;
        //if (event.key.keysym.sym == SDLK_c)
        //    cleardesign();
        //if (event.key.keysym.sym == SDLK_l)
        //    loaddesign();
        if (event.key.keysym.sym == HK_SDL_DECKUP)
        {
            if (currentdeck <56)
                currentdeck ++;

        }
        if (event.key.keysym.sym == HK_SDL_DECKDOWN)
        {
            if (currentdeck >0)
            currentdeck --;
        }
        if (event.key.keysym.sym == HK_SDL_SCROLLL)
        {
            SCROLLX += ssf;
        }
        if (event.key.keysym.sym == HK_SDL_SCROLLR)
        {
            SCROLLX -= ssf;
        }
        if (event.key.keysym.sym == HK_SDL_SCROLLU)
        {
            SCROLLY += ssf;
        }
        if (event.key.keysym.sym == HK_SDL_SCROLLD)
        {
            SCROLLY -= ssf;
        }
        if (event.key.keysym.sym == HK_SDL_ZOOMIN)
        {
            zoomin();
        }
        if (event.key.keysym.sym == HK_SDL_ZOOMOUT)
        {
            zoomout();
        }
        if (event.key.keysym.sym == HK_SDL_COPYDECK)
        {
            copydecktopart(activepart, currentdeck);
        }
        if (event.key.keysym.sym == HK_SDL_MOVEROOM)
        {
            if (!a_room)
            {
                for(int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            if (pointInPolygon2(deckrooms[t][r][currentdeck], mousemain.axpos, mousemain.aypos))
                            {
                                //finishroom2(a_room);
                                a_room = deckrooms[t][r][currentdeck];
                                //uimode = D_ROOM;
                            }
                        }
                    }
            }
            if (a_room)
            {
                if (finishroom2(a_room))
                    moveroomstart(a_room, currentdeck);
                else
                {
                    for (int t = 0; t < 22; t++)
                    {
                        for (int r = 0; r < 256; r++)
                        {
                            for (int d = 0; d < 57; d++)
                            {
                                if (deckrooms[t][r][d] == a_room)
                                    deckrooms[t][r][d] = NULL;
                            }
                        }
                    }
                    delete a_room;
                }
            }
            a_room = NULL;

        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        MouseUpdate(event.button.x, event.button.y);
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mousemain.lkeydown = true;
            mousemain.lkeypress = true;
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            mousemain.rkeydown = true;
            mousemain.rkeypress = true;
            mousedrag.Drag_On = true;
        }
        if (event.button.button == SDL_BUTTON_WHEELUP)
        {
            zoomin();
        }
        if (event.button.button == SDL_BUTTON_WHEELDOWN)
        {
            zoomout();
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mousemain.lkeydown = false;
            mousemain.lkeypress = false;
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            mousemain.rkeydown = false;
            mousemain.rkeypress = false;
            mousedrag.Drag_On = false;

        }
    }
    if (event.type == SDL_MOUSEMOTION)
    {
        MouseUpdate(event.button.x, event.button.y);

    }
    return true;
}


bool moveroomstart(ROOM *droom, int cdeck)
{
    if (!droom)
        return false;
    //use copypart
    copypart->addroom(droom);
    //mode set to move room
    uimode = X_MOVEROOM_START;
    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            if (equipgrid[x][y][cdeck])
                if (equipgrid[x][y][cdeck]->roomid == droom->globalnum)
                {
                    delete equipgrid[x][y][cdeck];
                    equipgrid[x][y][cdeck] = NULL;
                }

        }
    }
    int gid = droom->globalnum;
    rooms_g[gid-1] = false;
    for (int t = 0; t < 22; t++)
    {
        for (int r = 0; r < 256; r++)
        {
            if (deckrooms[t][r][cdeck] == droom)
                deckrooms[t][r][cdeck] = NULL;
        }
    }
    clearroom(droom);
    delete droom;
    hangingpart = true;
    hangingpartdeck = cdeck;
    SDL_WarpMouse(400, 400);



    //what we need: source room and destination, as well as origin. we copy the room to a part (or take an existing part: might have to
    //do this if we want to move it around and show it off without repurcussions.
    //we could do a makenull call to the .part, or a simple .clear, after we are done though. probably best.
    //so anyway, copy room and all equipment OR take an existing part and paste it in to the destination from origin.
    //since we need the roomlist[t][r][d] to worry about, we probably need to take care of that up elsewhere,
    //or feed in data.

    //step one: select room
    //step two: copy room
    //step two-a: get the origin [t][r][d] safe.  If you decide to move another room instead, be sure to change these and start over and delete the part.
    //step three: display the copied room and move it around with mouse.
    //step four:


    //copy room to copy part, start displaying copypart, set mode from Move-grabroom to Move-movingroom. Delete the
    //room in mian. Display the room as you move it.  If mode gets changed, set the room back where it was.
    //when reach new position, inset and clear copypart, and select the new room to a_room.

    return true;
}

bool moveroomend(int endtype, int px, int py, int pd)
{
    hangingpart = false;
    if (endtype == 1)
    {
        copypart->insetatcursor(px, py, pd);
        copypart->clearpart();
        return true;
    }
    else
    {
        //put it back where it came from originally!!!
        copypart->insetatcursor(400, 400, hangingpartdeck);
        copypart->clearpart();
        hangingpartdeck = 0;
    }
    return false;

}

enum {FINISHROOM, PARSEROOM, SAVEROOM, PARSEEQUIP, SAVEEQUIP};

bool savedisplay(int currstep, int totalstep, int savetype)
{
    //this is only called when saving in order to make it not do the 'not responding' thing. lets do this!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glDisable(GL_TEXTURE_2D);
    //glColor4f(0.4,0.4,0.4, 1.0);
    glLoadIdentity();


    //stuff
    char str1[20];
    char str2[20];
    itoa(currstep, str1, 10);
    itoa(totalstep, str2, 10);
    if (savetype == FINISHROOM)
    {
        char pt[] = "Finishing Rooms";
        glPrint(400,400,pt, 0, 1);
    }
    else
    if (savetype == PARSEROOM)
    {
        char pt[] = "Parsing Rooms";
        glPrint(400,400,pt, 0, 1);
    }
    else
    if (savetype == SAVEROOM)
    {
        char pt[] = "Saving Rooms";
        glPrint(400,400,pt, 0, 1);
    }
    else
    if (savetype == PARSEEQUIP)
    {
        char pt[] = "Parsing Equipment";
        glPrint(400,400,pt, 0, 1);
    }
    else
    if (savetype == SAVEEQUIP)
    {
        char pt[] = "Saving Equipment";
        glPrint(400,400,pt, 0, 1);
    }
    glPrint(400, 440, str1, 0, 1);
    glPrint(500, 440, str2, 0, 1);

    glFlush();
    SDL_GL_SwapBuffers();
    return true;
}

//its like a hatch/turret without checking below, BUT with checking a LOT of extra positions.
//hit every other vehicle placed, make an invalidation matrix, then check vs that matrix & position in room
bool addvehicle(int mousex, int mousey, int deckn)
{
    int px = cvtHEX2(mousex);
    int py = cvtHEX2(800-mousey);
    int collbox; //set based on vehicle
    bool placeable[161][161]; // set all to false, then collide with room to set to true, test to see if it
    //can even fit, THEN search out all the other equipment on this deck and flag and try again.
    for (int x = 0; x < 161; x++)
    {
        for (int y = 0; y < 161; y++)
        {
            placeable[x][y] = false;
        }
    }
    int rnum = -1;  //collide for rnum, then check everywhere for in AND not edge.
    for (int r = 0; r < 256; r++)
    {
        if (pointInPolygon3(deckrooms[13][r][deckn], px, py))
            rnum = deckrooms[13][r][deckn]->globalnum; // we check all equip with this globalnum
            //which should be only hatches and other vehicles: we need to test this stuff.
    }
    if (rnum <0)
        return false; // no hits
    if (equiptype == V_A_T)
    ;
    if (equiptype == V_A_F1)
    ;
    if (equiptype == V_A_F2)
    ;
    if (equiptype == V_G_M)
    ;
    if (equiptype == V_G_S)
    ;
    if (equiptype == V_G_A)
    ;
    /*
    V_A_T = 0x00, V_A_F1 = 0x00, V_A_F2 = 0x00, V_G_M = 0x00,
       V_G_S = 0x00, V_G_A = 0x00,
    */




    return true;
}


#endif

