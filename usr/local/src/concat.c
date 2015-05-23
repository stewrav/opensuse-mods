/*
	concat.c by Stewart Ravenhall, MSD/IET

	Concatenates command line argument files into an output file
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *file_ptr;
	FILE *outfile_ptr;
	int i, c, argc_1 = argc - 1;

	if(argc <= 3)
	/* Command line with too few arguments */
	{
		fprintf(stderr,"%s: Program by Stewart Ravenhall\n        Concatenates files\nUsage:  %s file1 file2... outfile\n", argv[0], argv[0]);
		exit(1);
	}

	for(i = 1; i < argc_1; i++)
	/* Loop through all arguments except last one */
	{
		if(!(file_ptr=fopen(argv[i],"rb")))
		/* Cannot open file */
		{
			fprintf(stderr,"%s: could not read %s\n", argv[0], argv[i]);
			exit(2);
		}
		fclose(file_ptr);
	}

	/* Check output file */
	if(!(outfile_ptr=fopen(argv[argc_1],"wb")))
	/* Cannot write to output file */
	{
		fprintf(stderr,"%s: could not write to %s\n", argv[0], argv[argc_1]);
		exit(3);
	}

	for(i = 1; i < argc_1; i++)
	/* Loop through each file */
	{
		file_ptr=fopen(argv[i],"rb");
		while((c = fgetc(file_ptr)) != EOF) fputc(c,outfile_ptr);
		fclose(file_ptr);
	}
	fclose(outfile_ptr);
	return 0;
}
