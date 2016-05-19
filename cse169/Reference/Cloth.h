#ifndef CLOTH_H
#define CLOTH_H

#include "Particle.h"
#include <vector>
#include <GLUT/glut.h>
#include "Vector3.h"

// Class that contains the cloth object
class Cloth 
{
private: 
	float m_dimensionWidth;				// Width of the cloth
	float m_dimensionHeight;			// Height of the cloth
	float m_distanceX;					// Width between particles
	float m_distanceY;					// Height between particles
	int m_width;						// Number of particles contained in the width
	int m_height;						// Number of particles contained in the height
	std::vector<Particle> m_particles;	// Contains all the particles of the cloth

	// Calculates the normals of all the particles
	void calculateClothNormals();

	// Gets a particle from the particle array
	Particle* getParticle(int x, int y);
	const Particle* getProtectedParticle(int x, int y) const;

	// Draws a triangle of the cloth
    static void drawTriangle(const Particle& p1, const Particle& p2, const Particle& p3, const Vector3& color);

	// Used to find normals of a triangle
	static Vector3 calculateTriangleCross(const Particle &p1, const Particle &p2, const Particle &p3);

	// Check if the ball is intersecting the cloth
	void ballCollision(const Vector3 &center, const float radius);
public: 
	// Constructor
	Cloth(float width, float height, int particles_in_width, int particles_in_height);

	// Draws the entire cloth
	void draw() const;

	// Step the cloth forward in time by the given duration
	void update(float duration);

	// Calculate the spring forces for clothlike behavior
	void calculateForces(const Vector3 &wind_dir, const Vector3 &ball_pos, float ball_radius);
};

#endif