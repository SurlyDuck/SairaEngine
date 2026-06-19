.PHONY: all linux windows android clear

CFLAGS:= -I -std=c11 -Wall -Wextra -pedantic
LFLAGS:= -L./engine/lib/ -lraylib -lm -lX11
GAME:= game
GAME_FILES:= game.c

all: linux windows android

linux: ../bin/linux/$(GAME)
windows: ../bin/windows/$(GAME)
android: ../bin/android/$(GAME)

../bin/linux/$(GAME): $(GAME_FILES)
	gcc $(CFLAGS) -o ../bin/linux/$(GAME) $(GAME_FILES) $(LFLAGS)

../bin/windows/$(GAME): 
	@echo "Windows not implemented yet"

../bin/android/$(GAME): 
	@echo "Android not implemented yet"

clear: 
	rm ../bin/linux/game
