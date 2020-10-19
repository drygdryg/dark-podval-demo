
void Hole::show(){
    SDL_Rect r={anim*64,640,64,64};
    showIMG(x*64*zoom+cx,y*64*zoom+cy,64*zoom,64*zoom,anim_tex,r);
}
void Rock::show(){
    if(!tm&&type==0){
        SDL_Rect r={256,64,64,64};
        showIMG(x*zoom+cx,y*zoom+cy,64*zoom,64*zoom,textures,r);
    }
    else if(type>1){
        SDL_Rect r={((type-2)%9-1)*64,704+(type-2)/9*64,64,64};
        if(r.x<0)r.x=0;
        showIMG(x*zoom+cx,y*zoom+cy,64*zoom,64*zoom,anim_tex,r);
    }
    else{
        int alpha=max(min(255-(int)(timer-tm-2800)/10,255),0);
        SDL_SetTextureAlphaMod(anim_tex,alpha);
        if(alpha<50)type=1;
        int a=min((int)(timer-tm)/100,23);
        SDL_Rect r={a%8*64,a/8*64+320,64,64};
        showIMG(x*zoom+cx,y*zoom+cy,64*zoom,64*zoom,anim_tex,r);
    }
    SDL_SetTextureAlphaMod(anim_tex,255);
}
void Subject::show(){
    if(dietimer==0||(dietimer<0&&(timer+dietimer)%200/100)){
        int c=0;
        switch(n){
            case 0:c=(short)y%64/16;break;
            case 1:c=(short)x%64/16;break;
            case 2:c=(short)y%64/16;break;
            case 3:c=(short)x%64/16;break;
        }
        if(bring&&n%2==1){
            SDL_Rect r;
            if(bring==1)r={408,598,20,20};
            if(bring==2)r={342,651,20,35};
            int xp,yp;
            if(n==1&&bring==1)xp=44,yp=27;
            if(n==3&&bring==1)xp=0,yp=27;
            if(n==1&&bring==2)xp=44,yp=14;
            if(n==3&&bring==2)xp=0,yp=14;
            showIMG((x+xp)*zoom+cx,(y+yp)*zoom+cy,r.w*zoom,r.h*zoom,textures,r);
        }
        SDL_Rect r={c*64+256*(push||bring),n*64,64,64};
        if(push&&n%2==0)move(5,n);
        if(push&&n%2==1)move(15,n);
        showIMG(x*zoom+cx,y*zoom+cy,64*zoom,64*zoom,texture,r);
        if(push&&n%2==0)move(-5,n);
        if(push&&n%2==1)move(-15,n);
        if(bring&&n==2){
            SDL_Rect r;
            if(bring==1)r={408,598,20,20};
            if(bring==2)r={342,651,20,36};
            int xp,yp;
            if(bring==1){xp=23;yp=35;}
            if(bring==2){xp=23;yp=22;}
            showIMG((x+xp)*zoom+cx,(y+yp)*zoom+cy,r.w*zoom,r.h*zoom,textures,r);
        }
    }
    else if(dietimer>0){
        int gg_anim=min((int)(timer-dietimer)/50,19)+n*20;
        SDL_Rect r={(gg_anim%8)*64,(gg_anim/8)*64+256,64,64};
        showIMG(x*zoom+cx,y*zoom+cy,64*zoom,64*zoom,texture,r);
    }
}
void Gun::show(){
    SDL_Rect r;
    if(type>=0&&type<4)r={64*((type*6+anim)%8),64*((type*6+anim)/8),64,64};
    else if(type>=4&&type<8)r={64*((type*4+anim)%8),(64*((type*4+anim)/8)+64),64,64};
    showIMG(x*64*zoom+cx,y*64*zoom+cy,64*zoom,64*zoom,anim_tex,r);
}
void Bullet::show(){
    if(type==0){
        SDL_SetRenderDrawColor(ren,255,128,39,255);
        if(n==0||n==2)SDL_RenderDrawLine(ren,(x+32)*zoom+cx,(y+11)*zoom+cy,(x+32)*zoom+cx,(y+55)*zoom+cy);
        else SDL_RenderDrawLine(ren,(x+11)*zoom+cx,(y+32)*zoom+cy,(x+55)*zoom+cx,(y+32)*zoom+cy);
    }
    else if(type==1){
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_Rect r;
        if(n==0||n==2)r={int((x+31)*zoom+cx),int((y+28)*zoom+cy),int(3*zoom),int(8*zoom)};
        else r={int((x+28)*zoom+cx),int((y+31)*zoom+cy),int(8*zoom),int(3*zoom)};
        SDL_RenderFillRect(ren,&r);
    }
    else if(type==2){
        SDL_Rect r={408,598,20,20};
        showIMG((x+22)*zoom+cx,(y+22)*zoom+cy,20*zoom,20*zoom,textures,r);
    }
}
void draw(bool gg_sh,bool wires,bool hints){
    for(int y=-cy/(64*zoom)-1;y<(-cy+SH)/(64*zoom)+2;y++){
        for(int x=-cx/(64*zoom)-1;x<(-cx+SW)/(64*zoom)+2;x++){
            if(x>=0&&y>=0&&x<w&&y<h)
                showIMG(cx+x*(zoom*64),cy+y*(zoom*64),(zoom*64),(zoom*64),flr);
        }
    }
    for(int y=-cy/(64*zoom)-1;y<(-cy+SH)/(64*zoom)+2;y++){
        for(int x=-cx/(64*zoom)-1;x<(-cx+SW)/(64*zoom)+2;x++){
            if(x>=0&&y>=0&&x<w&&y<h&&level[x][y]){
                SDL_Rect r={64*((level[x][y]-1)%8), 64*((level[x][y]-1)/8), 64, 64};
                if(level[x][y]==10&&hints)SDL_SetTextureAlphaMod(textures,150);
                showIMG(cx+x*(zoom*64),cy+y*(zoom*64),(zoom*64),(zoom*64),textures,r);
                if(level[x][y]==10&&hints)SDL_SetTextureAlphaMod(textures,255);
            }
        }
    }
    for(int q=0;q<(int)hole.size();q++)hole[q].show();
    for(int q=0;q<(int)gun.size();q++)gun[q].show();
    for(int q=0;q<(int)bullet.size();q++)bullet[q].show();
    if(gg_sh)gg.show();
    for(int q=0;q<(int)rock.size();q++)rock[q].show();
    if(darkost){
        SDL_Surface *src=create_surface(4,4);
        SDL_Rect r={1,1,2,2};
        SDL_FillRect(src,&r,0xFF000000);
        SDL_Texture *tex=SDL_CreateTextureFromSurface(ren,src);
        SDL_FreeSurface(src);
        for(int y=-cy/(64*zoom)-3;y<(-cy+SH)/(64*zoom)+6;y++){
            for(int x=-cx/(64*zoom)-3;x<(-cx+SW)/(64*zoom)+6;x++){
                if(!light[x][y]||x<0||y<0||x>=w||y>=h)
                    showIMG(cx+zoom*64*(x-1.25),cy+zoom*64*(y-1.25),zoom*224,zoom*224,tex);
            }
        }
        SDL_DestroyTexture(tex);
    }else{
        SDL_Surface *src=create_surface(w,h);
        for(int y=-cy/(64*zoom)-1;y<(-cy+SH)/(64*zoom)+2;y++){
            for(int x=-cx/(64*zoom)-1;x<(-cx+SW)/(64*zoom)+2;x++){
                if(light[x][y]!=255&&x>=0&&y>=0&&x<w&&y<h){
                    SDL_Color k={0,0,0,255};
                    SDL_Rect r={x,y,1,1};
                    SDL_FillRect(src,&r,SDL_MapRGB(src->format,k.r,k.g,k.b));
                }
            }
        }
        SDL_Texture *tex=SDL_CreateTextureFromSurface(ren,src);
        showIMG(cx,cy,w*zoom*64,h*zoom*64,tex);
        SDL_DestroyTexture(tex);
    }
}
#ifdef ANDROID
    void sensor_draw(){
        sensor=0;
        const SDL_Rect b[8]={
            {50, 50,    100,100},
            {150,SH-250,100,100},
            {250,SH-150,100,100},
            {150,SH-150,100,100},
            {50, SH-150,100,100},
            {SW-150,SH-150,100,100},
            {SW-150,SH-250,100,100},
            {SW-150,SH-350,100,100}
        };
        for(int q=0;q<8;q++){
            SDL_Rect r={q%4*50,q/4*50,50,50};
            showIMG(b[q],sensor_tex,r);
            if((e.type==SDL_FINGERDOWN||e.type==SDL_FINGERMOTION)&&in_rect(b[q]))sensor=q+1;
        }
    }
#endif
