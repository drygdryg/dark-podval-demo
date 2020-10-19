void destroy_subt(){
    buf="";
    qbuf="";
    txt="";
}
bool nadpis(string lid, int x,int y){
    int c=0;
    bool ok=1;
    for(int yp=0;yp<h&&ok;yp++){
        for(int xp=0;xp<w&&ok;xp++){
            if(level[xp][yp]==40||level[xp][yp]==41||level[xp][yp]==42)c++;
            if(xp==x&&yp==y)ok=0;
        }
    }
    SDL_Texture* image;
    string path=GLOBAL_DATA_DIR+"locales/"+loc+"/images/";
    path+=lid;
    path+='-';
    path+=(char)(c/10%10+'0');path+=(char)(c%10+'0');
    path+=".png";
    image=load(path.c_str());
    int xt,yt;
    SDL_QueryTexture(image,0,0,&xt,&yt);
    while(e.type!=SDL_MOUSEBUTTONDOWN&&!key[SDL_SCANCODE_ESCAPE]){
        SDL_WaitEvent(&e);
        if(e.type==SDL_QUIT)return 1;
        showIMG(SW/2,SH/2,xt,yt,image,1);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(image);
    return 0;
}
void subtitres(){
    int c=0;
    bool b=0;
    SDL_Color cl=black;
    if(txt[txt.size()-1]!=' ')txt+="$$$$ ";
    for(unsigned q=0;q<txt.size();q++){
        if(txt[q]=='$'||q==0){
            if(c++==(timer-timer_sbt)/delay){
                if(txt[txt.size()-1]!=' ')txt+="$$$$ ";
                int d=txt.find('$',q+1);
                string txt2;
                txt2.assign(txt,q,d-q);
                if(txt2[0]=='$')txt2.erase(txt2.begin());
                if(txt2.size())buf=txt2;
            }
        }
    }
    string s="";
    if(buf.size()>1&&buf[0]=='\\'){
        switch(buf[1]){
            case 'b':b    =1;      break;
            case 'r':cl   =red;    break;
            case 'g':cl   =green;  break;
            case 'y':cl   =yellow; break;
            case 'w':cl   =white;  break;
            case 'v':cl   =blue;   break;
        }
        s+="\\";
        s+=buf[1];
        buf.erase(buf.begin(),buf.begin()+2);
    }
    if(buf!=""&&buf!=" "){
        #ifdef ANDROID
            int w=SW-550;
        #else
            int w=SW/8*6;
        #endif
        int h=FC_GetColumnHeight(b?bold:text,w,buf.c_str());
        #ifdef ANDROID
            SDL_Rect r={400,SH-50-h,w,h};
        #else
            SDL_Rect r={SW/8,SH-50-h,w,h};
        #endif
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren,255,255,255,100);
        SDL_RenderFillRect(ren,&r);
        #ifdef ANDROID
            FC_DrawColumnColor(b?bold:text,ren,400,SH-50-h,w,cl,buf.c_str());
        #else
            FC_DrawColumnColor(b?bold:text,ren,SW/8,SH-50-h,w,cl,buf.c_str());
        #endif
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);
    }
    buf=s+buf;
    if(qbuf!=""&&qbuf!=" "){
        int w=SW/8*6;
        int h=FC_GetColumnHeight(text,w,qbuf.c_str());
        SDL_Rect r={SW/8,50,w,h};
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren,255,255,255,100);
        SDL_RenderFillRect(ren,&r);
        FC_DrawColumnColor(text,ren,SW/8,50,w,black,qbuf.c_str());
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);
    }
}
void intro(){
    int c=4;
    destroy_subt();
    #ifdef ANDROID
        stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/intro.txt").c_str());
    #else
        ifstream file(string(GLOBAL_DATA_DIR+"locales/"+loc+"/intro.txt").c_str());
    #endif
    getline(file,txt);
    #ifndef ANDROID
        file.close();
    #endif
    for(unsigned q=0;q<txt.size();q++)if(txt[q]=='$')c++;
    timer_sbt=timer;
    int th=timer;
    while(th+c*delay>timer){
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)quit(1);
        if(e.type==SDL_KEYDOWN)break;
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderClear(ren);
        subtitres();
        SDL_RenderPresent(ren);
    }
}
void action(){
    if(((key[SDL_SCANCODE_E]&&e.type==SDL_KEYDOWN)
        #ifdef ANDROID
        ||sensor==6
        #endif
        )&&gg.x%64==0&&gg.y%64==0){
        for(int x=-1;x<=1;x++){
            for(int y=-1;y<=1;y++){
                if((abs(x)+abs(y)==1)||(x==0&&y==0)){
                    int xp=gg.x/64+x,
                        yp=gg.y/64+y;
                    if(level[xp][yp]==40||level[xp][yp]==41||level[xp][yp]==42)nadpis(lid,xp,yp);
                    if(level[xp][yp]==73)level[xp][yp]=74;
                    else if(level[xp][yp]==74)level[xp][yp]=73;
                    if(level[xp][yp]==51&&profile.can_buy)buy();
                }
            }
        }
        bool ok=0;
        for(unsigned q=0;q<rock.size();q++){
            if(abs(rock[q].x-gg.x)/64+abs(rock[q].y-gg.y)/64==1&&!ok){
                if(rock[q].type%9==2&&gg.bring==1){
                    rock[q].type++;
                    gg.bring=0;
                    ok=1;
                }else if(rock[q].type%9==3){
                    rock[q].type++;
                }
            }
        }
    }
    if(((key[SDL_SCANCODE_R]&&e.type==SDL_KEYDOWN)
        #ifdef ANDROID
        ||sensor==7
        #endif
        )&&gg.x%64==0&&gg.y%64==0){
        for(unsigned q=0;q<rock.size();q++){
            if(abs(rock[q].x-gg.x)/64+abs(rock[q].y-gg.y)/64==1&&rock[q].type>1){
                if(rock[q].type>=29)rock[q].type-=27;
                else rock[q].type+=9;
            }
        }
    }
    if(((key[SDL_SCANCODE_Z]&&e.type==SDL_KEYDOWN)
        #ifdef ANDROID
        ||sensor==8
        #endif
        )&&gg.x%64==0&&gg.y%64==0){
        bool ok=1;
        for(int x=-1;x<=1;x++){
            for(int y=-1;y<=1;y++){
                if((abs(x)+abs(y)==1)||(x==0&&y==0)){
                    if(ok&&!gg.bring&&level[gg.x/64+x][gg.y/64+y]==79){
                        gg.bring=1;
                        level[gg.x/64+x][gg.y/64+y]=0;
                        ok=0;
                    }
                }
            }
        }
        if(ok){
            uint8_t level2[255][255];
            memcpy(level2,level,sizeof(level2));
            for(unsigned q=0;q<rock.size();q++)level2[rock[q].x/64][rock[q].y/64]=1;
            for(unsigned q=0;q<gun.size();q++)level2[gun[q].x][gun[q].y]=1;
            uint8_t xp=gg.x/64+(gg.n==1)-(gg.n==3);
            uint8_t yp=gg.y/64+(gg.n==2)-(gg.n==0);
            if(ok&&gg.bring&&level2[xp][yp]==0){
                if(gg.bring==1)level[xp][yp]=79;
                gg.bring=0;
                ok=0;
            }else if(ok&&gg.bring&&level2[gg.x/64][gg.y/64]==0){
                if(gg.bring==1)level[gg.x/64][gg.y/64]=79;
                gg.bring=0;
                ok=0;
            }
        }
    }
}
void set_camera(unsigned short x,unsigned short y,bool editor){
    if(!editor){
        cx=(-x-32)*zoom+SW/2;
        cy=(-y-32)*zoom+SH/2;
    }
    if(cx>0)cx=0;
    if(cy>50)cy=50;
    if(cx<-(w*64*zoom-SW))cx=-(w*64*zoom-SW);
    if(cy<-(h*64*zoom-SH))cy=-(h*64*zoom-SH);
}
void go(){
    if(gg.dietimer<=0){
        if(gg.x%64==0&&gg.y%64==0){
            uint8_t k[4]={SDL_SCANCODE_W,SDL_SCANCODE_D,SDL_SCANCODE_S,SDL_SCANCODE_A};
            bool ok=0;
            for(int q=0;q<4;q++){
                if((key[k[q]]
                    #ifdef ANDROID
                    ||sensor==q+2
                    #endif
                    )&&!ok){
                    gg.n=q;
                    if(gg.can_go(&gg.push,q)){
                        gg.sx=gg.x/64;
                        gg.sy=gg.y/64;
                        ok=1;
                        gg.timerr=timer;
                        gg.move(1,q);
                    }
                }
            }
        }else {
            float block=(timer-gg.timerr)/20.0*gg.speed+1;
            switch(gg.n){
                case 0:gg.y=gg.sy*64-block;break;
                case 1:gg.x=gg.sx*64+block;break;
                case 2:gg.y=gg.sy*64+block;break;
                case 3:gg.x=gg.sx*64-block;break;
            }
            if(abs(gg.sx*64-gg.x)>=64||abs(gg.sy*64-gg.y)>=64){
                gg.x=gg.sx*64;
                gg.y=gg.sy*64;
                gg.move(64,gg.n);
            }
        }
    }
    gg.clear();
}
string command(string command){
    string result;
    stringstream str;
    str<<command;
    str>>command;
    if(command=="nofog"){
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                light[x][y]=255;
            }
        }
        result="OK";
    }
    else if(command=="fog"){
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                light[x][y]=0;
            }
        }
        result="OK";
    }
    else if(command=="speed"){
        int speed=0;
        str>>speed;
        if(speed>255)result="speed can`t be > 255";
        else if(speed==0)result="speed can`t be 0";
        else if(speed<0)result="speed can`t be < 0";
        else{
            result="speed changed from "+to_string(gg.speed)+" to "+to_string(speed);
            gg.speed=speed;
        }
    }
    else if(command=="noclip"){
        gg.noclip=!gg.noclip;
        result="noclip "+string(gg.noclip?"ON":"OFF");
    }
    else if(command=="health"){
        gg.infhealth=!gg.infhealth;
        result="infinity health "+string(gg.infhealth?"ON":"OFF");
    }
    else if(command=="kill"){
        if(gg.infhealth)result="You have infinity health YET!";
        else{
            result="You are suicidal!";
            gg.kill();
        }
    }
    else if(command=="setblock"){
        int x=0,y=0,b=0;
        str>>x>>y;
             if(x<0)  result="X can`t < 0";
        else if(x>=w) result="X can`t > level width";
        else if(y<0)  result="X can`t < 0";
        else if(y>=h) result="X can`t > level height";
        else if(b<0)  result="Block can`t < 0";
        else if(b>255)result="Block can`t > 255";
        else{
            result="OK";
            level[x][y]=b;
        }
    }
    else if(command=="skip"){
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                if(level[x][y]==26){
                    result="OK";
                    gg.x=x*64;
                    gg.y=y*64;
                    return result;
                }
            }
        }
        result="Level has not exit";
    }
    else if(command=="effect"){
        string effect;
        string sw;
        str>>effect>>sw;
        int number=-1;
        if(effect=="force")        number=0;
        if(effect=="bad_memory")   number=1;
        if(effect=="blind")        number=2;
        if(effect=="minimap")      number=3;
        if(effect=="super_speed")  number=4;
        if(effect=="reincarnation")number=5;
        if(effect.size()==1){
            for(int q=0;q<TVR_COUNT;q++){
                if(effect[0]==char(q+'0'))number=q;
            }
        }
        if(number==-1)result="It is not effect";
        else if(sw!="on"&&sw!="off"&&sw!="0"&&sw!="1"&&sw!="ON"&&sw!="OFF")
            result="Invalid switch value";
        else{
            profile.effects[number]=(sw=="on"||sw=="1"||sw=="ON");
            result="Effect \""+effect+"\" ("+to_string(number)+") choosen to value "+to_string(sw=="on"||sw=="1"||sw=="ON");
                 if(number==4)gg.speed=(profile.effects[number]?4:3);
            else if(number==5)gg.lives=(profile.effects[number]?2:1);
            else if(number==3){
            }
            profile.save(profn);
        }
    }
    else if(command=="money"){
        int money;
        str>>money;
             if(money<0)result="Money can`t be < 0";
        else if(money>65535)result="Money can`t be > 65535";
        else {
            result="Money changed from "+to_string(profile.allmoney)+" to "+to_string(money);
            profile.allmoney=money;
            profile.save(profn);
        }
    }
    else if(command=="level_money"){
        int money;
        str>>money;
             if(money<0)result="Money can`t be < 0";
        else if(money>65535)result="Money can`t be > 65535";
        else {
            result="Level money changed from "+to_string(gg.money)+" to "+to_string(money);
            gg.money=money;
        }
    }
    else if(command=="zoom"){
        float z;
        str>>z;
        result="OK";
        zoom=z;
    }
    return result;
}
void load_subtitles(){
    timer_sbt=timer;
    if(st){
        #ifdef ANDROID
            stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/lnames.txt").c_str());
        #else
            ifstream file(string(GLOBAL_DATA_DIR+"locales/"+loc+"/lnames.txt").c_str());
        #endif
        int k=0;
        string str;
        while(k<=lidtoln(lid)){
            getline(file,str);
            k++;
        }
        if(str[str.size()-1]=='\r')str.pop_back();
        #ifndef ANDROID
            file.close();
        #endif
        txt=string(strings[67])+lid+"\n\""+str+"\"";
    }
    else txt=string(strings[67])+lid;
    for(unsigned q=0;q<questions.size();q++){
        if(questions[q]!=0)SDL_free(questions[q]);
        questions[q]=0;
    }
    int max_question=0;
    for(unsigned q=0;q<gate.size();q++){
        if(gate[q].type==35&&gate[q].value>max_question)max_question=gate[q].value+1;
    }
    questions.resize(max_question);
    for(unsigned q=0;q<gate.size();q++){
        if(gate[q].type==35){
            questions[gate[q].value]=new string;
            #ifdef ANDROID
                stringstream file=RWstream(string(GLOBAL_DATA_DIR+"locales/"+loc+"/questions.txt").c_str());
            #else
                ifstream file(string(GLOBAL_DATA_DIR+"locales/"+loc+"/questions.txt").c_str());
            #endif
            int k=0;

            while(k!=gate[q].value){
                getline(file,*questions[gate[q].value]);
                k++;
            }
            #ifndef ANDROID
                file.close();
            #endif
        }
    }
}
