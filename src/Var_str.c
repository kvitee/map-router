#include <stdlib.h>
#include <stdint.h>

#include "Var_str.h"


List *List__create(void) {
  List *list = malloc(sizeof(List));

  list->head = malloc(sizeof(List_Node));
  list->tail = malloc(sizeof(List_Node));

  list->head->next = list->tail;
  list->tail->prev = list->head;

  list->head->prev = NULL;
  list->tail->next = NULL;

  return list;
}

uint8_t List__empty(const List *list) {
  return (list == NULL) || (list->head->next == list->tail);
}

List_data_t List__head(const List *list) {
  if (List__empty(list)) {
    return LIST_DATA_DEFAULT;
  }

  return list->head->next->data;
}

List_data_t List__tail(const List *list) {
  if (List__empty(list)) {
    return LIST_DATA_DEFAULT;
  }

  return list->tail->prev->data;
}

void List__push_head(List *list, List_data_t data) {
  if (list == NULL) {
    return;
  }

  List_Node *node = malloc(sizeof(List_Node));

  node->data = data;
  node->prev = list->head;
  node->next = list->head->next;

  node->prev->next = node;
  node->next->prev = node;
}

void List__pop_head(List *list) {
  if (List__empty(list)) {
    return;
  }

  List_Node *node = list->head->next;

  node->prev->next = node->next;
  node->next->prev = node->prev;

  free(node);
}

void List__push_tail(List *list, List_data_t data) {
  if (list == NULL) {
    return;
  }

  List_Node *node = malloc(sizeof(List_Node));

  node->data = data;
  node->prev = list->tail->prev;
  node->next = list->tail;

  node->prev->next = node;
  node->next->prev = node;
}

void List__pop_tail(List *list) {
  if (List__empty(list)) {
    return;
  }

  List_Node *node = list->tail->prev;

  node->prev->next = node->next;
  node->next->prev = node->prev;

  free(node);
}

void List__clear(List *list) {
  while (!List__empty(list)) {
    List__pop_head(list);
  }
}

void List__free(List *list) {
  if (list == NULL) {
    return;
  }

  List__clear(list);

  free(list->head);
  free(list->tail);

  free(list);
}
