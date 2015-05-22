#include <stdio.h>
main() {
	int i;
	puts("\r\n256-byte test pattern:\r\n");
	for (i = 0; i < 256; i++)
		putchar(i); /* singles */
	puts("\r\n256-double-byte test pattern:\r\n");
	for (i = 0; i < 256; i++) {
		putchar(i);
		putchar(i);
	} /* in pairs */
	puts("\r\nEnd of test patterns\r\n");
}
