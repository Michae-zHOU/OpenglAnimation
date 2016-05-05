//
//  Skin.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_VERTEX_H
#define CSE169_VERTEX_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include <vector>
#include "Skin.h"
using namespace std;

class Skin;

class Vertex{
private:
    
public:
    Vector3 position;
    Vector3 normal;
    
    Vector3 np;
    Vector3 nn;
    
    vector< pair<int,float> > skinweight;
    
    //Constructor
    Vertex(Vector3);
    Vertex(float, float, float);
    
    void setPosition(Vector3);
    void setPosition(float,float,float);
    
    void setNormal(Vector3);
    void setNormal(float,float,float);
    
    void update(Skin*);
    void draw();
};

#endif