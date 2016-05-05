//
//  AnimationPlayer.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef AnimationPlayer_H
#define AnimationPlayer_H

#include "Skeleton.h"
#include "Animation.h"
#include <stdio.h>

class AnimationPlayer {
private:
    float time_start;
    float time_end;
public:
    bool exceptionHandler = false;
    
    Animation* anim;
    Skeleton* skel;
    
    void LoadAnim(char* file);
    void LoadSkel(Skeleton* skel);
    
    void Play(float t);
    
    float getStart(){
        return time_start;
    }
    float getEnd(){
        return time_end;
    }
};


#endif /* AnimationPlayer_h */
