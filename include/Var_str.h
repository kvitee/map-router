#ifndef VAR_STR_H_INCLUDED_
#define VAR_STR_H_INCLUDED_

#include <stdint.h>

#include "Map.h"


#ifndef LIST_DATA_T
#define LIST_DATA_T Waypoint
#endif  /* LIST_DATA_T */

/* Тип данных, хранящихся в списке. */
typedef LIST_DATA_T List_data_t;

/* Узел списка */
typedef struct list_node {
  /* Указатель на следующий узел. */
  struct list_node *next;

  /* Хранящиеся данные. */
  List_data_t data;
} List_Node;

/* Список */
typedef struct list {
  /* Указатель на узел, являющийся "головой" списка (первый узел). */
  List_Node *head;
} List;


/**
 * Создает пустой список и возвращает указатель на него.
 */
List *List__create(void);

/**
 * Проверяет, является ли список пустым.
 * Возвращает 1, если список не содержит данных, иначе 0.
 */
uint8_t List__empty(const List *list);

/**
 * Возвращает данные, хранящиеся в начале списка.
 */
List_data_t List__head(List *list);

/**
 * Добавляет данные в начало списка.
 */
void List__push(List *list, List_data_t data);

/**
 * Удаляет данные из начала списка.
 */
void List__pop(List *list);

/**
 * Удаляет все данные списка.
 */
void List__clear(List *list);

/**
 * Удаляет все данные списка и освобождает выделенную под него память.
 */
void List__free(List *list);


#endif  /* VAR_STR_H_INCLUDED_ */
