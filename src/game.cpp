void game_init(bool restart){
    if(lidtoln(lid)%11==9){
        Mix_Music *music=Mix_LoadMUS(string(GLOBAL_DATA_DIR+"music/5.mp3").c_str());
        Mix_PlayMusic(music,-1);
    }else if(lidtoln(lid)%11!=10){
        int c=rand()%3+1;
        Mix_Music *music=Mix_LoadMUS(string(GLOBAL_DATA_DIR+"music/"+to_string(c)+".mp3").c_str());
        Mix_VolumeMusic(music_vol);
        Mix_ResumeMusic();
        Mix_PlayMusic(music,-1);
    }
#ifdef ANDROID
    zoom=2;
    sensor=0;
#else
    zoom=1;
#endif
    subt_hash=0;
    gg.money=0;
    gg.bring=0;
    gg.dietimer=0;
    gg.speed=(profile.effects[4]?4:3);
    gg.lives=(profile.effects[5]?2 :1 );
    gg.push=0;
    gg.n=1;
    gg.bring=0;
    for(int q=0;q<12;q++)gg.key[q]=0;
    destroy_subt();
    load_subtitles();
    ltime=timer;
    for(int x=0;x<w;x++)for(int y=0;y<h;y++)light[x][y]=0;
    for(int q=0;q<(int)wire.size();q++)wire[q].active=0;
    for(int q=0;q<(int)gate.size();q++){
        gate[q].active=(gate[q].type==21);
        if(gate[q].type!=25&&gate[q].type!=27&&gate[q].type!=28&&gate[q].type!=35&&gate[q].type!=36&&gate[q].type!=37&&gate[q].type!=38)
            gate[q].value=0;
    }
    for(int x=0;x<w;x++){
        for(int y=0;y<h;y++){
            uint8_t b=level[x][y];
            if(b==27){
                gg.x=x*64;
                gg.y=y*64;
            }
            bool ok=1;
            if(b==13)            rock.push_back({(uint16_t)(x*64),(uint16_t)(y*64),0,0});
            else if(b>=36&&b<=39)gun.push_back({(uint8_t)x,(uint8_t)y,(uint8_t)(b-36),0});
            else if(b>=47&&b<=50)gun.push_back({(uint8_t)x,(uint8_t)y,(uint8_t)(b-43),0});
            else if(b==52||b==53)hole.push_back({(uint8_t)x,(uint8_t)y,uint8_t((b-52)*5)});
            else if(b>=80&&b<=83)rock.push_back({uint16_t(x*64),uint16_t(y*64),0,uint8_t(2+(b-80)*9)});
            #ifndef DEMO
                else if(b>=75&&b<=78)portal.push_back({(uint8_t)x,(uint8_t)y,uint8_t(((int)b-75)*4)});
                else if(b==86)       bomb.push_back({(uint8_t)x,(uint8_t)y,0});
            #endif // DEMO
            else ok=0;
            if(ok)level[x][y]=0;
        }
    }
    #ifndef DEMO
        if(map)SDL_FreeSurface(map);
        if(profile.effects[3]){
            int c=get_level_c(w,h);
            map=create_surface(c*w,c*h);
        }
    #endif // DEMO
}

bool game(){
    game_init(0);
    bool ok=1;
    while(ok){
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)quit(1);
        if(Back()
           #ifdef ANDROID
           ||sensor==1
            #endif
                                  ){
            if(pause())return 1;
            SDL_RenderSetViewport(ren,0);
        }
        go();
        action();
        if(gg.lives&&gg.dietimer<0&&-gg.dietimer<timer)gg.dietimer=0;
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                bool pb=p(x*64,y*64,gg.x,gg.y);
                bool ok=0;
                for(uint32_t q=0;q<rock.size();q++){
                    if(p(x*64,y*64,rock[q].x,rock[q].y))ok=1;
                    if(level[x][y]==12&&p(x*64,y*64,rock[q].x,rock[q].y)&&!rock[q].tm)rock[q].tm=timer;
                    if(timer>timer_trap+30&&
                      level[x][y]>=52&&
                      level[x][y]<57&&
                       p(x*64,y*64,rock[q].x,rock[q].y)){
                        timer_trap=timer;
                        level[x][y]++;
                    }
                }
                if(level[x][y]==14&&(pb||ok)){
                    level[x][y]=15;
                    Mix_VolumeChunk(button,sound_vol);
                    Mix_PlayChannel(-1,button,0);
                }
                if(level[x][y]==15&&!pb&&!ok){
                    level[x][y]=14;
                    Mix_VolumeChunk(buttoff,sound_vol);
                    Mix_PlayChannel(-1,buttoff,0);
                }
                if(level[x][y]==8&&pb){
                    level[x][y]=9;
                    Mix_VolumeChunk(dopen,sound_vol);
                    Mix_PlayChannel(-1,dopen,0);
                }
                if(level[x][y]==9&&!pb){
                    level[x][y]=8;
                    Mix_VolumeChunk(dclose,sound_vol);
                    Mix_PlayChannel(-1,dclose,0);
                }
                if(level[x][y]==12&&pb&&!gg.dietimer)gg.kill();
                if(level[x][y]==26&&pb&&!gg.dietimer){
                    if(st){
                        if(lidtoln(lid)%11==9){
                            profile.random();
                            profile.can_buy=(profile.level<=lidtoln(lid));
                        }
                        profile.level=max(lidtoln(lid)+1,profile.level);
                        if(gg.money>profile.money[lidtoln(lid)]){
                            profile.allmoney-=profile.money[lidtoln(lid)];
                            profile.money[lidtoln(lid)]=gg.money;
                            profile.allmoney+=profile.money[lidtoln(lid)];
                        }
                        unsigned short *tm= &profile.time[lidtoln(lid)];
                        if(*tm==0||*tm>(timer-ltime)/100)*tm=(timer-ltime)/100;
                        profile.save(profn);
                    }
                    if(end(0,st))return 1;
                }
                if(p(x*64,y*64,gg.x,gg.y)){
                    bool ok=1;
                    switch(level[x][y]){
                        case 16:case 17:case 18:case 19:case 20:case 21:gg.key[level[x][y]-16]++;break;
                        case 22:gg.money+=1;break;
                        case 23:gg.money+=2;break;
                        case 24:gg.money+=5;break;
                        case 25:gg.money+=10;break;
                        case 30:case 31:case 32:case 33:case 34:case 35:
                            gg.key[level[x][y]-24]++;
                            if(!profile.ach[2]){
                                profile.ach[2]=1;
                                profile.save(profn);
                            }
                        break;
                        default:ok=0;
                    }
                    if(ok){
                        Mix_VolumeChunk(moneta,sound_vol);
                        Mix_PlayChannel(-1,moneta,0);
                        level[x][y]=0;
                    }
                }

            }
        }
        for(int q=0;q<6;q++){
            if(gg.key[q]&&gg.block()==2+q){
                gg.replace(8);
                Mix_VolumeChunk(dkey,sound_vol);
                Mix_PlayChannel(-1,dkey,0);
            }
        }
        if(gg.block()==10){
            gg.replace(8);
            if(!profile.ach[3]){
                profile.ach[3]=1;
                profile.save(profn);
            }
        }
        if(gg.dietimer&&e.button.state==1)if(end(1,0))return 1;
        wires();
        gates();
        guns();
        holes();
        rocks();
        bullets();
        set_camera(gg.x,gg.y,0);
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderClear(ren);
        draw(1,0,0);
        #ifdef ANDROID
            sensor_draw();
        #endif
        if(sb_on)subtitres();
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_Rect r={0,0,SW,50};
        SDL_RenderFillRect(ren,&r);
        FC_Draw(menu,ren,50,10,string(strings[68]+to_string(gg.money)+"     "+to_string((timer-ltime)/60000)+":"+to_string((timer-ltime)%60000/1000)).c_str());
        SDL_RenderPresent(ren);
//        cout<<"FPS: "<<1000/(SDL_GetTicks()-gh)<<endl;
    }
    return 0;
}
