#ifndef DEMO
    //*******   SCROLLBAR   ********//
    void Scrollbar::set(int xp,int yp,int hp){
        x=xp;y=yp;h=hp;
        p=0;
    }
    int Scrollbar::update(bool u){
        SDL_Rect r={x,y,20,h};
        if(u){
            SDL_Rect v;
            SDL_RenderGetViewport(ren,&v);
            if(e.motion.state && in_rect(r))motion=1;
            if(!e.motion.state)motion=0;
            if(motion)p=e.button.y-v.y-25;
            if(p<30)p=30;
            if(p>h-20)p=h-20;
        }
        SDL_Rect v={x+3,p+5,14,40};
        SDL_RenderDrawRect(ren,&v);
        return (p-30.0f)/(h-50)*100;
    }
    int Scrollbar::getp(){return (p-30.0f)/(h-50)*100;}
    //*******   MOUSE SELECT   *******//
    void Select::select(){
        mouse.update();
        if(mouse.state&&mouse.b==SDL_BUTTON_LEFT){
            if(!drag){
                x1=test(((mouse.x-cx)/zoom+32)/64.0f,w+1);
                y1=test(((mouse.y-cy)/zoom+32)/64.0f,h+1);
                drag=1;
            }
            x2=test(((mouse.x-cx)/zoom+32)/64.0f,w+1);
            y2=test(((mouse.y-cy)/zoom+32)/64.0f,h+1);
        }else{
            drag=0;
            if(x1==x2||y1==y2||x2>w||y2>h){
                x1=0;
                y1=0;
                x2=w;
                y2=h;
            }
            if(x1>x2){
                x2^=(x1^=x2);
                x1^=x2;
            }
            if(y1>y2){
                y2^=(y1^=y2);
                y1^=y2;
            }
        }
    }
    //*******   GET MINIMAP CELL SIZE   *******//
    int get_level_c(int w,int h){
        int c=20;
        if(h>10||w>10)c=15;
        if(h>13||w>13)c=10;
        if(h>20||w>20)c=8;
        if(h>25||w>25)c=6;
        if(h>33||w>33)c=5;
        if(h>40||w>40)c=4;
        if(h>50||w>50)c=3;
        if(h>66||w>66)c=2;
        if(h>100||w>100)c=1;
        return c;
    }
    //*******   LEVEL EDITOR HISTORY   *******//
    void History::save(){
        wl=w;
        hl=h;
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                l[x][y]=level[x][y];
            }
        }
        wir.resize(0);
        gat.resize(0);
        for(unsigned q=0;q<wire.size();q++)wir.push_back(wire[q]);
        for(unsigned q=0;q<gate.size();q++)gat.push_back(gate[q]);
    }
    void History::load(){
        w=wl;
        h=hl;
        for(int x=0;x<wl;x++){
            for(int y=0;y<hl;y++){
                level[x][y]=l[x][y];
            }
        }
        wire.resize(0);
        gate.resize(0);
        for(unsigned q=0;q<wir.size();q++)wire.push_back(wir[q]);
        for(unsigned q=0;q<gat.size();q++)gate.push_back(gat[q]);
    }
#endif //DEMO
bool in_range(int x1,int x2,int x3){
    if(x3>x2&&x2>x1)return 1;
    if(x1>x2&&x2>x3)return 1;
    return 0;
}
bool p(int x1, int y1, int x2, int y2){
    if(abs(x1-x2)<40&&abs(y1-y2)<40)return 1;
    return 0;
}
bool l(int x1, int y1, int x2, int y2){
    if(abs(x1-x2)<64&&abs(y1-y2)<64)return 1;
    return 0;
}
bool Mouse::update(){
    SDL_Rect g;
    int zx,zy,mx,my;
    SDL_GetWindowSize(win,&zx,&zy);
    SDL_GetMouseState(&mx,&my);
    SDL_RenderGetViewport(ren,&g);
    mx=(float)mx/zx*SW-g.x;
    my=(float)my/zy*SH-g.y;
    if(e.type==SDL_MOUSEBUTTONDOWN){
        state=1;
        b=e.button.button;
        x=mx;
        y=my;
    }
    if(e.type==SDL_MOUSEMOTION&&state==1){
        x=mx;
        y=my;
    }
    if(e.type==SDL_MOUSEBUTTONUP)state=0;
    return 0;
}
void Mouse::wait(){
    while(state){
        SDL_WaitEvent(&e);
        update();
    }
}
int test(int n,int m){
    if(n>=0&&n<m)return n;
    return 0;
}
SDL_Surface *create_surface(int width,int height){
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    surface = SDL_CreateRGBSurface(0, width, height, 32,
                                       rmask, gmask, bmask, amask);
    return surface;
}
void Profile::clear(uint8_t n){
    level=0;
    allmoney=0;
    name=string("Player-0"+to_string(n+1));
    for(int j=0;j<77;j++){
        money[j]=0;
        time[j]=0;
        die[j]=0;
    }
    for(int j=0;j<32;j++){
        ach[j]=0;
        effects[j]=0;
    }
}
void Profile::new_name(){
    level=0;
    allmoney=0;
    for(int j=0;j<77;j++){
        money[j]=0;
        die[j]=0;
    }
    for(int j=0;j<32;j++){
        effects[j]=0;
    }

}
void Profile::random(){
    #ifdef ANDROID
        stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/items.txt").c_str());
    #else
        ifstream file(string(GLOBAL_DATA_DIR+"locales/"+loc+"/items.txt").c_str());
    #endif
    int price_all[TVR_COUNT];
    int delta_all[TVR_COUNT];
    int lucky_all[TVR_COUNT];
    bool is[TVR_COUNT];
    for(int q=0;q<TVR_COUNT;q++){
        is[q]=effects[q];
        string str;
        getline(file,str);
        getline(file,str);
        file>>price_all[q]>>delta_all[q]>>lucky_all[q];
        in(file);
    }
    for(int q=0;q<3;q++){
        int c=0;
        for(bool ok=0;ok==0;){
            c=rand()%TVR_COUNT;
            if(lucky_all[c]&&!is[c]&&(rand()%lucky_all[c])==0)ok=1;
        }
        tovars[q].price=price_all[c]+rand()%(delta_all[c]*2)-delta_all[c];
        tovars[q].effect=c;
        is[c]=1;
    }
}
bool Back(){
    if(key[SDL_SCANCODE_AC_BACK]||key[SDL_SCANCODE_ESCAPE]){
        while(key[SDL_SCANCODE_AC_BACK]||key[SDL_SCANCODE_ESCAPE])SDL_PollEvent(&e);
        return 1;
    }
    return 0;
}
