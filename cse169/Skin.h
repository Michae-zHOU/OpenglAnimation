//
//  Skin.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_SKIN_H
#define CSE169_SKIN_H

#include "vector3.h"
#include "Matrix34.h"
#include "DOF.h"
#include "token.h"
#include <vector>
#include "Triangle.h"
#include "Vertex.h"
#include "Skeleton.h"

using namespace std;

class Vertex;
class Triangle;

class Skin{
private:
    vector<Vertex> vertices;
    vector<Triangle> triangles;
    
public:
    bool exceptionHandler = false;
    
    vector<Matrix34> bindings;
    
    Skin(char *file);
    Skeleton* skeleton = NULL;
    
    void setSkeleton(Skeleton*);
    
    void Update();
    void Draw();
    
    bool Load(Tokenizer &token);
    bool LoadPositions(Tokenizer &token, int size);
    bool LoadNormals(Tokenizer &token, int size);
    bool LoadSkinWeight(Tokenizer &token, int size);
    bool LoadTriangles(Tokenizer &token, int size);
    bool LoadBindings(Tokenizer &token, int size);
    bool LoadMatrix(Tokenizer &token);
};

#endif