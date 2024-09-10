#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Var_str.h"
#include "File_out.h"


List *trace_route(Waypoint start, Waypoint end, List *visited) {
  /* Если в начале списка с посещенными ячейками находится не конец маршрута,
   * то маршрут не найден, возвращаем NULL - пустой маршрут.
   */
  if (!Waypoint__equal(List__head(visited), end)) {
    return NULL;
  }

  /* Создаем список, содержащий ячейки найденного маршрута. */
  List *route = List__create();

  /* Добавляем конец маршрута и вычисляем предыдущую точку маршрута (p). */
  List__push_head(route, end);
  Waypoint p = Waypoint__parent(end);

  /* Основной цикл, работает, пока есть посещенные ячейки.
   * Выполняем поиск предыдущей точки маршрута в списке посещенных.
   * Когда находим, добавляем ее в список маршрута и обновляем p,
   * чтобы на следующих итерациях искалась уже ее предыдущая точка.
   */
  while (!List__empty(visited)) {
    /* Если точка в начале списка не является искомой, пропускаем ее. */
    if (!Waypoint__equal(p, List__head(visited))) {
      List__pop_head(visited);
      continue;
    }

    /* Добавляем найденную точку в список маршрута. */
    List__push_head(route, List__head(visited));
    List__pop_head(visited);

    /* Если найденная точка - начало маршрута, выходим из цикла. */
    if (Waypoint__equal(List__head(route), start)) break;

    /* Обновляем искомую точку. */
    p = Waypoint__parent(List__head(route));
  }

  return route;
}

/**
 * Основная функция для поиска маршрута на карте. Не изменяет саму карту.
 * Возвращает список точек маршрута, если таковой найден, иначе NULL.
 */
List *find_route(const Map *m) {
  /* Список с ячейками, которые предстоит оценить. */
  List *actual = List__create();

  /* Список с посещенными ячейками, по которым в конце будет
   * производиться трассировка пути.
   */
  List *visited = List__create();

  /* Начало и конец маршрута. */
  Waypoint start = Map__find(m, START);
  Waypoint end = Map__find(m, END);

  /* Добавляем начало маршрута в список ячеек для оценки. */
  List__push_tail(actual, start);

  /* Главный цикл:
   * - выполняем, пока есть ячейки для оценки;
   * - если закончились, и первый элемент visited конечная точка,
   *   то маршрут найден, иначе - он не найден.
   */
  while (!List__empty(actual)) {
    /* Вытаскиваем следующую ячейку для оценки. */
    Waypoint c = List__head(actual);
    List__pop_head(actual);

    /* Добавляем текущую ячейку к посещенным. */
    List__push_head(visited, c);

    /* Если конец маршрута, выходим - маршрут найден! */
    if (Waypoint__equal(c, end)) break;

    /* Массив с парами смещений для каждой соседней ячейки. */
    static int8_t directions[8][2] = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}
    };

    /* Перебираем соседние ячейки. */
    Waypoint n;
    for (uint8_t i = 0; i < 8; i++) {
      /* Берем смещение из заранее заготовленного массива. */
      n.dx = directions[i][0];
      n.dy = directions[i][1];

      /* Вычисляем собственные координаты. */
      n.x = c.x - n.dx;
      n.y = c.y - n.dy;

      /* Увеличиваем количество пройденных ячеек на 1. */
      n.h = c.h + 1;

      /* Если в выбранном направлении невозможно переместиться,
       * пропускаем ячейку. */
      if (Waypoint__blocked(n, m)) continue;

      /* Если ячейка уже посещена, также пропускаем ее. */
      if (List__contains(visited, n)) continue;

      /* Если ячейка находится в списке actual,
       * добавлять ее туда второй раз не нужно. */
      if (List__contains(actual, n)) continue;

      /* Если все хорошо, добавляем в список для оценки. */
      List__push_tail(actual, n);
    }
  }

  /* Выполняем трассировку пути. */
  List *route = trace_route(start, end, visited);

  /* Очищаем все вспомогательные списки. */
  List__free(actual);
  List__free(visited);

  return route;
}


int main(int argc, char **argv) {
  char fn[50];
  FILE *f;

  /* Название файла из командной строки. */
  if (argc > 1) {
    f = fopen(argv[1], "r");
  }

  /* Файл по умолчанию - map.txt. */
  if (f == NULL) {
    f = fopen("map.txt", "r");
  }

  /* Запрашиваем название файла у пользователя. */
  if (f == NULL) {
    printf("Файл map.txt не существует.\n");
    printf("Введите название файла с картой: ");

    scanf("%s", fn);
    FILE *f = fopen(fn, "r");
  }

  /* Ни одного файла нет - завершаем программу. */
  if (f == NULL) {
    printf("Файл %s не существует.\n", fn);
    return 1;
  }

  /* Считываем карту и закрываем файл. */
  Map *map = Map__read(f);
  fclose(f);

  /* Выполняем поиск маршрута. */
  List *route = find_route(map);

  /* Если маршрут не найден, выводим сообщение об этом
   * и карту без маршрута. */
  if (route == NULL) {
    printf("Маршрут не найден.\n");
    print_map(map, stdout);

    return 0;
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

    List__pop_head(route);
  }

  /* Выводим карту с найденным маршрутом в консоль. */
  print_map(map, stdout);

  /* Очищаем список с маршрутом. */
  List__free(route);

  return 0;
}
