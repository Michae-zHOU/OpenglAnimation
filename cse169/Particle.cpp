//
//  ParticleSystem.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "ParticleSystem.h"
#include <iostream>

void Particle::Update(float deltaTime) {
    
    // Compute acceleration (Newton’s second law)
    Vector3 Accel=(1.0/Mass) * Force;
    
    if(Position.y < groundY){
        Position.y = 2*groundY - Position.y;

        Velocity.y = (-elastic)*Velocity.y;
        Velocity.x = (1-friction)*Velocity.x;
        Velocity.z = (1-friction)*Velocity.z;
    }
    else
        // Compute new position & velocity
        Velocity+=Accel*deltaTime;
    
    Position+=Velocity*deltaTime;
    // Zero out Force vector
    Force.Zero();
}