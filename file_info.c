// https://www.unix.com/man-page/posix/1posix/ls

#include "file_info.h"
#include <grp.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>

static void print_permissions(int r, int w, int x, int s, int t)
{
  int last_char;

  if(s)
    last_char = x ? 's' : 'S';
  else
    last_char = x ? 'x' : '-';

  if(t)
    last_char = 't';

  printf("%c%c%c", r ? 'r' : '-', w ? 'w' : '-', last_char);
}

static void print_id(const char* name, unsigned long long id)
{
  if(name)
    printf(" %8s", name);
  else
    printf(" %8llu", id);
}

static void print_date(const time_t *timep)
{
  char str[256];
  const char *fmt;
  time_t curseconds = time(NULL);
  struct tm curtime;
  struct tm filetime;

  if(!localtime_r(&curseconds, &curtime) || !localtime_r(timep, &filetime))
    goto fail;

  // According POSIX (show time or year).
  if((((curtime.tm_year - filetime.tm_year) * 12) + curtime.tm_mon - filetime.tm_mon) > 6)
    fmt = "%b %e  %Y";
  else
    fmt = "%b %e %H:%M";

  if(strftime(str, sizeof(str), fmt, &filetime) == 0)
    goto fail;

  printf(" %12s", str);
  return;

fail:
  printf("  ??? ?? ????");
}

void file_info_print(const char *file_path, const char *filename_to_print, const struct stat *st)
{
  // File type.
  {
    int type = '?';

         if(S_ISREG (st->st_mode)) type = '-';
    else if(S_ISDIR (st->st_mode)) type = 'd';
    else if(S_ISLNK (st->st_mode)) type = 'l';
    else if(S_ISCHR (st->st_mode)) type = 'c';
    else if(S_ISBLK (st->st_mode)) type = 'b';
    else if(S_ISFIFO(st->st_mode)) type = 'p';
    else if(S_ISSOCK(st->st_mode)) type = 's';

    printf("%c", type);
  }

  // Permissions.
  print_permissions(st->st_mode & S_IRUSR, st->st_mode & S_IWUSR, st->st_mode & S_IXUSR, st->st_mode & S_ISUID, 0);
  print_permissions(st->st_mode & S_IRGRP, st->st_mode & S_IWGRP, st->st_mode & S_IXGRP, st->st_mode & S_ISGID, 0);
  print_permissions(st->st_mode & S_IROTH, st->st_mode & S_IWOTH, st->st_mode & S_IXOTH, 0, st->st_mode & S_ISVTX);

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

  // Data and time.
  print_date(&st->st_mtime);

  // File name.
  printf(" %s", filename_to_print);

  // Value of symbolic link.
  if(S_ISLNK(st->st_mode))
  {
    char str[256];
    if(readlink(file_path, str, sizeof(str)) > 0)
      printf(" -> %s", str);
  }

  printf("\n");
}

