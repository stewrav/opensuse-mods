#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

int main()
{
  uid_t uid = getuid();
  struct passwd *pwd = getpwuid(uid);
  if (!pwd)
  {
    perror("Can't get password entry");
    fprintf(stderr, "User id: %d\n", uid);
    exit(1);
  }
  else
  {
    fprintf(stderr, "Everything seems ok.\n");
  }
  return 0;
}
