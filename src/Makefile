.PHONY: all run linux windows android clear

CFLAGS:= -I -std=c11 -Wall -Wextra -pedantic
LFLAGS_LINUX:= -L./lib_linux/ -lraylib -lm -lX11
LFLAGS_WINDOWS:= -L./lib_win/ -lraylib -lm -lgdi32 -lwinmm
GAME:= GameName
GAME_FILES:= ./game/game.c
ENGINE_FILES:= engine.c

all: linux windows android run

linux:   ../bin/linux/$(GAME)
windows: ../bin/windows/$(GAME).exe
android: ../bin/android/$(GAME)
run:
	../bin/linux/./$(GAME)

../bin/linux/$(GAME): $(GAME_FILES) $(ENGINE_FILES)
	gcc $(CFLAGS) -o ../bin/linux/$(GAME) $(ENGINE_FILES) $(GAME_FILES) $(LFLAGS_LINUX)

../bin/windows/$(GAME).exe: $(GAME_FILES) $(ENGINE_FILES)
	x86_64-w64-mingw32-cc $(CFLAGS) -o ../bin/windows/$(GAME).exe $(ENGINE_FILES) $(GAME_FILES) $(LFLAGS_WINDOWS)

../bin/android/$(GAME): 
	@echo "Android not implemented yet"

clear: 
	rm ../bin/linux/$(GAME) ../bin/windows/$(GAME).exe
