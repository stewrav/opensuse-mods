#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HANDLES  (100000)

int main()
{
  int   exitcode = EXIT_SUCCESS;
  int   i;
  FILE* handles[MAX_HANDLES];

  for(i = 0; i != MAX_HANDLES; ++i)
  {
    char  filename[101];
    int   n = sprintf(&filename[0], "file-%05d", i);

    handles[i] = fopen(filename, "w");

    if(NULL == handles[i])
    {
      fprintf(stderr,
   "failed to create %s: %s (%d); FOPEN_MAX=%d\n"
 , filename, strerror(errno), errno, FOPEN_MAX);

      exitcode = EXIT_FAILURE;
      break;
    }
  }

  for(--i; i >= 0; --i)
  {
    char filename[101];
    sprintf(&filename[0], "file-%05d", i);
    fclose(handles[i]);
    remove(filename);
  }

  return exitcode;
}
