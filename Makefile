CC = g++
CFLAGS = -std=c++11 -O3 -s -fomit-frame-pointer -fexpensive-optimizations -O3 -DDEMO
SOURCE = src/main.cpp src/FC/SDL_FontCache.c
PREFIX ?= /usr
INCLUDE = -I$(PREFIX)/include/SDL2/ -I$(PWD)/src/FC/
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEFINE = -Dlinux_install 

DESTDIR = /usr
BINDIR = $(DESTDIR)/bin
DATA_DIR = $(DESTDIR)/share/dark-podval-demo
OUTPUT=$(PWD)/bin/podval

.PHONY: all clean
all:
	$(CC) -o $(OUTPUT) $(CFLAGS) $(DEFINE) $(INCLUDE) $(SOURCE) $(LDFLAGS)
clean:
	@if [ -f $(OUTPUT) ]; then rm $(OUTPUT); fi;
install:
	install -m 755 -D $(OUTPUT) $(BINDIR)/dark-podval-demo
	rm -rf $(DATA_DIR)
	mkdir -p $(DESTDIR)/share/applications
	cp -r $(PWD)/bin/data $(DATA_DIR)
	cp $(PWD)/bin/dark-podval-demo.desktop $(DESTDIR)/share/applications/dark-podval-demo.desktop
uninstall:
	rm $(BINDIR)/dark-podval-demo
	rm -rf $(DATA_DIR)
	rm $(DESTDIR)/share/applications/dark-podval-demo.desktop
