#ifndef WAYPOINT_H_INCLUDED_
#define WAYPOINT_H_INCLUDED_

#include <stdint.h>


/* Точка маршрута */
typedef struct waypoint {
  /* Координаты текущей точки маршрута. */
  uint8_t x;
  uint8_t y;

  /* Направление предыдущей точки маршрута. */
  int8_t dx;
  int8_t dy;

  /*
   * Итого, координаты предыдущей точки маршрута вычисляются как:
   * px = x + dx;
   * py = y + dy;
   */

  /* Расстояние от данной точки маршрута до конечной. */
  float h;
} Waypoint;

/**
 * Сравнивает две точки маршрута.
 * Возвращает 1, если их координаты равны, иначе 0.
 */
uint8_t Waypoint__equal(Waypoint w1, Waypoint w2);

/**
 * Возвращает точку маршрута, предшествующую данной.
 */
Waypoint Waypoint__parent(Waypoint w);


#endif  /* WAYPOINT_H_INCLUDED_ */
