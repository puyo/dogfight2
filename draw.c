// DOGFIGHT 2
// Gregory McIntyre
// 1998

// game drawing functions

#include "allegro.h"

#include "structs.h"
#include "colour.h"
#include "datafile/dog2.h"


#define M_PI  3.14159265358979323846


// *** FUNCTION PROTOTYPES *****************************************

void redraw(struct GameInfo *, unsigned char);
void draw_vehicles(struct GameInfo *);
void draw_shots(struct GameInfo *);
void draw_stats(struct GameInfo *);
void draw_explosions(struct GameInfo *);
void draw_bits(struct GameInfo *);


// *** FUNCTION DEFINITIONS ****************************************

void redraw(struct GameInfo *i, unsigned char drects)
{
 // redraw the screen
 struct Bit *bptr = i->bend;
 struct Explosion *eptr = i->eend;
 unsigned char pcount;
 unsigned char scount;
 int col;

 // draw the backdrop over the buffer
 blit(i->level.backdrop, i->buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

 draw_vehicles(i);
 draw_shots(i);
 draw_explosions(i);
 draw_stats(i);

 vsync();
 if (!drects) {
   draw_bits(i);
   // write the whole buffer to the screen
   blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
 }
 else {
   // planes
   for (pcount = 0; pcount != i->players; pcount++) {
      // x, y
      blit(i->buffer, screen, i->p[pcount].x - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].y - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].x - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].y - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].buffer->w + 8,
                              i->p[pcount].buffer->w + 8);
      // oldx, oldy
      blit(i->buffer, screen, i->p[pcount].oldx - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].oldy - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].oldx - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].oldy - i->p[pcount].buffer->w/2 - 4,
                              i->p[pcount].buffer->w + 8,
                              i->p[pcount].buffer->w + 8);

      // shots
      for (scount = 0; scount != i->num_of_shots; scount++) {
         // x, y
         blit(i->buffer, screen, i->p[pcount].s[scount].x - i->shot->w,
                                 i->p[pcount].s[scount].y - i->shot->w,
                                 i->p[pcount].s[scount].x - i->shot->w,
                                 i->p[pcount].s[scount].y - i->shot->w,
                                 i->shot->w*2,
                                 i->shot->w*2);
         // oldx, oldy
         blit(i->buffer, screen, i->p[pcount].s[scount].oldx - i->shot->w,
                                 i->p[pcount].s[scount].oldy - i->shot->w,
                                 i->p[pcount].s[scount].oldx - i->shot->w,
                                 i->p[pcount].s[scount].oldy - i->shot->w,
                                 i->shot->w*2,
                                 i->shot->w*2);

      }

      // explosions
      while (eptr != NULL) {
        blit(i->buffer, screen, eptr->x - eptr->max_radius - 2,
                                eptr->y - eptr->max_radius - 2,
                                eptr->x - eptr->max_radius - 2,
                                eptr->y - eptr->max_radius - 2,
                                eptr->max_radius*2 + 4,
                                eptr->max_radius*2 + 4);
        eptr = eptr->link;
      }


      // bits
      while (bptr != NULL) {
        if (bptr->life > 0)
          putpixel(screen, bptr->x, bptr->y, bptr->colour);

        col = getpixel(i->buffer, bptr->oldx, bptr->oldy);

        if (col >= 0)
          putpixel(screen, bptr->oldx, bptr->oldy, col);

        bptr = bptr->link;
      }

   }

   // stats
   blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H/64 + 16 + 2);

 }
}



void draw_vehicles(struct GameInfo *i)
{
 // draw the vehicles on the buffer

 unsigned char pcount;
 long int angle256;

 for (pcount = 0; pcount != i->players; pcount++) {
    if (!i->p[pcount].dead) {
      // collision detection circle
      //circle(i->buffer, i->p[pcount].x, i->p[pcount].y, i->vehicle_radius, 254);

      if ((i->p[pcount].invincible < 0 && i->p[pcount].invincible % 3 != -1)
       || !i->p[pcount].invincible) {
        angle256 = ftofix((float)(i->p[pcount].heading * 128/M_PI));
        rotate_sprite(i->buffer, i->p[pcount].buffer,
                      i->p[pcount].x - i->p[pcount].buffer->w/2,
                      i->p[pcount].y - i->p[pcount].buffer->h/2,
                      angle256);
      }
    }
 }
}



void draw_shots(struct GameInfo *i)
{
 // draw the shots on the buffer

 unsigned char colour;
 unsigned char pcount, scount;
 long int angle256;

 for (pcount = 0; pcount != i->players; pcount++) {
    for (scount = 0; scount != i->num_of_shots; scount++) {
       if (i->p[pcount].s[scount].life) {
         // copy the default shot sprite to a temporary bitmap
         blit(i->shot, i->tempshot, 0, 0, 0, 0, i->tempshot->w, i->tempshot->h);
         // paint the shot a new colour
         colour = (unsigned char)(i->p[pcount].s[scount].life % 7);
         paint(i->tempshot, COL1 + PAL_DIFF * colour);

         angle256 = ftofix((float)(i->p[pcount].s[scount].heading * 128/M_PI));
         rotate_sprite(i->buffer, i->tempshot,
                       i->p[pcount].s[scount].x - i->shot->w/2,
                       i->p[pcount].s[scount].y - i->shot->h/2,
                       angle256);
       }
    }
 }
}



void draw_stats(struct GameInfo *i)
{
 // draw the player information on the buffer

 //struct Explosion *eptr = i->eend;
 //struct Bit *bptr = i->bend;

 unsigned char pcount, kcount;
 int meter_len, meter_start;
 float power_dif;
 char valstr[16];
 char kstr[16];

 //power_dif = i->max_power - i->min_power;
 power_dif = i->max_energy;

 meter_len = SCREEN_W/i->players;
 for (pcount = 0; pcount != i->players; pcount++) {

    meter_start = pcount * meter_len;

    // frame
    rect(i->buffer, meter_start, SCREEN_H / 320,
                    meter_start + meter_len-1, SCREEN_H / 64,
                    32 + pcount * PAL_DIFF);

    meter_start += 1;

    // meter in frame
    rectfill(i->buffer, meter_start,
             SCREEN_H / 320 + 1,
             meter_start + (meter_len-2) * i->p[pcount].energy/i->max_energy,
             SCREEN_H / 64 - 1,
             38 + pcount * PAL_DIFF);

    // cold hard values printed over meter
    sprintf(valstr, "%d", i->p[pcount].energy);
    textout_ex(i->buffer, font, valstr,
            (SCREEN_W/i->players) * pcount + 2, SCREEN_H / 320, 47 + pcount*PAL_DIFF, -1);

    // shot information (debugging)
    //for (scount = 0; scount != i->num_of_shots; scount++) {
    //   sprintf(valstr, "%3d", i->p[pcount].s[scount].life);
    //   textout_ex(i->buffer, font, valstr,
    //        (SCREEN_W/i->players) * pcount + 24*scount + 2, SCREEN_H / 320 + 16, 31, -1);
    //}

    // kills information
    for (kcount = 0; kcount != i->players; kcount++) {
       sprintf(kstr, "%d", i->p[pcount].k[kcount]);
       textout_ex(i->buffer, font, kstr,
               pcount * SCREEN_W/i->players + kcount*24, SCREEN_H/64 + 8 + 2,
               PAL_DIFF*(kcount+2)+15, -1);
    }
 }

 // frames per second
 sprintf(valstr, "%3.2f FPS", i->fps);
 textout_ex(i->buffer, (FONT *)i->data[SMALL_FONT].dat, valstr, 0, SCREEN_H / 64 + 2, -1, -1);

 /*
 // number of explosions (debugging)
 eptr = i->eend;
 meter_len = 0;
 while (eptr != NULL) {
   meter_len++;
   eptr = eptr->link;
 }

 sprintf(valstr, "%3d explosions", meter_len);
 textout_ex(i->buffer, (FONT *)i->data[SMALL_FONT].dat, valstr, 100, SCREEN_H / 64 + 2, -1, -1);
 */

 /*
 // number of bits (debugging)
 bptr = i->bend;
 meter_len = 0;
 while (bptr != NULL) {
   meter_len++;
   bptr = bptr->link;
 }
 sprintf(valstr, "%3d bits", meter_len);
 textout_ex(i->buffer, (FONT *)i->data[SMALL_FONT].dat, valstr, 100, SCREEN_H / 64 + 2, -1, -1);
 */
}



void draw_explosions(struct GameInfo *i)
{
 struct Explosion *eptr = i->eend;
 BITMAP *explosion;
 int n;

 color_map = &i->light_table;

 while (eptr != NULL) {
   if (eptr->type >= 0) {
     explosion = create_bitmap((int)eptr->current_radius*3, (int)eptr->current_radius*3);
     clear_to_color(explosion, 255);

     for (n = (int)eptr->current_radius; n != 0; n--)
       circlefill(explosion, explosion->w/2, explosion->h/2, n, n*128/(int)eptr->current_radius);

     if (eptr->type == 0)
       circlefill(explosion, explosion->w/2 - (int)eptr->current_radius/6,
                             explosion->h/2 - (int)eptr->current_radius/6,
                             (int)(eptr->current_radius*eptr->current_radius * 7/6/eptr->max_radius), 255);

     draw_trans_sprite(i->buffer, explosion, eptr->x - explosion->w/2,
                                       eptr->y - explosion->h/2);
     destroy_bitmap(explosion);
   }

   eptr = eptr->link;
 }

}



void draw_bits(struct GameInfo *i)
{
 struct Bit *bptr = i->bend;

 while (bptr != NULL) {
   if (bptr->life)
     putpixel(i->buffer, bptr->x, bptr->y, bptr->colour);
   bptr = bptr->link;
 }
}
