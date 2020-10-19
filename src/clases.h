struct Wire{
    Wire(){};
    Wire(uint8_t xp1,uint8_t yp1,uint8_t xp2,uint8_t yp2,bool ap=0){
        x1=xp1;
        y1=yp1;
        x2=xp2;
        y2=yp2;
        active=ap;
    }
    void show();
    uint8_t x1,y1,x2,y2;
    bool active=0;
};
std::vector<Wire> wire;
struct Gate{
    Gate(){};
    Gate(uint8_t xp,uint8_t yp,uint8_t tp,int vp=0,bool ap=0){
        x=xp;
        y=yp;
        type=tp;
        value=vp;
        active=ap;
    }
    void show();
    uint8_t x,y,type;
    int value=0;
    bool active=0;
};
std::vector<Gate> gate;
struct Subject{
    unsigned short x,y;
    uint8_t n;
    unsigned short money;
    void show();
    int bar(bool,uint8_t);
    int dietimer=0,timerr=0;
    uint8_t block();
    void move(int8_t,uint8_t);
    void replace(uint8_t);
    void replace_back(uint8_t,uint8_t);
    bool can_go(bool*,uint8_t);
    void kill();
    void clear();
    SDL_Texture* texture;
    uint8_t key[12];
    bool push=0;
    uint8_t speed=40;
    bool infhealth=0;
    bool noclip=0;
    uint8_t bring=0;
    uint8_t lives=1;
    uint8_t sx,sy;
}gg;
struct Bullet{
    unsigned short x,y;
    uint8_t n,type;
    void show();
    bool p();
    void move();
};
std::vector<Bullet> bullet;
struct Rock{
    unsigned short x,y;
    void show();
    int tm;
    uint8_t type;
};
std::vector<Rock> rock;
struct Gun{
    uint8_t x,y,type,anim;
    void show();
};
std::vector<Gun>gun;
struct Tovar{
    uint8_t effect;
    uint16_t price;
};
struct Profile{
    bool load(uint8_t n);
    bool save(uint8_t n);
    void clear(uint8_t n);
    void new_name();
    void random();
    string name;
    unsigned short money[77];
    unsigned short time [77];
    unsigned short die  [77];
    Tovar tovars[3];
    bool can_buy=1;
    bool ach[32];
    bool effects[32];
    //0-    супер сила
    //1-  беспамятство
    //2-  близорукость
    //3-     миникарта
    //4-супер скорость
    uint8_t level=0;
    unsigned short allmoney=0;
}profile;
struct Mouse{
    bool state=0;
    int x,y;
    int b;
    bool update();
    void wait();
} mouse;
struct Hole{
    void show();
    uint8_t x,y,anim;
};
std::vector<Hole> hole;
