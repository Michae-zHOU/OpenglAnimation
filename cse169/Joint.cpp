//
//  Skeleton.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//

#include "Joint.h"
#include <iostream>
#include "Math.h"

using namespace std;

void Joint::Update(Matrix34 &parent){
    
    float x = dofs[3].getValue(), y = dofs[4].getValue(), z = dofs[5].getValue();
    float cx =cos(x), cy = cos(y), cz = cos(z),  sy = sin(y), sx = sin(x), sz = sin(z);
    L = Matrix34(cy*cz, sx*sy*cz-cx*sz, cx*sy*cz+sx*sz, Offset.x+dofs[0].getValue(),
                 cy*sz, sx*sy*sz+cx*cz, cx*sy*sz-sx*cz, Offset.y+dofs[1].getValue(),
                 -sy, sx*cy, cx*cy,  Offset.z+dofs[2].getValue());
    
    
    W.Dot(parent,L);
    for (int i=0; i<children.size(); i++) {
        children.at(i)->Update(W);
    }
}

void Joint::Reset(Matrix34 p){
    dofs[0].SetValue(0);
    dofs[1].SetValue(0);
    dofs[2].SetValue(0);
    
    dofs[3].SetValue(pose.x);
    dofs[4].SetValue(pose.y);
    dofs[5].SetValue(pose.z);
    
    float x = dofs[3].getValue(), y = dofs[4].getValue(), z = dofs[5].getValue();
    float cx =cos(x), cy = cos(y), cz = cos(z),  sy = sin(y), sx = sin(x), sz = sin(z);
    L = Matrix34(cy*cz, sx*sy*cz-cx*sz, cx*sy*cz+sx*sz, Offset.x+dofs[0].getValue(),
                 cy*sz, sx*sy*sz+cx*cz, cx*sy*sz-sx*cz, Offset.y+dofs[1].getValue(),
                 -sy, sx*cy, cx*cy,  Offset.z+dofs[2].getValue());
    
    W.Dot(p, L);
    
    for (int i=0; i<children.size(); i++) {
        children.at(i)->Reset(W);
    }
}

bool Joint::Load(Tokenizer &token){
    token.FindToken("{");
    while (1) {
        char temp[256];
        token.GetToken(temp);
        
        if (strcmp(temp, "offset") == 0) {
            Offset.x = token.GetFloat();
            Offset.y = token.GetFloat();
            Offset.z = token.GetFloat();
        }
        else if (strcmp(temp, "boxmin") == 0) {
            boxmin.x = token.GetFloat();
            boxmin.y = token.GetFloat();
            boxmin.z = token.GetFloat();
        }
        else if (strcmp(temp, "boxmax") == 0) {
            boxmax.x = token.GetFloat();
            boxmax.y = token.GetFloat();
            boxmax.z = token.GetFloat();
        }
        else if (strcmp(temp, "pose") == 0){
            pose.x = token.GetFloat();
            pose.y = token.GetFloat();
            pose.z = token.GetFloat();
        }
        else if (strcmp(temp, "rotxlimit") == 0){
            float min = token.GetFloat();
            float max = token.GetFloat();
            dofs[3].SetMinMax(min, max);
        }
        else if (strcmp(temp, "rotylimit") == 0){
            float min = token.GetFloat();
            float max = token.GetFloat();
            dofs[4].SetMinMax(min, max);
        }
        else if (strcmp(temp, "rotzlimit") == 0){
            float min = token.GetFloat();
            float max = token.GetFloat();
            dofs[5].SetMinMax(min, max);
        }
        else if(strcmp(temp, "balljoint")==0){
            Joint* jnt = new Joint;
            token.GetToken(temp);
            jnt->name = temp;
            jnt->Load(token);
            Joint::addChild(*jnt);
        }
        else if(strcmp(temp, "}")==0){
            dofs[0].SetValue(0);
            dofs[1].SetValue(0);
            dofs[2].SetValue(0);
            
            dofs[3].SetValue(pose.x);
            dofs[4].SetValue(pose.y);
            dofs[5].SetValue(pose.z);
            return true;
        }
        else
            token.SkipLine();
    }
}

void Joint::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(W);
    
    if (chosen) {
        drawBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z, 1.0f, 0.0f, 0.0f);
        glColor4f(1.f, 1.f, 1.f, 1.f);
    }
    else{
        drawWireBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
    }
    

    for (int i = 0; i<children.size(); i++) {
        children.at(i)->Draw();
    }
}

void Joint::addChild(Joint &child){
    children.push_back(&child);
    child.parent = this;
}

void Joint::drawBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax, float r, float g, float b) {
    glPushMatrix();
        glTranslatef(0.5f*(xmin+xmax),0.5f*(ymin+ymax),0.5f*(zmin+zmax));
        glScalef(xmax-xmin,ymax-ymin,zmax-zmin);
        glColor4f(r,g,b,1.0f);
        glutWireCube(1.0f);
    glPopMatrix();
}