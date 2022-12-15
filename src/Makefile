#executable
exec = sencillo.exe
#connecting the makefile to the src folder with extence .c
sources = $(wildcard *.c)
#change the file extension
objects = $(sources:.c=.o)
#debugging
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

#creating the o files - detect file in the src file
%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./sencillo.exe

#clean command - recompile
clean:
	-rm *.exe
	-rm *.o
	-rm src/*.o
