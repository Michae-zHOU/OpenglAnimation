//
//  Skeleton.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//

#include "DOF.h"

void DOF::SetValue(float v){
    if (v < Min)
        Value = Min;
    else if (v > Max)
        Value = Max;
    else
        Value = v;
}

float DOF::getValue(){
    return Value;
}

void DOF::SetMinMax(float min, float max){
    Min = min;
    Max = max;
    
    if (Value < Min) {
        Value = Min;
    }
    if (Value > Max) {
        Value = Max;
    }
}