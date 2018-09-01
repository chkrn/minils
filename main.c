// https://www.unix.com/man-page/posix/1posix/ls

#include <dirent.h>
#include <grp.h>
#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>

void print_id(const char* name, unsigned long long id)
{
  if(name)
    printf(" %8s", name);
  else
    printf(" %8llu", id);
}

int print_file_stat(const char *pathname)
{
  struct stat st;

  if(stat(pathname, &st) != 0)
    return -1;

  { // UID -->
    struct passwd *pw = getpwuid(st.st_uid);
    print_id(pw ? pw->pw_name : NULL, st.st_uid);
  } // UID <--

  { // GID -->
    struct group *gr = getgrgid(st.st_gid);
    print_id(gr ? gr->gr_name : NULL, st.st_gid);
  } // GID <--

  // File size.
  printf(" %8llu", (unsigned long long)st.st_size);

  { // Data and time -->
    char str[256];
    struct tm *tm = localtime(&st.st_mtime);

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
  printf(" %s\n", pathname);

  return 0;
}

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

  tzset(); // Init for localtime.

  if(dirp)
  {
    struct dirent *dp;

    while(dp = readdir(dirp))
    {
      if(flag_all || dp->d_name[0] != '.')
        if(print_file_stat(dp->d_name) != 0)
        {
          perror("Failed to get file info");
          return -1;
        }
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
