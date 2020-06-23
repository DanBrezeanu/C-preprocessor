CC=gcc
CFLAGS=-Wall -Wextra -Wno-pointer-sign -Wno-sign-compare -Wno-implicit-function-declaration --std=gnu89
SRC_DIR=src

SOURCES=$(SRC_DIR)/*.c
EXEC=preproc

INCLUDE=-I include/

.PHONY:	build


build:
	$(CC) $(SOURCES) $(CFLAGS) $(INCLUDE) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
