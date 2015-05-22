/* Echo the given arguments without an EOF */

#include <stdio.h>

int main(int argc, char **argv)
{
	int i;
	/* Show first to penultimate arguments, plus space */
	for (i = 1; i < argc - 1; i++) printf("%s ", argv[i]);
	/* Show last argument (no space) */
	printf("%s", argv[argc - 1]);
	return 0;
}
