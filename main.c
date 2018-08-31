// https://www.garron.me/en/go2linux/ls-file-permissions.html

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  struct dirent *dp;
  DIR *dirp = opendir(".");

  if(dirp)
  {
    while(dp = readdir(dirp))
    {
      printf(">%s\n", dp->d_name);
    }

    if(closedir(dirp) != 0)
      perror("Failed to close dir");
  }
  else
  {
    perror("Failed to open dir");
    return -1;
  }

  return 0;
}
