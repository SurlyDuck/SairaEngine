.PHONY: all run clear

CFLAGS:= -ggdb -std=c99 -Wall -Wextra -pedantic
LFLAGS:= -lSDL3

all: editor run

run:
	./editor

editor: editor.c
	cc $(CFLAGS) -o editor editor.c $(LFLAGS)

clear:
	rm editor
