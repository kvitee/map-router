#include <stdlib.h>
#include <stdint.h>

#include "Var_str.h"


List *List__create(void) {
  List *list = malloc(sizeof(List));

  list->head = malloc(sizeof(List_Node));
  list->head->next = NULL;

  return list;
}

uint8_t List__empty(const List *list) {
  return (list == NULL) || (list->head->next == NULL);
}

List_data_t List__head(List *list) {
  if (list == NULL) {
    return (List_data_t){-1, -1, 0, 0, 0.0f};
  }

  return list->head->data;
}

void List__push(List *list, List_data_t data) {
  if (list == NULL) {
    return;
  }

  List_Node *node = malloc(sizeof(List_Node));

  node->next = list->head;
  node->data = data;

  list->head = node;
}

void List__pop(List *list) {
  if (List__empty(list)) {
    return;
  }

  List_Node *head = list->head;

  list->head = list->head->next;
  free(head);
}

void List__clear(List *list) {
  while (!List__empty(list)) {
    List__pop(list);
  }
}

void List__free(List *list) {
  if (list == NULL) {
    return;
  }

  List__clear(list);

  free(list->head);
  free(list);
}
