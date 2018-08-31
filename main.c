// https://www.garron.me/en/go2linux/ls-file-permissions.html

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  int flag_all = 0;
  DIR *dirp = opendir(".");

  switch(argc)
  {
    case 1:
    break; //< Ok, "ls -l" mode.

    case 2:
      if(argv[1][0] == 'a' && argv[1][1] == '\0') //< Ok, "ls -la" mode.
      {
        flag_all = 1;
        break;
      }

    default:
      fprintf(stderr, "Wrong options.\nUsage: %s [a]\n", argv[0]);
      return -1;
  }

  if(dirp)
  {
    struct dirent *dp;

    while(dp = readdir(dirp))
    {
      if(flag_all || dp->d_name[0] != '.')
        printf(">%s\n", dp->d_name);
    }

    if(closedir(dirp) != 0)
    {
      perror("Failed to close dir");
      return -1;
    }
  }
  else
  {
    perror("Failed to open dir");
    return -1;
  }

  return 0;
}
