#pragma once

#include <stdio.h>

typedef struct vector {
  void *data;
  size_t value_size;
  size_t length;
  size_t cap;

  void (*del)(struct vector *self);
  void (*push_back)(struct vector *self, void *value);
  void (*for_each)(struct vector *self, void (*f)(void *value));
} vector;

vector *vector_new(size_t value_size);