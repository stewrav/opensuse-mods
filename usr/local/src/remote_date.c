/* date command executed on remote host */
/* Reads ~/.netrc file */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

char *host[] = { "copley2.kau.roche.com" };
char *cmd = "date";

main(argc, argv)
char **argv;
int argc;
{
	char broiled;
	struct servent *servent;
	FILE *fp;
	int sd;

	servent = getservbyname("exec", "tcp");
	sd = rexec(host, servent->s_port, NULL, NULL, cmd, 0);
	fp = fdopen(sd, "r");
	while ((broiled = getc(fp)) != EOF)
		putchar(broiled);
}
