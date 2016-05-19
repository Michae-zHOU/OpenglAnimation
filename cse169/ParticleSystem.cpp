//
//  ParticleSystem.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::Update(float deltaTime) {
    // Compute forces
    deltaTime/=1000;
    
    Vector3 gravity(0,-9.8,0);
    
    if(gravityControl)
        for(int i=0;i<NumParticles;i++) {
            Vector3 force = gravity*P[i].Mass; // f=mg
            if(!P[i].fixed)
                P[i].ApplyForce(force);
        }

    if(stringDampingControl)
        for(int i=0;i<sd->size();i++) {
            sd->at(i)->ComputeForce();
        
        }
    if(aerodynamicControl)
        for(int i=0;i<t.size();i++) {
            t.at(i)->ComputeAeroDynamicForce(airForce);
        }
    
    // Integrate
    for(int i=0;i<NumParticles;i++){
        P[i].Update(deltaTime);
    }
}