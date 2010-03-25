// DOGFIGHT 2
// Gregory McIntyre
// 1998

// painting function

#include "allegro.h"

#include "colour.h"


// *** FUNCTION PROTOTYPES *****************************************

void paint(BITMAP *, unsigned char);



// *** FUNCTION DEFINITIONS ****************************************

void paint(BITMAP *sprite, unsigned char colour_new)
{
 // function checks each pixel of a bitmap and on finding
 // certain pixels, changes them to the given colour equivalent

 unsigned int x, y, colour_old, colour_equiv;

 for (y = 0; y <= sprite->h; y++) {
    for (x = 0; x <= sprite->w; x++) {
       colour_old = getpixel(sprite, x, y);
       if (colour_old >= COL1 && colour_old <= COL1 + PAL_DIFF) {
         // colour is able to be changed
         colour_equiv = colour_new + (colour_old-32);
         putpixel(sprite, x, y, colour_equiv);
       }
    }
 }

}
