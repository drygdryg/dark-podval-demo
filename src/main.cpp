#include "include.h"
int main(int argv, char** argc){
    srand(time(0));
    load_settings();
    save_settings();
    load_strings();
    profile.load(profn);
    profile.save(profn);
    init("Dark Podval");
    if(darkost)SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    save_settings();
    SDL_Texture *image=load(string(GLOBAL_DATA_DIR+"textures/menu.png").c_str());
    #ifdef ANDROID
        if(SW>1200&&SH>900)
            SDL_RenderSetLogicalSize(ren,SW/=1.5f,SH/=1.5f);
        else if(SW>1400&&SH>1050)
            SDL_RenderSetLogicalSize(ren,SW/=1.75f,SH/=1.75f);
        else if(SW>1600&&SH>1200)
            SDL_RenderSetLogicalSize(ren,SW/=2,SH/=2);
        else if(SW>2400&&SH>1800)
            SDL_RenderSetLogicalSize(ren,SW/=3,SH/=3);
    #else if defined(linux_install)
        system(string("mkdir "+SAVE_DATA_DIR).c_str());
    #endif
    while(1){
        SDL_Rect r={SW/3*2,SH/2+FC_GetHeight(text,"\n\n\n\n"),SW/3,25};
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderClear(ren);
        showIMG(0,0,SW,SH,image);
    #ifdef ANDROID
        for(int q=0;q<3;q++){
    #else
        for(int q=0;q<4;q++){
    #endif
            SDL_Rect r={SW/32,SH/4+q*48,400,48};
            SDL_Color c=in_rect(r)?gray:white;

            switch(q){
                case 0:FC_DrawBoxColor(menu,ren,r,c,strings[69].c_str());break;
                case 1:FC_DrawBoxColor(menu,ren,r,c,strings[0].c_str());break;
                case 2:FC_DrawBoxColor(menu,ren,r,c,strings[5].c_str());break;
                case 3:FC_DrawBoxColor(menu,ren,r,c,strings[6].c_str());break;
            }
        }
        FC_DrawColumn(menu,ren,SW/3*2,SH/2,SW/3,string(strings[7]+"\n\n"+profile.name+"\n").c_str());
        FC_DrawBoxColor(text,ren,r,in_rect(r)?gray:white,strings[8].c_str());
        SDL_RenderPresent(ren);
        SDL_WaitEvent(&e);
        mouse.update();
        if(e.type==SDL_QUIT)quit(1);
        else if(e.type==SDL_MOUSEBUTTONDOWN){
        #ifdef ANDROID
            for(int q=0;q<3;q++){
        #else
            for(int q=0;q<4;q++){
        #endif
                SDL_Rect r={SW/32,SH/4+q*48,400,48};
                if(in_rect(r)){
                    SDL_DestroyTexture(image);
                    switch(q){
                        case 0:{
                            bool ok=1;
                            if(profile.level)ok=alert();
                            if(ok){
                                intro();
                                profile.new_name();
                                profile.save(profn);
                                lid=lntolid(profile.level);
                                string path=string(GLOBAL_DATA_DIR+"levels/standart/"+lntolid(profile.level)+".lvl");
                                open(path);
                                st=1;
                                game();
                            }
                        }break;
                        case 1:{
                            if(profile.level){
                                lid=lntolid(profile.level);
                                string path=string(GLOBAL_DATA_DIR+"levels/standart/"+lntolid(profile.level)+".lvl");
                                open(path);
                                st=1;
                                game();
                            }
                        }break;
                        case 2:settings();break;
                        case 3:quit(1);break;
                    }
                    mouse.wait();
                    image=load(string(GLOBAL_DATA_DIR+"textures/menu.png").c_str());
                }
            }
            if(in_rect(r)){
                profiles();
                SDL_RenderSetViewport(ren,0);
            }
        }
    }
}
