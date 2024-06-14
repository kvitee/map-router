#include <stdlib.h>
#include <stdint.h>

#include "Var_str.h"


Stack *Stack__create(void) {
  Stack *s = malloc(sizeof(Stack));

  s->top = malloc(sizeof(Stack_Node));
  s->top->prev = NULL;

  return s;
}

uint8_t Stack__empty(const Stack *s) {
  return (s == NULL) || (s->top->prev == NULL);
}

Stack_data_t Stack__top(Stack *s) {
  if (s == NULL) {
    return (Stack_data_t){-1, -1, 0, 0, 0.0f};
  }

  return s->top->data;
}

void Stack__push(Stack *s, Stack_data_t data) {
  if (s == NULL) {
    return;
  }

  Stack_Node *n = malloc(sizeof(Stack_Node));

  n->prev = s->top;
  n->data = data;

  s->top = n;
}

void Stack__pop(Stack *s) {
  if (Stack__empty(s)) {
    return;
  }

  Stack_Node *top = s->top;

  s->top = s->top->prev;
  free(top);
}

void Stack__clear(Stack *s) {
  while (!Stack__empty(s)) {
    Stack__pop(s);
  }
}

void Stack__free(Stack *s) {
  if (s == NULL) {
    return;
  }

  Stack__clear(s);

  free(s->top);
  free(s);
}
