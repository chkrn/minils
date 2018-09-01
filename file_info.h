#ifndef __FILE_INFO_H__
#define __FILE_INFO_H__

#include <sys/stat.h>

int file_info_print(const char *filename, const struct stat *st);

#endif /* __FILE_INFO_H__ */
