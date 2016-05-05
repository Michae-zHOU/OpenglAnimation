//
//  Skin.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//

#include "Vertex.h"
#include <iostream>
#include "Math.h"
using namespace std;

Vertex::Vertex(Vector3 p){
    position.x = p.x;
    position.y = p.y;
    position.z = p.z;
}

Vertex::Vertex(float x, float y, float z){
    position.x = x;
    position.y = y;
    position.z = z;
}

void Vertex::setPosition(Vector3 p){
    position = p;
}

void Vertex::setPosition(float x, float y, float z){
    position.x = x;
    position.y = y;
    position.z = z;
}

void Vertex::setNormal(Vector3 n){
    normal = n;
}

void Vertex::setNormal(float x, float y, float z){
    normal.x = x;
    normal.y = y;
    normal.z = z;
}

void Vertex::update(Skin* s){
    Vector3 v;
    Vector3 n;
    
    if(s->skeleton == NULL){
        np = position;
        nn = normal;
        return;
    }

    for (int i = 0; i<skinweight.size(); i++) {
        Skeleton* skel = s->skeleton;
        
        float wi = skinweight.at(i).second;
        
        Matrix34 Mj = skel->GetMatrix(skinweight.at(i).first);
        
        Vector3 temp, tempN;
        
        Mj.Transform(position, temp);
        Mj.Transform3x3(normal, tempN);
        
        v += temp*wi;
        n += tempN*wi;
    }
    
    np = v;
    nn = n.Normalize();
}

void Vertex::draw(){
    glNormal3f(nn.x, nn.y, nn.z);
    glVertex3f(np.x, np.y, np.z);
}