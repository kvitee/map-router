#include <stdint.h>

#include "Map.h"


uint8_t Waypoint__equal(Waypoint w1, Waypoint w2) {
  return (w1.x == w2.x) && (w1.y == w2.y);
}

Waypoint Waypoint__previous(Waypoint w) {
  return (Waypoint){w.x + w.dx, w.y + w.dy, 0, 0, 0};
}

uint8_t Waypoint__blocked(Waypoint w, const Map *m) {
  /* Все ячейки с учетом размера корабля должны быть свободны. */
  for (uint8_t i = 0; i < m->s; i++) {
    for (uint8_t j = 0; j < m->s; j++) {
      if (Map__cell_blocked(m, w.x + j, w.y + i)) {
        return 1;
      }
    }
  }

  /* Если корабль больше одной ячейки, этого достаточно, можно двигаться. */
  if (m->s > 1) return 0;

  /* Проверка на ситуацию, когда корабль проходит сквозь диагональное
   * препятствие:
   * .     @
   *   . @
   *   @ .
   * @     .
   */
  if (Map__cell_blocked(m, w.x + w.dx, w.y) &&
      Map__cell_blocked(m, w.x, w.y + w.dy)) return 1;

  /* Если проверка пройдена, движению ничего не мешает. */
  return 0;
}
