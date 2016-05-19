//
//  ParticleSystem.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef Particle_H
#define Particle_H

#include <stdio.h>
#include <vector>
#include "vector3.h"
using namespace std;

class Particle {
public:
    float Mass; // Constant
    Vector3 Position; // Evolves frame to frame
    Vector3 Velocity; // Evolves frame to frame
    Vector3 Force; // Reset and re-computed each frame
    bool fixed;
    
    Vector3 init;
    float groundY = -9999999.0f;
    float elastic = 0.0f;
    float friction = 0.5f;
    
    // Constructor
    Particle(){};
    Particle(float mass, Vector3 position): Mass(mass), Position(position){
        Force = Vector3(0, 0, 0);
        Velocity = Vector3(0, 0, 0);
        fixed = false;
    };
    
    void Update(float deltaTime);
    void Draw();
    void ApplyForce(Vector3 f) {Force.Add(f);}
};


#endif /* Particle_h */
