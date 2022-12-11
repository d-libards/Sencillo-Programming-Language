#executable
exec = sencillo.exe
#connecting the makefile to the src folder with extence .c
sources = $(wildcard *.c)
#change the file extension
objects = $(sources:.c=.sn)
#debugging
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

#creating the sn files - detect file in the src file
%.sn: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./sencillo.exe/usr/local/bin/sencillo

#clean command - recompile
clean:
	-rm *.exe
	-rm *.sn
	-rm src/*.sn
