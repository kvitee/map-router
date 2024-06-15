#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Var_str.h"
#include "File_out.h"


/**
 * Проверяет, является ли ячейка посещенной на основе переданного
 * стека посещенных ячеек.
 *
 * Возвращает 1 если ячейка посещена (находится в visited), иначе 0.
 */
uint8_t waypoint_visited(Waypoint w, const Stack *visited) {
  Stack_Node *cn = visited->top;
  while (cn->prev != NULL) {
    if (Waypoint__equal(cn->data, w)) {
      return 1;
    }

    cn = cn->prev;
  }

  return 0;
}

/**
 * Сортирует массив точек маршрута arr длины len в порядке убывания
 * их расстояний от конца маршрута (величины h).
 */
void sort_waypoints(Waypoint *arr, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    for (uint8_t j = 0; j < len; j++) {
      if (arr[i].h > arr[j].h) {
        Waypoint tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}

/**
 * Основная функция для поиска маршрута на карте.
 * Не изменяет саму карту.
 * Возвращает стек точек маршрута, если таковой найден, иначе NULL.
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
  Waypoint start = Map__find(m, START);
  Waypoint end = Map__find(m, END);

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

    /* Массив с парами смещений для каждой соседней ячейки. */
    static int8_t directions[8][2] = {
      {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}
    };

    /* Перебираем соседние ячейки. */
    Waypoint n;
    for (uint8_t i = 0; i < 8; i++) {
      n.dx = directions[i][0];
      n.dy = directions[i][1];

      n.x = c.x - n.dx;
      n.y = c.y - n.dy;

      /* Если в выбранном направлении невозможно переместиться,
       * пропускаем ячейку. */
      if (Waypoint__blocked(n, m)) continue;

      /* Если ячейка уже посещена, также пропускаем ее. */
      if (waypoint_visited(n, visited)) continue;

      /* Вычисляем расстояние до конца маршрута. */
      n.h = Waypoint__distance(n, end);

      /* Сохраняем в массив соседей. */
      neighbors[nc++] = n;
    }

    /* Сортируем соседей по убыванию их расстояния до конца маршрута,
     * чтобы после добавления их в стек actual на вершине находилась
     * ячейка с наименьшим расстоянием до конца маршрута.
     */
    sort_waypoints(neighbors, nc);

    for (uint8_t i = 0; i < nc; i++) {
      Stack__push(actual, neighbors[i]);
    }
  }

  /* Если после завершения основного цикла на вершине стека с посещенными
   * ячейками находится не конец маршрута, то маршрут не найден, и
   * функция возвращает NULL - пустой маршрут.
   */
  if (!Waypoint__equal(Stack__top(visited), end)) {
    return NULL;
  }

  /* Создаем финальный стек, содержащий */
  Stack *route = Stack__create();

  /* Добавляем конец маршрута и вычисляем предыдущую точку маршрута (p). */
  Stack__push(route, end);
  Waypoint p = Waypoint__parent(end);

  /* Основной цикл, работает, пока есть посещенные вершины.
   * Выполняет поиск предыдущей точки маршрута в стеке посещенных,
   * Когда находит, добавляет ее в стек маршрута и обновляет p,
   * чтобы на следующих итерациях искалась уже ее предыдущая точка.
   */
  while (!Stack__empty(visited)) {
    /* Если точка на вершине стека не является искомой, пропускаем ее. */
    if (!Waypoint__equal(p, Stack__top(visited))) {
      Stack__pop(visited);
      continue;
    }

    /* Добавляем найденную точку в стек маршрута. */
    Stack__push(route, Stack__top(visited));
    Stack__pop(visited);

    /* Если найденная точка - начало маршрута, выходим из цикла. */
    if (Waypoint__equal(Stack__top(route), start)) break;

    /* Обновляем искомую точку. */
    p = Waypoint__parent(Stack__top(route));
  }

  /* Очищаем все вспомогательные стеки. */
  Stack__free(actual);
  Stack__free(visited);

  return route;
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

  /* Выполняем поиск маршрута. */
  Stack *route = find_route(map);

  /* Если маршрут не найден, выводим сообщение об этом. */
  if (route == NULL) {
    printf("Маршрут не найден.\n");
  }

  /* Отмечаем найденный маршрут на карте. */
  Waypoint c;
  while (!Stack__empty(route)) {
    c = Stack__top(route);

    for (uint8_t i = 0; i < map->s; i++) {
      for (uint8_t j = 0; j < map->s; j++) {
        if (Map__get(map, c.x + j, c.y + i) != FREE) continue;

        Map__set(map, c.x + j, c.y + i, ROUTE);
      }
    }

    Stack__pop(route);
  }

  /* Выводим карту с найденным маршрутом в консоль. */
  print_map(map, stdout);

  Stack__free(route);

  return 0;
}
