# Директории с алами заголовков.
INCLUDE_DIRECTORIES := include/

# Объектные файлы, общие для генерации карт и для поиска маршрутов.
COMMON_OBJECTS := bin/Map.o bin/Waypoint.o bin/File_out.o

# Объектные файлы, специфичные для генерации карт + общие.
GENMAP_OBJECTS := bin/Gen_file.o $(COMMON_OBJECTS)

# Объектные файлы, специфичные для поиска маршрутов + общие.
FINDROUTE_OBJECTS := bin/Alg.o bin/Var_str.o $(COMMON_OBJECTS)

# Сборка всех исполняемых файлов (genmap и findroute).
all: genmap findroute

# Сборка исполняемого файла для генерации карт.
genmap: Gen_file.o
	gcc $(GENMAP_OBJECTS) -o bin/genmap

# Сборка исполняемого файла для поиска маршрутов.
findroute: Alg.o
	gcc $(FINDROUTE_OBJECTS) -o bin/findroute

# Создание директории для генерируемых файлов (.o и исполняемых).
out_dirs:
	mkdir -p bin/

# Правила для генерации каждого объектного файла.

Gen_file.o: out_dirs Map.o File_out.o
	gcc -c src/Gen_file.c -I $(INCLUDE_DIRECTORIES) -o bin/Gen_file.o

Alg.o: out_dirs Map.o File_out.o Var_str.o
	gcc -c src/Alg.c -I $(INCLUDE_DIRECTORIES) -o bin/Alg.o

Map.o: out_dirs Waypoint.o
	gcc -c src/Map.c -I $(INCLUDE_DIRECTORIES) -o bin/Map.o

File_out.o: out_dirs
	gcc -c src/File_out.c -I $(INCLUDE_DIRECTORIES) -o bin/File_out.o

Waypoint.o: out_dirs
	gcc -c src/Waypoint.c -I $(INCLUDE_DIRECTORIES) -o bin/Waypoint.o

Var_str.o: out_dirs
	gcc -c src/Var_str.c -I $(INCLUDE_DIRECTORIES) -o bin/Var_str.o

# Удалить ненужные объектные файлы.
clean:
	rm bin/*.o
