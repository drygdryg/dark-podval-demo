CC = g++
CFLAGS = -std=c++11 -O3 -s -fomit-frame-pointer -fexpensive-optimizations -O3 -DDEMO
SOURCE = src/main.cpp src/FC/SDL_FontCache.c
PREFIX ?= /usr
INCLUDE = -I$(PREFIX)/include/SDL2/ -Isrc/FC/
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEFINE = -Dlinux_install 

DESTDIR = /
BINDIR = $(DESTDIR)/usr/bin
DATA_DIR = $(DESTDIR)/usr/share/dark-podval-demo
OUTPUT=bin/podval

.PHONY: all clean
all:
	$(CC) -o $(OUTPUT) $(CFLAGS) $(DEFINE) $(INCLUDE) $(SOURCE) $(LDFLAGS)
clean:
	@if [ -f $(OUTPUT) ]; then rm $(OUTPUT); fi;
install:
	install -m 755 -D $(OUTPUT) $(BINDIR)/dark-podval-demo
	rm -rf $(DATA_DIR)
	mkdir -p $(DESTDIR)/usr/share/applications
	cp -r bin/data $(DATA_DIR)
	cp bin/dark-podval-demo.desktop $(DESTDIR)/usr/share/applications/dark-podval-demo.desktop
uninstall:
	rm $(BINDIR)/dark-podval-demo
	rm -rf $(DATA_DIR)
	rm $(DESTDIR)/usr/share/applications/dark-podval-demo.desktop
