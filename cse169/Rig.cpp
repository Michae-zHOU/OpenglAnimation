//
//  Rig.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "Rig.h"


Rig::Rig(Skeleton* skel, Skin* skin){
    this->skel = skel;
    this->skin = skin;
}
void Rig::setSkelVisible(int visible){
    if(visible == -1)
        skelVisible = false;
    else
        skelVisible = true;
}
void Rig::Draw(){
    if(!skelVisible)
        skel->Draw();
    
    skin->Draw();
}

void Rig::Update(){
    skel->Update();
    skin->Update();
}