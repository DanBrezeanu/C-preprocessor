SRC_DIR=src
EXEC=preproc

build:
	cl.exe /MD /Iinclude $(SRC_DIR)/*.c /o$(EXEC).exe
clean:
	del $(EXEC).exe *.obj
