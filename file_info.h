#ifndef __FILE_INFO_H__
#define __FILE_INFO_H__

#include <sys/stat.h>

void file_info_print(const char *file_path, const char *filename_to_print, const struct stat *st);

#endif /* __FILE_INFO_H__ */
