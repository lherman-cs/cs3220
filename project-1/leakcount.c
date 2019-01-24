#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef char *string;

extern char **environ;

string join(const string *ar, const char sep) {
  size_t len = 0;
  for (const string *str = ar; *str != NULL; str++) {
    len += strlen(*str) + 1;
  }

  string joined = malloc(len);
  char *ptr = joined;

  for (const string *str = ar; *str != NULL; str++) {
    strcpy(ptr, *str);
    ptr += strlen(*str);
    *ptr = sep;
    ptr++;
  }

  joined[len] = '\0';
  return joined;
}

int main(int argc, char *argv[]) {
  string cmd = join(argv + 1, ' ');
  system(cmd);
  free(cmd);
}