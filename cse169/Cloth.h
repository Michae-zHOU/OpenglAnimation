//
//  Cloth.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef Cloth_H
#define Cloth_H

#include <stdio.h>
#include <vector>
#include "vector3.h"
#include "Triangle.h"
#include "Particle.h"
#include "Texture.h"
#include "SpringDamper.h"
using namespace std;

class Cloth {
public:
    float Width;
    float Height;
    int p_in_width;
    int p_in_height;
    float distanceX;
    float distanceY;
    
    Matrix34 toWorld;
    
    Texture* flag;
    
    bool sd_draw = false;
    
    // Pointer to Array
    Particle* particle;
    Vector3 color;
    
    vector<SpringDamper*> springdamper;
    vector<Triangle*> triangle;

    
    Cloth(float width, float height, int p_width, int p_height, float Mass);
    Cloth(float width, float height, int p_width, int p_height, float Mass, Vector3 init, char* filename);
    
    void Draw(){
        for(int i=0; i<triangle.size(); i++){
            triangle.at(i)->draw(toWorld);
        }
        
        if(sd_draw)
            for(int i=0; i<springdamper.size(); i++){
                springdamper.at(i)->draw(toWorld);
            }
    };
    
    void Draw(Vector3 Color){
        for(int i=0; i<triangle.size(); i++){
            triangle.at(i)->draw(toWorld, Color);
        }
        
        if(sd_draw)
            for(int i=0; i<springdamper.size(); i++){
                springdamper.at(i)->draw(toWorld);
            }
    };
};


#endif /* Cloth_h */
