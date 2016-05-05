#include "Vector4.h"
#include <math.h>
#include <iostream>
#include <cstring>

Vector4::Vector4()
{
    std::memset(m, 0, sizeof(m));
}

Vector4::Vector4(float m0, float m1, float m2)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
    m[3] = 1;
}

Vector4::Vector4(float m0, float m1, float m2, float m3)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
    m[3] = m3;
}

float* Vector4::ptr()
{
    return &m[0];
}

void Vector4::set(float x, float y, float z, float w)
{
    m[0] = x;
    m[1] = y;
    m[2] = z;
    m[3] = w;
}

float& Vector4::operator [] (int loc)
{
    return m[loc];
}

Vector4 Vector4::add(Vector4& a)
{
    Vector4 b;
    
    //
    b.set(m[0]+a.m[0] , m[1]+a.m[1], m[2]+a.m[2], m[3]+a.m[3]);
    
    return b;
}

Vector4 Vector4::operator + (Vector4 a)
{
    return add(a);
}

Vector4 Vector4::subtract(Vector4& a)
{
    Vector4 b;
    
    //
    b.set(m[0]-a.m[0] , m[1]-a.m[1], m[2]-a.m[2], m[3]-a.m[3]);
    
    return b;
}

Vector4 Vector4::operator - (Vector4 a)
{
    return subtract(a);
}

Vector4 Vector4::dehomogenize()
{
    Vector4 b;
    
    //
    b.set(m[0]/m[3] , m[1]/m[3], m[2]/m[3], 1);
    
    return b;
}

float Vector4::dot(Vector4 a)
{
    return (m[0] * a.m[0]) + (m[1] * a.m[1]) + (m[2] * a.m[2]) + (m[3] * a.m[3]);
}

void Vector4::print(std::string comment)
{
    std::cout << comment << std::endl;
    std::cout << "<x:" << m[0] <<  ", y:" << m[1] << ", z:" << m[2] << ", w:" << m[3] << ">" << std::endl;
}