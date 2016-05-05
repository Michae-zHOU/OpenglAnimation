//
//  Skin.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//

#include "Skin.h"
#include <iostream>
#include "Math.h"
using namespace std;

Skin::Skin(char* file){
    Tokenizer token;
    if(!token.Open(file)){
        exceptionHandler = true;
        return;
    }
    if(Skin::Load(token))
        cout<<"Successfully Load Skin."<<endl;
    else{
        exceptionHandler = true;
        cout<<"Fail to Load Skin."<<endl;
    }
    
    token.Close();
}

void Skin::setSkeleton(Skeleton* s){
    skeleton = s;

    for(int i = 0; i < bindings.size(); i++){
        bindings.at(i).Inverse();
        s->jointVec.at(i)->Bi = bindings.at(i);
    }
}

void Skin::Update(){
    for (int i=0; i<vertices.size(); i++) {
        vertices.at(i).update(this);
    }
}

void Skin::Draw(){
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    
    glBegin(GL_TRIANGLES);
    
    for (int i=0; i<triangles.size(); i++) {
        Triangle t = triangles.at(i);
        
        vertices.at(t.v[0]).draw();
        vertices.at(t.v[1]).draw();
        vertices.at(t.v[2]).draw();
    }
    
    glPopMatrix();
    
    glEnd();
}

bool Skin::Load(Tokenizer &token){
    while(1) {
        char temp[256];
        token.GetToken(temp);
        
        if (strcmp(temp, "positions") == 0) {
            int size = token.GetInt();
            
            if(!LoadPositions(token,size))
                return false;
        }
        else if (strcmp(temp, "normals") == 0){
            int size = token.GetInt();
            if(!LoadNormals(token, size))
                return false;
        }
        else if (strcmp(temp, "skinweights") == 0){
            int size = token.GetInt();
            
            if(!LoadSkinWeight(token, size))
                return false;
        }
        else if (strcmp(temp, "triangles") == 0){
            int size = token.GetInt();
            
            if(!LoadTriangles(token, size))
                return false;
        }
        else if (strcmp(temp, "bindings") == 0){
            int size = token.GetInt();
            
            if(!LoadBindings(token, size))
                return false;
        }
        else if(strcmp(temp, "}") == 0){
            return true;
        }
        else
            token.SkipLine();
    }
}

bool Skin::LoadPositions(Tokenizer &token, int size){
    token.FindToken("{");
    
    for(int i=0; i < size; i++){
        Vector3 position;
        position.x = token.GetFloat();
        position.y = token.GetFloat();
        position.z = token.GetFloat();
        
        Vertex v(position);
        vertices.push_back(v);
    }
    
    char temp[256];
    token.GetToken(temp);
    
    if(strcmp(temp, "}")==0)
        return true;
    else
        return false;
}

bool Skin::LoadNormals(Tokenizer &token, int size){
    token.FindToken("{");
    
    for(int i = 0; i < size; i++){
        Vector3 normal;
        normal.x = token.GetFloat();
        normal.y = token.GetFloat();
        normal.z = token.GetFloat();
        
        vertices.at(i).setNormal(normal);
    }
    
    char temp[256];
    token.GetToken(temp);
    
    if(strcmp(temp, "}")==0)
        return true;
    else
        return false;
}

bool Skin::LoadSkinWeight(Tokenizer &token, int size){
    token.FindToken("{");
    
    for(int i=0; i < size; i++){
        int numattachments = token.GetInt();
        
        for (int j=0; j<numattachments; j++){
            int jointIndex = token.GetInt();
            float weight = token.GetFloat();
            
            vertices.at(i).skinweight.push_back(make_pair(jointIndex, weight));
        }
    }
    
    char temp[256];
    token.GetToken(temp);
    
    if(strcmp(temp, "}")==0)
        return true;
    else
        return false;
}

bool Skin::LoadTriangles(Tokenizer &token, int size){
    token.FindToken("{");
    
    for(int i=0; i < size; i++){
        int v1 = token.GetInt();
        int v2 = token.GetInt();
        int v3 = token.GetInt();
        
        Triangle t(v1, v2, v3);
        
        triangles.push_back(t);
    }
    
    char temp[256];
    token.GetToken(temp);
    
    if(strcmp(temp, "}")==0)
        return true;
    else
        return false;
}

bool Skin::LoadBindings(Tokenizer &token, int size){
    token.FindToken("{");
    
    for(int i = 0; i < size; i++){
        char temp[256];
        token.GetToken(temp);
        if (strcmp(temp, "matrix") == 0){
            if(!LoadMatrix(token))
                return false;
        }
        else
            return false;
    }
    return true;
}

bool Skin::LoadMatrix(Tokenizer &token){
    token.FindToken("{");
    Vector3 a,b,c,d;
    a.x = token.GetFloat();
    a.y = token.GetFloat();
    a.z = token.GetFloat();
        
    b.x = token.GetFloat();
    b.y = token.GetFloat();
    b.z = token.GetFloat();
        
    c.x = token.GetFloat();
    c.y = token.GetFloat();
    c.z = token.GetFloat();
    
    d.x = token.GetFloat();
    d.y = token.GetFloat();
    d.z = token.GetFloat();
        
    Matrix34 m;
    m.a = a;
    m.b = b;
    m.c = c;
    m.d = d;
        
    bindings.push_back(m);
    
    char temp[256];
    token.GetToken(temp);
    
    if(strcmp(temp, "}")==0)
        return true;
    else
        return false;
}

