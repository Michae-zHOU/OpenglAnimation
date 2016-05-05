//
//  Animation.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef Animation_H
#define Animation_H

#include <stdio.h>
#include "Channel.h"

class Animation {

public:
    bool exceptionHandler = false;
    
    Animation(char* file);
    
    float time_start, time_end;
    Channel* clist;
    int numofchannel;
    
    bool Load(char* file);
    
    float Evaluate(float time, int channelNum);
};


#endif /* Animation_h */
