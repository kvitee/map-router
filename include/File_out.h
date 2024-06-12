#ifndef FILE_OUT_H_INCLUDED_
#define FILE_OUT_H_INCLUDED_

#include <stdio.h>

#include "Map.h"


/**
 * Печатает карту в удобном для пользователя виде в f.
 */
void print_map(const Map *m, FILE *f);

/**
 * Сохраняет карту в формате, пригодном для считывания при помощи Map__read.
 */
void save_map(const Map *m, FILE *f);


#endif  /* FILE_OUT_H_INCLUDED_ */
