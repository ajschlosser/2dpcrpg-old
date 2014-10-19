# Sources
SDIR = source
SRCS = game.cpp engine.cpp interface.cpp map.cpp objects.cpp thing.cpp tile.cpp unit.cpp
# Objects
ODIR = build/
OBJS = $(SRCS:.cpp=.o)
# Executable
TARGET = 2dpcrpg
# Compiler
CC = g++
# Debug
DEBUG = -g
# Flags
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
# Libraries
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

all: $(TARGET) $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(TARGET)

