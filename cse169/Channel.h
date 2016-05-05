//
//  Channel.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef Channel_H
#define Channel_H

#include "Keyframe.h"
#include <stdio.h>

class Channel {

public:
    enum Extrapolation_mode
    {
        constant,
        linear,
        cycle,
        cycle_offset,
        bounce
    };
    
    Extrapolation_mode modebefore;
    Extrapolation_mode modeafter;
    
    Keyframe* KeyFramelist;
    float Evaluate(float time);
    bool Load(Tokenizer*);
    void SetTangent();
    void Precompute();
    int keysize;
    
    float time_start, time_end;
};


#endif /* Channel_h */
