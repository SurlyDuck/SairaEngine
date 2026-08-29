.PHONY: all run clear

CFLAGS:= -ggdb -std=c99 -Wall -Wextra -pedantic
LFLAGS:= -lSDL3 -lSDL3_image -lSDL3_ttf

all: editor run

run:
	./editor

editor: editor.h editor.c menu.c ui.c level.c
	cc $(CFLAGS) -o editor $^ $(LFLAGS)

clear:
	rm editor
