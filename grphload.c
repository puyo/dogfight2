// DOGFIGHT 2
// Gregory McIntyre
// 1998

// graphics loading functions

#include <string.h>
#include "allegro.h"

#include "structs.h"
#include "colour.h"


// *** FUNCTION PROTOTYPES *****************************************

unsigned char load_backdrop(struct GameInfo *);
void load_next_backdrop(struct GameInfo *);
unsigned char load_vehicle(struct GameInfo *);
void load_next_vehicle(struct GameInfo *);
unsigned char load_shot(struct GameInfo *);
void load_next_shot(struct GameInfo *);


// *** FUNCTION DEFINITIONS ****************************************

unsigned char load_backdrop(struct GameInfo *i)
{
 // loads i->backdrop_name (pcx file) into
 // i->backdrop (memory bitmap)
 // accounts for errors such as the file not existing,
 // the pcx file being any size, etc.

 PALETTE pal;
 BITMAP *temp;
 char filename[80];

 strcpy(filename, "backdrop/");
 strcat(filename, i->backdrop_name);

 if (file_exists(filename, 0, NULL)) {
   // load the backdrop into the level backdrop buffer
   temp = load_pcx(filename, pal);
   stretch_blit(temp, i->level.backdrop,
                0, 0, temp->w, temp->h,
                0, 0, i->level.backdrop->w, i->level.backdrop->h);
   destroy_bitmap(temp);
 }
 else
   return 1;

 return 0;
}



void load_next_backdrop(struct GameInfo *i)
{
 // given an existing backdrop filename, loads the
 // next pcx file in the backdrop directory into
 // the i->backdrop memory bitmap
 // ie. cycles through possible backdrops
/*
 struct ffblk f;

 // initiate the findfirst()/findnext() functions to start
 // at the currently selected backdrop
 findfirst("backdrop/*.pcx", &f, 0);
 while (stricmp(i->backdrop_name, f.ff_name))
   findnext(&f);

 // find the next backdrop
 // iterate if the end of the directory is reached
 if (findnext(&f))
   findfirst("backdrop/*.pcx", &f, 0);
 strcpy(i->backdrop_name, strlwr(f.ff_name));

 load_backdrop(i);
*/
}



unsigned char load_vehicle(struct GameInfo *i)
{
 // same as load_backdrop, only for vehicles

 PALETTE pal;
 BITMAP *temp;
 BITMAP *vehicle;
 unsigned char count;
 char filename[80];

 strcpy(filename, "vehicle/");
 strcat(filename, i->vehicle_name);

 if (file_exists(filename, 0, NULL)) {
   vehicle = create_bitmap(i->p[0].buffer->w, i->p[0].buffer->h);

   // stretch the file bitmap across a temporary memory bitmap
   temp = load_pcx(filename, pal);
   stretch_blit(temp, vehicle, 0, 0, temp->w, temp->h,
                               0, 0, vehicle->w, vehicle->h);
   destroy_bitmap(temp);

   // copy the temp bitmap as a sprite, to all the player
   // buffers appropriately
   for (count = 0; count != 4; count++) {
      clear(i->p[count].buffer);
      if (i->p[count].flip)
        draw_sprite_v_flip(i->p[count].buffer, vehicle, 0, 0);
      else
        draw_sprite(i->p[count].buffer, vehicle, 0, 0);

      // colour the vehicles
      paint(i->p[count].buffer, COL1 + count*PAL_DIFF);
   }
   destroy_bitmap(vehicle);
 }
 else
   return 1;

 return 0;
}



void load_next_vehicle(struct GameInfo *i)
{
 // same as load_next_backdrop, only for vehicles
/*
 struct ffblk f;

 // initiate the findfirst()/findnext() functions to start
 // at the currently selected backdrop
 findfirst("vehicle/*.pcx", &f, 0);
 while (stricmp(i->vehicle_name, f.ff_name))
   findnext(&f);

 // find the next backdrop
 // iterate if the end of the directory is reached
 if (findnext(&f))
   findfirst("vehicle/*.pcx", &f, 0);
 strcpy(i->vehicle_name, strlwr(f.ff_name));

 load_vehicle(i);
*/
}



unsigned char load_shot(struct GameInfo *i)
{
 // same as load_backdrop, only for shots

 PALETTE pal;
 BITMAP *temp;
 char filename[80];

 strcpy(filename, "shot/");
 strcat(filename, i->shot_name);

 if (file_exists(filename, 0, NULL)) {
   // load the shot into the shot buffer
   temp = load_pcx(filename, pal);
   stretch_blit(temp, i->shot,
                0, 0, temp->w, temp->h,
                0, 0, i->shot->w, i->shot->h);
   destroy_bitmap(temp);
 }
 else
   return 1;

 return 0;
}



void load_next_shot(struct GameInfo *i)
{
 // same as load_next_backdrop, only for shots
/*
 struct ffblk f;

 // initiate the findfirst()/findnext() functions to start
 // at the currently selected backdrop
 findfirst("shot/*.pcx", &f, 0);
 while (stricmp(i->shot_name, f.ff_name))
   findnext(&f);

 // find the next backdrop
 // iterate if the end of the directory is reached
 if (findnext(&f))
   findfirst("shot/*.pcx", &f, 0);
 strcpy(i->shot_name, strlwr(f.ff_name));

 load_shot(i);
*/
}
