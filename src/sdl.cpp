int SW=0;
int SH=0;
#ifdef ANDROID
    bool fullscreen=1;
#else
    bool fullscreen=0;
#endif
extern SDL_Event e;
SDL_Window *win = 0;
SDL_Renderer *ren = 0;
SDL_Color white={255,255,255,255};
SDL_Color black={0,0,0,255};
SDL_Color gray={128,128,128,255};
SDL_Color yellow={255,255,0,255};
SDL_Color green={50,255,0,255};
SDL_Color red={255,128,0,255};
SDL_Color blue={0,0,255,255};
SDL_Texture * load(const char * way) {
    SDL_Surface * img = 0;
    img=IMG_Load(way);
    SDL_Texture * tex = SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);
    return tex;
}
bool init(const char * name) {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    #ifndef ANDROID
    if(SW==0)
    #endif
    {
        SDL_DisplayMode c;
        SDL_GetCurrentDisplayMode(0, &c);
        SW=c.w;
        #ifdef ANDROID
            SH=c.h;
        #else
            SH=c.h-!fullscreen*25;
        #endif
    }
    #ifdef ANDROID
        win = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SW-50,SH, fullscreen?1:4);
    #else
        win = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SW,SH, fullscreen?1:4);
    #endif
    ren = SDL_CreateRenderer(win, 0, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3|MIX_INIT_MOD);
    Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
    textures = load(string(GLOBAL_DATA_DIR+"textures/a.png").c_str());
    anim_tex = load(string(GLOBAL_DATA_DIR+"textures/b.png").c_str());
    flr   =    load(string(GLOBAL_DATA_DIR+"textures/f.png").c_str());
    gg.texture=load(string(GLOBAL_DATA_DIR+"textures/gg.png").c_str());
    #ifdef ANDROID
        sensor_tex=load(GLOBAL_DATA_DIR+"textures/s.png");
    #endif
    text=FC_CreateFont();
    bold=FC_CreateFont();
    menu=FC_CreateFont();

    FC_LoadFont(text,ren,string(GLOBAL_DATA_DIR+"fonts/text.ttf").c_str(),20,white,TTF_STYLE_NORMAL);
    FC_LoadFont(bold,ren,string(GLOBAL_DATA_DIR+"fonts/bold.ttf").c_str(),20,white,TTF_STYLE_NORMAL);
    FC_LoadFont(menu,ren,string(GLOBAL_DATA_DIR+"fonts/menu.ttf").c_str(),30,white,TTF_STYLE_NORMAL);

    spike_open= Mix_LoadWAV         (string(GLOBAL_DATA_DIR+"sound/spikes/open.wav").c_str());
    spike_close=Mix_LoadWAV         (string(GLOBAL_DATA_DIR+"sound/spikes/close.wav").c_str());
    pulemet=Mix_LoadWAV             (string(GLOBAL_DATA_DIR+"sound/pulemet.wav").c_str());
    crossbow_fire=Mix_LoadWAV       (string(GLOBAL_DATA_DIR+"sound/crossbow/fire.wav").c_str());
    crossbow_load=Mix_LoadWAV       (string(GLOBAL_DATA_DIR+"sound/crossbow/load.wav").c_str());
    button=Mix_LoadWAV              (string(GLOBAL_DATA_DIR+"sound/button/on.wav").c_str());
    buttoff=Mix_LoadWAV             (string(GLOBAL_DATA_DIR+"sound/button/off.wav").c_str());
    dopen=Mix_LoadWAV               (string(GLOBAL_DATA_DIR+"sound/door/open.wav").c_str());
    dclose=Mix_LoadWAV              (string(GLOBAL_DATA_DIR+"sound/door/close.wav").c_str());
    dkey=Mix_LoadWAV                (string(GLOBAL_DATA_DIR+"sound/door/key.wav").c_str());
    boom=Mix_LoadWAV                (string(GLOBAL_DATA_DIR+"sound/boom.wav").c_str());
    moneta=Mix_LoadWAV              (string(GLOBAL_DATA_DIR+"sound/moneta.wav").c_str());
    return 0;
}
void quit(bool ex){
    SDL_DestroyTexture(flr);
    SDL_DestroyTexture(textures);
    SDL_DestroyTexture(anim_tex);
    SDL_DestroyTexture(gg.texture);
    #ifdef ANDROID
        SDL_DestroyTexture(sensor_tex);
    #endif
    Mix_FreeChunk(spike_open);
    Mix_FreeChunk(spike_close);
    Mix_FreeChunk(pulemet);
    Mix_FreeChunk(crossbow_fire);
    Mix_FreeChunk(crossbow_load);
    Mix_FreeChunk(button);
    Mix_FreeChunk(buttoff);
    Mix_FreeChunk(dopen);
    Mix_FreeChunk(dclose);
    Mix_FreeChunk(dkey);
    Mix_FreeChunk(boom);
    Mix_FreeChunk(moneta);
    FC_FreeFont(text);
    FC_FreeFont(menu);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_StopTextInput();
    if(ex){
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        exit(1);
    }
}
SDL_Color color(uint8_t r,uint8_t g,uint8_t b){
    SDL_Color c={r,g,b,255};
    return c;
}
void showIMG(int x,int y,int w,int h,SDL_Texture * img,bool align=0) {
    SDL_Rect r = {x,y,w,h};
    if(align)r={x-w/2,y-h/2,w,h};
    SDL_RenderCopy(ren, img, 0, &r);
}
void showIMG(SDL_Rect r,SDL_Texture* img,bool align=0){
    showIMG(r.x,r.y,r.w,r.h,img,align);
}
void showIMG(int x,int y,int w,int h,SDL_Texture * img,SDL_Rect tex,bool align=0) {
    SDL_Rect r;
    if(align)r={x-w/2,y-h/2,w,h};
    else r = {x,y,w,h};
    SDL_RenderCopy(ren, img, &tex, &r);
}
void showIMG(SDL_Rect r,SDL_Texture* img,SDL_Rect tex,bool align=0){
    showIMG(r.x,r.y,r.w,r.h,img,tex,align);
}
bool in_rect(SDL_Rect r){
    SDL_Rect v;
    SDL_RenderGetViewport(ren,&v);
    SDL_Rect h={r.x+v.x,r.y+v.y,r.w,r.h};
    int mx,my;
    int zx,zy;
    SDL_GetMouseState(&mx,&my);
    SDL_GetWindowSize(win,&zx,&zy);
    mx=(float)mx/zx*SW;
    my=(float)my/zy*SH;
    if(mx>h.x && my>h.y && mx<h.x+h.w && my<h.y+h.h)return 1;
    else return 0;
}
