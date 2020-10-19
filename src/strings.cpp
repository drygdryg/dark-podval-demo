uint8_t lidtoln(string lid){
    uint8_t ln=0;
    ln+=(lid[0]-'0')*110;
    ln+=(lid[1]-'0'-1)*11;
    if(lid[3]=='v')ln+=10;
    else{
        ln+=(lid[3]-'0')*10;
        ln+=(lid[4]-'0'-1);
    }
    return ln;
}

string lntolid(uint8_t ln){
    string lid;
    int ch=ln/11+1;
    int lvl=ln%11+1;
    lid+=ch/10+'0';lid+=ch%10+'0';lid+='-';
    if(lvl==11)lid+='v';
    else{lid+=lvl/10+'0';lid+=lvl%10+'0';}
    return lid;
}

string postlid(string lid){
    if(lid[3]=='v'){
        lid[3]='0';
        lid+='1';
        if(lid[1]<'9')lid[1]++;
        else{lid[1]='0';lid[0]++;}
    }
    else if(lid[4]<'9'&&lid[4]>'0')lid[4]++;
    else if(lid[4]=='9'){lid[4]='0';lid[3]++;}
    else if(lid[4]=='0'){lid.pop_back();lid[3]='v';}
    return lid;
}
