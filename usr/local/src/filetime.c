/*
   filetime / filesize / inode / owner

   To compile with debug mode on:
   gcc -DDEBUG -Wall -o ./filetime ./filetime.c

   To compile for production use:
   gcc -Wall -O3 -o ./filetime ./filetime.c

   (1) If the executable is called "filetime", then
       show the last modification time of the specified files.
   (2) If the executable is called "filesize", then
       show the file size of the specified files, in bytes.
   (3) If the executable is called "inode", then
       show the inode number of the specified files.
   (4) If the executable is called "owner", then
       show the owner of the files.

TODO: filetime: update output to conform to Linux "touch -md" date string.
TODO: owner:    add "show user name not just user id number" option.

***
Please remember to update the VERSION number below if you update the code.
***

--------------------------------------------------------------------------------
Flags / command line options:
--------------------------------------------------------------------------------
 -h  : help      : show help and then exit
 -L  : Links     : show results for symlink instead of following it
 -n  : nofile    : do not show the file names even when more than one file
 -N  : Names     : show file names even if there is only one file
 -e  : epoch     : show number of seconds since 1 Jan 1970 (filetime only)
 -s  : seconds   : show the seconds (filetime only)
 -v  : verbose   : show results in a user friendly format (filetime only)
 -t  : timefirst : show time then date (not default date then time)
--------------------------------------------------------------------------------

To see how the options interact, see "optmode", below.
*/

#define VERSION "5.5"

#include <libgen.h>    // for basename
#include <string.h>    // for strerror, strcpy, etc
#include <stdio.h>     // for printf
#include <sys/stat.h>  // for stat and lstat
#include <unistd.h>    // for getopt
#include <time.h>      // for localtime
#include <errno.h>     // for errno on Linux

#if defined(DEBUG) && DEBUG > 0
#define show_debug(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
__FILE__, __LINE__, __func__, ##args)
#else
#define show_debug(fmt, args...) /* show_debug shows nothing when DEBUG undefined */
#endif

#define USAGE_SIZE 50
#define TIME_SIZE  40

int main(int argc, char **argv) {

int int_index;             // Loop index
int int_numfiles;          // Count of the number of files

int int_options;           // For getting options
extern char *optarg;       // Used by getopt
extern int optind, optopt; // Used by getopt

int int_main_rc   = 0;     // Default to main return code zero
int int_verbose   = 0;     // Default to not being verbose (not "-v")
int int_seconds   = 0;     // Default to not showing seconds (not "-s")
int int_epoch     = 0;     // Default to not returning epoch time (not "-e")
int int_showlinks = 0;     // Default to following links (not "-L" to show links)
int int_nofile    = 0;     // Option "-n" means no file names
int int_names     = 0;     // Option "-N" means file names
int int_shownames;         // Whether to show file names
int int_timefirst = 0;     // Default to not showing time first (not "-t")

int int_optmode;           // Combination of -e, -s, -v, & -t modes

int    int_stat_rc;        // Return code from stat or lstat
struct stat stat_buf;      // Used by strftime
struct tm tm_buf;          // Used by strftime
char   char_time[TIME_SIZE]; // Used by strftime

char char_usage[USAGE_SIZE]; // Usage note
long long ll_size = 0;       // The size of the file in bytes

show_debug( "DEBUG == %d\n",     (int) DEBUG );
show_debug( "argv[0] == %s\n",    argv[0]    );
show_debug( "VERSION == %s\n",    VERSION    );
show_debug( "USAGE_SIZE == %d\n", USAGE_SIZE );
show_debug( "TIME_SIZE == %d\n",  TIME_SIZE  );

/* Specify the usage note, based on what the executable is called */
if( strcmp( basename(argv[0]), "filetime" ) == 0 )
strcpy( char_usage, "Usage: %s [-hvsetLnN] file1 [file2] [...]\n" );
else if( strcmp( basename(argv[0]), "filesize" ) == 0 )
strcpy( char_usage, "Usage: %s [-hLnN] file1 [file2] [...]\n" );
else if( strcmp( basename(argv[0]), "inode" ) == 0 )
strcpy( char_usage, "Usage: %s [-hLnN] file1 [file2] [...]\n" );
else if( strcmp( basename(argv[0]), "owner" ) == 0 )
strcpy( char_usage, "Usage: %s [-hLnN] file1 [file2] [...]\n" );
else {
fprintf( stderr, "%s: did not expect to be called this!\n", basename(argv[0]) );
return 2;
}

/* Get command line options */
while( (int_options = getopt(argc, argv, "hvsetLnN")) != -1 ) {

switch(int_options) {
case 'v':
if( strcmp( basename(argv[0]), "filetime" ) != 0 ) {
fprintf( stderr, "%s: \"-v\" mode not valid.\n", basename(argv[0]) );
return 1;
}
int_verbose = 1;
break;

case 's':
if( strcmp( basename(argv[0]), "filetime" ) != 0 ) {
fprintf( stderr, "%s: \"-s\" mode not valid.\n", basename(argv[0]) );
return 1;
}
int_seconds = 1;
break;

case 'e':
if( strcmp( basename(argv[0]), "filetime" ) != 0 ) {
fprintf( stderr, "%s: \"-e\" mode not valid.\n", basename(argv[0]) );
return 1;
}
int_epoch = 1;
break;

case 't':
if ( strcmp( basename(argv[0]), "filetime" ) != 0 ) {
fprintf( stderr, "%s: \"-t\" mode not valid.\n", basename(argv[0]) );
return 1;
}
int_timefirst = 1;
break;

case 'L':
int_showlinks = 1;
break;

case 'h':
/* Show version number */
fprintf( stderr, "filetime/filesize/inode/owner version %s\n\n", VERSION);

/* Show usage note */
fprintf( stderr, char_usage, basename(argv[0]) );

/* Show additional help */
fprintf( stderr, "  -h : show this help note\n" );
if( strcmp( basename(argv[0]), "filetime" ) == 0 ) {
fprintf( stderr, "  -v : show results more verbosely\n" );
fprintf( stderr, "  -s : show seconds\n" );
fprintf( stderr, "  -e : show UNIX epoch time\n" );
fprintf( stderr, "  -t : show time before date\n" );
}
fprintf( stderr, "  -L : show symlink instead of following it\n" );
fprintf( stderr, "  -n : do not show file names (default if one file)\n" );
fprintf( stderr, "  -N : show file names (default if many files)\n" );

/* Show details */
if( strcmp( basename(argv[0]), "filetime" ) == 0 ) {
fprintf( stderr, "\n" );
fprintf( stderr, "How the options work in combination:\n" );
fprintf( stderr, "\n" );
fprintf( stderr, "-------------------------------------------------------------------------------\n" );
fprintf( stderr, "epoch seconds verbose timefirst -> results\n" );
fprintf( stderr, "  -e     -s      -v      -t\n"                );                                        // optmode
fprintf( stderr, "-------------------------------------------------------------------------------\n" );  // -------
fprintf( stderr, "   0      0       0       0        YYYYmmddHHMM (what \"touch -mt\" expects)\n" );  //    0
fprintf( stderr, "   0      0       0       1        HHMM dd.mm.YYYY (this is what \"at\" expects)\n" );  //    1
fprintf( stderr, "   0      0       1       0        YYYY-mm-dd HH:MM ZZ\n" );                            //    2
fprintf( stderr, "   0      0       1       1        HH:MM YYYY-mm-dd (this is what \"at\" expects)\n" ); //    3
fprintf( stderr, "   0      1       0       0        YYYYmmddHHMM.SS (what \"touch -mt\" expects)\n" );//   4
fprintf( stderr, "   0      1       0       1        HHMM.SS dd.mm.YYYY\n" );                             //    5
fprintf( stderr, "   0      1       1       0        YYYY-mm-dd HH:MM:SS ZZ\n" );                         //    6
fprintf( stderr, "   0      1       1       1        HH:MM:SS YYYY-mm-dd\n" );                            //    7
fprintf( stderr, "   1      0       0       0        123456789\n" );                                      //    8
fprintf( stderr, "   1      0       0       1        123456789\n" );                                      //    9
fprintf( stderr, "   1      0       1       0        123456789 YYYY-mm-dd HH:MM ZZ\n" );                  //   10
fprintf( stderr, "   1      0       1       1        123456789 HH:MM ZZ YYYY-mm-dd\n" );                  //   11
fprintf( stderr, "   1      1       0       0        123456789\n" );                                      //   12
fprintf( stderr, "   1      1       0       1        123456789\n" );                                      //   13
fprintf( stderr, "   1      1       1       0        123456789 YYYY-mm-dd HH:MM:SS ZZ\n" );               //   14
fprintf( stderr, "   1      1       1       1        123456789 HH:MM:SS ZZ YYYY-mm-dd\n" );               //   15
fprintf( stderr, "-------------------------------------------------------------------------------\n");
}
return 0;
break;

case 'n':
int_nofile = 1;  // Option "-n" means never show file names
break;

case 'N':
int_names = 1;   // Option "-N" means always show file names
break;

case '?':
return 1;
break;
}

}  // end while getopt

show_debug( "int_verbose   == %d\n", int_verbose   );
show_debug( "int_seconds   == %d\n", int_seconds   );
show_debug( "int_epoch     == %d\n", int_epoch     );
show_debug( "int_timefirst == %d\n", int_timefirst );
show_debug( "int_showlinks == %d\n", int_showlinks );
show_debug( "int_nofile    == %d\n", int_nofile    );
show_debug( "int_names     == %d\n", int_names     );

/* The number of files is equal to the number of arguments
  minus the number of options */
int_numfiles = argc - optind;
show_debug( "int_numfiles  == %d\n", int_numfiles  );

/* If no files were specified then output a usage note */
if( int_numfiles == 0 ) {
show_debug( "no files detected\n" );
fprintf( stderr, char_usage, basename(argv[0]) );
return 1;
}

/* Combination of -e, -s, -v, & -t modes */
int_optmode = (int_epoch << 3) + (int_seconds << 2) + (int_verbose << 1) + int_timefirst;
show_debug( "int_optmode   == %d\n", int_optmode );

/* Ensure that user did not specify both "no file names" and "show file names" */
if( int_nofile + int_names == 2 ) {
fprintf( stderr, "%s: cannot use \"-n\" and \"-N\" together.\n", basename(argv[0]) );
return 1;
}

/* Decide whether to show file names */
if( int_names == 1 )          // -N
int_shownames = 1;
else if( int_nofile == 1 )    // -n
int_shownames = 0;
else if( int_numfiles == 1 )  // only one file
int_shownames = 0;
else                          // multiple files
int_shownames = 1;
show_debug( "int_shownames == %d\n", int_shownames );

/* Loop through each file specified */
for( int_index = optind; int_index < argc; int_index++ ) {

show_debug( "int_index == %d\n", int_index );

/* Write a pointer to the stat (or link stat) info in the stat buffer */
if( int_showlinks == 0 ) {
int_stat_rc = stat( argv[int_index], &stat_buf );
}
else {
int_stat_rc = lstat( argv[int_index], &stat_buf );
}
show_debug( "int_stat_rc == %d\n", int_stat_rc );

/* If stat or lstat returned zero (good) */
if( int_stat_rc == 0 ) {

if( strcmp( basename(argv[0]), "filetime" ) == 0 ) {
/* Extract the last modification time from the stat buffer (stat_buf)
  and store a pointer to it in the time buffer (called "tm_buf") */
tm_buf = *localtime(&stat_buf.st_mtime);
}
else if( strcmp( basename(argv[0]), "filesize" ) == 0 ) {
/* Extract the file size in bytes from the stat buffer (stat_buf)
  and store it as a long long (called "ll_size") */
ll_size = stat_buf.st_size;
}
else if( strcmp( basename(argv[0]), "owner" ) == 0 ) {
/* Extract the (numeric) owner uid from the stat buffer and
  store it in "ll_size" */
ll_size = stat_buf.st_uid;
}
else { /* basename must be "inode", so store the inode in "ll_size" */
ll_size = stat_buf.st_ino;
}

/* Choose the correct output format (see the "optmode" table in the "-h" output, above)
  for storing in the char_time array */
switch( int_optmode ) {
case 0:
strftime( char_time, sizeof(char_time), "%Y%m%d%H%M",           &tm_buf );
break;
case 1:
strftime( char_time, sizeof(char_time), "%H%M %d.%m.%Y",        &tm_buf );
break;
case 2:
strftime( char_time, sizeof(char_time), "%Y-%m-%d %H:%M %Z",    &tm_buf );
break;
case 3:
strftime( char_time, sizeof(char_time), "%H:%M %Y-%m-%d",       &tm_buf );
break;
case 4:
strftime( char_time, sizeof(char_time), "%Y%m%d%H%M.%S",        &tm_buf );
break;
case 5:
strftime( char_time, sizeof(char_time), "%H%M.%S %d.%m.%Y",     &tm_buf );
break;
case 6:
strftime( char_time, sizeof(char_time), "%Y-%m-%d %H:%M:%S %Z", &tm_buf );
break;
case 7:
strftime( char_time, sizeof(char_time), "%H:%M:%S %Y-%m-%d",    &tm_buf );
break;
case 8:
case 9:
case 12:
case 13:
strftime( char_time, sizeof(char_time), "%s",                    &tm_buf );
break;
case 10:
strftime( char_time, sizeof(char_time), "%s %Y-%m-%d %H:%M %Z",   &tm_buf );
break;
case 11:
strftime( char_time, sizeof(char_time), "%s %H:%M %Z %Y-%m-%d",    &tm_buf );
break;
case 14:
strftime( char_time, sizeof(char_time), "%s %Y-%m-%d %H:%M:%S %Z", &tm_buf );
break;
case 15:
strftime( char_time, sizeof(char_time), "%s %H:%M:%S %Z %Y-%m-%d", &tm_buf );
break;
default:
/* If we got to here then something went wrong with our
  calculation of "optmode" based on the options provided.
  Remember that if there will be 2 possibile combinations with 1 option (on or off)
  then there will be 2**(n-1) possibile combinations with n options, e.g.
  16 possible combinations (0 to 15) with 4 options. */
fprintf( stderr, "%s: internal option error %d\n", basename(argv[0]), int_optmode );
int_main_rc = 2;
break;
}
show_debug( "char_time == %s\n", char_time );

/* If we should show files names then
  prefix each result with the file name and a tab */
if( int_shownames == 1 ) {
show_debug( "writing file name and tab prefix\n" );
fprintf( stdout, "%s\t", argv[int_index] );
}

if( strcmp( basename(argv[0]), "filetime" ) == 0 ) {
fprintf( stdout, "%s", char_time );
}
else {
/* basename(argv[0]) == "filesize" or "inode" or "owner" */
fprintf( stdout, "%lld", ll_size );
}
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
show_debug( "int_main_rc == %d\n", int_main_rc );
}

/* Return the appropriate return code */
show_debug( "returning from main()\n" );
return int_main_rc;
}

