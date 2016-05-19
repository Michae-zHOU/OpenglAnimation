//
//  Triangle.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//
#ifndef CSE169_TRIANGLE_H
#define CSE169_TRIANGLE_H

#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include "Particle.h"
#include "Texture.h"
#include <vector>

using namespace std;

class Triangle{
public:
    int v[3];
    
    Vector3* vertex;
    Vector3 normal;
    
    Particle* P1;
    Particle* P2;
    Particle* P3;
    
    float pConstant;
    float CdConstant;
    Vector3 Vaero;
    
    Texture* flag;
    float xCoord, yCoord;
    float xDistance, yDistance;
    
    Triangle(int, int, int);
    Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    Triangle(Particle* p1, Particle* p2, Particle* p3);
    
    void CalculateNormal();
    void CalculateParticleNormal();
    
    void ComputeAeroDynamicForce(const Vector3& Vair);
    
    void SetupTexture(Texture* texture, float x, float y, float x_distance, float y_distance){
        flag = texture;
        xCoord = x;
        yCoord = y;
        
        xDistance = x_distance;
        yDistance = y_distance;
    };
    
    void draw();
    void draw(Matrix34 W);
    void draw(Matrix34 W, Vector3 Color);
};

#endif