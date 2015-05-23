/*
	lowcase.c
	by Stewart Ravenhall

	Changes standard input to lower case on standard output
*/

#include <stdio.h>

int main ()
{
	char c;
	while ((c=getchar()) != EOF)
		if (c >= 'A' && c <= 'Z')
			printf("%c", c + ' ');
		else
			printf("%c", c);
	return 0;
}
