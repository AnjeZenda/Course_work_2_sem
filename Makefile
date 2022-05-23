all: main.o help.o copy.o transform.o rects.o frames.o
	gcc main.o help.o copy.o transform.o rects.o frames.o -lpng -o PNG.out

main.o: main.c
	gcc -c main.c

help.o: help.c
	gcc -c help.c

copy.o: copy.c
	gcc -c copy.c

transform.o: transform.c
	gcc -c transform.c

rects.o: rects.c
	gcc -c rects.c

frames.o: frames.c
	gcc -c frames.c

clean:
	rm -r *.o