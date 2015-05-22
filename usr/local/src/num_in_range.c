/* num_in_range: display whether an argument is numeric and in range */
/* Usage example: num_in_range wibble 3 56 9 hello -4 3b -91 beep f9 */

#define LOWVAL -10L
#define HIGHVAL 10L

int validnum(const char* string, long low, long hi, long* val)
{
	char* ep;
	*val = strtol(string, &ep, 0);
	if (ep == string || *ep != '\0') /* Empty string or parsing stopped before end */
		return 0;
	return *val >= low && *val <= hi;
}

int main(int argc, char **argv)
{
	int i;
	long val;
	if (argc == 1)
	/* No arguments */
	{
		printf("Usage: %s num1 [num2] [num3] ...\n", argv[0]);
		return(1);
	}
	for (i = 1; i < argc; i++)
		if (validnum(argv[i], LOWVAL, HIGHVAL, &val))
			printf("%s is valid\n", argv[i]); 
		else
			printf("%s is not valid\n", argv[i]);
	return 0;
}
