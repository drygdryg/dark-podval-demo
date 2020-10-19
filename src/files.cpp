#ifdef ANDROID
    char *RWget(const char *filename) {
        SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
        if (rw == NULL) return NULL;

        Sint64 res_size = SDL_RWsize(rw);
        char* res = (char*)malloc(res_size + 1);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = res;
        while (nb_read_total < res_size && nb_read != 0) {
                nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
                nb_read_total += nb_read;
                buf += nb_read;
        }
        SDL_RWclose(rw);
        if (nb_read_total != res_size) {
                free(res);
                return NULL;
        }
        res[nb_read_total] = '\0';
        return res;
    }
    stringstream RWstream(const char *filename){
        stringstream a;
        char *b=RWget(filename);
        a<<b;
        delete[]b;
        return a;

    }
#endif
char in(ifstream& in){
    char c;
    in.read(reinterpret_cast<char*>(&c),1);
    return c;
}
char in(stringstream& in){
    char c;
    in.read(reinterpret_cast<char*>(&c),1);
    return c;
}
void open_old(string path){
    #ifdef ANDROID
        stringstream f=RWstream(path.c_str());
    #else
        ifstream f(path.c_str(),ios_base::in|ios_base::binary);
    #endif
    f>>w>>h;
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int n;
            f>>n;level[x][y]=n;
        }
    }
    gate.resize(0);
    wire.resize(0);
    #ifndef ANDROID
        f.close();
    #endif
}
void save(string path){
     #ifdef ANDROID
        stringstream f=RWstream(path.c_str());
    #else
        ofstream f(path,ios_base::out|ios_base::binary);
    #endif
    f<<(char)w<<(char)h;
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            f<<(char)(level[x][y]);
        }
    }
    f<<(char)gate.size();
    for(int q=0;q<(int)gate.size();q++){
        f<<(char)gate[q].x
         <<(char)gate[q].y
         <<(char)gate[q].type
         <<(char)(gate[q].value/256)<<(char)(gate[q].value%256);
    }
    f<<(char)wire.size();
    for(int q=0;q<(int)wire.size();q++){
        f<<(char)wire[q].x1
         <<(char)wire[q].y1
         <<(char)wire[q].x2
         <<(char)wire[q].y2;
    }
    #ifndef ANDROID
        f.close();
    #endif
}
void open(string path){
    #ifdef ANDROID
        SDL_RWops *f=SDL_RWFromFile(path.c_str(),"rb");
        w=SDL_ReadU8(f);
        h=SDL_ReadU8(f);
        for(int y=0;y<h;y++){
            for(int x=0;x<w;x++){
                level[x][y]=SDL_ReadU8(f);
            }
        }
        gate.resize(SDL_ReadU8(f));
        for(int q=0;q<(int)gate.size();q++){
            gate[q].x=SDL_ReadU8(f);
            gate[q].y=SDL_ReadU8(f);
            gate[q].type=SDL_ReadU8(f);
            gate[q].value=SDL_ReadBE16(f);
        }
        wire.resize(SDL_ReadU8(f));
        for(int q=0;q<(int)wire.size();q++){
            wire[q].x1=SDL_ReadU8(f);
            wire[q].y1=SDL_ReadU8(f);
            wire[q].x2=SDL_ReadU8(f);
            wire[q].y2=SDL_ReadU8(f);
        }
        f->close(f);
    #else
        ifstream f(path.c_str(),ios_base::in|ios_base::binary);
        w=in(f);
        h=in(f);
        for(int y=0;y<h;y++){
            for(int x=0;x<w;x++){
                level[x][y]=in(f);
            }
        }
        gate.resize(in(f));
        for(int q=0;q<(int)gate.size();q++){
            gate[q].x=in(f);
            gate[q].y=in(f);
            gate[q].type=in(f);
            gate[q].value=in(f);
            gate[q].value*=256;
            gate[q].value+=in(f);
        }
        wire.resize(in(f));
        for(int q=0;q<(int)wire.size();q++){
            wire[q].x1=in(f);
            wire[q].y1=in(f);
            wire[q].x2=in(f);
            wire[q].y2=in(f);
        }
        f.close();
    #endif
    rock.resize(0);
    gun.resize(0);
    bullet.resize(0);
    hole.resize(0);
}
void save_settings(){
    string str=SAVE_DATA_DIR+"settings.ini";
    ofstream f(str.c_str());
    f<<SW<<' '<<SH<<' '<<fullscreen<<' '<<sb_on<<' '<<delay<<' '<<(int)profn<<' '<<loc<<' '<<(int)sound_vol<<' '<<(int)music_vol<<' '<<(int)darkost;
    f.close();
}
void load_settings(){
    string str=SAVE_DATA_DIR+"settings.ini";
    ifstream f(str.c_str());
    if(!f)return;
    int pr;
    int vs,vm;
    f>>SW>>SH>>fullscreen>>sb_on>>delay>>pr>>loc>>vs>>vm>>darkost;
    profn=pr;
    sound_vol=vs;
    music_vol=vm;
}
bool Profile::save(uint8_t n){
    string str=SAVE_DATA_DIR+"profiles/"+(char)(n+'0')+".pfl";
    ofstream file(str.c_str(),ios_base::binary);
    if(!file)return 1;
    file<<name<<';'<<(char)level<<(char)(allmoney/256)<<(char)(allmoney%256);
    for(int q=0;q<77;q++)file<<(char)money[q];
    for(int q=0;q<77;q++)file<<(char)time[q];
    for(int q=0;q<77;q++)file<<(char)die[q];
    for(int q=0;q<32;q++)file<<(char)ach[q];
    for(int q=0;q<32;q++)file<<(char)effects[q];
    for(int q=0;q<3;q++){
        file<<(char)tovars[q].effect;
        file<<(char)(tovars[q].price/256)<<(char)(tovars[q].price%256);
    }
    file<<(char)can_buy;
    file.close();
    return 0;
}
bool Profile::load(uint8_t n){
    string str=SAVE_DATA_DIR+"profiles/"+(char)(n+'0')+".pfl";
    ifstream file(str.c_str(),ios_base::binary);
    if(!file){
        clear(n);
        return 1;
    }
    getline(file,name,';');
    level=in(file);
    uint8_t a=in(file);
    uint8_t b=in(file);
    allmoney=(a*256+b);
    for(int q=0;q<77;q++)money[q]  =in(file);
    for(int q=0;q<77;q++)time[q]   =in(file);
    for(int q=0;q<77;q++)die[q]    =in(file);
    for(int q=0;q<32;q++)ach[q]    =in(file);
    for(int q=0;q<32;q++)effects[q]=in(file);
    for(int q=0;q<3;q++){
        tovars[q].effect=in(file);
        uint8_t a=in(file);
        uint8_t b=in(file);
        tovars[q].price=(a*256+b);
    }
    can_buy=in(file);
    file.close();
    return 0;
}
void load_strings(){
    string path=GLOBAL_DATA_DIR+"locales/"+loc+"/strings.txt";
    #ifdef ANDROID
        stringstream file=RWstream(path.c_str());
    #else
        ifstream file(path.c_str());
    #endif
    for(int q=0;q<STRINGS_COUNT;q++){
        getline(file,strings[q]);
        if(strings[q][strings[q].size()-1]=='\r')strings[q].pop_back();
    }
    #ifndef ANDROID
        file.close();
    #endif
}
