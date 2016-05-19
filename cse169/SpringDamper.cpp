//
//  SpringDamper.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "SpringDamper.h"
#include <iostream>

SpringDamper::SpringDamper(Particle* p1, Particle* p2){
    P1 = p1;
    P2 = p2;
    
    RestLength = (p2->Position - p1->Position).Mag();
    
    SpringConstant = 1000.0f;
    DampingFactor = 1.0f;
}

void SpringDamper::ComputeForce(){
    Vector3 e = P2->Position - P1->Position;
    
    float currlength = e.Mag();
    
    if(currlength != 0)
        e.Normalize();
    else
        e = Vector3();
    
    float v1, v2;
    v1 = e.Dot(P1->Velocity);
    v2 = e.Dot(P2->Velocity);
    
    
    float Fsd = - SpringConstant * (RestLength - currlength) - DampingFactor * (v1 - v2);
    
    Vector3 f1 = Fsd * e;
    Vector3 f2 = - f1;
    
    if(!P1->fixed)
        P1->ApplyForce(f1);
    if(!P2->fixed)
        P2->ApplyForce(f2);
}