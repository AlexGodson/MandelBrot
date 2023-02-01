CC = gcc

all: mandel

mandel: main.c
	$(CC) -Wall -Werror -O2 main.c -o mandel -lm

run: mandel
	./mandel

clear:
	rm mandel
	rm MandelbrotSet.bmp