.PHONY: all clear build run

# Targets: LINUX, WINDOWS, ANDROID
TARGET ?= LINUX

PROJECT_NAME          ?= saira_template
VERSION               ?= 0.0.1
PROJECT_PATH          ?= ./game/
LINUX_BIN_PATH        ?= ../bin/linux/
WINDOWS_BIN_PATH      ?= ../bin/windows/

# Build mode: DEBUG OR RELEASE
BUILD_MODE ?= DEBUG

# Default compiller: gcc
CC ?= gcc

ifeq ($(TARGET),WINDOWS)
	CC = x86_64-w64-mingw32-cc
	LFLAGS:= -L./lib_win/ -lraylib -lm -lgdi32 -lwinmm
else 
	LFLAGS:= -L./lib_linux/ -lraylib -lm -lX11
endif

CFLAGS:= -I -std=c11 -Wall -Wextra -pedantic

ifeq ($(BUILD_MODE),DEBUG)
	CFLAGS:= -ggdb ${CFLAGS}
endif

# Objects files required
# NOTE: If you create a new list remember to add to the ´build´ recipe
ENGINE = \
	saira.o \
	animation.o

GAME = \
	./game/game.o \
	./game/characters.o

all: $(ENGINE) $(GAME) build

%: %.c
	$(CC) -c $<

build: $(ENGINE) $(GAME)
ifeq ($(TARGET),LINUX)
	$(CC) $(CFLAGS) -o $(LINUX_BIN_PATH)$(PROJECT_NAME)$(VERSION) $? $(LFLAGS)
	make run
endif 
ifeq ($(TARGET),WINDOWS)
	$(CC) $(CFLAGS) -o $(WINDOWS_BIN_PATH)$(PROJECT_NAME)$(VERSION).exe $? $(LFLAGS)
	make run TARGET=WINDOWS
endif
ifeq ($(TARGET),ANDROID)
	echo "Android target not implemented yet."
endif

run:
ifeq ($(TARGET),LINUX)
	$(LINUX_BIN_PATH)$(PROJECT_NAME)$(VERSION)
endif 
ifeq ($(TARGET),WINDOWS)
	wine $(WINDOWS_BIN_PATH)$(PROJECT_NAME)$(VERSION).exe
endif

clear:
	rm *.o
	rm $(PROJECT_PATH)*.o

