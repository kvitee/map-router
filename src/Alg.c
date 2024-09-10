#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Var_str.h"
#include "File_out.h"


/**
 * Проверяет, является ли ячейка посещенной на основе переданного
 * списка посещенных ячеек.
 *
 * Возвращает 1 если ячейка посещена (находится в visited), иначе 0.
 */
uint8_t waypoint_visited(Waypoint w, const List *visited) {
  List_Node *cn = visited->head;
  while (cn->next != NULL) {
    if (Waypoint__equal(cn->data, w)) {
      return 1;
    }

    cn = cn->next;
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
 * Возвращает список точек маршрута, если таковой найден, иначе NULL.
 */
List *find_route(const Map *m) {
  /* Актуальный список - список с ячейками, которые предстоит оценить. */
  List *actual = List__create();

  /* Список с посещенными ячейками, по которым в конце будет
   * производиться трассировка пути.
   */
  List *visited = List__create();

  /* Начало и конец маршрута. */
  Waypoint start = Map__find(m, START);
  Waypoint end = Map__find(m, END);

  /* Добавляем начало маршрута в список ячеек для оценки
   * и запускаем главный цикл:
   * - выполняем, пока есть ячейки для оценки;
   * - если закончились, а на вершине visited не конечная
   *   точка - маршрут до точки не существует.
   */
  List__push(actual, start);
  while (!List__empty(actual)) {
    /* Вытаскиваем следующую ячейку для оценки. */
    Waypoint c = List__head(actual);
    List__pop(actual);

    /* Добавляем ее к посещенным. */
    List__push(visited, c);

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
     * чтобы после добавления их в список actual на вершине находилась
     * ячейка с наименьшим расстоянием до конца маршрута.
     */
    sort_waypoints(neighbors, nc);

    for (uint8_t i = 0; i < nc; i++) {
      List__push(actual, neighbors[i]);
    }
  }

  /* Если после завершения основного цикла на вершине списка с посещенными
   * ячейками находится не конец маршрута, то маршрут не найден, и
   * функция возвращает NULL - пустой маршрут.
   */
  if (!Waypoint__equal(List__head(visited), end)) {
    return NULL;
  }

  /* Создаем финальный список, содержащий ячейки найденного маршрута. */
  List *route = List__create();

  /* Добавляем конец маршрута и вычисляем предыдущую точку маршрута (p). */
  List__push(route, end);
  Waypoint p = Waypoint__parent(end);

  /* Основной цикл, работает, пока есть посещенные вершины.
   * Выполняет поиск предыдущей точки маршрута в списке посещенных,
   * Когда находит, добавляет ее в список маршрута и обновляет p,
   * чтобы на следующих итерациях искалась уже ее предыдущая точка.
   */
  while (!List__empty(visited)) {
    /* Если точка на вершине списка не является искомой, пропускаем ее. */
    if (!Waypoint__equal(p, List__head(visited))) {
      List__pop(visited);
      continue;
    }

    /* Добавляем найденную точку в список маршрута. */
    List__push(route, List__head(visited));
    List__pop(visited);

    /* Если найденная точка - начало маршрута, выходим из цикла. */
    if (Waypoint__equal(List__head(route), start)) break;

    /* Обновляем искомую точку. */
    p = Waypoint__parent(List__head(route));
  }

  /* Очищаем все вспомогательные списки. */
  List__free(actual);
  List__free(visited);

  return route;
}


int main() {
  FILE *f = fopen("map.txt", "r");

  if (f == NULL) {
    printf("Файл map.txt не существует.\n");
    return 1;
  }

  Map *map = Map__read(f);
  fclose(f);

  /* Выполняем поиск маршрута. */
  List *route = find_route(map);

  /* Если маршрут не найден, выводим сообщение об этом. */
  if (route == NULL) {
    printf("Маршрут не найден.\n");
  }

  /* Отмечаем найденный маршрут на карте. */
  Waypoint c;
  while (!List__empty(route)) {
    c = List__head(route);

    for (uint8_t i = 0; i < map->s; i++) {
      for (uint8_t j = 0; j < map->s; j++) {
        if (Map__get(map, c.x + j, c.y + i) != FREE) continue;

        Map__set(map, c.x + j, c.y + i, ROUTE);
      }
    }

    List__pop(route);
  }

  /* Выводим карту с найденным маршрутом в консоль. */
  print_map(map, stdout);

  /* Очищаем список с маршрутом. */
  List__free(route);

  return 0;
}
