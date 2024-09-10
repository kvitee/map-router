#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Map.h"


Map *Map__create(uint8_t w, uint8_t h) {
  Map *m = malloc(sizeof(Map));
  *m = (Map){w, h, " #@AB."};

  m->cells = malloc(sizeof(Map_symbol_code) * w*h);

  for (uint16_t i = 0; i < w*h; i++) {
    m->cells[i] = FREE;
  }

  return m;
}

Map *Map__read(FILE *f) {
  /* Считываем размеры карты. */
  uint8_t w, h;
  fscanf(f, "%hhu;%hhu;", &w, &h);

  /* Создаем пустую карту. */
  Map *m = Map__create(w, h);

  /* Считываем символы для отрисовки карты. */
  for (uint8_t i = 0; i < MAP_SYMBOLS_COUNT; i++) {
    m->symbols[i] = fgetc(f);
  }

  /* Считываем информацию о ячейках карты. */
  for (uint16_t i = 0; i < w*h; i++) {
    m->cells[i] = fgetc(f) - '0';
  }

  /* Ищем начало маршрута и на основе его размера узнаем размер корабля. */
  for (uint16_t i = 0; i < m->s == 0 && w*h; i++) {
    while (m->cells[i] == START) {
      m->s++;
      i++;
    }
  }

  return m;
}

Map_symbol_code Map__get(const Map *m, uint8_t x, uint8_t y) {
  return m->cells[y * m->w + x];
}

void Map__set(Map *m, uint8_t x, uint8_t y, Map_symbol_code s) {
  m->cells[y * m->w + x] = s;
}

Waypoint Map__find(const Map *m, Map_symbol_code s) {
  for (uint8_t i = 0; i < m->h; i++) {
    for (uint8_t j = 0; j < m->w; j++) {
      if (Map__get(m, j, i) == s) {
        return (Waypoint){j, i, 0, 0, 0.0f};
      }
    }
  }

  return (Waypoint){-1, -1, 0, 0, 0.0f};
}

uint8_t Map__cell_blocked(const Map *m, uint8_t x, uint8_t y) {
  const Map_symbol_code c = Map__get(m, x, y);

  return (c == BORDER) || (c == OBSTACLE);
}

void Map__free(Map *m) {
  free(m->cells);
  free(m);
}
