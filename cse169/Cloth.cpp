//
//  Cloth.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "Cloth.h"

Cloth::Cloth(float width, float height, int p_width, int p_height, float Mass)
: Width(width), Height(height), p_in_width(p_width), p_in_height(p_height)
{

    particle = new Particle[p_width*p_height];
    
    float x_distance = width/(float) p_width;
    float y_distance = height/(float) p_height;
    
    distanceX = x_distance;
    distanceY = y_distance;
    
    Triangle* t1;
    Triangle* t2;
    
    SpringDamper* sd;
    
    for(int i = 0; i < p_width; i++){
        for(int j = 0; j < p_height; j++){
            float x = width * ((float)i/(float)p_width);
            float y = 0.0f;
            float z = height * ((float)j/(float)p_height);
            
            Vector3 position( x, y, z);
            Particle p(Mass, position);
            
            particle[j*p_width+i] = p;
        }
    }
    
    for(int i=0; i<p_width;i++){
        particle[i].fixed = true;
    }
    
    // Create Triangles
    for(int j = 0; j < p_height-1; j++){
        for(int i = 0; i < p_width-1; i++){
            Particle* p1 = &(particle[j*p_width+i]);
            Particle* p2 = &(particle[j*p_width+i+1]);
            Particle* p3 = &(particle[(j+1)*p_width+i]);
            Particle* p4 = &(particle[(j+1)*p_width+i+1]);
            
            t1 = new Triangle( p1, p2, p3);
            t2 = new Triangle( p4, p3, p2);
            
            t1->CalculateParticleNormal();
            t2->CalculateParticleNormal();
            
            triangle.push_back(t1);
            triangle.push_back(t2);
        }
    }
    
    //Create Spring Dampers
    for(int i = 0; i < p_width; i++){
        for(int j = 0; j < p_height; j++){
        
            Particle* p1 = &(particle[j*p_width+i]);
            
            if(j != 0){
                Particle* p5 = &(particle[(j-1)*p_width+i+1]);
                sd = new SpringDamper(p1, p5);
                springdamper.push_back(sd);
            }
            
            if(i != p_width-1){
                Particle* p2 = &(particle[j*p_width+i+1]);
                
                sd = new SpringDamper(p1,p2);
                springdamper.push_back(sd);
            }
            if(j != p_height-1){
                Particle* p3 = &(particle[(j+1)*p_width+i]);
                sd = new SpringDamper(p1,p3);
                springdamper.push_back(sd);
            }
            
            if(i != p_width-1 && j != p_height-1){
                Particle* p4 = &(particle[(j+1)*p_width+i+1]);
                sd = new SpringDamper(p1,p4);
                springdamper.push_back(sd);
            }
        }
    }
    
}

Cloth::Cloth(float width, float height, int p_width, int p_height, float Mass, Vector3 init, char* filename)
: Width(width), Height(height), p_in_width(p_width), p_in_height(p_height)
{
    flag = new Texture(filename);
    
    //flag = new Texture("/Users/ziyaozhou/Desktop/cse169/Ucsd-logo.ppm");
    
    particle = new Particle[p_width*p_height];
    
    float x_distance = width/(float) p_width;
    float y_distance = height/(float) p_height;
    
    distanceX = x_distance;
    distanceY = y_distance;
    
    Triangle* t1;
    Triangle* t2;
    
    SpringDamper* sd;
    
    for(int i = 0; i < p_width; i++){
        for(int j = 0; j < p_height; j++){
            float x = width * ((float)i/(float)p_width);
            float y = 0.0f;
            float z = height * ((float)j/(float)p_height);
            
            Vector3 position( x, y, z);
            Particle p(Mass, position);
            
            p.init = init;
            particle[j*p_width+i] = p;
        }
    }
    
    for(int i=0; i<p_width;i++){
        particle[i].fixed = true;
    }
    
    // Create Triangles
    for(int j = 0; j < p_height-1; j++){
        for(int i = 0; i < p_width-1; i++){
            Particle* p1 = &(particle[j*p_width+i]);
            Particle* p2 = &(particle[j*p_width+i+1]);
            Particle* p3 = &(particle[(j+1)*p_width+i]);
            Particle* p4 = &(particle[(j+1)*p_width+i+1]);
            
            t1 = new Triangle( p1, p2, p3);
            t2 = new Triangle( p4, p3, p2);
            
            float x_dist = distanceX/width;
            float y_dist = distanceY/height;
            float xval = (float)i/(float)p_width;
            float yval = (float)j/(float)p_height;
            
            t1->SetupTexture(flag, xval, yval, x_dist, y_dist);
            t2->SetupTexture(flag, xval+x_dist, yval+y_dist, -x_dist, -y_dist);
            
            t1->CalculateParticleNormal();
            t2->CalculateParticleNormal();
            
            triangle.push_back(t1);
            triangle.push_back(t2);
        }
    }
    
    //Create Spring Dampers
    for(int i = 0; i < p_width; i++){
        for(int j = 0; j < p_height; j++){
            
            Particle* p1 = &(particle[j*p_width+i]);
            
            if(i != p_width-1){
                Particle* p2 = &(particle[j*p_width+i+1]);
                sd = new SpringDamper(p1,p2);
                springdamper.push_back(sd);
            }
            if(j != p_height-1){
                Particle* p3 = &(particle[(j+1)*p_width+i]);
                sd = new SpringDamper(p1,p3);
                springdamper.push_back(sd);
            }
            
            if(i != p_width-1 && j != p_height-1){
                Particle* p4 = &(particle[(j+1)*p_width+i+1]);
                sd = new SpringDamper(p1,p4);
                springdamper.push_back(sd);
            }
            
            if(j != 0){
                Particle* p5 = &(particle[(j-1)*p_width+i+1]);
                sd = new SpringDamper(p1, p5);
                springdamper.push_back(sd);
            }
            
            // smooth
            if(i != p_width-1){
                Particle* p2 = &(particle[j*p_width+i+1]);
                
                sd = new SpringDamper(p1,p2);
                sd->SpringConstant /= 2.0;
                sd->DampingFactor *= 2.0;
                springdamper.push_back(sd);
            }
            if(j != p_height-1){
                Particle* p3 = &(particle[(j+1)*p_width+i]);
                
                sd = new SpringDamper(p1,p3);
                sd->SpringConstant /= 2.0;
                sd->DampingFactor *= 2.0;
                springdamper.push_back(sd);
            }
            
            if(i != p_width-1 && j != p_height-1){
                Particle* p4 = &(particle[(j+1)*p_width+i+1]);
                
                sd = new SpringDamper(p1,p4);
                sd->SpringConstant /= 2.0;
                sd->DampingFactor *= 3.0;
                springdamper.push_back(sd);
            }
            
        }
    }
    
    toWorld.MakeTranslate(init);
}

