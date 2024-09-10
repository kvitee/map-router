#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Map.h"
#include "File_out.h"


/**
 * Генерирует случайную карту размера w x h с количеством препятствий c,
 * по которой сможет двигаться летательный аппарат размером s.
 */
Map *generate_map(uint8_t w, uint8_t h, uint8_t s, uint8_t c) {
  Map *m = Map__create(w, h);
  m->s = s;

  /* Отмечаем границы карты. */
  for (uint8_t i = 0; i < m->h; i++) {
    for (uint8_t j = 0; j < m->w; j++) {
      if (
        i == 0 || i == m->h - 1 ||
        j % m->w == 0 || (j + 1) % m->w == 0
      ) {
        Map__set(m, j, i, BORDER);
      }
    }
  }

  /* Отмечаем полосу вдоль границ, на которой не может быть препятствий. */
  for (uint8_t i = 1; i < m->h - 1; i++) {
    for (uint8_t j = 1; j < m->w - 1; j++) {
      if (i > s && i < m->h - s - 1 && 
          j > s && j < m->w - s - 1) {
        continue;
      }

      Map__set(m, j, i, ROUTE);
    }
  }

  /* Основной цикл генерации препятствий.
   * Завершается, когда сгенерировано нужное количество препятствий.
   */
  while (c > 0) {
    /* Случайная точка, из которой будет проведено препятствие. */
    uint8_t x0 = rand() % w;
    uint8_t y0 = rand() % h;

    /* Если занята, пробуем снова. */
    if (Map__get(m, x0, y0) != FREE) continue;

    /* Выбор случайного направления для препятствия.
     * 0 - вправо;
     * 1 - вправо-вверх;
     * 2 - вправо-вниз;
     * 3 - вниз.
     */
    uint8_t d = rand() % 4;

    int8_t dx = (d < 3);
    int8_t dy = (d & 2) - (d > 0);

    /* Случайная длина препятствия. */
    uint8_t len = rand() % (w < h ? w : h) + 1;

    /* Отмечаем на карте ячейки с препятствиями, максимум len штук,
     * либо пока не упремся в занятую ячейку.
     */
    for (uint8_t k = 0; k < len; k++) {
      uint8_t x = x0 + k*dx;
      uint8_t y = y0 + k*dy;

      if (Map__get(m, x, y) != FREE) break;

      Map__set(m, x, y, OBSTACLE);
    }

    /* Вокруг каждой ячейки с препятствием отмечаем ячейки как те,
     * в которых нельзя располагать другие препятствия.
     */
    for (uint8_t k = 0; k < len; k++) {
      uint8_t x = x0 + k*dx;
      uint8_t y = y0 + k*dy;

      if (Map__get(m, x, y) != OBSTACLE) break;

      for (int8_t i = -s; i <= s; i++) {
        for (int8_t j = -s; j <= s; j++) {
          if (Map__get(m, x + j, y + i) == OBSTACLE) continue;

          Map__set(m, x + j, y + i, ROUTE);
        }
      }
    }

    c -= 1;
  }

  /* Убираем с карты все дополнительные обозначения,
   * использованные при генерации препятствий.
   */
  for (uint16_t i = 0; i < m->w * m->h; i++) {
    if (m->cells[i] == ROUTE) {
      m->cells[i] = FREE;
    }
  }

  return m;
}

/**
 * Функция, которая запрашивает у пользователя точки начала и конца маршрута и наносит их на карту.
 */
void choose_endpoints(Map *m) {
  Waypoint start;
  printf("Введите начальную точку маршрута (x y): ");

  while (1) {
    scanf("%hhu %hhu", &start.x, &start.y);

    if (!Waypoint__blocked(start, m)) break;

    printf("Точка пересекается с препятствием, попробуйте снова: ");
  }

  Waypoint end;
  printf("Введите конечную точку маршрута (x y): ");

  while (1) {
    scanf("%hhu %hhu", &end.x, &end.y);

    if (!Waypoint__blocked(end, m)) break;

    printf("Точка пересекается с препятствием, попробуйте снова: ");
  }

  for (uint8_t i = 0; i < m->s; i++) {
    for (uint8_t j = 0; j < m->s; j++) {
      Map__set(m, start.x + j, start.y + i, START);
      Map__set(m, end.x + j, end.y + i, END);
    }
  }
}

int main() {
  srand(time(0));

  printf("Введите размеры карты.\n");

  uint8_t width;
  printf("Ширина: ");
  scanf("%hhu", &width);

  uint8_t height;
  printf("Высота: ");
  scanf("%hhu", &height);

  uint8_t ship_size;
  printf("Введите размер летательного аппарата: ");
  scanf("%hhu", &ship_size);

  uint8_t obstacles_count;
  printf("Введите количество препятствий: ");
  scanf("%hhu", &obstacles_count);

  Map *map = generate_map(width, height, ship_size, obstacles_count);

  print_map(map, stdout);

  while (getc(stdin) != '\n');

  printf("Если Вы хотите выбрать начало и конец маршрута, введите 1: ");
  if (getc(stdin) == '1') {
    choose_endpoints(map);

    print_map(map, stdout);
  }

  FILE *f = fopen("map.txt", "w");
  save_map(map, f);
  fclose(f);

  Map__free(map);

  return 0;
}
