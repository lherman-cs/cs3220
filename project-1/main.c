#include <stdint.h>
#include "vector.h"

void f(void *value) {
  uint64_t *num = (uint64_t *)value;
  printf("%llu\n", *num);
}

int main() {
  vector *v = vector_new(sizeof(uint64_t));
  uint64_t ar[] = {1, 2, 3};

  v->push_back(v, &ar[0]);
  v->push_back(v, &ar[1]);
  v->push_back(v, &ar[2]);
  v->for_each(v, f);

  printf("done\n");

  v->del(v);
}