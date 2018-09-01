// https://www.unix.com/man-page/posix/1posix/ls

#include "file_info.h"
#include <grp.h>
#include <langinfo.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>

static void print_id(const char* name, unsigned long long id)
{
  if(name)
    printf(" %8s", name);
  else
    printf(" %8llu", id);
}

int file_info_print(const char *filename, const struct stat *st)
{

/*
  // File type.
  {
      d      Directory.
       b      Block special file.
       c      Character special file.
       l (ell) Symbolic link.
       p      FIFO.
       -      Regular file.
  }

  // Permissions in symbolic notation.
  printf("%c%c%c%c%c%c%c%c%c%c%c",
*/

  // Number of links
  printf(" %3llu", (unsigned long long)st->st_nlink);

  { // UID -->
    struct passwd *pw = getpwuid(st->st_uid);
    print_id(pw ? pw->pw_name : NULL, st->st_uid);
  } // UID <--

  { // GID -->
    struct group *gr = getgrgid(st->st_gid);
    print_id(gr ? gr->gr_name : NULL, st->st_gid);
  } // GID <--

  // File size.
  printf(" %8llu", (unsigned long long)st->st_size);

  { // Data and time -->
    char str[256];
    struct tm *tm = localtime(&st->st_mtime);

    if(tm == NULL)
    {
      perror("Failed to get local time");
      return -1;
    }

    if(strftime(str, sizeof(str), nl_langinfo(D_T_FMT), tm) == 0)
    {
      perror("Failed to format time string");
      return -1;
    }

    printf(" %10s", str);
  } // Data and time <--

  // File name.
  printf(" %s\n", filename);

  return 0;
}

