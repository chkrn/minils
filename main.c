// https://www.unix.com/man-page/posix/1posix/ls

#include "file_info.h"
#include <dirent.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
  int rval = -1;
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
    goto fail;
  }

  tzset(); // Init for localtime.

  if(dirp)
  {
    struct dirent *dp;

    while(dp = readdir(dirp))
    {
      if(flag_all || dp->d_name[0] != '.')
        if(file_info_print(dp->d_name) != 0)
        {
          perror("Failed to get file info");
          goto fail;
        }
    }

    if(closedir(dirp) != 0)
    {
      perror("Failed to close dir");
      goto fail;
    }
  }
  else
  {
    perror("Failed to open dir");
    goto fail;
  }

  rval = 0;

fail:
  // TODO free smth

  return rval;
}
