//
//  SpringDamper.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef SpringDamper_H
#define SpringDamper_H

#include <stdio.h>
#include <vector>
#include "Particle.h"
#include "matrix34.h"
using namespace std;

class SpringDamper {
public:
    float SpringConstant,DampingFactor;
    float RestLength;
    Particle* P1;
    Particle* P2;
    
    SpringDamper(Particle* p1, Particle* p2);
    
    void ComputeForce();
    
    void draw(Matrix34 W){
        Vector3 v1 = P1->Position;
        Vector3 v2 = P2->Position;
        
        glLoadMatrixf(W);
        glMatrixMode(GL_MODELVIEW);
        
        glLineWidth(1.5);
        glBegin(GL_LINES);
        
        if(!P1->fixed)
            glColor3f(1.0f,1.0f,0.0f);
        else
            glColor3f(0.0f,0.0f,1.0f);
        
        glVertex3f(v1.x,v1.y, v1.z);
        
        glColor3f(0.0f,1.0f,0.0f);
        
        if(!P2->fixed)
            glColor3f(1.0f,1.0f,0.0f);
        else
            glColor3f(0.0f,0.0f,1.0f);
        
        glVertex3f(v2.x,v2.y, v2.z);
        
        glPopMatrix();
        
        glEnd();
    };
};


#endif /* SpringDamper_h */
