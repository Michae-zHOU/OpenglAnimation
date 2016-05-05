//
//  Skeleton.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#include <stack>
#include "Skeleton.h"

Skeleton::Skeleton(char *file){
    if(!Load(file))
        exceptionHandler = true;
}

void Skeleton::Update(){
    Root->W.Identity();
    Root->Update(Root->W);
}

void Skeleton::Draw(){
    Root->Draw();
}

void Skeleton::Reset(){
    Root->W.Identity();
    Root->Reset(Root->W);
}

bool Skeleton::Load(char *file){
    Tokenizer token;
    if(!token.Open(file))
        return false;
    if(!token.FindToken("balljoint"))
        return false;
    
    Root = new Joint;
    
    char temp[256];
    token.GetToken(temp);
    Root->name = temp;
    
    Root->Load(token);
    Root->index = 0;
    setJointIndex();
    token.Close();
    return true;
}

void Skeleton::setJointIndex(){
    stack<Joint*> s;
    s.push(Root);
    int index = 0;
    cout<<endl<<"Joint List: "<<endl;
    while (!s.empty()) {
        Joint* curr = s.top();
        s.pop();
        curr->index = index;
        index++;
        jointVec.push_back(curr);
        
        cout<<curr->name<<" : "<<curr->index<<endl;
        for(int i = curr->children.size()-1; i >= 0 ; i--) {s.push(curr->children.at(i));}
    }
    cout<<endl;
}

Matrix34 Skeleton::GetMatrix(int index){
    Matrix34 m;
    Joint* j = jointVec.at(index);
    m.Dot(j->W, j->Bi);
    return m;
}
