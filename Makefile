all:
	mkdir bin/; gcc src/*.c -I include/ -lm -o bin/main
