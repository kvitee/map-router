# Поиск маршрута на карте

## О проекте

Проект представляет собой программу для поиска маршрута на двумерной карте.
В последней своей версии использует алгоритм, позволяющий находить минимальный
по длине маршрут.

## Результат работы

```
# # # # # # # # # # # # # # # # # # # #
#                                     #
#   @         @     @       @         #
#     @             @ . A     @       #
#       @       @   .             @   #
#         @       . @ @ @ @ @ @   @   #
#   @       @     .               @   #
#             @   .                   #
#   @   @ @     @ .     @     @       #
#                 @ .     @           #
#     @             @ .     @     @   #
#             @       @ .       @     #
#     @   @             @ .   @       #
#     @   @     @         @ .     @   #
#     @   @       @         @ .       #
#     @   @   B .   @         @ . @   #
#     @   @     @ .   @ . . . .       #
#     @   @   @     . .   @       @   #
#                                     #
# # # # # # # # # # # # # # # # # # # #
```

## Сборка

Для сборки проекта необходимо воспользоваться `make`:

```sh
make
```

После чего в директории `bin/` будут доступны 2 исполняемых файла:

- `genmap` - Позволяет сгенерировать новую карту.
- `findroute` - Выполняет поиск маршрута на имеющейся карте.

Также, при помощи `make` можно удалить ненужные объектные файлы:

```sh
make clean
```
