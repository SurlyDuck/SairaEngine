.PHONY: all run clear

CFLAGS:= -ggdb -std=c99 -Wall -Wextra -pedantic
LFLAGS:= -lSDL3 -lSDL3_image -lSDL3_ttf -L./nfd -lnfd `pkg-config --libs gtk+-3.0`

all: editor run

run:
	./editor

editor: editor.h editor.c menu.c ui.c level.c
	cc $(CFLAGS) -o editor editor.c menu.c ui.c level.c $(LFLAGS)

clear:
	rm editor
