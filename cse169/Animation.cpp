//
//  Animation.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "Animation.h"

Animation::Animation(char* file){
    if(!Load(file))
        exceptionHandler = true;
}

bool Animation::Load(char* file){
    Tokenizer token;
    if(!token.Open(file)){
        return false;
    }
    while(1) {
        char temp[256];
        token.GetToken(temp);
        
        if (strcmp(temp, "range") == 0) {
            time_start = token.GetFloat();
            time_end = token.GetFloat();
        }
        else if (strcmp(temp, "numchannels") == 0) {
            numofchannel = token.GetInt();
    
            clist = new Channel[numofchannel];
            for(int i=0; i<numofchannel; i++){
                token.FindToken("channel");
                token.GetToken(temp);
                if (strcmp(temp, "{") == 0) {
                   // clist[i].time_start = time_start;
                   // clist[i].time_end = time_end;
                    clist[i].Load(&token);
                }
            }
        }
        else if(strcmp(temp, "}") == 0){
            token.Close();
            return true;
        }
        else
            token.SkipLine();
    }
}

float Animation::Evaluate(float time, int channelNum){
    return clist[channelNum].Evaluate(time);
}