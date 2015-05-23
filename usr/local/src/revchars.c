/* Reverse standard input and send to standard output */

#include <stdio.h>
#define SIZE 255

int main()
{
	char str[SIZE + 1], *sptr = str;
	while (sptr <= str + SIZE && (*sptr++ = getchar()) != EOF);
	--sptr;
	while (--sptr >= str)
		putchar(*sptr);
	return 0;
}
