#include <stdio.h>
#include <stdint.h>

#include "Menu.h"


int main() {
  uint8_t finished = 0;

  while (!finished) {
    printf("Выберите функцию программы:\n");
    printf("[g] Сгенерировать новую карту;\n");
    printf("[f] Найти маршрут на карте;\n");
    printf("[q] Завершить программу.\n");
    printf("\n");

    char choice;
    printf("Ваш выбор: ");

    while (1) {
      choice = getc(stdin);

      if (choice == 'g') {
        gen_file_option();
      }
      else if (choice == 'f') {
        find_route_option();
      }
      else if (choice == 'q') {
        finished = 1;
      }
      else {
        printf("Такой функции нет, попробуйте снова: ");
        continue;
      }

      break;
    }
  }

  return 0;
}
