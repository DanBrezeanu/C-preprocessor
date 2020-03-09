build:
	cl.exe /MD /Iinclude utils.c main.c Stack.c HashMap.c parse_args.c polish_notation.c  preprocessor.c /oso-cpp.exe
clean:
	del so-cpp.exe *.obj
