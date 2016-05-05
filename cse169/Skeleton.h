//
//  Skeleton.h
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_SKELETON_H
#define CSE169_SKELETON_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"
#include "Joint.h"
#include "token.h"

class Joint;

class Skeleton{
public:
    bool exceptionHandler = false;
    
    Skeleton(char *file);
    
    bool Load(char *file);
    void Update();
    void Draw();
    void Reset();

    Matrix34 GetMatrix(int);
    vector<Joint*> jointVec;
private:
    Joint* Root;
    
    void setJointIndex();
};

#endif