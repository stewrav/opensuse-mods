/*
List command line arguments, excluding the program name

gcc -Wall -O2 arg_list.c -o arg_list
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

int main(int argc, char **argv)
{
	char errflag = 0; /* Error flag          */
	int i;            /* Loop index          */
	int numargs;      /* Number of arguments */
	int c;         /* Character from options */
	int opt_a = 0;    /* Option a            */
	int opt_b = 0;    /* Option b            */
	int opt_f = 0;    /* Option f            */
	int opt_o = 0;    /* Option o            */
	int arg_f;  /* Specified argument with option f */
	int arg_o;  /* Specified argument with option o */
	extern char *optarg;
	extern int optind, optopt;

	numargs = argc - 1;

	while ((c = getopt(argc, argv, "abf:o:")) != -1) {
		switch (c) {
			case 'a':
				if (opt_b)
					errflag = 1;
				else
					opt_a++;
				break;
			case 'b':
				if (opt_a)
					errflag = 1;
				else
					opt_b++;
				break;
			case 'f':
				opt_f++;
				arg_f = optarg; /* Maybe should use strncpy (from string.h) instead */
				break;
			case 'o':
				opt_o++;
				arg_o = optarg;
				break;
			case ':':        /* -f or -o without arguments */
				fprintf(stderr, "Option -%c requires an argument\n", optopt);
				errflag = 1;
				break;
			case '?':
				fprintf(stderr, "Unrecognized option: - %c\n", optopt);
				errflag = 1;
		}

		if (errflag) {
			fprintf(stderr, "Usage: %s [-a|-b] [-f arg1] [-o arg2]\n", basename(argv[0]));
			exit (2);
		}
	}

	printf("Number of arguments is %d\n", numargs);
	printf("option a is %d\n",     opt_a);
	printf("option b is %d\n",     opt_b);
	printf("option f is %d\n",     opt_f);
	printf("\targument f is %s\n", arg_f);
	printf("option o is %d\n",     opt_o);
	printf("\targument o is %s\n", arg_o);

	printf("parameters are:\n");
	for (i = optind; i < argc; i++) printf("\t%s\n", argv[i]);
	return 0;
}
