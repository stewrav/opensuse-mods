/*
	seq.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

int main(int argc, char **argv)
{
	int i = 0;
	int lbound, hbound;
	char *progname, *dup0;

	if(!(argc == 3))
	{
		/* Wrong number of arguments */
		dup0=strdup(argv[0]);
		progname=basename(dup0);
		fprintf(stderr, "Usage: %s low-bound high-bound\ne.g. %s 1 5\n", progname, progname);
		exit(1);
	}

	/* Increment from low-bound to high-bound */
	lbound = atoi((char *)argv[1]);
	hbound = atoi((char *)argv[2]);
	for (i = lbound; i <= hbound; i++) {
		printf("%d\n", i);
	}

	return 0;
}
