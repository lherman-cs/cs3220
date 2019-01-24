#define _GNU_SOURCE

void __attribute__((constructor)) init(void);
void __attribute__((destructor)) cleanup(void);

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *(*orig_malloc)(size_t size) = NULL;
void (*orig_free)(void *ptr) = NULL;

typedef struct node {
  struct node *next;
  void *address;
  size_t size;
} node;

node *head = NULL;
node *tail = NULL;

void cleanup(void) {
  size_t total_bytes = 0;
  uint16_t total_counts = 0;
  for (node *cur = head->next; cur != NULL; cur = cur->next) {
    fprintf(stderr, "LEAK\t%zu\n", cur->size);
    total_bytes += cur->size;
    total_counts++;
  }
  fprintf(stderr, "TOTAL\t%u\t%zu\n", total_counts, total_bytes);

  // clean up
  node *cur = head;
  while (cur != NULL) {
    node *next = cur->next;
    orig_free(cur);
    cur = next;
  }
}

void init(void) {
  if (orig_malloc == NULL) {
    orig_malloc = dlsym(RTLD_NEXT, "malloc");
  }

  if (orig_free == NULL) {
    orig_free = dlsym(RTLD_NEXT, "free");
  }

  // initialize linkedlist with dummy node
  if (head == NULL) {
    head = orig_malloc(sizeof(node));
    tail = head;
  }
}

void *malloc(size_t size) {
  void *address = orig_malloc(size);
  node *next = orig_malloc(sizeof(node));
  next->address = address;
  next->next = NULL;
  next->size = size;
  tail->next = next;
  tail = next;

  return address;
}

void free(void *ptr) {
  if (ptr == NULL) return;

  node *prev = head;
  for (node *cur = head; cur != NULL; cur = cur->next) {
    if (cur->address == ptr) {
      prev->next = cur->next;
      orig_free(cur);
      break;
    }
    prev = cur;
  }

  orig_free(ptr);
}
