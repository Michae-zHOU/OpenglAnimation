#ifndef CSE169_LIGHT_H
#define CSE169_LIGHT_H

#include "Matrix34.h"
#include "Color.h"
#include "Vector4.h"
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Light
{
    
protected:
    
    int bindID = -1;
    
public:
    
    Vector4 position;
    
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    
public:
    
    Light();
    virtual ~Light(void);
    
    void bind(int);
    void unbind(void);
    
};

#endif

