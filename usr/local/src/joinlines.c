/*
	joinlines.c

	Written by Stewart Ravenhall

	Joins lines together, adding a space, plus a newline at the very end

	Alternatives:
		sed 'H; $!d; g; s/\n/ /g' (memory problems with large files)
		tr -s '\012' ' ' (needs to use standard input)
		tr '\n' ' '
		paste -s -d ' '
*/

#define _POSIX_C_SOURCE 200112L
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1

#include <stdio.h>

int main()
{
	char c;

	while( ( c=getchar() ) != EOF)
	{
		if( c == '\n' )
			printf( " " );
		else
			printf( "%c", c );
	}
	printf( "\n" );
	return 0;
}
