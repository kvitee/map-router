#ifndef VAR_STR_H_INCLUDED_
#define VAR_STR_H_INCLUDED_

#include <stdint.h>

#include "Waypoint.h"


/* Тип данных, хранящихся в стеке. */
typedef Waypoint Stack_data_t;

/* Узел стека */
typedef struct stack_node {
  /* Указатель на предыдущий узел. */
  struct stack_node *prev;

  /* Хранящиеся данные. */
  Stack_data_t data;
} Stack_Node;

/* Стек */
typedef struct stack {
  /* Указатель на узел, являющийся вершиной стека. */
  Stack_Node *top;
} Stack;


/**
 * Создает пустой стек и возвращает указатель на него.
 */
Stack *Stack__create(void);

/**
 * Проверяет, является ли стек пустым.
 * Возвращает 1, если стек не содержит данных, иначе 0.
 */
uint8_t Stack__empty(const Stack *s);

/**
 * Возвращает данные, хранящиеся на вершине стека.
 */
Stack_data_t Stack__top(Stack *s);

/**
 * Добавляет данные на вершину стека.
 */
void Stack__push(Stack *s, Stack_data_t data);

/**
 * Удаляет данные с вершины стека.
 */
void Stack__pop(Stack *s);

/**
 * Удаляет все данные стека.
 */
void Stack__clear(Stack *s);

/**
 * Удаляет все данные стека и освобождает выделенную под него память.
 */
void Stack__free(Stack *s);


#endif  /* VAR_STR_H_INCLUDED_ */
