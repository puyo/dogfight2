// Random number functions
// Gregory McIntyre
// 1998

#include <stdio.h>
#include <math.h>
#include <time.h>


void seed_random();
int rand_int(int, int);
int circ_rand_int(int, int, int);
int grad_rand_int(int, int, int);


void seed_random()
{
 srandom(time(NULL));
}



int rand_int(int min, int max)
{
 return min + (random() % (max - min + 1));
}



int circ_rand_int(int min, int max, int circ)
{
 float r;
 float mid = (max + min)/2;
 int count;

 r = (random() % (int)(mid - min)) + 1;

 for (count = 0; count != circ; count++)
   r *= sin((double)((random() % 1000) + 1) * M_PI/1000);

 r = mid + r*cos((double)((random() % 1000) + 1) * M_PI/1000);

 return (int)r;
}



int grad_rand_int(int min, int max, int grad)
{
 float r;
 float mid = (max + min)/2;
 int count;

 r = (random() % (int)(max - min)) + 1;

 for (count = 0; count != grad; count++)
   r *= sin((double)((random() % 1000) + 1) * M_PI/1000);

 r = min + r*sin((double)((random() % 1000) + 1) * M_PI/1000);

 return r;
}
