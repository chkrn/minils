// https://www.unix.com/man-page/posix/1posix/ls

#include "file_info.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int is_visible(const struct dirent *d)
{
  return (d->d_name[0] != '.');
}


int main(int argc, char *argv[])
{
  int i;
  int rval = -1;

  char *user_path = NULL;

  int files_num = 0;
  struct dirent **files = NULL;

  int flag_all = 0; //< '-a' option.
  if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'a' && argv[1][2] == '\0')
    flag_all = 1;

  switch(argc)
  {
    case 1:
    break;

    case 2:
      if(!flag_all)
        user_path = argv[1];
    break;

    case 3:
      if(flag_all)
      {
        user_path = argv[2];
        break;
      }

    default:
      fprintf(stderr, "Wrong options.\nUsage: %s [-a] [PATH]\n", argv[0]);
    goto fail;
  }

  tzset(); // Init for localtime.

  files_num = scandir(user_path ? user_path : ".", &files, flag_all ? NULL : is_visible, alphasort);
  if(files_num < 0)
  {
    files_num = 0;
    perror("Failed to scan direcctory");
    goto fail;
  }

  for(i = 0; i < files_num; i++)
  {
    char *full_file_path = NULL;

    if(user_path)
    {
      size_t full_file_path_len = strlen(user_path) + 1 /*delimeter*/ + strlen(files[i]->d_name);
      full_file_path = malloc(full_file_path_len + 1 /*NULL-terminator*/);
      snprintf(full_file_path, full_file_path_len + 1, "%s/%s", user_path, files[i]->d_name);
    }
    else
      full_file_path = files[i]->d_name;

    if(file_info_print(full_file_path) != 0)
    {
      perror("Failed to get file info");
      goto fail;
    }

    if(user_path)
      free(full_file_path);
  }

  rval = 0;

fail:
  for(i = 0; i < files_num; i++)
    free(files[i]);

  free(files);

  return rval;
}
