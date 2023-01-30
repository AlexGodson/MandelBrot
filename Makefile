CC = gcc

all: mandel

mandel: main.c
	$(CC) -O3 main.c -o mandel

run: mandel
	./mandel

clear:
	rm mandel
	rm MandelbrotSet.bmp