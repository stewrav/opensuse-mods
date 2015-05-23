/*
$Source: /opt/BIOSTAT/razordb/RAZOR_UNIVERSE/DOMAIN_01/system/Archive/RZ_VCS/src/filetime.c,v $
$Id: filetime.c,v 4.1 2014-09-13 13:47:23+02 ravenhas Exp $

Get the last modification time of specified files.
Please remember to update the version number in the usage note if you update the code.

-----------------------------------------------------------------
Flags / command line options:
-----------------------------------------------------------------
 -h  : help    : show help and then exit
 -L  : Links   : show results for symlink instead of following it
 -e  : epoch   : show number of seconds since 1 Jan 1970
 -s  : seconds : show the seconds
 -v  : verbose : show the results in a user friendly format
-----------------------------------------------------------------

How the options work in combination:

epoch seconds verbose -> results
  -e     -s      -v
-----------------------------------------------------------------
  0      0       0    yyyymmddhhmm (this is what "touch" expects)
  0      0       1    yyyy-mm-dd hh:mm zz
  0      1       0    yyyymmddhhmmss
  0      1       1    yyyy-mm-dd hh:mm:ss zz
  1     (0)      0    123456789
  1      0       1    123456789 yyyy-mm-dd hh:mm zz
  1     (1)      0    123456789
  1      1       1    123456789 yyyy-mm-dd hh:mm:ss zz
-----------------------------------------------------------------

To compile on HP-UX:
	gcc -Wall -O3 -mlp64 filetime.c -o filetime
	strip filetime
	file filetime

To compile on Linux:
	gcc -Wall -O3 filetime.c -o filetime
	strip filetime
	file filetime
*/

#include <libgen.h>         // for basename
#include <string.h>         // for strerror
#include <stdio.h>          // for printf
#include <sys/stat.h>       // for stat and lstat
#include <unistd.h>         // for getopt
#include <time.h>           // for localtime

#ifdef __hpux
	#include <stdlib.h>     // for errno on HP-UX

	#define _POSIX_C_SOURCE 200112L
	#define _FILE_OFFSET_BITS 64
	#define _LARGEFILE_SOURCE 1
	#define _GNU_SOURCE 1
#else
	#ifdef linux
		#include <errno.h>  // for errno on Linux
	#endif
#endif

int main(int argc, char **argv) {

	int int_index;             // Loop index
	int int_numfiles;          // Count of the number of files

	int int_options;           // For getting options
	extern char *optarg;       // Used by getopt
	extern int optind, optopt; // Used by getopt

	int int_main_rc   = 0;     // Default to main return code zero
	int int_verbose   = 0;     // Default to not being verbose
	int int_seconds   = 0;     // Default to not showing seconds
	int int_epoch     = 0;     // Default to not returning epoch time
	int int_showlinks = 0;     // Default to following not showing links

	int    int_stat_rc;        // Return code from stat or lstat
	struct stat stat_buf;      // Used by strftime
	struct tm   tm_buf;        // Used by strftime
	char   char_time[30];      // Used by strftime

	char char_usage[40] = "Usage: %s [-hvseL] file1 [file2] [...]\n";

	/* Get command line options */
	while ( (int_options = getopt(argc, argv, "hvseL")) != -1 ) {

		switch (int_options) {
			case 'v':
				int_verbose = 1;
			break;

			case 's':
				int_seconds = 1;
			break;

			case 'e':
				int_epoch = 1;
			break;

			case 'L':
				int_showlinks = 1;
			break;

			case 'h':
				/* Show version number */
				fprintf( stderr, "%s version 4.1\n\n", basename(argv[0]) );

				/* Show usage note */
				fprintf( stderr, char_usage, basename(argv[0]) );

				/* Show additional help */
				fprintf( stderr, "  -h : show this help note\n" );
				fprintf( stderr, "  -v : show results more verbosely\n" );
				fprintf( stderr, "  -s : show seconds\n" );
				fprintf( stderr, "  -e : show UNIX epoch time\n" );
				fprintf( stderr, "  -L : show symlink instead of following it\n" );
				return 1;
			break;
		}

	}  // end while getopt

	/* The number of files is equal to the number of arguments
	   minus the number of options */
	int_numfiles = argc - optind;

	/* If no files were specified then output a usage note */
	if( int_numfiles == 0 ) {
		fprintf( stderr, char_usage, basename(argv[0]) );
		return 1;
	}

	/* Loop through each file specified */
	for( int_index = optind; int_index < argc; int_index++ ) {

		/* Write a pointer to the stat (or link stat) info in the stat buffer */
		if( int_showlinks == 0 ) {
			int_stat_rc = stat( argv[int_index], &stat_buf );
		}
		else {
			int_stat_rc = lstat( argv[int_index], &stat_buf );
		}

		/* If stat or lstat returned zero (good) */
		if( int_stat_rc == 0 ) {

			/* Extract the last modification time from the stat buffer (stat_buf)
			   and store a pointer to it in the time buffer (tm_buf) */
			tm_buf = *localtime(&stat_buf.st_mtime);

			/* Make local time pretty and store it in the char_time character array */
			if( int_verbose == 1 ) {
				if( int_seconds == 1 ) {
					strftime( char_time, sizeof(char_time), "%Y-%m-%d %H:%M:%S %Z", &tm_buf );
				}
				else {
					strftime( char_time, sizeof(char_time), "%Y-%m-%d %H:%M %Z", &tm_buf );
				}
			}

			else {
				if( int_seconds == 1 ) {
					strftime( char_time, sizeof(char_time), "%Y%m%d%H%M%S", &tm_buf );
				}
				else {
					/* Important that this matches the format expected by "touch" */
					strftime( char_time, sizeof(char_time), "%Y%m%d%H%M", &tm_buf );
				}
			}

			/* If more than one file was specified then
			   prefix each result with the file name and a tab */
			if( int_numfiles > 1 )
				fprintf( stdout, "%s\t", argv[int_index] );

			/* Show epoch time if "-e" was specified */
			if( int_epoch == 1 )
				fprintf( stdout, "%d", (int) stat_buf.st_mtime );

			/* Need a space if we have multiple files, or if we have epoch & verbose */
			if( int_numfiles > 1 || int_verbose + int_epoch == 2 )
				fprintf( stdout, " " );

			/* Show the last modification time in the chosen pretty format,
			   if there was one */
			if( int_epoch == 0 || int_verbose + int_epoch == 2 )
				fprintf( stdout, "%s", char_time );

			fprintf( stdout, "\n" );

		}
		/* Otherwise stat/lstat returned non-zero (bad) */
		else {

			/* Show error message received from operating system */
			fprintf( stderr, "%s: error parsing %s: %s\n",
				basename(argv[0]), argv[int_index], strerror(errno) );

			/* Set return code but do not exit from program */
			int_main_rc = 2;
		}
	}

	/* Return the appropriate return code */
	return int_main_rc;
}
