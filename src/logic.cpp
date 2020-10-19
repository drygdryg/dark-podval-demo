float max(float a,float b){return (a>b)?a:b;}
float min(float a,float b){return (a<b)?a:b;}
void wires(){
    int all_open=0;
    int all_close=0;
    for(int q=0;q<(int)wire.size();q++){
        uint8_t v=level[wire[q].x1][wire[q].y1];
        if(v==14||v==15)wire[q].active=v-14;
        if(v==71||v==72)wire[q].active=v-71;
        if(v==74||v==73)wire[q].active=v-73;
        uint8_t u=level[wire[q].x2][wire[q].y2];
        float metres=sqrt(pow(gg.x/64.0-wire[q].x2,2)+pow(gg.y/64.0-wire[q].y2,2));
        float volume=max(10-metres,0)/20.0*(MIX_MAX_VOLUME);
        if(u==28||u==29)level[wire[q].x2][wire[q].y2]=wire[q].active+28;
        if(u==11&&!wire[q].active){
            all_close+=volume;
            level[wire[q].x2][wire[q].y2]=12;
        }
        if(u==12&&wire[q].active){
            all_open+=volume;
            level[wire[q].x2][wire[q].y2]=11;
        }
        if(u==69||u==70)level[wire[q].x2][wire[q].y2]=wire[q].active+69;
        for(int w=0;w<(int)gate.size();w++){
            if(gate[w].x==wire[q].x1&&gate[w].y==wire[q].y1)wire[q].active=gate[w].active;
        }
    }
    if(all_close){
        Mix_VolumeChunk(spike_close,min(all_close,MIX_MAX_VOLUME)/128.0*sound_vol);
        Mix_PlayChannel(-1,spike_close,0);
    }
    if(all_open){
        Mix_VolumeChunk(spike_open,min(all_open,MIX_MAX_VOLUME)/128.0*sound_vol);
        Mix_PlayChannel(-1,spike_open,0);
    }
}
void gates(){
    qbuf="";
    for(int q=0;q<(int)gate.size();q++){
        switch(gate[q].type){
            case 0:{
                bool ok=1;
                for(int w=0;w<(int)wire.size();w++){
                    if(!wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=0;
                }
                gate[q].active=ok;
            }break;
            case 1:{
                bool ok=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                }
                gate[q].active=ok;
            }break;
            case 2:{
                int c=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)c+=wire[w].active;
                }
                gate[q].active=(c>0&&c!=(int)wire.size());
            }break;
            case 3:{
                bool ok=1;
                for(int w=0;w<(int)wire.size();w++){
                    if(!wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=0;
                }
                gate[q].active=!ok;
            }break;
            case 4:{
                bool ok=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                }
                gate[q].active=!ok;
            }break;
            case 5:{
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)gate[q].active=!wire[w].active;
                }
            }break;
            case 6:case 7:case 8:case 9:{
                int t=0;
                if(gate[q].type==6)t=1000;
                if(gate[q].type==7)t=2000;
                if(gate[q].type==8)t=5000;
                if(gate[q].type==9)t=10000;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active){gate[q].value=timer;}
                }
                gate[q].active=!(gate[q].value+t<(int)timer);
            }break;
            case 10:case 11:case 12:case 13:case 14:{
                int t=2500;
                if(gate[q].type==10)t=250;
                else if(gate[q].type==11)t=500;
                else if(gate[q].type==12)t=1000;
                else if(gate[q].type==13)t=2000;
                if(gate[q].value+t<(int)timer){
                    gate[q].value=(int)timer;
                    gate[q].active=!gate[q].active;
                }
            }break;
            case 15:case 16:case 17:case 18:case 19:{
                int t=5000;
                if(gate[q].type==15)t=500;
                else if(gate[q].type==16)t=1000;
                else if(gate[q].type==17)t=2000;
                else if(gate[q].type==18)t=4050;
                gate[q].active=0;
                if(gate[q].value+t<(int)timer){
                    gate[q].value=(int)timer;
                    gate[q].active=1;
                }
            }break;
            case 20:{
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active)gate[q].active=1;
                }
            }break;
            case 21:{
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active)gate[q].active=0;
                }
            }break;
            case 22:{
                gate[q].active=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active)gate[q].active=1;
                }
                if(gate[q].active){
                    for(int w=0;w<(int)gun.size();w++){
                        if(gun[w].x==gate[q].x+1&&gun[w].y==gate[q].y){
                            if(gun[w].type%4==3)gun[w].type-=3;
                            else gun[w].type++;
                        }
                    }
                    uint8_t v=level[gate[q].x+1][gate[q].y];
                    if(v>=54&&v<=68){
                        if(v==57)     level[gate[q].x+1][gate[q].y]=54;
                        else if(v==61)level[gate[q].x+1][gate[q].y]=58;
                        else if(v==63)level[gate[q].x+1][gate[q].y]=62;
                        else if(v==67)level[gate[q].x+1][gate[q].y]=64;
                        else if(v!=68)level[gate[q].x+1][gate[q].y]++;
                    }
                }
            }break;
            case 23:{
                gate[q].active=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active)gate[q].active=1;
                }
                if(gate[q].active){
                    for(int w=0;w<(int)gun.size();w++){
                        if(gun[w].x==gate[q].x+1&&gun[w].y==gate[q].y){
                            if(gun[w].type%4==0)gun[w].type+=3;
                            else gun[w].type--;
                        }
                    }
                    uint8_t v=level[gate[q].x+1][gate[q].y];
                    if(v>=54&&v<=68){
                        if(v==54)     level[gate[q].x+1][gate[q].y]=57;
                        else if(v==58)level[gate[q].x+1][gate[q].y]=61;
                        else if(v==62)level[gate[q].x+1][gate[q].y]=63;
                        else if(v==64)level[gate[q].x+1][gate[q].y]=67;
                        else if(v!=68)level[gate[q].x+1][gate[q].y]--;
                    }
                }
            }break;
            case 24:{
                if(gg.x/64==gate[q].x&&gg.y/64==gate[q].y)gate[q].active=1;
                else gate[q].active=0;
            }break;
            case 25:{
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active&&!gate[q].active){
                        gate[q].active=1;
                        #ifdef ANDROID
                            stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/replies.txt").c_str());
                        #else
                            ifstream file(string(GLOBAL_DATA_DIR+"locales/"+loc+"/replies.txt").c_str());
                        #endif
                        int k=0;
                        while(k!=gate[q].value){
                            getline(file,txt);
                            k++;
                        }
                        #ifndef ANDROID
                            file.close();
                        #endif
                        timer_sbt=timer;
                        break;
                    }
                }
            }break;
            case 26:{
                gate[q].active=0;
                for(unsigned w=0;w<rock.size();w++){
                    if(rock[w].x/64==gate[q].x&&rock[w].y/64==gate[q].y)gate[q].active=1;
                }
            }break;
            case 27:{
                gate[q].active=(gg.key[gate[q].value]==1);
            }break;
            case 28:{
                gate[q].active=(gg.key[gate[q].value]==2);
            }break;
            case 29:{
                gate[q].active=(gg.dietimer>0);
            }break;
            case 30:{
                for(unsigned w=0;w<wire.size();w++){
                    if(wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y&&wire[w].active&&!gate[q].active){
                        gate[q].active=1;
                        gg.kill();
                    }
                }
            }break;
            case 31:case 32:case 33:case 34:{
                int t=0;
                if(gate[q].type==31)t=1000;
                if(gate[q].type==32)t=2000;
                if(gate[q].type==33)t=5000;
                if(gate[q].type==34)t=10000;
                gate[q].active=0;
                bool ok=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                }
                if(ok&&!gate[q].value)gate[q].value=timer;
                if(timer>gate[q].value+t){
                    if(!ok)gate[q].value=0;
                    if(ok)gate[q].active=1;
                }
            }break;
            case 35:{
                if(gate[q].value){
                    bool ok=0;
                    for(int w=0;w<(int)wire.size();w++){
                        if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                    }
                    if(!ok)break;
                    int n=0;
                    for(unsigned q=0;q<qbuf.size();q++)if(qbuf[q]==63)n++;
                    gate[q].active=0;
                    SDL_Rect r={SW/8,50+n*30,SW/8*6,20};
                    if(key[SDL_SCANCODE_1+n]||(e.button.state&&in_rect(r))){
                        gate[q].active=1;
                        gate[q].value=0;
                        while(key[SDL_SCANCODE_1+n])SDL_PollEvent(&e);
                        while(e.button.state&&in_rect(r))SDL_PollEvent(&e);
                        break;
                    }
                    qbuf+=string((qbuf.size()?"\n":"")+to_string(n+1)+". "+*questions[gate[q].value]);
                }
            }break;
            case 36:{
                bool ok=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                }
                if(ok&&!profile.ach[gate[q].value]){
                    profile.ach[gate[q].value]=1;
                    profile.save(profn);
                }
            }break;
            case 37:{
                if(st){
                    gate[q].active=(profile.die[lidtoln(lid)]==gate[q].value);
                }
            }break;
            case 38:{
                bool ok=0;
                for(int w=0;w<(int)wire.size();w++){
                    if(wire[w].active&&wire[w].x2==gate[q].x&&wire[w].y2==gate[q].y)ok=1;
                }
                if(ok&&!gate[q].active){
                    gate[q].active=1;
                    Mix_Music *music=Mix_LoadMUS(string(GLOBAL_DATA_DIR+"music/"+to_string(gate[q].value)+".mp3").c_str());
                    Mix_VolumeMusic(music_vol);
                    Mix_ResumeMusic();
                    Mix_PlayMusic(music,-1);
                }
            }
        }
    }
}
void guns(){
    float all_pul=0;
    float all_fire=0;
    float all_load=0;
    bool pm=0,ar=0;
    if(timer>timer_pm+70){timer_pm=timer;pm=1;}
    if(timer>timer_ar+100){timer_ar=timer;ar=1;}
    for(int q=0;q<(int)gun.size();q++){
        float metres=sqrt(pow(gg.x/64.0-gun[q].x,2)+pow(gg.y/64.0-gun[q].y,2));
        float volume=max(20-metres,0)/20.0*(MIX_MAX_VOLUME);
        bool ok=0;
        for(int w=0;w<(int)wire.size();w++){
            if(wire[w].x2==gun[q].x&&
               wire[w].y2==gun[q].y&&wire[w].active)ok=1;
        }
        if(gun[q].type>=0&&gun[q].type<4){
            if(ok){
                gun[q].anim=0;
                bullet.push_back({(uint16_t)((int)gun[q].x*64),(uint16_t)((int)gun[q].y*64),(uint8_t)((int)gun[q].type%4),0});
                all_fire+=volume;
            }
            if(ar&&gun[q].anim==4)all_load+=volume;
            if(ar&&gun[q].anim<5)gun[q].anim++;
        }
        if(gun[q].type>=4&&gun[q].type<8){
            if(ok)gun[q].anim=0;
            else if(pm){
                if(gun[q].anim==3)gun[q].anim=1;
                else if(gun[q].anim<3)gun[q].anim++;
                bullet.push_back({(uint16_t)((int)gun[q].x*64),(uint16_t)((int)gun[q].y*64),(uint8_t)((int)gun[q].type%4),1});
                all_pul+=volume;
            }
        }
    }
    if(all_pul){
        Mix_VolumeChunk(pulemet,min(all_pul,128)/128.0*sound_vol);
        Mix_PlayChannel(-1,pulemet,0);
    }
    if(all_fire){
        Mix_VolumeChunk(crossbow_fire, min(all_fire,128)/128.0*sound_vol);
        Mix_PlayChannel(-1, crossbow_fire, 0);
    }
    if(all_load){
        Mix_VolumeChunk(crossbow_load, min(all_load,128)/128.0*sound_vol);
        Mix_PlayChannel(-1, crossbow_load, 0);
    }
}
void holes(){
    for(unsigned q=0;q<hole.size();q++){
        if(timer>timer_trap+30&&((!hole[q].anim&&p(hole[q].x*64,hole[q].y*64,gg.x,gg.y))||
                                 (hole[q].anim&&hole[q].anim<5))){
            timer_trap=timer;
            hole[q].anim++;
        }
    }
}
void rocks(){
    for(int q=0;q<(int)rock.size();q++){
        if(rock[q].type!=1){
            bool ok=0;
            if(gg.n==1&&gg.x+64>rock[q].x&&gg.x<rock[q].x&&gg.y==rock[q].y){rock[q].x=gg.x+64;ok=1;}
            if(gg.n==2&&gg.y+64>rock[q].y&&gg.y<rock[q].y&&gg.x==rock[q].x){rock[q].y=gg.y+64;ok=1;}
            if(gg.n==3&&gg.x<rock[q].x+64&&gg.x>rock[q].x&&gg.y==rock[q].y){rock[q].x=gg.x-64;ok=1;}
            if(gg.n==0&&gg.y<rock[q].y+64&&gg.y>rock[q].y&&gg.x==rock[q].x){rock[q].y=gg.y-64;ok=1;}
            if(ok&&profile.effects[0]){
                for(unsigned w=0;w<rock.size();w++){
                    if(gg.n==1&&rock[q].x+64>rock[w].x&&rock[q].x<rock[w].x&&gg.y==rock[w].y)rock[w].x=rock[q].x+64;
                    if(gg.n==2&&rock[q].y+64>rock[w].y&&rock[q].y<rock[w].y&&gg.x==rock[w].x)rock[w].y=rock[q].y+64;
                    if(gg.n==3&&rock[q].x<rock[w].x+64&&rock[q].x>rock[w].x&&gg.y==rock[w].y)rock[w].x=rock[q].x-64;
                    if(gg.n==0&&rock[q].y<rock[w].y+64&&rock[q].y>rock[w].y&&gg.x==rock[w].x)rock[w].y=rock[q].y-64;
                }
            }
            if(rock[q].type==0){
                for(unsigned w=0;w<hole.size();w++){
                    if(hole[w].anim==5&&hole[w].x*64==rock[q].x&&hole[w].y*64==rock[q].y&&
                       rock[q].x%64==0&&rock[q].y%64==0)
                        {rock.erase(rock.begin()+q);hole[w].anim=6;}
                    if(hole[w].anim==0&&p(hole[w].x*64,hole[w].y*64,rock[q].x,rock[q].y))hole[w].anim=1;
                }
            }else if(rock[q].type>1&&rock[q].type%9!=2&&rock[q].type%9!=3){
                if(timer-rock[q].tm>50){
                    rock[q].tm=timer;
                    if(rock[q].type%9==1)rock[q].type-=8;
                    else rock[q].type++;
                    if(rock[q].type%9==7){
                        bullet.push_back({rock[q].x,rock[q].y,(uint8_t)((int)rock[q].type/9),2});
                        Mix_PlayChannel(-1,boom,0);
                    }
                }
            }
        }
    }
}
bool Bullet::p(){
    uint8_t level2[255][255];
    uint8_t ar[]={1,2,3,4,5,6,7,28,40,41,42,43,44,45,46,51,8,10,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,84,87};
    memcpy(level2,level,sizeof(level2));
    for(unsigned q=0;q<rock.size();q++){
        if(rock[q].type==0){
            level2[(rock[q].x+32)/64][(rock[q].y+32)/64]=1;
        }
    }
    for(unsigned q=0;q<sizeof(ar);q++){
        if(level2[(x+32)/64][(y+32)/64]==ar[q])return 1;
    }
    return 0;
}
void Bullet::move(){
    y-=(n==0)*16;
    x+=(n==1)*16;
    y+=(n==2)*16;
    x-=(n==3)*16;
}
void bullets(){
    if(timer>timer_bl+10){
        timer_bl=timer;
        for(int q=0;q<(int)bullet.size();q++){
            bullet[q].move();
            uint8_t *block=&level[(bullet[q].x+32)/64][(bullet[q].y+32)/64];
            if(bullet[q].type==2){
                if(*block==87||*block==69||*block==70)*block=88;
            }
            if(bullet[q].p()){
                if(bullet[q].type==2){
                    if(*block==84)*block=85;
                    if(*block==1||*block==40||*block==42||*block==43||
                       *block==44||*block==45||*block==46)*block=84;
                    for(unsigned w=0;w<rock.size();w++){
                        if(rock[w].type==0&&
                           rock[w].x/64==(bullet[q].x+32)/64&&
                           rock[w].y/64==(bullet[q].y+32)/64){
                            rock[w].tm=timer;
                        }
                    }
                }
                bullet.erase(bullet.begin()+q);
            }
            if(abs(gg.x-bullet[q].x)<32&&abs(gg.y-bullet[q].y)<32){
                if(!gg.dietimer)gg.kill();
                bullet.erase(bullet.begin()+q);
            }
        }
    }
}
