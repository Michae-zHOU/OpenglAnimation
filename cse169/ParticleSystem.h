//
//  ParticleSystem.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef ParticleSystem_H
#define ParticleSystem_H

#include <stdio.h>
#include <vector>
#include "vector3.h"
#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"
#include "Cloth.h"
using namespace std;

class ParticleSystem {
public:
    int NumParticles;
    
    Particle *P;
    Vector3 airForce;
    
    vector<SpringDamper*>* sd;
    vector<Triangle*> t;
    
    void Update(float deltaTime);
    void Draw();
    
    Cloth* ground;
    
    bool gravityControl = false;
    bool stringDampingControl = false;
    bool aerodynamicControl = false;
    
    void SetUp(Cloth* cloth){
        airForce = Vector3(0,0,0);
        NumParticles = cloth->p_in_width * cloth->p_in_height;
        P = cloth->particle;
        sd = &(cloth->springdamper);
        t = cloth->triangle;
    };
    
    void SetGround(Cloth* ground, float elastic){
        this->ground = ground;
        float y = ground->toWorld.d.y;

        for(int i=0; i<NumParticles; i++){
            P[i].groundY = y-P[i].init.y+0.05;
            P[i].elastic = elastic;
        }
    }
    void UpdateGround(Cloth* ground, Matrix34 W, float elastic){
        float y = ground->toWorld.d.y;
        for(int i=0; i<NumParticles; i++){
            P[i].groundY = y-W.d.y+0.05;
            P[i].elastic = elastic;
        }
    }
};


#endif /* ParticleSystem_h */
