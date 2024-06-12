#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "File_out.h"


int main(int argc, char **argv) {
  char fn[50];

  if (argc > 1) {
    strcpy(fn, argv[1]);
  } else {
    printf("Введите название файла с картой: ");
    scanf("%s", fn);
  }

  FILE *f = fopen(fn, "r");

  if (f == NULL) {
    printf("Файл %s не существует.\n", fn);
    return 1;
  }

  Map *map = Map__read(f);

  fclose(f);

  print_map(map, stdout);

  Map__free(map);

  return 0;
}
