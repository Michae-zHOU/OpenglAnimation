//
//  Particle.h
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
private:
    Vector3 m_position;			// Current Location of the particle
    Vector3 m_velocity;			// Direction and speed the particle is traveling in
    Vector3 m_acceleration;		// Speed at which the particle is accelerating (used for gravity)
    Vector3 m_forceAccumulated;	// Force that has been accumulated since the last update
    Vector3 m_normal;				// Normal of this Particle, used to light the cloth when drawing
    float m_damping;				// Amount of velocity lost per update
    float m_inverseMass;			// Inverse of the weight (since the value is 1 this is effectively not used)
    bool m_stationary;				// Whether this particle is currently capible of movement

public:
    float Mass; // Constant
    Vector3 Position; // Evolves frame to frame
    Vector3 Velocity; // Evolves frame to frame
    Vector3 Force; // Reset and re-computed each frame
    
    void Update(float deltaTime);
    void Draw();
    void ApplyForce(Vector3 &f) {Force = Force + f;}
    
    
    // Copy Assignment Operator
    Particle& operator= (const Particle& particle);
    
    // Constructors
    Particle(Vector3 position, Vector3 acceleration);
    Particle(const Particle& particle);
    Particle();
    
    // Move the Particle forward in time by applying acceleration to the velocity and moving the position by the velocity
    void integrate(float duration);
    
    // Accessor functions
    inline Vector3 getPosition() const {return m_position;}
    inline Vector3 getVelocity() const {return m_velocity;}
    inline Vector3 getAcceleration() const {return m_acceleration;}
    inline Vector3 getForceAccumulated() const {return m_forceAccumulated;}
    inline Vector3 getNormal() const {return m_normal;}
    inline float getDamping() const {return m_damping;}
    inline float getInverseMass() const {return m_inverseMass;}
    inline bool getStationary() const {return m_stationary;}
    
    inline void addNormal(const Vector3& normal) {m_normal = m_normal + normal;}
    inline void addForceAccumulated(const Vector3& forceAccumulated) {m_forceAccumulated = m_forceAccumulated + forceAccumulated;}
    inline void mulVelocity(const float scalar) {m_velocity *= scalar;}
    inline void mulForceAccumulated(const float scalar) {m_forceAccumulated *= scalar;}
    
    inline void setPosition(const Vector3& position) {m_position = position;}
    inline void setVelocity(const Vector3& velocity) {m_velocity = velocity;}
    inline void setAcceleration(const Vector3& acceleration) {m_acceleration = acceleration;}
    inline void setForceAccumulated(const Vector3& forceAccumulated) {m_forceAccumulated = forceAccumulated;}
    inline void setNormal(const Vector3& normal) {m_normal = normal;}
    inline void setDamping(const float damping) {m_damping = damping;}
    inline void setInverseMass(const float inverseMass) {m_inverseMass = inverseMass;}
    inline void setStationary(const bool stationary) {m_stationary = stationary;}
};


#endif /* Particle_h */
