#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *block1 = malloc(8);
  void *block2 = malloc(8);
  void *block3 = malloc(8);

  printf("%p\n", block1);
  printf("%p\n", block2);
  printf("%p\n", block3);

  long long diff = block3 - block1;
  printf("%lld\n", diff);
}