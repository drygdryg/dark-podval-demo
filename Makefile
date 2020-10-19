CC = g++
CFLAGS = -std=c++11 -O3 -s -fomit-frame-pointer -fexpensive-optimizations -O3 -DDEMO
SOURCE = src/main.cpp src/FC/SDL_FontCache.c
INCLUDE = -I/usr/include/SDL2/ -Isrc/FC/
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
OUTPUT = 'bin/podval'
DATA_DIR = 'bin/data'
OUTPUT_DATA_DIR = '/usr/share/dark-podval-demo'
OUTPUT_INSTALL_BIN = '/usr/bin/dark-podval-demo'
SHOTCUT = 'bin/dark-podval-demo.desktop'
SHOTCUT_DIR = '/usr/share/applications/dark-podval-demo.desktop'
DEFINE = -Dlinux_install 
.PHONY: all clean
all:
	$(CC) -o $(OUTPUT) $(CFLAGS) $(DEFINE) $(INCLUDE) $(SOURCE) $(LDFLAGS)
clean:
	@if [ -f $(OUTPUT) ]; then rm $(OUTPUT); fi;
install:
	install -m 755 -D $(OUTPUT) $(OUTPUT_INSTALL_BIN)
	rm -rf $(OUTPUT_DATA_DIR)
	cp -r $(DATA_DIR) $(OUTPUT_DATA_DIR)
	cp $(SHOTCUT) $(SHOTCUT_DIR)
