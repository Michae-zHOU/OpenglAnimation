//
//  Skeleton.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_DOF_H
#define CSE169_DOF_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"

class DOF{
public:
    DOF(){
        Min = -100000;
        Max = 100000;
    }
    void SetValue(float v);
    float getValue();
    void SetMinMax(float min, float max);
    

    float Value;
    float Min;
    float Max;
private:
};

#endif