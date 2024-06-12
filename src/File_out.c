#include <stdio.h>

#include "Map.h"


void save_map(const Map *m, FILE *f) {
  fprintf(f, "%hhu;%hhu;%s", m->w, m->h, m->symbols);

  for (int i = 0; i < m->w * m->h; i++) {
    fputc('0' + m->cells[i], f);
  }

  fputc('\n', f);
}

void print_map(const Map *m, FILE *f) {
  for (int i = 0; i < m->w * m->h; i++) {
    fputc(m->symbols[m->cells[i]], f);

    if ((i + 1) % m->w == 0) {
      fputc('\n', f);
    } else {
      fputc(' ', f);
    }
  }
}