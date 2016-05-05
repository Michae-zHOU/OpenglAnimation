//
//  Triangle.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_TRIANGLE_H
#define CSE169_TRIANGLE_H

#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include <vector>


using namespace std;

class Triangle{
public:
    int v[3];
    
    Triangle(int, int, int);
};

#endif