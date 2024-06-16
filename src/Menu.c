#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Menu.h"


int main() {
#ifdef _WIN32
  SetConsoleCP(65001);
  SetConsoleOutputCP(65001);
#endif

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

    while (getc(stdin) != '\n');
  }

  return 0;
}
