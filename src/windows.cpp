#ifdef WIN32
    std::vector<string>get_levels(){
        std::vector<string> names;
        WIN32_FIND_DATA fd;
        HANDLE hFind = ::FindFirstFile(GLOBAL_DATA_DIR+"levels/custom/*", &fd);
        if(hFind != INVALID_HANDLE_VALUE){
            do{
                if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))names.push_back(fd.cFileName);
            }while(::FindNextFile(hFind, &fd));
            ::FindClose(hFind);
        }
        return names;
    }
#else
    std::vector<string>get_levels(){
        std::vector<string> names;
        char var[128];
        FILE *fp = popen("ls data/levels/custom","r");
        while (fgets(var, sizeof(var), fp) != NULL){
            names.push_back(string(var));
            names[names.size()-1].pop_back();
        }
        pclose(fp);
        return names;
    }
#endif
void CLEAR(SDL_Rect window){
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    #ifdef ANDROID
        SDL_RenderSetViewport(ren,0);
        SDL_RenderClear(ren);
    #endif
    SDL_RenderSetViewport(ren,&window);
    SDL_RenderFillRect(ren,0);
    SDL_SetRenderDrawColor(ren,255,255,255,255);
    SDL_RenderDrawRect(ren,0);

}
bool gin(uint8_t* c){
    bool ok=0;
    if(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN&&*c > 0){
    #ifndef ANDROID
        while(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN)SDL_PollEvent(&e);
    #endif
        *c/=10;
        ok=1;
    }
    if(e.text.type==SDL_TEXTINPUT){
        if(e.text.text[0]>='0'&&e.text.text[0]<='9'){
            *c*=10;
            *c+=(int)(e.text.text[0]-'0');
            ok=1;
        #ifndef ANDROID
            while(e.text.type==SDL_TEXTINPUT)SDL_PollEvent(&e);
        #endif
        }
    }
    return ok;
}
bool gin(int* c){
    bool ok=0;
    if(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN&&*c > 0){
    #ifndef ANDROID
        while(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN)SDL_PollEvent(&e);
    #endif
        *c/=10;
        ok=1;
    }
    if(e.text.type==SDL_TEXTINPUT){
        if(e.text.text[0]>='0'&&e.text.text[0]<='9'){
            *c*=10;
            *c+=(int)(e.text.text[0]-'0');
            ok=1;
        #ifndef ANDROID
            while(e.text.type==SDL_TEXTINPUT)SDL_PollEvent(&e);
        #endif
        }
    }
    return ok;
}
bool gin(string* c){
    bool ok=0;
    if(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN&&c->size() > 0){
    #ifndef ANDROID
        while(e.key.keysym.sym==SDLK_BACKSPACE&&e.type==SDL_KEYDOWN)SDL_PollEvent(&e);
    #endif
        int textlen=c->size();

         do {
             if (textlen==0)
             {
                 break;
             }
             if (((*c)[textlen-1] & 0x80) == 0x00)
             {
                 (*c)[textlen-1]=0x00;
                 c->pop_back();
                 break;
             }
             if (((*c)[textlen-1] & 0xC0) == 0x80)
             {
                 (*c)[textlen-1]=0x00;
                 textlen--;c->pop_back();
             }
             if (((*c)[textlen-1] & 0xC0) == 0xC0)
             {
                 (*c)[textlen-1]=0x00;
                 c->pop_back();
                 break;
             }
         } while(1);
         ok=1;
    }
    if(e.text.type==SDL_TEXTINPUT){
        *c+=e.text.text;
        ok=1;
    #ifndef ANDROID
        while(e.text.type==SDL_TEXTINPUT)SDL_PollEvent(&e);
    #endif
    }
    return ok;
}
bool alert(){
    SDL_Rect window={SW/2-100,SH/2-75,200,150};
    SDL_RenderSetViewport(ren,&window);
    SDL_Rect button[2]={{20,80,60,50},{110,80,60,50}};
    while(1){
        CLEAR(window);
        FC_DrawAlign(text,ren,window.w/2,25,FC_ALIGN_CENTER,strings[14].c_str());
        for(int q=0;q<2;q++){
            SDL_RenderDrawRect(ren,&button[q]);
            FC_DrawButtonCenter(text,ren,button[q],strings[16-q].c_str());
        }
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
        if(e.type==SDL_QUIT)quit(1);
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            for(int q=0;q<2;q++){
                if(in_rect(button[q])){
                    mouse.wait();
                    SDL_RenderSetViewport(ren,0);
                    return !q;
                }
            }
        }
    }
}
bool standart(){
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Texture *arrow  =   load(string(GLOBAL_DATA_DIR+"textures/ar.png").c_str());
    st=1;
    SDL_Rect window={SW/2-275,SH/2-200,550,400};
    SDL_RenderSetViewport(ren,&window);
    int sh=0;
    string cnames[10],lnames[110];
    {
        #ifdef ANDROID
            stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/cnames.txt").c_str());
        #else
            fstream file(GLOBAL_DATA_DIR+"locales/"+loc+"/cnames.txt");
        #endif
        for(int q=0;q<10;q++)getline(file,cnames[q]);
        #ifndef ANDROID
            file.close();
        #endif
    }
    {
        #ifdef ANDROID
            stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/lnames.txt").c_str());
        #else
            fstream file(GLOBAL_DATA_DIR+"locales/"+loc+"/lnames.txt");
        #endif
        for(int q=0;q<110;q++)getline(file,lnames[q]);
        #ifndef ANDROID
            file.close();
        #endif
    }
    while(1){
        if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            SDL_DestroyTexture(arrow);
            quit(1);
        }
        CLEAR(window);
        SDL_RenderDrawLine(ren,0,30,550,30);
        SDL_RenderDrawLine(ren,370,30,370,400);
        SDL_RenderDrawLine(ren,0,60,370,60);
        FC_Draw(text,ren,15,5,strings[22].c_str());
        FC_DrawAlign(text,ren,185,35,FC_ALIGN_CENTER,cnames[sh/11].c_str());
        SDL_Rect cr={520,0,30,30};
        showIMG(cr,cross);
        SDL_Rect ar[2]={{0,30,30,30},{340,30,30,30}};
        showIMG(ar[0],arrow,FC_MakeRect(0,(sh>10)*30,30,30));
        showIMG(ar[1],arrow,FC_MakeRect(30,(sh<100)*30,30,30));
        for(int q=0;q<11;q++){
            SDL_Rect r={20,65+q*30,330,30};
            FC_DrawButtonLeft(text,ren,r,lnames[sh/11*11+q].c_str());
            if(q==sh%11)SDL_RenderDrawRect(ren,&r);
        }
        FC_DrawColumn(text,ren,380,60,160,
            string(strings[23]
                   +to_string(profile.money[sh])
                   +strings[24]
                   +"\n\n\n"
                   +strings[25]
                   +to_string(profile.time[sh]/600)
                   +strings[26]+to_string(profile.time[sh]/10%60)
                   +"."+to_string(profile.time[sh]%10)
                   +strings[27]).c_str());
        SDL_Rect b={385,350,150,40};
        SDL_RenderDrawRect(ren,&b);
        FC_DrawButtonCenter(text,ren,b,strings[28].c_str());
        bool j=0;
        if(mouse.state){
            if(in_rect(ar[0])&& sh>10){sh-=11;mouse.wait();j=1;}
            if(in_rect(ar[1])&&sh<100){sh+=11;mouse.wait();j=1;}
        }
        for(int q=0;q<11;q++){
            SDL_Rect r={20,65+q*30,330,30};
            if(in_rect(r)&&mouse.state)sh=sh/11*11+q;
        }
        if(in_rect(b)&&mouse.state){
            mouse.wait();
            j=1;
            lid=lntolid(sh);
            open(string(GLOBAL_DATA_DIR+"levels/standart/"+lid+".lvl"));
            SDL_RenderSetViewport(ren,0);
            game();
            return 1;
        }
        SDL_RenderPresent(ren);
        if(in_rect(cr)&&e.button.state==1)break;
        if(!j)SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_DestroyTexture(arrow);
    SDL_DestroyTexture(cross);
    SDL_RenderSetViewport(ren,0);
    return 0;
}
bool settings(){
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Rect window={SW/2-275,SH/2-200,550,400};
    SDL_DisplayMode c;
    SDL_GetCurrentDisplayMode(0, &c);
    int sh=0;
    int w=SW,h=SH,vs=sound_vol,vm=music_vol;
    bool wf=fullscreen,sb=sb_on;
    string l=loc;
    bool run=1;
    while(run){
        SDL_Rect button[]={{335,45, 80,30},
                           {445,45, 80,30},
                           {495,85,30,30},
                           {495,125,30,30},
                           {445,165,80,30},
                           {264,205,261,30},
                           {264,245,261,30},
                           {495,285,30,30},
                           {25,330,225,50},
                           {300,330,225,50}};
        CLEAR(window);
        SDL_Rect cr={520,0,30,30};
        SDL_RenderDrawLine(ren,0,30,550,30);
        showIMG(cr,cross);
        FC_Draw(text,ren,15,5,strings[5].c_str());
        for(int q=0;q<10;q++){
            SDL_RenderDrawRect(ren,&button[q]);
            switch(q){
                case 0:FC_DrawButtonLeft(text,ren,button[0],string(" "+to_string(w)+(sh==0?"_":"")).c_str());break;
                case 1:FC_DrawButtonLeft(text,ren,button[1],string(" "+to_string(h)+(sh==1?"_":"")).c_str());break;
                case 2:if(wf)FC_DrawButtonCenter(text,ren,button[2],"██");            break;
                case 3:if(sb)FC_DrawButtonCenter(text,ren,button[3],"██");            break;
                case 4:FC_DrawButtonCenter(text,ren,button[4],l.c_str());             break;
                case 5:{
                    SDL_Rect r={button[q].x+3,button[q].y+3,vs*2,button[q].h-6};
                    SDL_RenderFillRect(ren,&r);
                }break;
                case 6:{
                    SDL_Rect r={button[q].x+3,button[q].y+3,vm*2,button[q].h-6};
                    SDL_RenderFillRect(ren,&r);
                }break;
                case 7:if(darkost)FC_DrawButtonCenter(text,ren,button[7],"██");            break;
                case 8:case 9:FC_DrawButtonCenter(text,ren,button[q],strings[32+q].c_str());break;
            }
        }
        if(w>c.w)w=c.w;
        if(h>c.h)h=c.h;
        FC_Draw(text,ren,25, 50,strings[43].c_str());
        FC_Draw(text,ren,25, 90,strings[44].c_str());
        FC_Draw(text,ren,25,130,strings[46].c_str());
        FC_Draw(text,ren,25,170,strings[45].c_str());
        FC_Draw(text,ren,25,210,strings[73].c_str());
        FC_Draw(text,ren,25,250,strings[74].c_str());
        FC_Draw(text,ren,25,290,strings[75].c_str());
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&e);
        mouse.update();
        if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            quit(1);
        }
        else if(e.type==SDL_KEYDOWN||e.type==SDL_TEXTINPUT){
            if(Back())run=0;
            else if(sh==0)gin(&w);
            else if(sh==1)gin(&h);
        }
        else if(e.type==SDL_MOUSEBUTTONDOWN||e.type==SDL_MOUSEMOTION){
            if(e.type==SDL_MOUSEBUTTONDOWN){
                if(in_rect(cr))run=0;
                for(int q=0;q<10;q++){
                    if(in_rect(button[q])&&sh!=q){
                        if(q!=5&&q!=6)mouse.wait();
                        sh=q;
                    }
                }
                if(sh==4){
                         if(l=="ru")l="en";
                    else if(l=="en")l="ru";
                    sh=100;
                }
                else if(sh==9){
                    if(w<640)w=640;
                    if(h<480)h=480;
                    SH=h;
                    SW=w;
                    fullscreen=wf;
                    loc=l;
                    load_strings();
                    sb_on=sb;
                    sound_vol=vs;
                    music_vol=vm;
                    if(darkost)SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
                    else SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0");
                    save_settings();
                    #ifndef ANDROID
                        quit(0);
                        init("Dark Podval");
                    #endif
                    SDL_RenderSetViewport(ren,0);
                    run=0;
                }
                else if(sh==2){
                    #ifndef ANDROID
                        wf=!wf;sh=100;
                    #endif
                }
                else if(sh==3){sb=!sb;sh=100;}
                else if(sh==7){darkost=!darkost;sh=100;}
            }
            if(sh==5){
                int t=mouse.x-button[sh].x;
                if(t<0)t=0;
                if(t>256)t=256;
                vs=t/2;
            }
            else if(sh==6){
                int t=mouse.x-button[sh].x;
                if(t<0)t=0;
                if(t>256)t=256;
                vm=t/2;
            }
        }
    }
    SDL_DestroyTexture(cross);
    SDL_RenderSetViewport(ren,0);
    return 0;
}
bool pause(){
    Mix_PauseMusic();
    SDL_Rect window={SW/2-125,SH/2-150,250,300};
    SDL_RenderSetViewport(ren,&window);
    int time=SDL_GetTicks();
    bool ok=1;
    while(ok){
        if(e.type==SDL_QUIT)quit(1);
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            #ifdef ANDROID
            for(int q=0;q<4;q++){
                SDL_Rect r={0,q*60+30,250,42};
            #else
            for(int q=0;q<5;q++){
                SDL_Rect r={0,q*50+30,250,42};
            #endif
                if(in_rect(r)){
                    mouse.wait();
                    switch(q){
                        case 1:ok=0;Mix_ResumeMusic();break;
                        case 2:{
                            string path=(string)(st?GLOBAL_DATA_DIR+"levels/standart/":GLOBAL_DATA_DIR+"levels/custom/")+lid+(string)".lvl";
                            open(path);
                            SDL_RenderSetViewport(ren,0);
                            Mix_HaltMusic();
                            game_init(1);
                            return 0;
                        }break;
                        case 3:{
                            Mix_HaltMusic();
                            SDL_RenderSetViewport(ren,0);
                            return 1;
                        }break;
                        case 4:if(alert()){Mix_HaltMusic();quit(1);}break;
                    }
                    SDL_RenderSetViewport(ren,&window);
                }
            }
        }
        else if(Back()){
            ok=0;
            Mix_ResumeMusic();
        }
        CLEAR(window);
        #ifdef ANDROID
        for(int q=0;q<4;q++){
            SDL_Rect r={0,q*60+30,250,42};
        #else
        for(int q=0;q<5;q++){
            SDL_Rect r={0,q*50+30,250,42};
        #endif
            FC_DrawButtonCenter((q==0)?menu:text,ren,r,strings[47+q].c_str());
        }
        SDL_RenderPresent(ren);
        if(ok)SDL_WaitEvent(&e);
        mouse.update();
    }
    timer_pause+=(SDL_GetTicks()-time);
    return 0;
}
bool end(bool die,bool b3){
    Mix_HaltMusic();
    int tm=(timer-ltime)/100;
    SDL_Rect window={SW/2-200,SH/2-150,400,300};
    SDL_RenderSetViewport(ren,&window);
    while(1){
        SDL_Rect button[3]={{b3?25:66,175,100,100},{b3?150:232,175,100,100},{275,175,100,100}};
        if(e.type==SDL_QUIT)quit(1);
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            for(int q=0;q<2+b3;q++){
                if(in_rect(button[q])){
                    mouse.wait();
                    mouse.wait();
                    switch(q){
                        case 0:{
                            SDL_RenderSetViewport(ren,0);
                            return 1;
                        }break;
                        case 1:{
                            string path=(string)(st?GLOBAL_DATA_DIR+"levels/standart/":GLOBAL_DATA_DIR+"levels/custom/")+lid+(string)".lvl";
                            open(path);
                            game_init(0);
                            SDL_RenderSetViewport(ren,0);
                            return 0;
                        }break;
                        case 2:{
                            lid=postlid(lid);
                            string path=(string)(st?GLOBAL_DATA_DIR+"levels/standart/":GLOBAL_DATA_DIR+"levels/custom/")+lid+(string)".lvl";
                            open(path);
                            game_init(0);
                            SDL_RenderSetViewport(ren,0);
                            return 0;
                        }break;
                    }
                }
            }
        }else if(e.type==SDL_KEYDOWN){
            if(Back())return 0;
        }
        CLEAR(window);
        FC_DrawAlign(text,ren,200,25,FC_ALIGN_CENTER,(die?strings[52]:string(strings[53]+lid+strings[54])).c_str());
        FC_DrawColumn(text,ren,25,75,350,
            string(strings[55]
                   +to_string(tm/600)
                   +strings[26]+to_string(tm/10%60)
                   +"."+to_string(tm%10)
                   +strings[27]+"\n"+strings[23]
                   +to_string(gg.money)
                   +strings[24]).c_str());
        for(int q=0;q<2+b3;q++){
            SDL_RenderDrawRect(ren,&button[q]);
            FC_DrawButtonCenter(text,ren,button[q],strings[56+q].c_str());
        }
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_RenderSetViewport(ren,0);
    return 0;
}
bool inputname(string *name){
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Rect window={SW/2-200,SH
    #ifdef ANDROID
        /4
    #else
        /2-60
    #endif
    ,400,120};
    SDL_RenderSetViewport(ren,&window);
    string backup=*name;
    SDL_StartTextInput();
    bool ok=1;
    while(ok){
        SDL_Rect input[2]={{25,40,350,30},
                           {25,80,350,30}};
        SDL_Rect cr={370,0,30,30};
        if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            quit(1);
        }
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            if(in_rect(input[1])&&name->size()>0){
                SDL_StopTextInput();
                ok=0;
            }
            if(in_rect(cr)){
                *name=backup;
                SDL_StopTextInput();
                ok=0;
            }
        }
        //else if(e.type==SDL_TEXTINPUT||e.type==SDL_KEYDOWN){
        gin(name);
        if(Back()){
            *name=backup;
            SDL_StopTextInput();
            ok=0;
        }
        //}
        CLEAR(window);
        SDL_RenderDrawLine(ren,0,30,400,30);
        showIMG(cr,cross);
        FC_Draw(text,ren,15,5,strings[59].c_str());
        for(int q=0;q<2;q++){
            SDL_RenderDrawRect(ren,&input[q]);
            if(q==1){
                FC_DrawButtonCenter(text,ren,input[q],strings[13].c_str());
            }else{
                FC_DrawButtonLeft(text,ren,input[q],string(*name+"_").c_str());
            }
        }
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_DestroyTexture(cross);
    SDL_RenderSetViewport(ren,0);
    return 0;
}
bool profiles(){
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Rect window={SW/2-275,SH/2-200,550,400};
    SDL_RenderSetViewport(ren,&window);
    int sh=profn;
    Profile profs[5];
    bool ok=1;
    for(int q=0;q<5;q++)profs[q].load(q);
    while(ok){
        SDL_Rect button[3]={{25,360,150,30},{200,360,150,30},{375,360,150,30}};
        SDL_Rect cr={520,0,30,30};
        if(e.type==SDL_MOUSEBUTTONDOWN){
            if(in_rect(cr))ok=0;
            for(int q=0;q<3;q++){
                if(in_rect(button[q])){
                    mouse.wait();
                    switch(q){
                        case 0:{
                            if(alert()){
                                profs[sh].clear(sh);
                                profs[sh].save(sh);
                                if(sh==profn)profile.load(sh);
                            }
                        }break;
                        case 1:{
                            inputname(&profs[sh].name);
                            profs[sh].save(sh);
                            profile.load(sh);
                        }break;
                        case 2:{
                            profn=sh;
                            profile.load(sh);
                            save_settings();
                            SDL_RenderSetViewport(ren,0);
                            return 0;
                        }break;
                    }
                    SDL_RenderSetViewport(ren,&window);
                }
            }
            for(int q=0;q<5;q++){
                SDL_Rect r={15,65+q*55,520,55};
                if(in_rect(r))sh=q;
            }
        }
        else if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            quit(1);
        }
        else if(e.type==SDL_KEYDOWN){
            if(Back())ok=0;
        }
        CLEAR(window);
        SDL_RenderDrawLine(ren,0,30,550,30);
        SDL_RenderDrawLine(ren,0,350,550,350);
        SDL_RenderDrawLine(ren,0,60,550,60);
        showIMG(cr,cross);
        FC_Draw(text,ren,25,5,strings[60].c_str());
        FC_Draw(text,ren,25,35,strings[61].c_str());
        for(int q=0;q<5;q++){

            FC_Draw(text,ren, 25,80+q*55,          profs[q].name           .c_str());
            FC_Draw(text,ren,291,80+q*55,to_string(profs[q].allmoney)      .c_str());
            FC_Draw(text,ren,457,80+q*55,  lntolid(profs[q].level)         .c_str());
            SDL_Rect r={15,65+q*55,520,55};
            if(q==sh)SDL_RenderDrawRect(ren,&r);
        }
        for(int q=0;q<3;q++){
            SDL_RenderDrawRect(ren,&button[q]);
            FC_DrawButtonCenter(text,ren,button[q],strings[62+q].c_str());
        }
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_DestroyTexture(cross);
    SDL_RenderSetViewport(ren,0);
    return 0;
}
void buy_end(int effect){
    int time=SDL_GetTicks();
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Rect window={SW/2-275,SH/2-200,550,400};
    SDL_RenderSetViewport(ren,&window);
    string name;
    string descr;
    #ifdef ANDROID
        stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/items.txt").c_str());
    #else
        fstream file(GLOBAL_DATA_DIR+"locales/"+loc+"/items.txt");
    #endif
    for(int q=0;q<=effect;q++){
        getline(file,name);
        getline(file,descr);
        string str;
        getline(file,str);
    }
    #ifndef ANDROID
        file.close();
    #endif
    bool run=1;
    while(run){
        SDL_Rect cr={520,0,30,30};
        if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            quit(1);
        }
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            if(in_rect(cr))run=0;
        }
        else if(e.type==SDL_KEYDOWN){
            if(Back())run=0;
        }
        CLEAR(window);
        SDL_RenderDrawLine(ren,0,30,550,30);
        showIMG(cr,cross);
        FC_Draw(text,ren,15,5,strings[42].c_str());
        FC_Draw(text,ren,20,45,strings[72].c_str());
        FC_Draw(text,ren,20,75,name.c_str());
        FC_DrawColumn(text,ren,20,105,510,descr.c_str());
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_DestroyTexture(cross);
    SDL_RenderSetViewport(ren,0);
    timer_pause+=(SDL_GetTicks()-time);
    return;
}
bool buy(){
    SDL_Texture *cross  =   load(string(GLOBAL_DATA_DIR+"textures/c.png").c_str());
    SDL_Texture *tovars =   load(string(GLOBAL_DATA_DIR+"textures/t.png").c_str());
    SDL_Rect window={SW/2-125,SH/2-115,250,230};
    SDL_RenderSetViewport(ren,&window);
    int sh=-1;
    int time=SDL_GetTicks();
    mouse.state=0;
    bool run=1;
    while(run){
        SDL_Rect button[]={{14,75,64,64},
                           {93,75,64,64},
                           {172,75,64,64}};
        SDL_Rect cr={220,0,30,30};
        bool ok=(sh>=0&&(int)profile.allmoney+gg.money>=(int)profile.tovars[sh].price);
        if(e.type==SDL_QUIT){
            SDL_DestroyTexture(cross);
            SDL_DestroyTexture(tovars);
            quit(1);
        }
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            for(int q=0;q<3;q++){
                if(in_rect(button[q]))sh=q;
            }
            SDL_Rect r={14,180,222,40};
            if(in_rect(r)&&ok&&profile.can_buy){
                int effect=profile.tovars[sh].effect;
                profile.effects[effect]=1;
                     if(effect==4)gg.speed=4;
                else if(effect==5)gg.lives=2;
                else if(effect==3){
                }
                int c=gg.money-profile.tovars[sh].price;
                if(c<0)profile.allmoney+=c;
                gg.money=max(0,c);
                profile.can_buy=0;
                profile.save(profn);
                SDL_DestroyTexture(cross);
                SDL_DestroyTexture(tovars);
                buy_end(effect);
                run=0;
            }
            if(in_rect(cr))run=0;
        }
        else if(e.type==SDL_KEYDOWN){
            if(Back())run=0;
        }
        CLEAR(window);
        SDL_RenderDrawLine(ren,0,30,250,30);
        showIMG(cr,cross);
        FC_Draw(text,ren,15,5,strings[42].c_str());
        for(int q=0;q<3;q++){
            SDL_Rect r={profile.tovars[q].effect*64,0,64,64};
            showIMG(button[q],tovars,r);
            if(sh==q){
                SDL_Rect r={button[q].x-3,button[q].y-3,button[q].w+6,button[q].h+6};
                SDL_RenderDrawRect(ren,&r);
            }
            FC_DrawAlign(text,ren,button[q].x+button[q].w/2,150,FC_ALIGN_CENTER,to_string(profile.tovars[q].price).c_str());

        }
        FC_Draw(text,ren,14,45,(strings[71]+to_string(profile.allmoney+gg.money)).c_str());
        SDL_Rect r={14,180,222,40};
        SDL_RenderDrawRect(ren,&r);
        FC_DrawButtonCenter(text,ren,r,strings[70].c_str());
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
    }
    SDL_DestroyTexture(cross);
    SDL_DestroyTexture(tovars);
    SDL_RenderSetViewport(ren,0);
    mouse.wait();
    timer_pause+=(SDL_GetTicks()-time);
    return 0;
}
