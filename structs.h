// DOGFIGHT 2
// Gregory McIntyre
// 1998

// structure definitions

#include "allegro.h"

#define MAXPATH 80


typedef struct Shot
{
 float x, y;
 float oldx, oldy;
 unsigned char life;
 float speed;
 float heading;
 unsigned char type;
} Shot;


typedef struct Player
{
 float x, y;
 float oldx, oldy;
 float engine_power;
 float speed;
 float crash_vel_x;
 float engine_heading;
 float heading;
 int start_x, start_y;
 unsigned char k[5];    // room for total kills
 unsigned char flip;

 unsigned char key_up, key_down, key_left, key_right, key_fire;
 BITMAP *buffer;

 struct Shot s[256];    // room for up to 256 shots

 char energy;
 unsigned char dead;

 char spin;
 char invincible;

} Player;


typedef struct Level
{
 float gravity;
 float friction;
 BITMAP *backdrop;
} Level;


typedef struct Explosion
{
 int x, y;
 int max_radius;
 float current_radius;
 float speed;
 char type;
 struct Explosion *link;
} Explosion;


typedef struct Bit
{
 float x, y;
 float oldx, oldy;
 float velx, vely;
 int life;
 char colour;
 struct Bit *link;
} Bit;


typedef struct GameInfo
{
 BITMAP *buffer;
 BITMAP *backdrop;
 DATAFILE *data;

 PALETTE pal;
 RGB_MAP rgb_table;
 COLOR_MAP light_table;

 unsigned int res_w;
 unsigned int res_h;

 struct Player p[4];
 struct Level level;

 // game options
 unsigned char players;
 unsigned char coralie_mode;
 float fps;

 char backdrop_name[MAXPATH];

 // vehicle statistics
 float turn_speed;
 float power_accel;
 float min_power;
 float max_power;
 float vehicle_mass;
 unsigned char vehicle_radius;
 char max_energy;

 unsigned char num_of_shots;
 unsigned char shot_life;
 unsigned char shot_lag;
 char shot_base_speed;
 unsigned char shot_power;
 unsigned char invincibility_life;
 unsigned char spin_time;

 char vehicle_name[MAXPATH];

 char shot_name[MAXPATH];
 BITMAP *shot;
 BITMAP *tempshot;

 int bit_life;
 unsigned char frame_delay;

 struct Explosion *eend;

 struct Bit *bend;

} GameInfo;
