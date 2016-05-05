//
//  Skin.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_KEYFRAME_H
#define CSE169_KEYFRAME_H

#include "vector3.h"
#include "Matrix34.h"
#include "DOF.h"
#include "token.h"
#include <vector>

using namespace std;

class Keyframe{
private:
    
public:
    enum tangent_Mode{
        flat, //(tangent = 0)
        linear, //(tangent points to next/last key)
        smooth, //(automatically adjust tangent for smooth results)
        fixed //(user can arbitrarily specify a value)
    };
    
    float Time;
    float Value;
    float span;
    float TangentIn,TangentOut;
    tangent_Mode RuleIn,RuleOut; // Tangent rules
    float A,B,C,D; // Cubic coefficients
};

#endif