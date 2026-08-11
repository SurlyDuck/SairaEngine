.PHONY: all run clear

CFLAGS:= -ggdb -std=c99 -Wall -Wextra -pedantic
LFLAGS:= -lSDL3 -lSDL3_image

all: editor run

run:
	./editor

editor: editor.c menu.c ui.c
	cc $(CFLAGS) -o editor editor.c menu.c ui.c $(LFLAGS)

clear:
	rm editor
