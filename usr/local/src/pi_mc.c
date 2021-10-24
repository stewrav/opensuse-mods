/*
  pi_mc : calculate pi using Monte Carlo estimation

  Do not forget the "-lm" at the end of the compiler command:
  gcc -Wall -O3 -o ./pi_mc ./pi_mc.c -lm
*/

#define VERSION "1.1"
#define ITER_MAX 9999999999

#include <stdio.h>    // printf
#include <stdlib.h>   // rand
#include <math.h>     // sqrt
#include <time.h>     // time

double r2() {
    return (double)rand() / (double)RAND_MAX ;
}

int main(int argc, char **argv) {

  int i = 0;    // iteration counter
  double x;     // x coordinate
  double y;     // y coordinate
  double hyp;   // hypotenuse (distance of x,y from origin)
  double piest; // pi estimate
  int in = 0;   // number of iterations inside the circle
  
  srand(time(NULL));  // Random number initialization, called once only
  
  while( i < ITER_MAX ) {
    i++;
    x = r2();
    y = r2();
    hyp = sqrt( (x*x) + (y*y) );
    if (hyp <= 1) {
      in++;
    }
    piest = ( (double)in / i ) * 4;
    printf("iter=%d x=%f y=%f hyp=%f in_count=%d pi=%.21f\n", i, x, y, hyp, in, piest);
  }
  return 0;
}
