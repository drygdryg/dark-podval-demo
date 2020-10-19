#define STRINGS_COUNT 76

//#define linux_install

#if  defined(linux_install)
    #define GLOBAL_DATA_DIR string("/usr/share/podval-demo/")
    #define SAVE_DATA_DIR string((string)getenv("HOME")+"/.local/share/podval-demo/")
#elif defined(linux)
    #define GLOBAL_DATA_DIR string("data/")
    #define SAVE_DATA_DIR string("data/")
#elif defined(WIN32)
    #define GLOBAL_DATA_DIR string("data/")
    #define SAVE_DATA_DIR string("data/")
#elif defined(ANDROID)
    #define GLOBAL_DATA_DIR string("")
    #define SAVE_DATA_DIR string(SDL_AndroidGetInternalStoragePath())
#endif

#ifdef WIN32
    #define SDL_MAIN_HANDLED
#endif
#define TVR_COUNT 6

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_FontCache.h"

#ifdef ANDROID
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <android/log.h>
    #define TAG "PODVAL"
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)
#else
    #define LOGE(...) printf(__VA_ARGS__)
    #define LOGW(...) printf(__VA_ARGS__)
    #define LOGI(...) printf(__VA_ARGS__)
    #define LOGD(...) printf(__VA_ARGS__)
#endif
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#ifdef WIN32
    #include <windows.h>
#endif
#include <sstream>
#include <ctime>
#include <math.h>
#include <iostream>
using namespace std;
#include "strings.cpp"
#include "clases.h"
#include "func.h"
SDL_Texture *flr, *textures, *anim_tex, *gates_tex;
#ifdef ANDROID
    SDL_Texture *sensor_tex;
#endif
Mix_Chunk *spike_open, *spike_close, *pulemet, *crossbow_fire, *crossbow_load, *button, *buttoff, *dopen, *dclose, *dkey, *boom, *moneta;
FC_Font *text, *menu, *bold;
SDL_Event e;
#define timer ((int)SDL_GetTicks()-timer_pause)
#include "sdl.cpp"
uint8_t w,h;
string lid;
uint8_t level[255][255];
uint8_t light[255][255];
const uint8_t* key=SDL_GetKeyboardState(0);
#ifdef ANDROID
    float zoom=2;
    uint8_t sensor=0;
#else
    float zoom=1;
#endif
uint8_t ch_bl=1,ch_gt=0;
bool darkost=1;
unsigned int subt_hash=0;
uint8_t mode=0;
int cx=0,cy=0;
uint8_t profn=0;
bool st=1;
bool drag=0;
int ltime;
int timer_sbt=0,    //subtitles
    timer_trap=0,   //hidden holes
    timer_bl=0,     //bullets
    timer_pm=0,     //guns
    timer_ar=0,     //crossbows
    timer_pause=0;  //timer for pause

int delay=1000; //delay for subtitles
bool sb_on=1;
uint8_t sound_vol=128,music_vol=128; //sound and music volume
string txt=" ",buf=" ",qbuf=" ";     //current subtitles, subtitles buffer and questions buffer
string loc="en";                     //current locale (russian or english)
string strings[STRINGS_COUNT];   //strings for current locale
std::vector< std::string* > questions(0);
#include "draw.cpp"
#include "clases/sbj.cpp"
#include "clases/small_clases.cpp"
#include "logic.cpp"
#include "game.cpp"
#include "files.cpp"
#include "windows.cpp"
#include "gameevents.cpp"
