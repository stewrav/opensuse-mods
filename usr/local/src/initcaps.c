/*
	initcaps.c

	Written by Stewart Ravenhall.

	Changes standard input to lower case with initial capitals
	and prints it to the standard output.
*/

/* stdio library needed for EOF reference. */
#include <stdio.h>

int main()
{
	/* Declare input characters to be of type character.
	   Normally would use type integer but do not need to cope with binary files.
	   Also declare capitalise indicator, which can be '1' or '0', as character.
	   It is initially set to '1' to capitalise the first byte if it is a letter. */
	char c, cap = '1';

	/* Loop through each byte until the end of the input stream. */
	while(( c = getchar() ) != EOF )
	{
		/* If byte is lower case letter and should be upper then capitalise it. */
		if( c >= 'a' && c <= 'z' && cap == '1' )
			printf( "%c", c - ' ' );

		/* If byte is upper case letter and should be lower then make it so. */
		else if( c >= 'A' && c <= 'Z' && cap == '0' )
			printf( "%c", c + ' ' );

		/* Otherwise just print the byte as it is. */
		else
			printf( "%c", c );

		/* If byte is a quote then leave the capitalise indicator as it is. */
		if( c == '\'' || c == '\"' );

		/* If byte is one that occurs before an initial byte
		   then set capitalise indicator. */
		else if( c == ' ' || c == '\n' || c == '\t' )
			cap = '1';

		/* Otherwise unset capitalise indicator. */
		else
			cap = '0';
	}
	return 0;
}
