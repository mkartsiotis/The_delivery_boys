# Compiler and flags
CC := gcc   
TARGET := game.exe               
#LINUX AND MAC TARGET := game
CFLAGS := -Wall -Wextra -O2 -std=c17 -I./raylib_2/include
ifeq ($(OS),Windows_NT)
    # WINDOWS FLAGS
    LDFLAGS = -L./raylib_2/lib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32
    TARGET = game.exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # LINUX FLAGS
        LDFLAGS = -L./raylib_2/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        TARGET = game
    endif
    ifeq ($(UNAME_S),Darwin)
        # MAC FLAGS
        LDFLAGS = -L./raylib_2/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
        TARGET = game
    endif
endif


all: compile_all #This is the command that is sent in full
compile_all: main.o layout.o player_movement.o draw.o gamehandling.o astar_search.o npc.o cam.o audio.o #Put here the dependencies(all the files) 
	$(CC) main.o layout.o player_movement.o draw.o gamehandling.o astar_search.o npc.o cam.o audio.o $(LDFLAGS) -o $(TARGET)

main.o: main.c headers.h
	$(CC) $(CFLAGS) -c main.c -o main.o

layout.o: layout.c headers.h 
	$(CC) $(CFLAGS) -c layout.c -o layout.o

player_movement.o: player_movement.c headers.h 
	$(CC) $(CFLAGS) -c player_movement.c -o player_movement.o

draw.o: draw.c headers.h
	$(CC) $(CFLAGS) -c draw.c -o draw.o

gamehandling.o: gamehandling.c headers.h
	$(CC) $(CFLAGS) -c gamehandling.c -o gamehandling.o

astar_search.o: astar_search.c headers.h
	$(CC) $(CFLAGS) -c astar_search.c -o astar_search.o

npc.o: npc.c headers.h
	$(CC) $(CFLAGS) -c npc.c -o npc.o
cam.o: cam.c headers.h
	$(CC) $(CFLAGS) -c cam.c -o cam.o

audio.o: audio.c headers.h
	$(CC) $(CFLAGS) -c audio.c -o audio.o

clean:
#	rm -f *.o game for linux and mac 
	del *.o game.exe 2>nul || true 
#for Windows


# OR IF YOU NEED AUTO DETECTION YOU CAN USE A STANDARD MAKEFILE LIKE THE FOLOWING(WILL WORK)
## Compiler and flags
#CC := x86_64-w64-mingw32-gcc        # Safer on MinGW-w64 (avoids old linker issues)
#CFLAGS := -Wall -Wextra -O2 -std=c17 -I./raylib_2/include
#LDFLAGS := -L./raylib_2/lib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32

# Target name
#TARGET := game.exe

# Automatically find all .c files (future-proof!)
#SOURCES := $(wildcard *.c)
#OBJECTS := $(SOURCES:.c=.o)

# Default target
#all: $(TARGET)

# Link everything into the executable
#$(TARGET): $(OBJECTS)
#	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Compile rule for any .c → .o
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

# Clean
#clean:
#	del *.o $(TARGET) 2>nul || true

# Declare phony targets
#.PHONY: all clean
