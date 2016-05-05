#ifndef CSE169_VECTOR4_h
#define CSE169_VECTOR4_h

#include <string>

class Vector4
{

protected:
    
    float m[4];
    
public:
    
    Vector4();
    Vector4(float, float, float);
    Vector4(float, float, float, float);
    
    float* ptr();
    float& operator [] (int);
    
    void set(float,float,float,float);
    
    Vector4 add(Vector4&);
    Vector4 operator + (Vector4);
    
    Vector4 subtract(Vector4&);
    Vector4 operator - (Vector4);
    
    Vector4 dehomogenize();
    
    float dot(Vector4);
    
    void print(std::string);
    
};

#endif
