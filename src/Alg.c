#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Var_str.h"
#include "File_out.h"


/**
 * Вычисляет расстояние между двумя точками маршрута.
 */
float dist(const Waypoint p1, const Waypoint p2) {
  return hypotf(p1.x - p2.x, p1.y - p2.y);
}

/**
 * Ищет первую секцию карты, содержащую символ sym.
 * Возвращает соответствующую ей точку маршрута.
 */
Waypoint find_symbol(const Map *m, Map_symbol_code s) {
  for (uint8_t i = 0; i < m->h; i++) {
    for (uint8_t j = 0; j < m->w; j++) {
      if (Map__get(m, j, i) == s) {
        return (Waypoint){j, i, 0, 0};
      }
    }
  }

  return (Waypoint){-1, -1, 0, 0};
}

/**
 * Проверяет, является ли секция посещенной на основе переданного
 * стека посещенных секций.
 *
 * Возвращает 1 если секция посещена (находится в visited), иначе 0.
 */
uint8_t is_visited(Waypoint w, const Stack *visited) {
  Stack_Node *cn = visited->top;
  while (cn->prev != NULL) {
    if (Waypoint__equal(cn->data, w)) {
      return 1;
    }

    cn = cn->prev;
  }

  return 0;
}

void sort_waypoints(Waypoint *arr, uint8_t len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (arr[i].h > arr[j].h) {
        Waypoint tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}

Stack *get_route(Waypoint start, Waypoint end, Stack *visited) {
  Stack *route = Stack__create();

  Stack__push(route, end);
  Waypoint p = Waypoint__parent(end);
  while (!Stack__empty(visited)) {
    if (!Waypoint__equal(p, Stack__top(visited))) {
      Stack__pop(visited);
      continue;
    }

    Stack__push(route, Stack__top(visited));
    Stack__pop(visited);

    if (Waypoint__equal(Stack__top(route), start)) break;

    p = Waypoint__parent(Stack__top(route));
  }

  return route;
}


/**
 * Основная функция для поиска маршрута на карте.
 * Не изменяет саму карту, а возвращает стек точек маршрута.
 * 
 */
Stack *find_route(const Map *m) {
  /* Актуальный стек - стек с секциями, которые предстоит
   * оценить.
   */
  Stack *actual = Stack__create();

  /* Стек с посещенными секциями, по которым в конце будет
   * производиться трассировка пути.
   */
  Stack *visited = Stack__create();

  /* Начало и конец маршрута. */
  Waypoint start = find_symbol(m, START);
  Waypoint end = find_symbol(m, END);

  /* Добавляем начало маршрута в стек секции для оценки
   * и запускаем главный цикл:
   * - выполняем, пока есть секции для оценки;
   * - если закончились, а на вершине visited не конечная
   *   точка - маршрут до точки не существует.
   */
  Stack__push(actual, start);
  while (!Stack__empty(actual)) {
    /* Вытаскиваем следующую секцию для оценки. */
    Waypoint c = Stack__top(actual);
    Stack__pop(actual);

    /* Добавляем ее к посещенным. */
    Stack__push(visited, c);

    /* Если конец маршрута, выходим - маршрут найден! */
    if (Waypoint__equal(c, end)) break;

    /* Создаем массив под максимум 8 соседей. */
    Waypoint neighbors[8];
    uint8_t nc = 0;

    Waypoint n;
    for (uint8_t i = 0; i < 8; i++) {
      n.dx = ((0x901a >> i*2) & 3) - 1;
      n.dy = ((0xa901 >> i*2) & 3) - 1;

      n.x = c.x - n.dx;
      n.y = c.y - n.dy;

      if (n.dx != 0 && n.dy != 0 && Map__get(m, n.x + n.dx, n.y) != FREE && Map__get(m, n.x, n.y + n.dy) != FREE) continue;

      if (Map__get(m, n.x, n.y) == BORDER) continue;
      if (Map__get(m, n.x, n.y) == OBSTACLE) continue;

      if (is_visited(n, visited)) continue;

      n.h = dist(n, end);

      neighbors[nc++] = n;
    }

    sort_waypoints(neighbors, nc);

    for (uint8_t i = 0; i < nc; i++) {
      Stack__push(actual, neighbors[i]);
    }
  }

  Stack *route = get_route(start, end, visited);

  Stack__free(actual);
  Stack__free(visited);

  return route;
}

void mark_route(Map *m, Stack *r) {
  Waypoint c;
  while (!Stack__empty(r)) {
    c = Stack__top(r);

    if (Map__get(m, c.x, c.y) == FREE) {
      Map__set(m, c.x, c.y, ROUTE);
    }

    Stack__pop(r);
  }
}


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

  Stack *route = find_route(map);

  mark_route(map, route);

  print_map(map, stdout);

  Stack__free(route);

  return 0;
}
