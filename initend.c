// DOGFIGHT 2
// Gregory McIntyre
// 1998

// initialisation and cleanup functions

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "allegro.h"

#include "structs.h"
#include "defaults.h"
#include "datafile/dog2.h"


#define VERSION  "2.01"
#define WEB_ADDRESS  "http://www.pcug.org.au/~gregm/"



// *** FUNCTION PROTOTYPES *****************************************

void init_info(struct GameInfo *);
void finishup(struct GameInfo *);



// *** FUNCTION DEFINITIONS ****************************************

void init_info(struct GameInfo *i)
{
 BITMAP *palbitmap;
 FILE *cfg;
 char filename[256];
 int err_vmode;
 int count;
 int incount;
/*
 clrscr();
 textattr(15 + (4 << 4));
 cprintf("                                 DOGFIGHT: v" VERSION "                                ");
 textattr(7);
*/
 printf("\n=============================================================================");
 printf("\n                             Please distribute!");
 printf("\n     An original copy can be obtained from " WEB_ADDRESS);
 printf("\n=============================================================================");

 // initialise allegro
 allegro_init();
 install_timer();
 install_keyboard();

 printf("\n\nReading configuration file & initialising game variables...");
 fflush(stdout);

 // check for the existence of the configuration file
 if (file_exists("dog2.cfg", 0, NULL) == 0) {
   // print an error message and exit
   printf("\n");
   printf("Error: The configuration file \"dog2.cfg\" could not be located.\n");
   printf("       It should be in the working directory. eg. \"C:\\dog2\\dog2.cfg\"\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }
 else {
   set_config_file("dog2.cfg");

   // resolution
   i->res_w = get_config_int("resolution", "res_w", DEFAULT_RES_W);
   i->res_h = get_config_int("resolution", "res_h", DEFAULT_RES_W);

   // game options
   i->players      = get_config_int("game_defaults", "players",      DEFAULT_PLAYERS);
   i->coralie_mode = get_config_int("game_defaults", "coralie_mode", DEFAULT_CORALIE_MODE);
   i->frame_delay  = get_config_int("game_defaults", "frame_delay",  DEFAULT_FRAME_DELAY);

   // level defaults
   i->level.gravity  = get_config_float("level_defaults", "gravity",  DEFAULT_GRAVITY);
   i->level.friction = get_config_float("level_defaults", "friction", DEFAULT_FRICTION);
   strcpy(i->backdrop_name, get_config_string("level_defaults", "backdrop", ""));

   // vehicle defaults
   strcpy(i->vehicle_name, get_config_string("vehicle_defaults", "vehicle", ""));
   strcpy(i->shot_name,    get_config_string("vehicle_defaults", "shot",    ""));
   i->turn_speed         = get_config_float("vehicle_defaults", "turn_speed",         DEFAULT_TURN_SPEED);
   i->power_accel        = get_config_float("vehicle_defaults", "power_acceleration", DEFAULT_POWER_ACCEL);
   i->min_power          = get_config_float("vehicle_defaults", "min_power",          DEFAULT_MIN_POWER);
   i->max_power          = get_config_float("vehicle_defaults", "max_power",          DEFAULT_MAX_POWER);
   i->vehicle_mass       = get_config_float("vehicle_defaults", "mass",               DEFAULT_VEHICLE_MASS);
   i->vehicle_radius     = get_config_int("vehicle_defaults", "radius",               DEFAULT_VEHICLE_RADIUS);
   i->max_energy         = get_config_int("vehicle_defaults", "energy",               DEFAULT_MAX_ENERGY);
   i->num_of_shots       = get_config_int("vehicle_defaults", "num_of_shots",         DEFAULT_NUM_OF_SHOTS);
   i->shot_life          = get_config_int("vehicle_defaults", "shot_life",            DEFAULT_SHOT_LIFE);
   i->shot_lag           = get_config_int("vehicle_defaults", "shot_lag",             DEFAULT_SHOT_LAG);
   i->shot_base_speed    = get_config_int("vehicle_defaults", "shot_base_speed",      DEFAULT_SHOT_BASE_SPEED);
   i->shot_power         = get_config_int("vehicle_defaults", "shot_power",           DEFAULT_SHOT_POWER);
   i->invincibility_life = get_config_int("vehicle_defaults", "invincibility_life",   DEFAULT_INVINCIBILITY_LIFE);
   i->spin_time          = get_config_int("vehicle_defaults", "spin_time",            DEFAULT_SPIN_TIME);

   // individual player controls/settings
   i->p[0].key_up    = get_config_int("player_1", "key_up",    DEFAULT_P1_KEY_UP);
   i->p[0].key_down  = get_config_int("player_1", "key_down",  DEFAULT_P1_KEY_DOWN);
   i->p[0].key_left  = get_config_int("player_1", "key_left",  DEFAULT_P1_KEY_LEFT);
   i->p[0].key_right = get_config_int("player_1", "key_right", DEFAULT_P1_KEY_RIGHT);
   i->p[0].key_fire  = get_config_int("player_1", "key_fire",  DEFAULT_P1_KEY_FIRE);
   i->p[1].key_up    = get_config_int("player_2", "key_up",    DEFAULT_P2_KEY_UP);
   i->p[1].key_down  = get_config_int("player_2", "key_down",  DEFAULT_P2_KEY_DOWN);
   i->p[1].key_left  = get_config_int("player_2", "key_left",  DEFAULT_P2_KEY_LEFT);
   i->p[1].key_right = get_config_int("player_2", "key_right", DEFAULT_P2_KEY_RIGHT);
   i->p[1].key_fire  = get_config_int("player_2", "key_fire",  DEFAULT_P2_KEY_FIRE);
   i->p[2].key_up    = get_config_int("player_3", "key_up",    DEFAULT_P3_KEY_UP);
   i->p[2].key_down  = get_config_int("player_3", "key_down",  DEFAULT_P3_KEY_DOWN);
   i->p[2].key_left  = get_config_int("player_3", "key_left",  DEFAULT_P3_KEY_LEFT);
   i->p[2].key_right = get_config_int("player_3", "key_right", DEFAULT_P3_KEY_RIGHT);
   i->p[2].key_fire  = get_config_int("player_3", "key_fire",  DEFAULT_P3_KEY_FIRE);
   i->p[3].key_up    = get_config_int("player_4", "key_up",    DEFAULT_P4_KEY_UP);
   i->p[3].key_down  = get_config_int("player_4", "key_down",  DEFAULT_P4_KEY_DOWN);
   i->p[3].key_left  = get_config_int("player_4", "key_left",  DEFAULT_P4_KEY_LEFT);
   i->p[3].key_right = get_config_int("player_4", "key_right", DEFAULT_P4_KEY_RIGHT);
   i->p[3].key_fire  = get_config_int("player_4", "key_fire",  DEFAULT_P4_KEY_FIRE);
   i->p[0].flip = 0;
   i->p[1].flip = 1;
   i->p[2].flip = 1;
   i->p[3].flip = 0;

   // other
   i->eend = NULL;
   i->bend = NULL;
 }
 printf("\nDone!\n\n");

 // load datafile
 printf("Loading datafile...");
 fflush(stdout);
 set_color_conversion(COLORCONV_NONE);
 if (file_exists("datafile/dog2.dat", 0, NULL))
   i->data = load_datafile("datafile/dog2.dat");
 else {
   printf("\n");
   printf("Error: The datafile \"dog2.dat\" could not be located.\n");
   printf("       It should be in the datafile directory.");
   printf(" eg. \"C:\\dog2\\datafile\\dog2.dat\"\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }
 printf("\nDone!\n\n");


 printf("Initiating graphics buffers...");
 fflush(stdout);
 // get the palette
 if (file_exists("data/pal.pcx", 0, NULL)) {
   palbitmap = load_pcx("data/pal.pcx", i->pal);
   destroy_bitmap(palbitmap);
 }

 // set up main game buffer
 i->buffer = create_bitmap(i->res_w, i->res_h);
 clear(i->buffer);

 // set up the background buffer
 i->level.backdrop = create_bitmap(i->res_w, i->res_h);

 // set up shot buffers
 if (i->res_w < 640 && i->res_h < 480) {
   i->shot = create_bitmap(8 * i->res_w/640, 8 * i->res_h/480);
   i->tempshot = create_bitmap(8 * i->res_w/640, 8 * i->res_h/480);
 }
 else {
   i->shot = create_bitmap(8, 8);
   i->tempshot = create_bitmap(8, 8);
 }
 clear(i->shot);
 clear(i->tempshot);

 // set up player buffers
 for (count = 0; count != 4; count++) {
    if (i->res_w < 640 && i->res_h < 480)
      i->p[count].buffer = create_bitmap(32 * i->res_w/640, 32 * i->res_h/480);
    else
      i->p[count].buffer = create_bitmap(32, 32);
    clear(i->p[count].buffer);
 }

 // load default graphics
 if (load_backdrop(i)) {
   // the specified backdrop was not found - use a blank one
   printf("\nError: Specified backdrop \"%s\" could not be found.\n",
          i->backdrop_name);
   printf("       Check \"dog2.cfg\".\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }

 if (load_vehicle(i)) {
   // the specified vehicle was not found - use the first one
   // in the directory
   printf("\nError: Specified vehicle \"%s\" could not be found.\n",
          i->vehicle_name);
   printf("       Check \"dog2.cfg\".\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }

 if (load_shot(i)) {
   // the specified vehicle was not found - use the first one
   // in the directory
   printf("\nError: Specified shot \"%s\" could not be found.\n",
          i->shot_name);
   printf("       Check \"dog2.cfg\".\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }

 printf("\nDone!\n\n");

 printf("Generating colour mapping tables...");
 fflush(stdout);
 create_rgb_table(&i->rgb_table, i->pal, NULL);
 rgb_map = &i->rgb_table;
 create_light_table(&i->light_table, i->pal, 63, 63, 63, NULL);
 printf("\nDone!\n\n");

 // prompt to start program
 //printf("Press any key to continue...");
 //fflush(stdout);
 //readkey();

 // enter graphics mode
 err_vmode = set_gfx_mode(GFX_AUTODETECT_WINDOWED, i->res_w, i->res_h, 0, 0);
 if (err_vmode != 0) {
   printf("Error setting graphics mode:\n%s.\n", allegro_error);
   printf("Please check the file \"dog2.cfg\" to correct the problem.\n");
   printf("\nDogfight was aborted...\n");
   allegro_exit();
   exit(1);
 }
 set_palette(i->pal);
}



void finishup(struct GameInfo *i)
{
 int count;

 // free allocated buffers
 destroy_bitmap(i->buffer);
 destroy_bitmap(i->level.backdrop);
 destroy_bitmap(i->shot);
 destroy_bitmap(i->tempshot);
 for (count = 0; count != 4; count++)
    destroy_bitmap(i->p[count].buffer);

 // free datafile
 unload_datafile(i->data);

 // exit allegro, return to text mode
 allegro_exit();

 #define BACK    4
 #define NORM   14
 #define BRIGHT 11
 #define HL     15

 #define PRINTHL(x)  textattr(HL + (BACK << 4)); \
                     cprintf(x); \
                     textattr(NORM + (BACK << 4));

 // print an exiting message
/*
 window(10, 1, 70, 25);
 textattr(NORM + (BACK << 4));
 cprintf(" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 \n\r");
 PRINTHL("                      DOGFIGHT: v" VERSION "                       \n\r");
 cprintf(" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 \n\r");
 cprintf("                                                            \n\r");
 gotoxy(1, 5);
 PRINTHL("  DOGFIGHT 2 ");
 gotoxy(14, 5);
 cprintf("is free. You are free to distribute it in its  \n\r");
 cprintf("        original archived file form (DOG2???.ZIP).          \n\r");
 cprintf("                                                            \n\r");
 cprintf("                   I can be contacted at:                   \n\r");
 //cprintf("               컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�              \n\r");
 cprintf("                                                            \n\r");
 textattr(BRIGHT + (BACK << 4));
 cprintf("                      gregm@pcug.org.au                     \n\r");
 textattr(NORM + (BACK << 4));
 cprintf("                             or                             \n\r");
 textattr(BRIGHT + (BACK << 4));
 cprintf("                " WEB_ADDRESS "              \n\r");
 textattr(NORM + (BACK << 4));
 //cprintf("               컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�              \n\r");
 cprintf("                                                            \n\r");
 cprintf("                      Gregory McIntyre                      \n\r");
 cprintf("                                                            \n\r");
 cprintf("                   Have a nice day! Kupo!!                  \n\r");
 cprintf("                                                            \n\r");
 cprintf(" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 \n\r");
*/
}
