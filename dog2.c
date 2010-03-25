// DOGFIGHT 2
// Gregory McIntyre
// 1998

// main function file

#include <stdio.h>
#include "allegro.h"

#include "structs.h"



// *** MAIN FUNCTION ***********************************************

int main(void)
{
 unsigned char choice;
 struct GameInfo i;

 init_info(&i);

 do {
   choice = title_page(&i);

   if (choice == 1)
     game(&i);
   else if (choice == 2)
     options_page(&i);

 } while (choice);

 finishup(&i);
 exit(0);
}

END_OF_MAIN();
