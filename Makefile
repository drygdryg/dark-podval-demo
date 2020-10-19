CC = g++
CFLAGS = -std=c++11 -O3 -s -fomit-frame-pointer -fexpensive-optimizations -O3 -DDEMO
SOURCE = src/main.cpp src/FC/SDL_FontCache.c
INCLUDE = -I/usr/include/SDL2/ -Isrc/FC/
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
OUTPUT = bin/podval
DEFINE = -Dlinux_install 
.PHONY: all clean
all:
	$(CC) -o $(OUTPUT) $(CFLAGS) $(DEFINE) $(INCLUDE) $(SOURCE) $(LDFLAGS)
clean:
	@if [ -f $(OUTPUT) ]; then rm $(OUTPUT); fi;
