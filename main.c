// https://www.unix.com/man-page/posix/1posix/ls

#include "file_info.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int is_visible(const struct dirent *d)
{
  return (d->d_name[0] != '.');
}


int main(int argc, char *argv[])
{
  int rval = -1;
  int flag_all = 0;

  int i;
  int files_num = 0;
  struct dirent **files = NULL;

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

  files_num = scandir(".", &files, flag_all ? NULL : is_visible, alphasort);
  if(files_num < 0)
  {
    files_num = 0;
    perror("Failed to scan direcctory");
    goto fail;
  }

  tzset(); // Init for localtime.
  

  for(i = 0; i < files_num; i++)
    if(file_info_print(files[i]->d_name) != 0)
    {
      perror("Failed to get file info");
      goto fail;
    }

  rval = 0;

fail:
  for(i = 0; i < files_num; i++)
    free(files[i]);

  free(files);

  return rval;
}
