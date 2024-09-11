#ifndef MAP_H_INCLUDED_
#define MAP_H_INCLUDED_

#include <stdio.h>
#include <stdint.h>


/* Количество различных символов карты, используемых для ее отрисовки. */
#define MAP_SYMBOLS_COUNT 6

/* Перечисление кодов символов карты. */
typedef enum map_symbol_code {
  /* Свободная ячейка */
  FREE,
  /* Граница карты */
  BORDER,
  /* Препятствие */
  OBSTACLE,
  /* Начало маршрута */
  START,
  /* Конец маршрута */
  END,
  /* Промежуточная точка маршрута */
  ROUTE,
} Map_symbol_code;

/* Карта */
typedef struct map {
  /* Размеры карты (длина и ширина). До 255x255 */
  uint8_t w;
  uint8_t h;

  /* Символы, используемые для отрисовки. */
  char symbols[MAP_SYMBOLS_COUNT];

  /* Размер корабля. */
  uint8_t s;

  /* Разметка карты (что в какой ячейке находится).
   * Для удобства хранится как одномерный массив,
   * хотя логически он все еще двумерный.
   */
  Map_symbol_code *cells;
} Map;

/* Точка маршрута */
typedef struct waypoint {
  /* Координаты текущей точки маршрута. */
  uint8_t x;
  uint8_t y;

  /* Направление до предыдущей точки маршрута. */
  int8_t dx;
  int8_t dy;

  /*
   * Итого, координаты предыдущей точки маршрута вычисляются как:
   * px = x + dx;
   * py = y + dy;
   */

  /* Количество ячеек, пройденных от начала маршрута. */
  uint32_t h;
} Waypoint;


/**
 * Создает карту с размерами w и h и возвращает указатель на нее.
 * По умолчанию, используется следующий набор символов для отрисовки:
 * ' ' - свободная ячейка;
 * '#' - граница карты;
 * '@' - препятствие;
 * 'A' - начало маршрута;
 * 'B' - конец маршрута;
 * '.' - промежуточная точка маршрута.
 */
Map *Map__create(uint8_t w, uint8_t h);

/**
 * Создает новую карту, считав ее данные из файлового потока f,
 * чаще всего - открытого на чтение файла.
 */
Map *Map__read(FILE *f);

/**
 * Возвращает код символа карты по координатам x и y.
 */
Map_symbol_code Map__get(const Map *m, uint8_t x, uint8_t y);

/**
 * Устанавливает код символа карты по координатам x и y на s.
 */
void Map__set(Map *m, uint8_t x, uint8_t y, Map_symbol_code s);

/**
 * Ищет первую ячейку карты, содержащую символ s.
 * Возвращает соответствующую ей точку маршрута.
 */
Waypoint Map__find(const Map *m, Map_symbol_code s);

/* Проверяет, является ли ячейка карты m с координатами x и y
 * заблокированной для нахождения в ней, то есть является
 * границей карты или препятствием.
 */
uint8_t Map__cell_blocked(const Map *m, uint8_t x, uint8_t y);

/**
 * Освобождает выделенную под карту память.
 */
void Map__free(Map *m);

/**
 * Сравнивает две точки маршрута.
 * Возвращает 1, если их координаты равны, иначе 0.
 */
uint8_t Waypoint__equal(Waypoint w1, Waypoint w2);

/**
 * Возвращает точку маршрута, предшествующую данной.
 */
Waypoint Waypoint__previous(Waypoint w);

/**
 * Проверяет, перекрыт ли путь между точкой w и предшествующей ей точкой
 * с учетом препятствий на карте m.
 * Возвращает 1, если да, иначе 0.
 */
uint8_t Waypoint__blocked(Waypoint w, const Map *m);


#endif  /* MAP_H_INCLUDED_ */
