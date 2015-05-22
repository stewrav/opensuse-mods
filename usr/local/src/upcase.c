/*
	upcase.c
	by Stewart Ravenhall

	Changes standard input to upper case on standard output
*/

#include <stdio.h>

int main ()
{
	char c;
	while ((c=getchar()) != EOF)
		if (c >= 'a' && c <= 'z')
			printf("%c", c - ' ');
		else
			printf("%c", c);
	return 0;
}
