#include "file_info.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_visible(const struct dirent *d)
{
  return (d->d_name[0] != '.');
}

static int alphacasesort(const struct dirent **a, const struct dirent **b)
{
  return strcasecmp((*a)->d_name, (*b)->d_name);
}


int main(int argc, char *argv[])
{
  int i;
  int rval = 1;

  int files_num = 0;
  struct dirent **files = NULL;
  char **full_files_paths = NULL;
  struct stat *st = NULL;

  // Cmd line options -->
    char *user_path = NULL;

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
  // Cmd line options <--

  // If user passed file (not a directory) as path -->
  if(user_path)
  {
    struct stat st;
    if(lstat(user_path, &st) == 0)
    {
      if(!S_ISDIR(st.st_mode))
      {
        file_info_print(user_path, user_path, &st);
        goto ok;
      }
    }
    else
    {
      perror("Failed to get info about user path");
      goto fail;
    }
  }
  // If user passed file (not a directory) as path <--

  files_num = scandir(user_path ? user_path : ".", &files, flag_all ? NULL : is_visible, alphacasesort);
  if(files_num < 0)
  {
    files_num = 0;
    perror("Failed to scan directory");
    goto fail;
  }

  if(user_path)
  {
    full_files_paths = malloc(files_num * sizeof(char*));

    for(i = 0; i < files_num; i++)
    {
      size_t full_file_path_len = strlen(user_path) + 1 /*delimeter*/ + strlen(files[i]->d_name);
      full_files_paths[i] = malloc(full_file_path_len + 1 /*NULL-terminator*/);
      snprintf(full_files_paths[i], full_file_path_len + 1, "%s/%s", user_path, files[i]->d_name);
    }
  }

  st = malloc(files_num * sizeof(struct stat));

  {
    unsigned long long total = 0;

    for(i = 0; i < files_num; i++)
      if(lstat(full_files_paths ? full_files_paths[i] : files[i]->d_name, st + i) != 0)
      {
        perror("Failed to get file info");
        goto fail;
      }
      else
        total += st[i].st_blocks;

    // According 'man stat' st_blocks is mesaured in 512B blocks,
    // but 'info coreutils ls' says that 'ls' shows 'total' in 1024B blocks.
    total /= 2;

    printf("total %llu\n", total);
  }

  for(i = 0; i < files_num; i++)
    file_info_print(full_files_paths ? full_files_paths[i] : files[i]->d_name, files[i]->d_name, st + i);

ok:
  rval = 0;

fail:
  if(files)
  {
    for(i = 0; i < files_num; i++)
      free(files[i]);

    free(files);
    files = NULL;
  }

  if(full_files_paths)
  {
    for(i = 0; i < files_num; i++)
      free(full_files_paths[i]);

    free(full_files_paths);
    full_files_paths = NULL;
  }

  free(st);
  st = NULL;

  return rval;
}
