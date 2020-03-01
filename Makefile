.PHONY:	build



build:
	gcc main.c Stack.c HashMap.c parse_args.c polish_notation.c utils.c -Wall -Wextra -Wno-pointer-sign -Wno-sign-compare -Wno-implicit-function-declaration --std=gnu89 -o so -g

clean:
	rm -rf *.o so polish