#include <stdint.h>

#include "Waypoint.h"


uint8_t Waypoint__equal(Waypoint w1, Waypoint w2) {
  return (w1.x == w2.x) && (w1.y == w2.y);
}

Waypoint Waypoint__parent(Waypoint w) {
  return (Waypoint){w.x + w.dx, w.y + w.dy, 0, 0, 0.0f};
}
