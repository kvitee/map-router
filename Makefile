INCLUDE_DIRECTORIES := include/

COMMON_OBJECTS := bin/Map.o bin/Waypoint.o bin/File_out.o
GENMAP_OBJECTS := bin/Gen_file.o $(COMMON_OBJECTS)
FINDROUTE_OBJECTS := bin/Alg.o bin/Var_str.o $(COMMON_OBJECTS)

all: genmap findroute

genmap: Gen_file.o
	gcc $(GENMAP_OBJECTS) -o bin/genmap

findroute: Alg.o
	gcc $(FINDROUTE_OBJECTS) -o bin/findroute

out_dirs:
	mkdir -p bin/

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

clean:
	rm bin/*.o
