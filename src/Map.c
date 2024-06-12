#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Map.h"


Map *Map__create(uint8_t w, uint8_t h) {
  Map *m = malloc(sizeof(Map));
  *m = (Map){w, h, " #@AB."};

  m->cells = malloc(sizeof(Map_symbol_code) * w*h);
  memset(m->cells, FREE, w*h);

  return m;
}

Map *Map__read(FILE *f) {
  uint8_t w, h;
  fscanf(f, "%hhu;%hhu;", &w, &h);

  Map *m = Map__create(w, h);

  for (uint8_t i = 0; i < MAP_SYMBOLS_COUNT; i++) {
    m->symbols[i] = fgetc(f);
  }

  for (uint16_t i = 0; i < w*h; i++) {
    m->cells[i] = fgetc(f) - '0';
  }

  return m;
}

Map_symbol_code Map__get(const Map *m, uint8_t x, uint8_t y) {
  return m->cells[y * m->w + x];
}

void Map__set(Map *m, uint8_t x, uint8_t y, Map_symbol_code s) {
  m->cells[y * m->w + x] = s;
}

void Map__free(Map *m) {
  free(m->cells);
  free(m);
}
