//
//  Skeleton.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_JOINT_H
#define CSE169_JOINT_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include <vector>
using namespace std;

class Joint{
public:
    Joint(){
        Offset = Vector3(0, 0, 0);
        boxmin = Vector3(-0.1,-0.1,-0.1);
        boxmax = Vector3(0.1,0.1,0.1);
        pose = Vector3(0, 0, 0);
    }
    bool Load(Tokenizer &token);
    void Update(Matrix34 &parent);
    void Draw();
    void addChild(Joint &child);
    void Reset(Matrix34);
    
    string name;
    
    vector<Joint*> children;
    Joint* parent;
    
    Matrix34 L;
    Matrix34 W;
    
    Matrix34 Bi;
    
    int index;
    DOF dofs[6];
    
    bool chosen = false;
    void drawBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax, float r, float g, float b);
private:
    Vector3 Offset;
    Vector3 boxmin;
    Vector3 boxmax;
    Vector3 pose;
    DOF old[6];
};

#endif