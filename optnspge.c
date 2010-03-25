// DOGFIGHT 2
// Gregory McIntyre
// 1998

// options page functions

#include <stdio.h>
#include "allegro.h"

#include "structs.h"
#include "defaults.h"
#include "datafile/dog2.h"

#define PI  3.14159265358979323846

#define L1N            3
#define L2N           15
#define L3N            3
#define L4N            3
#define NUM_OPTIONS   (L1N + L2N + L3N + L4N)

#define L1      1
#define L2     (L1 + L1N)
#define L3     (L2 + L2N)
#define L4     (L3 + L3N)

#define L1X    8
#define L1Y   40

#define L2X    8
#define L2Y  128

#define L3X   (SCREEN_W/2)
#define L3Y  128

#define L4X    8
#define L4Y   (SCREEN_H - OPTD*(L4N + 1))

#define OPTW     160  // option width
#define OPTH      12  // option height
#define OPTD      16  // option (row) difference


#define HL_W   280
#define HL_COL   5


// *** STRUCTURE DEFINITIONS ***************************************

typedef struct MenuInfo
{
 unsigned char highlight;
 char increment;
} MenuInfo;



// *** FUNCTION PROTOTYPES *****************************************

void options_page(struct GameInfo *);
void draw_optionspage(struct GameInfo *, struct MenuInfo *, BITMAP *);
void change_val(struct GameInfo *, struct MenuInfo *);
void save_options(struct GameInfo *);
void restore_defaults(struct GameInfo *);



// *** FUNCTION DEFINITIONS ****************************************

void options_page(struct GameInfo *i)
{
 BITMAP *backdrop;
 unsigned char c;
 struct MenuInfo menu;
 unsigned char finished;

 menu.highlight = 1;
 menu.increment = 0;

 // set the palette
 clear(screen);
 set_palette(black_palette);
 color_map = (COLOR_MAP *)i->data[OPTIONS_TRANS_TABLE].dat;

 // keep a copy of the options backdrop
 backdrop = create_bitmap(SCREEN_W, SCREEN_H);
 stretch_blit((BITMAP *)i->data[OPTIONS_BACKDROP].dat, backdrop,
              0, 0, 640, 480, 0, 0, SCREEN_W, SCREEN_H);
 draw_optionspage(i, &menu, backdrop);
 fade_in(i->data[OPTIONS_PAL].dat, 6);

 // main options page loop - redraws whole screen each iteration
 finished = 0;
 do {
   draw_optionspage(i, &menu, backdrop);

   c = readkey() >> 8;
   clear_keybuf();
   if (c == KEY_DOWN && menu.highlight < NUM_OPTIONS)
     menu.highlight++;
   else if (c == KEY_UP && menu.highlight > 1)
     menu.highlight--;
   else if (c == KEY_LEFT) {
     menu.increment = -menu.highlight;
     change_val(i, &menu);
   }
   else if (c == KEY_RIGHT || c == KEY_ENTER) {
     menu.increment = menu.highlight;
     if (menu.highlight == NUM_OPTIONS && c == KEY_ENTER)
       finished = 2;
     change_val(i, &menu);
   }
   else if (c == KEY_ESC)
     finished = 1;

 } while (!finished);

 destroy_bitmap(backdrop);
}



void draw_optionspage(struct GameInfo *i, struct MenuInfo *menu, BITMAP *backdrop)
{
 char valstr[128];
 char count;
 float anglecount;

 FONT *sf = (FONT *)i->data[SMALL_FONT].dat;

 // redraw the backdrop
 blit(backdrop, i->buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

 // draw the base options page
 // text
 textout_centre_ex(i->buffer, (FONT *)i->data[BIG_FONT].dat, "OPTION!!", SCREEN_W/2, 8, -1, -1);

 // draw highlight
 drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
 if (menu->highlight < L2)
   rectfill(i->buffer, L1X - 4, L1Y + OPTD*(menu->highlight - L1 + 1) - 2, L1X + HL_W, L1Y + OPTD*(menu->highlight - L1 + 1) + OPTH - 2, HL_COL);
 else if (menu->highlight >= L2 && menu->highlight <  L3)
   rectfill(i->buffer, L2X - 4, L2Y + OPTD*(menu->highlight - L2 + 1) - 2, L2X + HL_W, L2Y + OPTD*(menu->highlight - L2 + 1) + OPTH - 2, HL_COL);
 else if (menu->highlight >= L3 && menu->highlight <  L4)
   rectfill(i->buffer, L3X - 4, L3Y + OPTD*(menu->highlight - L3 + 1) - 2, L3X + HL_W, L3Y + OPTD*(menu->highlight - L3 + 1) + OPTH - 2, HL_COL);
 else if (menu->highlight >= L4)
   rectfill(i->buffer, L4X - 4, L4Y + OPTD*(menu->highlight - L4) - 2, L4X + HL_W, L4Y + OPTD*(menu->highlight - L4) + OPTH - 2, HL_COL);
 drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

 // print option names
 textout_ex(i->buffer, i->data[SMALL_FONT].dat, "GAME OPTIONS", L1X, L1Y, -1, -1);
 textout_ex(i->buffer, sf, "Number of Players : { * * * * }", L1X, L1Y + OPTD,   -1, -1);
 textout_ex(i->buffer, sf, "Coralie Mode      :",             L1X, L1Y + OPTD*2, -1, -1);
 textout_ex(i->buffer, sf, "Frame Delay (ms)  :",             L1X, L1Y + OPTD*3, -1, -1);

 textout_ex(i->buffer, sf, "VEHICLE STATISTICS", L2X, L2Y,      -1, -1);
 textout_ex(i->buffer, sf, "Vehicle           :", L2X, L2Y + OPTD,    -1, -1);
 textout_ex(i->buffer, sf, "Shot              :", L2X, L2Y + OPTD* 2, -1, -1);
 textout_ex(i->buffer, sf, "Energy            :", L2X, L2Y + OPTD* 3, -1, -1);
 textout_ex(i->buffer, sf, "Turning Speed (dg):", L2X, L2Y + OPTD* 4, -1, -1);
 textout_ex(i->buffer, sf, "Power Acceleration:", L2X, L2Y + OPTD* 5, -1, -1);
 textout_ex(i->buffer, sf, "Minimum Power     :", L2X, L2Y + OPTD* 6, -1, -1);
 textout_ex(i->buffer, sf, "Maximum Power     :", L2X, L2Y + OPTD* 7, -1, -1);
 textout_ex(i->buffer, sf, "Mass              :", L2X, L2Y + OPTD* 8, -1, -1);
 textout_ex(i->buffer, sf, "Radius            :", L2X, L2Y + OPTD* 9, -1, -1);
 textout_ex(i->buffer, sf, "Number of Shots   :", L2X, L2Y + OPTD*10, -1, -1);
 textout_ex(i->buffer, sf, "Shot Life         :", L2X, L2Y + OPTD*11, -1, -1);
 textout_ex(i->buffer, sf, "Shot Lag          :", L2X, L2Y + OPTD*12, -1, -1);
 textout_ex(i->buffer, sf, "Shot Base Speed   :", L2X, L2Y + OPTD*13, -1, -1);
 textout_ex(i->buffer, sf, "Shot Power        :", L2X, L2Y + OPTD*14, -1, -1);
 textout_ex(i->buffer, sf, "Invincibility Life:", L2X, L2Y + OPTD*15, -1, -1);

 textout_ex(i->buffer, sf, "LEVEL OPTIONS", L3X, L3Y,          -1, -1);
 textout_ex(i->buffer, sf, "Gravity           :", L3X, L3Y + OPTD,   -1, -1);
 textout_ex(i->buffer, sf, "Frictional Coeff. :", L3X, L3Y + OPTD*2, -1, -1);
 textout_ex(i->buffer, sf, "Background        :", L3X, L3Y + OPTD*3, -1, -1);

 textout_ex(i->buffer, sf, "Restore Defaults",           L4X, L4Y,          -1, -1);
 textout_ex(i->buffer, sf, "Save to configuration file", L4X, L4Y + OPTD,   -1, -1);
 textout_ex(i->buffer, sf, "Return to title screen",     L4X, L4Y + OPTD*2, -1, -1);


 // print values

 // GAME OPTIONS
 // number of players
 for (count = 0; count != i->players; count++)
    textout_ex(i->buffer, font, "*", L1X + OPTW + 16 + count*16, L1Y + OPTD, 28, -1);

 // coralie mode
 if (i->coralie_mode)
   textout_ex(i->buffer, sf, "ON",  L1X + OPTW, L1Y + OPTD*2, -1, -1);
 else
   textout_ex(i->buffer, sf, "OFF", L1X + OPTW, L1Y + OPTD*2, -1, -1);

 // frame delay
 sprintf(valstr, "%d", i->frame_delay);
 textout_ex(i->buffer, sf, valstr, L1X + OPTW, L1Y + OPTD*3, -1, -1);

 // VEHICLE OPTIONS
 // vehicle graphic
 textout_ex(i->buffer, sf, i->vehicle_name, L2X + OPTW, L2Y + OPTD, -1, -1);
 // shot graphic
 textout_ex(i->buffer, sf, i->shot_name, L2X + OPTW, L2Y + OPTD*2, -1, -1);
 // max energy
 sprintf(valstr, "%u", i->max_energy);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 3, -1, -1);
 // turning speed
 sprintf(valstr, "%.2f", i->turn_speed * 180/PI);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 4, -1, -1);
 // power accel
 sprintf(valstr, "%.2f", i->power_accel);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 5, -1, -1);
 // minimum power
 sprintf(valstr, "%.2f", i->min_power);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 6, -1, -1);
 // maximum power
 sprintf(valstr, "%.2f", i->max_power);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 7, -1, -1);
 // mass
 sprintf(valstr, "%.2f", i->vehicle_mass);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 8, -1, -1);
 // radius
 sprintf(valstr, "%u", i->vehicle_radius);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD* 9, -1, -1);
 // number of shots
 sprintf(valstr, "%u", i->num_of_shots);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*10, -1, -1);
 // shot life
 sprintf(valstr, "%u", i->shot_life);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*11, -1, -1);
 // shot lag
 sprintf(valstr, "%u", i->shot_lag);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*12, -1, -1);
 // shot base speed
 sprintf(valstr, "%u", i->shot_base_speed);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*13, -1, -1);
 // shot power
 sprintf(valstr, "%u", i->shot_power);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*14, -1, -1);
 // invincibility life
 sprintf(valstr, "%u", i->invincibility_life);
 textout_ex(i->buffer, sf, valstr, L2X + OPTW, L2Y + OPTD*15, -1, -1);

 // LEVEL OPTIONS
 // gravity
 sprintf(valstr, "%.2f", i->level.gravity);
 textout_ex(i->buffer, sf, valstr, L3X + OPTW, L3Y + OPTD,   -1, -1);
 // friction
 sprintf(valstr, "%.3f", i->level.friction);
 textout_ex(i->buffer, sf, valstr, L3X + OPTW, L3Y + OPTD*2, -1, -1);
 // background
 textout_ex(i->buffer, sf, i->backdrop_name, L3X + OPTW, L3Y + OPTD*3, -1, -1);

 blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}



void change_val(struct GameInfo *i, struct MenuInfo *menu)
{

 switch (menu->increment) {
   // GAME OPTIONS
   // number of players
   case L1 :
        if (i->players < 4)
          i->players++;
        break;
   case -L1 :
        if (i->players > 2)
          i->players--;
        break;
   // coralie mode
   case L1 + 1 :
   case -(L1 + 1) :
        if (!i->coralie_mode)
          i->coralie_mode = 1;
        else
          i->coralie_mode = 0;
        break;
   // frame delay
   //case L1 + 2 :
        //i->frame_delay += 1;
        //break;
   //case -(L1 + 2) :
        //i->frame_delay -= 1;
        //break;

   // VEHICLE OPTIONS
   case L2 :
   case -L2 :
        load_next_vehicle(i);
        break;

   case L2 + 1 :
   case -(L2 + 1) :
        load_next_shot(i);
        break;

   case L2 + 2 :
        i->max_energy += 1;
        if (i->max_energy < 0)
          i->max_energy = 1;
        break;
   case -(L2 + 2) :
        i->max_energy -= 1;
        if (i->max_energy < 1)
          i->max_energy = 127;
        break;


   case L2 + 3 :
        i->turn_speed += 0.001;
        if (i->turn_speed > 2*PI)
          i->turn_speed -= 2*PI;
        break;
   case -(L2 + 3) :
        i->turn_speed -= 0.001;
        if (i->turn_speed < 0)
          i->turn_speed += 2*PI;
        break;

   case L2 + 4 :
        i->power_accel += 0.01;
        if (i->power_accel > 128)
          i->power_accel = -128;
        break;
   case -(L2 + 4) :
        i->power_accel -= 0.01;
        if (i->power_accel < -128)
          i->power_accel = 128;
        break;

   case L2 + 5 :
        i->min_power += 0.05;
        if (i->min_power > i->max_power)
          i->min_power = -128;
        break;
   case -(L2 + 5) :
        i->min_power -= 0.05;
        if (i->min_power < -128)
          i->min_power = i->max_power;
        break;

   case L2 + 6 :
        i->max_power += 0.05;
        if (i->max_power > 128)
          i->max_power = i->min_power;
        break;
   case -(L2 + 6) :
        i->max_power -= 0.05;
        if (i->max_power < i->min_power)
          i->max_power = 128;
        break;

   case L2 + 7 :
        i->vehicle_mass += 0.1;
        if (i->vehicle_mass > 64)
          i->vehicle_mass = 0.1;
        break;
   case -(L2 + 7) :
        i->vehicle_mass -= 0.1;
        if (i->vehicle_mass < 0.1)
          i->vehicle_mass = 64;
        break;

   case L2 + 8 :
        i->vehicle_radius += 1;
        if (i->vehicle_radius > 32)
          i->vehicle_radius = 0;
        break;
   case -(L2 + 8) :
        i->vehicle_radius -= 1;
        if (i->vehicle_radius == 0)
          i->vehicle_radius = 32;
        break;

   case L2 + 9 :
        i->num_of_shots += 1;
        break;
   case -(L2 + 9) :
        i->num_of_shots -= 1;
        break;

   case L2 + 10 :
        i->shot_life += 1;
        break;
   case -(L2 + 10) :
        i->shot_life -= 1;
        break;

   case L2 + 11 :
        i->shot_lag += 1;
        break;
   case -(L2 + 11) :
        i->shot_lag -= 1;
        break;

   case L2 + 12 :
        i->shot_base_speed += 1;
        break;
   case -(L2 + 12) :
        i->shot_base_speed -= 1;
        break;

   case L2 + 13 :
        i->shot_power += 1;
        break;
   case -(L2 + 13) :
        i->shot_power -= 1;
        break;

   case L2 + 14 :
        i->invincibility_life += 1;
        break;
   case -(L2 + 14) :
        i->invincibility_life -= 1;
        break;

   // LEVEL OPTIONS
   case L3 :
        i->level.gravity += 0.05;
        if (i->level.gravity > 20.0)
          i->level.gravity = -20.0;
        break;
   case -L3 :
        i->level.gravity -= 0.05;
        if (i->level.gravity < -20.0)
          i->level.gravity = 20.0;
        break;

   case L3 + 1 :
        i->level.friction += 0.005;
        if (i->level.friction > 1)
          i->level.friction = 0;
        break;
   case -(L3 + 1) :
        i->level.friction -= 0.005;
        if (i->level.friction < 0)
          i->level.friction = 1;
        break;

   case L3 + 2 :
   case -(L3 + 2) :
        load_next_backdrop(i);
        break;

   // other options
   case L4 :
        restore_defaults(i);
        break;
   case L4 + 1 :
        save_options(i);
        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        rectfill(screen, SCREEN_W/2 - 100, (SCREEN_H - 40)/2, SCREEN_W/2 + 100, (SCREEN_H + 40)/2, HL_COL);
        textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat, "Options Saved", SCREEN_W/2, (SCREEN_H - text_height((FONT *)i->data[SMALL_FONT].dat))/2, -1, -1);
        drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
        rest(500);
        break;
 };

 menu->increment = 0;
}



void save_options(struct GameInfo *i)
{
 // save the GameInfo structure to the config structure
 // Allegro will write changes to this to the config file
 // when the program exits

 set_config_int("game_defaults", "players", i->players);
 set_config_int("game_defaults", "coralie_mode", i->coralie_mode);
 set_config_int("game_defaults", "frame_delay", i->frame_delay);

 set_config_float("vehicle_defaults", "turn_speed", i->turn_speed);
 set_config_float("vehicle_defaults", "power_acceleration", i->power_accel);
 set_config_float("vehicle_defaults", "min_power", i->min_power);
 set_config_float("vehicle_defaults", "max_power", i->max_power);
 set_config_float("vehicle_defaults", "mass", i->vehicle_mass);
 set_config_int("vehicle_defaults", "radius", i->vehicle_radius);
 set_config_int("vehicle_defaults", "num_of_shots", i->num_of_shots);
 set_config_int("vehicle_defaults", "shot_life", i->shot_life);
 set_config_int("vehicle_defaults", "shot_lag", i->shot_lag);
 set_config_int("vehicle_defaults", "shot_base_speed", i->shot_base_speed);
 set_config_int("vehicle_defaults", "shot_power", i->shot_power);
 set_config_int("vehicle_defaults", "invincibility_life", i->invincibility_life);

 set_config_float("level_defaults", "gravity", i->level.gravity);
 set_config_float("level_defaults", "friction", i->level.friction);
}



void restore_defaults(struct GameInfo *i)
{
   // game options
   i->players      = DEFAULT_PLAYERS;
   i->coralie_mode = DEFAULT_CORALIE_MODE;
   i->frame_delay  = DEFAULT_FRAME_DELAY;

   // level defaults
   i->level.gravity  = DEFAULT_GRAVITY;
   i->level.friction = DEFAULT_FRICTION;

   // vehicle defaults
   i->turn_speed         = DEFAULT_TURN_SPEED;
   i->power_accel        = DEFAULT_POWER_ACCEL;
   i->min_power          = DEFAULT_MIN_POWER;
   i->max_power          = DEFAULT_MAX_POWER;
   i->vehicle_mass       = DEFAULT_VEHICLE_MASS;
   i->vehicle_radius     = DEFAULT_VEHICLE_RADIUS;
   i->max_energy         = DEFAULT_MAX_ENERGY;
   i->num_of_shots       = DEFAULT_NUM_OF_SHOTS;
   i->shot_life          = DEFAULT_SHOT_LIFE;
   i->shot_lag           = DEFAULT_SHOT_LAG;
   i->shot_base_speed    = DEFAULT_SHOT_BASE_SPEED;
   i->shot_power         = DEFAULT_SHOT_POWER;
   i->invincibility_life = DEFAULT_INVINCIBILITY_LIFE;
   i->spin_time          = DEFAULT_SPIN_TIME;

   /*
   // individual player controls/settings
   i->p[0].key_up    = DEFAULT_P1_KEY_UP;
   i->p[0].key_down  = DEFAULT_P1_KEY_DOWN;
   i->p[0].key_left  = DEFAULT_P1_KEY_LEFT;
   i->p[0].key_right = DEFAULT_P1_KEY_RIGHT;
   i->p[0].key_fire  = DEFAULT_P1_KEY_FIRE;
   i->p[1].key_up    = DEFAULT_P2_KEY_UP;
   i->p[1].key_down  = DEFAULT_P2_KEY_DOWN;
   i->p[1].key_left  = DEFAULT_P2_KEY_LEFT;
   i->p[1].key_right = DEFAULT_P2_KEY_RIGHT;
   i->p[1].key_fire  = DEFAULT_P2_KEY_FIRE;
   i->p[2].key_up    = DEFAULT_P3_KEY_UP;
   i->p[2].key_down  = DEFAULT_P3_KEY_DOWN;
   i->p[2].key_left  = DEFAULT_P3_KEY_LEFT;
   i->p[2].key_right = DEFAULT_P3_KEY_RIGHT;
   i->p[2].key_fire  = DEFAULT_P3_KEY_FIRE;
   i->p[3].key_up    = DEFAULT_P4_KEY_UP;
   i->p[3].key_down  = DEFAULT_P4_KEY_DOWN;
   i->p[3].key_left  = DEFAULT_P4_KEY_LEFT;
   i->p[3].key_right = DEFAULT_P4_KEY_RIGHT;
   i->p[3].key_fire  = DEFAULT_P4_KEY_FIRE;
   */
}
