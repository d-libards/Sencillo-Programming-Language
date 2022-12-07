#executable
exec = sencillo.exe
#connecting the makefile to the src folder with extence .c
sources = $(wildcard src/*c)
#change the file extension
object = $(sources:.c=.sn)
#debugging
flags = -g