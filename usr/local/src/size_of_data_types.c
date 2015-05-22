/*
	size of data types: display sizes of basic types

	sizeof(char) <= sizeof(short)
	             <= sizeof(int)
	             <= sizeof(long)

	sizeof(float) <= sizeof(double)

	gcc -O -Wall size_of_data_types.c -o size_of_data_types
*/

#include <stdio.h>

int main(void)
{
	printf("char %d, short %d, int %d, long %d,",
		sizeof(char), sizeof(short),
		sizeof(int), sizeof(long));
	printf(" float %d, double %d, void* %d\n",
		sizeof(float), sizeof(double), sizeof(void *));
	return 0;
}

