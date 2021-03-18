/*
    factorial

    gcc -DDEBUG -Wall -o factorial factorial.c
    gcc -Wall -O3 -o factorial factorial.c
*/

#include <stdio.h>   // printf
#include <libgen.h>  // basename
#include <stdlib.h>  // atoi

#if defined(DEBUG) && DEBUG > 0
    #define show_debug(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
        __FILE__, __LINE__, __func__, ##args)
#else
    /* show_debug shows nothing when DEBUG undefined */
    #define show_debug(fmt, args...)
#endif

long int factorial(long int n) {
    show_debug("n == %ld\n",n);
    if ( n > 1 )
        return n * factorial( n-1 );
    else
        return 1;
}

int main(int argc, char **argv) {
    int numargs;  // number of command line arguments
    int num;      // command line argument, hopefully
    show_debug("starting\n");

    numargs = argc - 1;
    show_debug("numargs == %d\n",numargs);
    if (numargs != 1) {
        printf( "Usage: %s <num>\n", basename(argv[0]) );
        return 1;
    }
    
    num = atoi((char *)argv[1]);
    show_debug("num == %d\n", num);
    printf("%ld\n", factorial(num));
    show_debug("ending\n");
    return 0; 
}
