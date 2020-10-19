int Subject::bar(bool fat,uint8_t n){
    uint8_t ar[]={1,2,3,4,5,6,7,28,40,41,42,43,44,45,46,51,
                (uint8_t)(fat?8:1),
                (uint8_t)(fat?9:1),
                (uint8_t)(fat?10:1),
                (uint8_t)(fat?29:1),
                54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,
                (uint8_t)(fat?79:1),84,87};
    uint8_t level2[255][255];
    memcpy(level2, level, sizeof(level2));
    if(!fat){
        for(unsigned q=0;q<hole.size();q++){
            int xp=hole[q].x,yp=hole[q].y;
            if(hole[q].anim<6&&
               p(xp*64,yp*64,gg.x,gg.y)){
                level2[xp-1][yp]=1;
                level2[xp+1][yp]=1;
                level2[xp][yp-1]=1;
                level2[xp][yp+1]=1;
            }
        }
    }
    for(unsigned q=0;q<gun.size();q++){
        level2[gun[q].x][gun[q].y]=1;
    }
    for(unsigned q=0;q<sizeof(ar);q++){
        if(level2
            [(short)x/64-(n==3)+(n==1)]
            [(short)y/64-(n==0)+(n==2)]==ar[q])return 1;
    }
    for(unsigned q=0;q<rock.size();q++){
        if(rock[q].x/64==x/64-(n==3)+(n==1)
         &&rock[q].y/64==y/64-(n==0)+(n==2)&&rock[q].type!=1)return 2;
    }
    return 0;
}
uint8_t Subject::block(){
    uint8_t block=0;
        switch(n){
            case 0: block=level[(short)x/64][((short)y-1)/64]; break;
            case 1: block=level[(short)x/64+1][(short)y/64]; break;
            case 2: block=level[(short)x/64][(short)y/64+1]; break;
            case 3: block=level[((short)x-1)/64][(short)y/64]; break;
        }
    return block;
}
void Subject::replace(uint8_t a){
    switch(n){
        case 0: level[(short)x/64][((short)y-1)/64]=a; break;
        case 1: level[(short)x/64+1][(short)y/64]=a; break;
        case 2: level[(short)x/64][(short)y/64+1]=a; break;
        case 3: level[((short)x-1)/64][(short)y/64]=a; break;
    }
}
void Subject::move(int8_t c,uint8_t n){
    switch(n){
        case 0:gg.y-=c;break;
        case 1:gg.x+=c;break;
        case 2:gg.y+=c;break;
        case 3:gg.x-=c;break;
    }
}
bool Subject::can_go(bool *ps,uint8_t n){
    uint8_t i=4;
    bool ok=0;
    for(int q=0;q<(int)rock.size();q++){
        switch(n){
            case 0:if(y-rock[q].y<=80&&y-rock[q].y>0&&rock[q].x==x&&rock[q].type!=1)i=0;break;
            case 1:if(rock[q].x-x<=80&&rock[q].x-x>0&&rock[q].y==y&&rock[q].type!=1)i=1;break;
            case 2:if(rock[q].y-y<=80&&rock[q].y-y>0&&rock[q].x==x&&rock[q].type!=1)i=2;break;
            case 3:if(x-rock[q].x<=80&&x-rock[q].x>0&&rock[q].y==y&&rock[q].type!=1)i=3;break;
        }
    }
    if(noclip||bar(0,n)!=1){
        *ps=0;
        if(i==n){
            *ps=1;
            move(64,n);
            if(noclip||!(bar(1,n)))ok=1;
            else if(bar(1,n)==2&&profile.effects[0]){
                move(64,n);
                if(noclip||!(bar(1,n)))ok=1;
                move(-64,n);
            }
            move(-64,n);
        }else ok=1;
    }
    return ok;
}
void Subject::kill(){
    if(!infhealth&&!dietimer&&lives){
        lives--;
        if(!lives){
            dietimer=timer;
            profile.die[lidtoln(lid)]++;
            if(!bar(1,n)){
                switch(n){
                    case 1:x+=12;break;
                    case 3:x-=12;break;
                }
            }
        }else dietimer=(-(int)(timer))-2000;
    }
}
void Subject::clear(){
    if(profile.effects[1]){
        for(int xp=0;xp<w;xp++){
            for(int yp=0;yp<h;yp++){
                light[xp][yp]=0;
            }
        }
    }
    for(float a=0;a<6.2831853072;a+=0.005){
        bool ok=1;
        float xp=x,yp=y;
        while(ok){
            xp+=cos(a)*20;
            yp+=sin(a)*20;
            uint8_t b[]={1,2,3,4,5,6,7,8,10,28,40,41,42,43,44,45,46,84};
            for(unsigned q=0;q<sizeof(b);q++){
                if(level[(int)(xp+32)/64][(int)(yp+32)/64]==b[q])ok=0;
            }
            float d=sqrt(pow(x-xp,2)+pow(y-yp,2))/64.0f;
            if(d>8)ok=0;
            else{
                light[(int)(xp+32)/64][(int)(yp+32)/64]=255;
            }
        }
    }
    if(profile.effects[2]){
        for(int xp=0;xp<w;xp++){
            for(int yp=0;yp<h;yp++){
                if(light[xp][yp]&&sqrt(pow((x+32)/64-xp,2)+pow((y+32)/64-yp,2))>2.5)light[xp][yp]=0;
            }
        }
    }
}
