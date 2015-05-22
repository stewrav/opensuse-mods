#include <sys/param.h>
#include <sys/pstat.h>
#include <sys/unistd.h>

main()
{
	struct pst_static buf;
	pstat_getstatic(&buf, sizeof(buf), 0, 0);
	printf("Physical RAM = %ldMB\n", buf.physical_memory/256);
}
