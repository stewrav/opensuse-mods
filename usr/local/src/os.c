/*
Which OS are we on?
*/

#include <stdio.h>

int main(int argc, char **argv) {
	#ifdef __hpux
		printf("HP-UX");
	#else
		#ifdef linux
			printf("Linux");
		#else
			printf("Other");
		#endif
	#endif
	printf("\n");
	return 0;
}
