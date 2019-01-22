#define _GNU_SOURCE

void __attribute__((constructor)) init(void);
void __attribute__((destructor)) cleanup(void);

#define ID_BITS 16
#define ID_BYTES (ID_BITS >> 3)
#define TABLE_SIZE (1 << ID_BITS)

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void *(*orig_malloc)(size_t size) = NULL;
void (*orig_free)(void *ptr) = NULL;

size_t mem_table[TABLE_SIZE] = {0};

// Called when the library is unloaded
void cleanup(void) {
  // you would use this function to close any files that were still open
  // or do any other clean up.
  printf("cleaning up.\n");

  size_t total_bytes = 0;
  uint16_t total_counts = 0;
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if (mem_table[i] == 0) continue;

    printf("LEAK\t%zu\n", mem_table[i]);
    total_bytes += mem_table[i];
    total_counts++;
  }
  printf("TOTAL\t%u\t%zu\n", total_counts, total_bytes);
}

void init(void) {
  if (orig_malloc == NULL) {
    orig_malloc = dlsym(RTLD_NEXT, "malloc");
  }

  if (orig_free == NULL) {
    orig_free = dlsym(RTLD_NEXT, "free");
  }
  printf("initializing library.\n");
}

void *malloc(size_t size) {
  static uint16_t id = 0;
  void *vm = orig_malloc(ID_BYTES + size);
  // Embed id info into the memory block
  *(uint16_t *)vm = id;
  // Store requested size in the table
  mem_table[id] = size;
  id++;

  return (void *)((uint8_t *)vm + ID_BYTES);
}

void free(void *ptr) {
  void *actual_ptr = (void *)((uint8_t *)ptr - ID_BYTES);
  uint16_t id = *((uint16_t *)actual_ptr);
  mem_table[id] = 0;

  orig_free(actual_ptr);
}