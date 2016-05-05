//
//  Channel.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "AnimationPlayer.h"

void AnimationPlayer::LoadAnim(char* file){
    anim = new Animation(file);
    
    time_start = anim->time_start;
    time_end = anim->time_end;
    
    if(anim->exceptionHandler)
        exceptionHandler = true;
}

void AnimationPlayer::LoadSkel(Skeleton* skel){
    this->skel = skel;
}

void AnimationPlayer::Play(float t){
    
    float dof0 = anim->Evaluate(t, 0);
    float dof1 = anim->Evaluate(t, 1);
    float dof2 = anim->Evaluate(t, 2);
    
    skel->jointVec.at(0)->dofs[0].SetValue(dof0);
    skel->jointVec.at(0)->dofs[1].SetValue(dof1);
    skel->jointVec.at(0)->dofs[2].SetValue(dof2);
    
    for(int i = 0;  i<skel->jointVec.size(); i++){
        int j = i+1;
        
        float dof3 = anim->Evaluate(t, 3*j);
        float dof4 = anim->Evaluate(t, 3*j+1);
        float dof5 = anim->Evaluate(t, 3*j+2);

        skel->jointVec.at(i)->dofs[3].SetValue(dof3);
        skel->jointVec.at(i)->dofs[4].SetValue(dof4);
        skel->jointVec.at(i)->dofs[5].SetValue(dof5);
        
        if(skel->jointVec.at(i)->dofs[3].getValue()<-100)
            cout<<dof3<<" -Overflow3 at "<<t<<"s"<<endl;
        
        if(skel->jointVec.at(i)->dofs[5].getValue()<-100)
            cout<<dof5<<" -Overflow5 at "<<t<<"s"<<endl;
    }
}