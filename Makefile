# Compiler and flags
CC := gcc   
TARGET := game.exe             
CFLAGS := -Wall -Wextra -O2 -std=c17 -I./raylib_2/include
LDFLAGS := -L./raylib_2/lib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32


all: compile_all #This is the command that is sent in full
compile_all: main.o layout.o player_movement.o draw.o #Put here the dependencies(all the files) 
	$(CC) main.o layout.o player_movement.o draw.o $(LDFLAGS) -o $(TARGET)

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c -o main.o

layout.o: layout.c 
	$(CC) $(CFLAGS) -c layout.c -o layout.o

player_movement.o: player_movement.c 
	$(CC) $(CFLAGS) -c player_movement.c -o player_movement.o

draw.o: draw.c 
	$(CC) $(CFLAGS) -c draw.c -o draw.o
clean:
#	rm -f *.o game.exe for linux and mac 
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
