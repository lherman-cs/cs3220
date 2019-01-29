#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

static void del(vector *self);
static void push_back(vector *self, void *value);
static void for_each(vector *self, void (*f)(void *value));

vector *vector_new(size_t value_size) {
  vector *v = calloc(1, sizeof(vector));
  v->data = calloc(1, value_size);
  v->cap = 1;
  v->value_size = value_size;
  v->del = del;
  v->push_back = push_back;
  v->for_each = for_each;
  return v;
}

static void del(vector *self) {
  free(self->data);
  free(self);
}

static void grow(vector *self) {
  self->cap *= 2;
  self->data = realloc(self->data, self->cap * self->value_size);
}

static void push_back(vector *self, void *value) {
  if (self->length == self->cap) grow(self);

  size_t offset = self->length * self->value_size;
  uint8_t *ptr = self->data;

  memcpy((void *)(ptr + offset), value, self->value_size);
  self->length++;
}

static void for_each(vector *self, void (*f)(void *value)) {
  uint8_t *ptr = self->data;
  for (size_t i = 0; i < self->length; i++) {
    f((void *)ptr);
    ptr += self->value_size;
  }
}
