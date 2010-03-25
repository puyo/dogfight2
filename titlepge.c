// DOGFIGHT 2
// Gregory McIntyre
// 1998

// title page functions

#include "allegro.h"

#include "structs.h"
#include "datafile/dog2.h"


#define NUM_OPTIONS  3
#define MENU_Y       (SCREEN_H*9/10 - NUM_OPTIONS*32)



// *** STRUCTURE DEFINITIONS ***************************************

typedef struct MenuInfo
{
 unsigned char highlight;
 unsigned char chosen;
} MenuInfo;



// *** FUNCTION PROTOTYPES *****************************************

unsigned char title_page(struct GameInfo *);
void redraw_menu(BITMAP *, DATAFILE *, struct MenuInfo *);



// *** FUNCTION DEFINITIONS ****************************************

unsigned char title_page(struct GameInfo *i)
{
 PALETTE pal;
 MenuInfo titlemenu;
 int c;

 titlemenu.highlight = 1;
 titlemenu.chosen = 0;

 // set the transparancy table and palette
 clear(screen);
 //set_palette(i->data[TITLE_PAL].dat);
 set_palette(black_palette);
 color_map = (COLOR_MAP *)i->data[TITLE_TRANS_TABLE].dat;

 // draw the backdrop titlepage
 stretch_blit((BITMAP *)i->data[TITLE_BACKDROP].dat, i->buffer,
              0, 0, 640, 480, 0, 0, SCREEN_W, SCREEN_H);
 redraw_menu(i->buffer, i->data, &titlemenu);
 blit(i->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

 fade_in(i->data[TITLE_PAL].dat, 6);

 // main loop - only refreshes the menu
 do {
   redraw_menu(i->buffer, i->data, &titlemenu);

   // change menu item highlighted
   clear_keybuf();
   c = readkey() >> 8;
   clear_keybuf();
   if (c == KEY_DOWN && titlemenu.highlight < NUM_OPTIONS)
     titlemenu.highlight++;
   else if (c == KEY_UP && titlemenu.highlight > 1)
     titlemenu.highlight--;
   else if (c == KEY_ENTER) {
     // flashy animation?
     titlemenu.chosen = titlemenu.highlight;
   }
   else if (c == KEY_ESC)
     titlemenu.chosen = NUM_OPTIONS;
 } while (!titlemenu.chosen);

 // set the chosen option such that 0 will represent
 // exit regardless of how many options there are
 if (titlemenu.chosen == 3)
   titlemenu.chosen = 0;

 return titlemenu.chosen;
}



void redraw_menu(BITMAP *buffer, DATAFILE *data,
                 struct MenuInfo *titlemenu)
{

 // copy the background over the menu to redraw it
 stretch_blit((BITMAP *)data[TITLE_BACKDROP].dat, buffer,
              640/6, 480/2,
              640 * 5/6, 480,
              SCREEN_W/6, SCREEN_H/2,
              SCREEN_W * 5/6, SCREEN_H);


 drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

 // draw the cursor/highlight the cursor menu item
 rectfill(buffer, SCREEN_W/4,
                  MENU_Y + (titlemenu->highlight - 1) * 32 - 5,
                  SCREEN_W * 3/4,
                  MENU_Y + (titlemenu->highlight - 1) * 32 + 27,
                  3);
 drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

 // print the other options
 textout_centre_ex(buffer, (FONT *)data[BIG_FONT].dat, "Start Game",
                SCREEN_W/2, MENU_Y, -1, -1);
 textout_centre_ex(buffer, (FONT *)data[BIG_FONT].dat, "Options",
                SCREEN_W/2, MENU_Y + 32, -1, -1);
 textout_centre_ex(buffer, (FONT *)data[BIG_FONT].dat, "Exit",
                SCREEN_W/2, MENU_Y + 64, -1, -1);

 blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}
