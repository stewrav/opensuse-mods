/*
	sizeof.c by Stewart Ravenhall

	Finds the size of a file
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int c, i = 0;
	FILE *infile_ptr;

	if(!(argc == 2))
	{
		/* Wrong number of arguments */
		fprintf(stderr, "%s: program by Stewart Ravenhall\nUsage:  %s infile\n", argv[0], argv[0]);
		exit(1);
	}

	if(!(infile_ptr=fopen(argv[1], "rb")))
	{
		/* Unable to open file as read-only */
		fprintf(stderr, "%s: could not open %s\n", argv[0], argv[1]);
		exit(2);
	}

	while((c=fgetc(infile_ptr)) != EOF) i++;
	fclose(infile_ptr);
	fprintf(stdout, "%d\n", i);
	return 0;
}
