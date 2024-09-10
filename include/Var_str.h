#ifndef VAR_STR_H_INCLUDED_
#define VAR_STR_H_INCLUDED_

#include <stdint.h>

#include "Map.h"


#ifndef LIST_DATA_T
#define LIST_DATA_T Waypoint
#define LIST_DATA_DEFAULT (LIST_DATA_T){ -1, -1, 0, 0, 0 }
#endif  /* LIST_DATA_T */

/* Тип данных, хранящихся в списке. */
typedef LIST_DATA_T List_data_t;

/* Узел списка */
typedef struct list_node {
  /* Указатель на следующий узел. */
  struct list_node *next;

  /* Указатель на предыдущий узел. */
  struct list_node *prev;

  /* Хранящиеся данные. */
  List_data_t data;
} List_Node;

/* Список (двусвязный) */
typedef struct list {
  /* Указатель на узел, являющийся "головой" списка (первый узел). */
  List_Node *head;

  /* Указатель на узел, являющийся "хвостом" списка (последний узел). */
  List_Node *tail;

  /* При этом сами head и tail не хранят никаких данных.
   * Логически, первый элемент списка хранится в узле head->next,
   * а последний - в tail->prev.
   * Это более удобно с точки зрения реализации, и при этом почти никак
   * не сказыается на производительности, особенно, если используется
   * малое количество списков с большим количеством данных. */
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
 * Возвращает первый элемент списка.
 */
List_data_t List__head(const List *list);

/**
 * Возвращает последний элемент списка.
 */
List_data_t List__tail(const List *list);

/**
 * Добавляет данные в начало списка.
 */
void List__push_head(List *list, List_data_t data);

/**
 * Удаляет первый элемент списка.
 */
void List__pop_head(List *list);

/**
 * Добавляет данные в конец списка.
 */
void List__push_tail(List *list, List_data_t data);

/**
 * Удаляет последний элемент списка.
 */
void List__pop_tail(List *list);

/**
 * Удаляет все элементы списка.
 */
void List__clear(List *list);

/**
 * Удаляет все элементы списка и освобождает выделенную под него память.
 */
void List__free(List *list);


#endif  /* VAR_STR_H_INCLUDED_ */
