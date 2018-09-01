#include "dynar.h"

dynar_t *dynar_new()
{
  dynar_t *self = malloc(sizeof(dynar_t));

  self->_alloc_size = 32 * sizeof(char*); //< Default _alloc_size.
  self->data = malloc(self->_alloc_size * sizeof(char*));
  self->size = 0;

  return self;
}

void dynar_free(dynar_t *self)
{
  int i;
  for(i = 0; i < self->size; i++)
    free(self->data[i]);

  free(self->data);
  free(self);
}

void dynar_append(dynar_t *self, char *data)
{
  int new_size = self->size + 1;

  if(new_size > self->_alloc_size)
  {
    self->_alloc_size *= 2;
    self->data = realloc(self->data, self->_alloc_size * sizeof(char*));
  }

  self->data[self->size] = data;
  self->size = new_size;
}
