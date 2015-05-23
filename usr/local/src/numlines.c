/*
	numlines.c by Stewart Ravenhall

	Finds the number of lines in the standard input stream
*/

#include <stdio.h>

int main()
{
	int c, i = 0;

	while(( c = getchar() ) != EOF)
	{
		if( c == '\n')
			i++;
	}

	printf("%d\n", i);
	return 0;
}
