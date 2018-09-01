#ifndef __DYNAR_H__
#define __DYNAR_H__

#include <stdlib.h>

/**
* Dynamic array to store dynamically allocated memory chunks.
* @data: Arrray of memory chunks.
* @size: Number of memory chunks in dynar_t.
*/
typedef struct dynar_s
{
  char **data;
  int size;

// private:
  size_t _alloc_size;
}
dynar_t;

/**
* New dynar_t object, free it with dynar_free.
*/
dynar_t *dynar_new();

void dynar_free(dynar_t *self);

/**
* Append data to the end of array.
* @data: Dynamically allocated data. Ownership transfers: dynar will automatically free it with free(). 
*/
void dynar_append(dynar_t *self, char *data);

#endif /* __DYNAR_H__ */
