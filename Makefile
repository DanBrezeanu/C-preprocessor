CC=gcc
CFLAGS=-Wall -Wextra -Wno-pointer-sign -Wno-sign-compare -Wno-implicit-function-declaration --std=gnu89 -g

SOURCES=main.c Stack.c HashMap.c parse_args.c polish_notation.c utils.c preprocessor.c
EXEC=so-cpp

INCLUDE=-I include/

.PHONY:	build


build:
	$(CC) $(SOURCES) $(CFLAGS) $(INCLUDE) -o $(EXEC)

clean:
	rm -rf *.o so polish