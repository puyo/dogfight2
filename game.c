// DOGFIGHT 2
// Gregory McIntyre
// 1998

// game functions

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "allegro.h"

#include "colour.h"
#include "structs.h"
#include "datafile/dog2.h"


// *** FUNCTION PROTOTYPES *****************************************

void game(struct GameInfo *);

void init_game(struct GameInfo *);
void handle_keys(struct GameInfo *);
void detect_collisions(struct GameInfo *);
void collide_vehicles(struct GameInfo *, char, char);
void explosion(struct GameInfo *, int, int, int, float, char);
void bit(struct GameInfo *, int, int, float, float, int, char);

void move(struct GameInfo *);


// *** FUNCTION DEFINITIONS ****************************************

void game(struct GameInfo *i)
{
 // a call to this function will start a new game with the information
 // in the GameInfo structure
 unsigned char finished = 0;
 unsigned char pcount;
 unsigned int frames;
 clock_t last_time;

 clear(screen);
 set_palette(black_palette);

 init_game(i);

 i->fps = 0;
 redraw(i, 0);
 fade_in(i->pal, 2);
 rest(200);

 last_time = clock();
 do {
   redraw(i, 1);
   handle_keys(i);
   detect_collisions(i);
   move(i);

   clock_t this_time = clock();
   clock_t diff = this_time - last_time;
   last_time = this_time;
   clock_t wait = MAX(0, 17 - (diff * 1000/CLOCKS_PER_SEC)); // 17ms ~ 58 fps
   if (frames > 100) {
     //printf("%d * %d / %d\n", frames, diff, CLOCKS_PER_SEC);
     i->fps = (float)frames*diff*1000/CLOCKS_PER_SEC;
     frames = 0;
   } else {
     frames++;
   }
   rest(wait);

 } while (!finished && !key[KEY_ESC]);
}



void init_game(struct GameInfo *i)
{
 // reset game variables for a new game

 unsigned char pcount;
 unsigned char incount;

 // reset player positions
 i->p[0].heading = 0;
 i->p[0].start_x = i->p[0].oldx = i->p[0].x = i->res_w * 1/4;
 i->p[0].start_y = i->p[0].oldy = i->p[0].y = i->res_h * 1/5;
 i->p[1].heading = M_PI;
 i->p[1].start_x = i->p[1].oldx = i->p[1].x = i->res_w * 3/4;
 i->p[1].start_y = i->p[1].oldy = i->p[1].y = i->res_h * 4/5;
 i->p[2].heading = M_PI;
 i->p[2].start_x = i->p[2].oldx = i->p[2].x = i->res_w * 3/4;
 i->p[2].start_y = i->p[2].oldy = i->p[2].y = i->res_h * 2/5;
 i->p[3].heading = 0;
 i->p[3].start_x = i->p[3].oldx = i->p[3].x = i->res_w * 1/4;
 i->p[3].start_y = i->p[3].oldy = i->p[3].y = i->res_h * 3/5;

 for (pcount = 0; pcount != i->players; pcount++) {
    i->p[pcount].dead = 0;
    i->p[pcount].engine_power = 0;
    i->p[pcount].speed = 0;
    i->p[pcount].energy = i->max_energy;
    i->p[pcount].spin = 0;
    i->p[pcount].invincible = -i->invincibility_life;

    // shots
    for (incount = 0; incount != 255; incount++) {
       i->p[pcount].s[incount].life = 0;
       i->p[pcount].s[incount].x = 0;
       i->p[pcount].s[incount].y = 0;
       i->p[pcount].s[incount].oldx = 0;
       i->p[pcount].s[incount].oldy = 0;
    }

    // kills
    for (incount = 0; incount != 5; incount++)
       i->p[pcount].k[incount] = 0;
 }
 color_map = &i->light_table;

 while (i->eend != NULL) {
   free(i->eend);
   i->eend = i->eend->link;
 }
 while (i->bend != NULL) {
   free(i->bend);
   i->bend = i->bend->link;
 }
}



void handle_keys(struct GameInfo *i)
{
 // key checking

 unsigned char pcount, scount;
 unsigned char last_shot_life, free_shot, free_shot_flag;

 for (pcount = 0; pcount != i->players; pcount++) {
    if (!i->p[pcount].spin) {
      // up
      if (key[i->p[pcount].key_up])
        i->p[pcount].engine_power += i->power_accel;
      // down
      if (key[i->p[pcount].key_down])
        i->p[pcount].engine_power -= i->power_accel;
      // left
      if (key[i->p[pcount].key_left]) {
        i->p[pcount].heading -= i->turn_speed;
      }
      // right
      if (key[i->p[pcount].key_right])
        i->p[pcount].heading += i->turn_speed;

      // fire
      if (key[i->p[pcount].key_fire]) {
        if (!i->p[pcount].dead) {
          // find a free slot
          free_shot_flag = 0;
          free_shot = 0;
          for (scount = 0; scount != i->num_of_shots; scount++) {
             if (!free_shot_flag) {
               if (!i->p[pcount].s[scount].life) {
                 free_shot = scount;
                 free_shot_flag = 1;
               }
             }
          }
          if (free_shot_flag) {
            // calculate whether or not enough time has passed
            // since the last shot was fired (shot lag)
            last_shot_life = 0;
            for (scount = 0; scount != i->num_of_shots; scount++) {
               if (i->p[pcount].s[scount].life > last_shot_life)
                 last_shot_life = i->p[pcount].s[scount].life;
            }
  
            if (last_shot_life < i->shot_life - i->shot_lag) {
              // fire!! - set up shot info
              i->p[pcount].s[free_shot].life    = i->shot_life;
              i->p[pcount].s[free_shot].x       = i->p[pcount].x;
              i->p[pcount].s[free_shot].y       = i->p[pcount].y;
              i->p[pcount].s[free_shot].speed   = i->shot_base_speed
                                                  + i->p[pcount].speed;
              i->p[pcount].s[free_shot].heading = i->p[pcount].heading;
            }
          }
        }
        else {
          // restart player
          i->p[pcount].x = (float)i->p[pcount].start_x;
          i->p[pcount].y = (float)i->p[pcount].start_y;
          i->p[pcount].dead = 0;
          i->p[pcount].engine_power = 0;
          i->p[pcount].speed = 0;
          i->p[pcount].energy = i->max_energy;
          i->p[pcount].spin = 0;
          i->p[pcount].invincible = -i->invincibility_life;

          if (pcount == 0 || pcount == 3)
            i->p[pcount].heading = 0;
          else
            i->p[pcount].heading = M_PI;
      
          // shots
          for (scount = 0; scount != 255; scount++) {
             i->p[pcount].s[scount].life = 0;
             i->p[pcount].s[scount].x = 0;
             i->p[pcount].s[scount].y = 0;
          }
        }
      }
    }
 }

 // help - space to continue
 if (key[KEY_F1]) {
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "PAUSE", SCREEN_W/2, SCREEN_H/2-8*4, -1, -1);
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "UP - increase engine power, DOWN - decrease engine power, LEFT/RIGHT - steer",
                  SCREEN_W/2, SCREEN_H/2-8*2, -1, -1);
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
                  "F1 - help, F12 - screen capture",
                  SCREEN_W/2, SCREEN_H/2, -1, -1);
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "Press the spacebar to continue...",
                  SCREEN_W/2, SCREEN_H/2+8*3, -1, -1);
   do {} while (!key[KEY_SPACE]);

   // copy the buffer back over the screen
   blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
 }

 if (key[KEY_F12]) {
   // capture the screen to a pcx file
   save_bitmap("dogscrn.pcx", i->buffer, i->pal);

   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "SCREEN CAPTURED", SCREEN_W/2, SCREEN_H/2-8*3, -1, -1);
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "The screen has been captured to the file \"dogscrn.pcx\".",
                  SCREEN_W/2, SCREEN_H/2-8, -1, -1);
   textout_centre_ex(screen, (FONT *)i->data[SMALL_FONT].dat,
   "Press the spacebar to continue...",
                  SCREEN_W/2, SCREEN_H/2+8, -1, -1);
   do {} while (!key[KEY_SPACE]);

   // copy the buffer back over the screen
   blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
 }

}



void detect_collisions(struct GameInfo *i)
{
 unsigned char pcount, chkcount, scount;
 float dist, distx2, disty2;

 for (pcount = 0; pcount != i->players; pcount++) {
    if (i->p[pcount].energy) {
      // vehicle/vehicle
      for (chkcount = 0; chkcount != i->players; chkcount++) {
         if (chkcount != pcount
          && !i->p[pcount].dead && !i->p[chkcount].dead
          && i->p[pcount].invincible >= 0) {

           if (i->p[chkcount].invincible >= 0) {

             distx2 = (i->p[pcount].x - i->p[chkcount].x)
                    * (i->p[pcount].x - i->p[chkcount].x);
             disty2 = (i->p[pcount].y - i->p[chkcount].y)
                    * (i->p[pcount].y - i->p[chkcount].y);
             dist = (float)sqrt(distx2 + disty2);
  
             if (dist < (float)(i->vehicle_radius * 2)) {
               // 2 players have collided
               collide_vehicles(i, pcount, chkcount);
             }
           }

           // vehicle/shot
           if (!i->p[chkcount].spin) {
             for (scount = 0; scount != i->num_of_shots; scount++) {
                if (i->p[chkcount].s[scount].life) {
                  distx2 = (i->p[pcount].x - i->p[chkcount].s[scount].x)
                         * (i->p[pcount].x - i->p[chkcount].s[scount].x);
                  disty2 = (i->p[pcount].y - i->p[chkcount].s[scount].y)
                         * (i->p[pcount].y - i->p[chkcount].s[scount].y);
                  dist = (float)sqrt(distx2 + disty2);
                  if (dist < (float)(i->vehicle_radius + i->shot->w/2)) {
                    // player chkcount has shot player pcount
                    explosion(i, i->p[pcount].x, i->p[pcount].y,
                                 i->shot_power/3, 1, 0);
                    i->p[pcount].energy -= i->shot_power;
                    i->p[pcount].invincible = -i->shot_base_speed*3;
                    if (i->p[pcount].energy <= 0)
                      i->p[chkcount].k[pcount]++;
                  }
                }
             }
           }
         }

         if (i->p[pcount].energy <= 0) {
           explosion(i, i->p[pcount].x, i->p[pcount].y, i->vehicle_radius*2, 1, 0);
         }
      }
    }
 }
}



void collide_vehicles(struct GameInfo *i, char p1, char p2)
{
 float midx, midy;
 int count;

 // make the players non-destructive invincible for a while
 i->p[p1].invincible = -i->spin_time;
 i->p[p2].invincible = -i->spin_time;

 // change the players' headings so they look like they've been bumped
 i->p[p1].heading += circ_rand_int(-100, 100, 0) * M_PI/500;
 i->p[p2].heading += circ_rand_int(-100, 100, 0) * M_PI/500;

 // create an explosion
 midx = i->p[p2].x + (i->p[p1].x - i->p[p2].x)/2;
 midy = i->p[p2].y + (i->p[p1].y - i->p[p2].y)/2;
 explosion(i, (int)midx, (int)midy, i->vehicle_radius*2, 1, 0);

 // create a spray of bits
/*
 for (count = 0; count != 50; count++)
    bit(i, rand_int((int)i->p[p1].x - 5, (int)i->p[p1].x + 5), rand_int((int)i->p[p1].y - 5, (int)i->p[p1].y+ 5),
           rand_int(-2, 2), rand_int(-2, 2), 20, COL1 + p1*PAL_DIFF + PAL_DIFF - 1);

 for (count = 0; count != 50; count++)
    bit(i, rand_int((int)i->p[p2].x - 5, (int)i->p[p2].x + 5), rand_int((int)i->p[p2].y - 5, (int)i->p[p2].y + 5),
           rand_int(-2, 2), rand_int(-2, 2), 20, COL1 + p2*PAL_DIFF + PAL_DIFF - 1);
*/

 i->p[p1].energy -= i->max_energy/10*i->vehicle_mass*(i->p[p1].speed + i->p[p2].speed)/2;
 i->p[p2].energy -= i->max_energy/10*i->vehicle_mass*(i->p[p1].speed + i->p[p2].speed)/2;

 // award points if one of the vehicles has crashed
 // if the vehicle has not already crashed, deduct energy
 if (i->p[p1].energy <= 0 && !i->p[p1].spin)
   i->p[p2].k[p1]++;

 if (i->p[p2].energy <= 0 && !i->p[p2].spin)
   i->p[p1].k[p2]++;
}



void explosion(struct GameInfo *i, int newx, int newy, int newradius, float newspeed, char newtype)
{
 struct Explosion *temp;

 // create a new explosion
 temp = (struct Explosion *)malloc(sizeof(struct Explosion));

 temp->x              = newx;
 temp->y              = newy;
 temp->max_radius     = newradius;
 temp->current_radius = newspeed;
 temp->speed          = newspeed;
 temp->type           = newtype;

 temp->link = i->eend;
 i->eend = temp;
}



void bit(struct GameInfo *i, int newx, int newy, float newvelx, float newvely, int newlife, char newcol)
{
 struct Bit *temp;

 // create a new bit
 temp = (struct Bit *)malloc(sizeof(struct Bit));

 temp->x              = (float)newx;
 temp->y              = (float)newy;
 temp->oldx           = (float)newx;
 temp->oldy           = (float)newy;
 temp->velx           = newvelx;
 temp->vely           = newvely;
 temp->life           = newlife;
 temp->colour         = newcol;

 temp->link = i->bend;
 i->bend = temp;
}



void move(struct GameInfo *i)
{
 // move the game elements

 struct Explosion *eptr;
 struct Bit *bptr;
 unsigned char pcount, scount;
 float acceleration, acceleration_x, acceleration_y;
 float vel_x, vel_y;

 for (pcount = 0; pcount != i->players; pcount++) {
    if (!i->p[pcount].dead) {
      // before doing anything else, record the current x and y
      // as oldx and oldy for the next iteration
      i->p[pcount].oldx = i->p[pcount].x;
      i->p[pcount].oldy = i->p[pcount].y;

      // apply limits to the values that have been changed
      // player coodinates
      // x
      if (i->p[pcount].x > SCREEN_W)
        i->p[pcount].x = 0;
      else if (i->p[pcount].x < 0)
        i->p[pcount].x = SCREEN_W;
      // y
      if (i->p[pcount].y > SCREEN_H)
        i->p[pcount].y = 0;
      else if (i->p[pcount].y < 0)
        i->p[pcount].y = SCREEN_H;
  
      // shot coodinates
      for (scount = 0; scount != i->num_of_shots; scount++) {
         i->p[pcount].s[scount].oldx = i->p[pcount].s[scount].x;
         i->p[pcount].s[scount].oldy = i->p[pcount].s[scount].y;

         // x
         if (i->p[pcount].s[scount].x > SCREEN_W)
           i->p[pcount].s[scount].x = 0;
         else if (i->p[pcount].s[scount].x < 0)
           i->p[pcount].s[scount].x = SCREEN_W;
         // y
         if (i->p[pcount].s[scount].y > SCREEN_H)
           i->p[pcount].s[scount].y = 0;
         else if (i->p[pcount].s[scount].y < 0)
           i->p[pcount].s[scount].y = SCREEN_H;
      }

      // player heading
      if (i->p[pcount].heading > 2*M_PI)
        i->p[pcount].heading -= 2*M_PI;
      else if (i->p[pcount].heading < 0)
        i->p[pcount].heading += 2*M_PI;
  
      // player engine power
      if (i->p[pcount].engine_power > i->max_power)
        i->p[pcount].engine_power = i->max_power;
      else if (i->p[pcount].engine_power < i->min_power)
        i->p[pcount].engine_power = i->min_power;
  
      // apply other factors
  
      // calculate x and y acceleration
      if (i->vehicle_mass)
        acceleration = i->p[pcount].engine_power / i->vehicle_mass
                        - i->p[pcount].speed * i->level.friction;
      else
        acceleration = i->p[pcount].engine_power
                        - i->p[pcount].speed * i->level.friction;
      acceleration_x = acceleration * cos(i->p[pcount].heading);
      acceleration_y = acceleration * sin(i->p[pcount].heading)
  //                     - i->level.gravity
                       ;
  
      // calculate the current x and y velocities
      vel_x = i->p[pcount].speed * cos(i->p[pcount].heading);
      vel_y = i->p[pcount].speed * sin(i->p[pcount].heading);
  
      // apply acceleration to them
      vel_y += acceleration_y;
      vel_x += acceleration_x;
  
      // re-store as a speed
      i->p[pcount].speed = sqrt(vel_x*vel_x + vel_y*vel_y);
  
      // apply the x and y velocities to the coodinates
      if (i->p[pcount].spin) {
        i->p[pcount].x += vel_x + i->p[pcount].crash_vel_x;
        i->p[pcount].y += vel_y - i->level.gravity;
      }
      else {
        i->p[pcount].x += vel_x;
        i->p[pcount].y += vel_y;
      }
  
  
      // things that wear off
  
      // invincibility
      if (i->p[pcount].invincible > 0)
        i->p[pcount].invincible--;
      if (i->p[pcount].invincible < 0)
        i->p[pcount].invincible++;
  
      // spins
      if (i->p[pcount].spin > 0) {
        i->p[pcount].heading += 2*i->turn_speed;
        i->p[pcount].spin--;
      }
      if (i->p[pcount].spin < 0) {
        i->p[pcount].heading -= 2*i->turn_speed;
        i->p[pcount].spin++;
      }
  
      // 'housekeeping'
      // when a player's energy is a negative value
      if (i->p[pcount].energy < 0) {
        i->p[pcount].energy = 0;
        i->p[pcount].crash_vel_x = (i->p[pcount].speed * cos(i->p[pcount].heading) / 4);
      }
      if (!i->p[pcount].energy) {
        i->p[pcount].spin = 127;
        explosion(i, i->p[pcount].x, i->p[pcount].y, i->vehicle_radius/3, 0.2, 1);

        if (i->p[pcount].y > SCREEN_H) {
          // the player has crashed on the bottom of the screen -- blow them up!!
          explosion(i, i->p[pcount].x, i->p[pcount].y, i->vehicle_radius*4, 2, 0);
          i->p[pcount].dead = 1;
          i->p[pcount].spin = 0;
        }
      }
  
  
      // SHOTS
      // calculate the current x and y velocities
      for (scount = 0; scount != i->num_of_shots; scount++) {
         if (i->p[pcount].s[scount].life) {
           vel_x = i->p[pcount].s[scount].speed * cos(i->p[pcount].s[scount].heading);
           vel_y = i->p[pcount].s[scount].speed * sin(i->p[pcount].s[scount].heading);
           i->p[pcount].s[scount].x += vel_x;
           i->p[pcount].s[scount].y += vel_y;
  
           i->p[pcount].s[scount].life--;
           if (i->p[pcount].s[scount].life >= 255)
             i->p[pcount].s[scount].life = 0;
         }
      }

    }
 }

 // advance explosion states
 eptr = i->eend;
 while (eptr != NULL) {
   if (eptr->type == -1)
     eptr->type = -2;
   else if (eptr->current_radius <= 0.0)
     eptr->type = -1;
   else if (eptr->current_radius > eptr->max_radius)
     if (eptr->type == 0)
       eptr->type = -1;
     else if (eptr->type == 1) {
       eptr->current_radius = (float)eptr->max_radius;
       eptr->speed *= -1;
     }

   eptr->current_radius += eptr->speed;

   eptr = eptr->link;
 }

 // free any unused explosion structures
 eptr = i->eend;
 while (eptr != NULL && eptr->link != NULL) {
   if (eptr->link->type == -2) {
     free(eptr->link);
     eptr->link = eptr->link->link;
   }
   eptr = eptr->link;
 }

 // advance bit states
 bptr = i->bend;
 while (bptr != NULL) {
   bptr->oldx = bptr->x;
   bptr->oldy = bptr->y;

   bptr->x += bptr->velx;
   bptr->y += bptr->vely;
   bptr->vely -= i->level.gravity/10;

   if (bptr->life >= 0)
     bptr->life--;

   if (bptr->x > SCREEN_W)
     bptr->x = 0.0;
   else if (bptr->x < 0.0)
     bptr->x = SCREEN_W;
   if (bptr->y > SCREEN_H) {
     bptr->y = SCREEN_H;
     bptr->vely *= -1;
   }
   else if (bptr->y < 0.0) {
     bptr->y = 0.0;
     bptr->vely *= -1;
   }

   bptr = bptr->link;
 }

 // free any unused bit structures
 bptr = i->bend;
 while (bptr != NULL && bptr->link != NULL) {
   if (bptr->link->life < 0) {
     free(bptr->link);
     bptr->link = bptr->link->link;
   }
   bptr = bptr->link;
 }

}
