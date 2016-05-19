//
//  Triangle.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/1/16.
//
//

#include "Triangle.h"
#include <iostream>
#include "Math.h"
using namespace std;

Triangle::Triangle(int v1, int v2, int v3){
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
}

Triangle::Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3){
    vertex = new Vector3[3];
    
    vertex[0] = v1;
    vertex[1] = v2;
    vertex[2] = v3;
}


Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3){
    P1 = p1;
    P2 = p2;
    P3 = p3;
    
    pConstant = 1.225f;
    CdConstant = 10.0f;
    Vaero = Vector3(0,0,0);
}

void Triangle::CalculateNormal(){
    normal.Cross(vertex[1]-vertex[0], vertex[2]-vertex[0]);
    normal.Normalize();
}

void Triangle::CalculateParticleNormal(){
    Vector3 v1 = P1->Position;
    Vector3 v2 = P2->Position;
    Vector3 v3 = P3->Position;
    
    normal.Cross(v2-v1, v3-v1);
    normal.Normalize();
}

void Triangle::ComputeAeroDynamicForce(const Vector3& Vair){
    CalculateParticleNormal();
    
    Vector3 v1 = P1->Position;
    Vector3 v2 = P2->Position;
    Vector3 v3 = P3->Position;
    
    Vector3 v;
    v.Cross(v2-v1, v3-v1);
    
    float a0 = 0.5*(v.Mag());

    Vector3 Vsurface = (P1->Velocity+P2->Velocity+P3->Velocity)/3;
    
    Vector3 V = Vsurface - Vair;
    
    Vector3 vn = V;
    
    if(vn.x != 0 || vn.y != 0 || vn.z != 0)
        vn.Normalize();
    
    float a = a0*vn.Dot(normal);
    
    Vector3 Faero = -0.5*pConstant*V.Mag()*V.Mag()*CdConstant*a*normal;
    Faero /= 3;
    
    if(!P1->fixed)
        P1->ApplyForce(Faero);
    if(!P2->fixed)
        P2->ApplyForce(Faero);
    if(!P3->fixed)
        P3->ApplyForce(Faero);
}

void Triangle::draw(){
    CalculateParticleNormal();
    
    Vector3 v1 = P1->Position;
    Vector3 v2 = P2->Position;
    Vector3 v3 = P3->Position;

    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.x, v1.y, v1.z);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v2.x, v2.y, v2.z);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v3.x, v3.y, v3.z);
    
    glPopMatrix();
    
    glEnd();
}

void Triangle::draw(Matrix34 W){
    CalculateParticleNormal();
    
    Vector3 v1 = P1->Position;
    Vector3 v2 = P2->Position;
    Vector3 v3 = P3->Position;

    glLoadMatrixf(W);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);
    flag->bind();
    
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0f,1.0f,1.0f);
    glTexCoord2f(xCoord,yCoord);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.x, v1.y, v1.z);
    
    glTexCoord2f(xCoord+xDistance,yCoord);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v2.x, v2.y, v2.z);
    
    glTexCoord2f(xCoord,yCoord+yDistance);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v3.x, v3.y, v3.z);
    
    glPopMatrix();
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Triangle::draw(Matrix34 W, Vector3 Color){
    CalculateParticleNormal();
    
    Vector3 v1 = P1->Position;
    Vector3 v2 = P2->Position;
    Vector3 v3 = P3->Position;
    
    glLoadMatrixf(W);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    
    glBegin(GL_TRIANGLES);
    
    glColor3fv(&Color[0]);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.x, v1.y, v1.z);
    
    glColor3fv(&Color[0]);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v2.x, v2.y, v2.z);
    
    glColor3fv(&Color[0]);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v3.x, v3.y, v3.z);
    
    glPopMatrix();
    
    glEnd();
}

