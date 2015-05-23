#include <stdio.h>

int main()
{
	char buffer[200];

	printf("Press return to continue\n");
	fgets(buffer, sizeof(buffer), stdin);

	exit(0);
}
