CFLAGS=-Wall -Werror

.PHONY: clean run-main run-interactive

build: main interactive

run-main: main
	./main

run-interactive: interactive
	./interactive

interactive: interactive.c imageprocessing.c bmp.c
	gcc -c interactive.c imageprocessing.c bmp.c $(CFLAGS)
	gcc interactive.o imageprocessing.o bmp.o -o interactive

main: main.c imageprocessing.c bmp.c
	gcc -c main.c imageprocessing.c bmp.c $(CFLAGS)
	gcc main.o imageprocessing.o bmp.o -o main

clean:
	rm -f main interactive *.o