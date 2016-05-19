#include "Cloth.h"
#include <iostream>
using namespace std;

// Constructors
Cloth::Cloth(float width, float height, int particles_in_width, int particles_in_height)
 : m_width(particles_in_width), m_height(particles_in_height), m_dimensionWidth(width), m_dimensionHeight(height),
   m_distanceX(width/ (float)particles_in_width), m_distanceY(height/(float)particles_in_height)
{

	// Set the particle array to the given size
	m_particles.resize(m_width*m_height);

	// Create the particles to simulate the cloth
	for (int x = 0; x < m_width; ++x) {
		for (int y=0; y < m_height; ++y) {
			// Place the particles of the cloth, lift the edges to give the wind more effect as the cloth falls
			Vector3 position = Vector3(m_dimensionWidth * (x / (float)m_width),
										 ((x==0)||(x==m_width-1)||(y==0)||(y==m_height-1)) ? m_distanceY/2.0f:0,
										 m_dimensionHeight * (y / (float)m_height));
			// The gravity effect is applied to new particles
			m_particles[y * m_width + x] = Particle(position,Vector3(0,-0.06,0));
		}
	}

	// Calculate the normals for the first time so the initial draw is correctly lit
	calculateClothNormals();
}

// Calculates the normals of all the particles
void Cloth::calculateClothNormals(){
	// For each quad
	for (int x = 0; x < m_width -1; ++x) {
		for (int y =0; y < m_height-1; ++y)
		{
			// Calculate the normals of both triangles and apply the normal to the particles
			Vector3 normal = calculateTriangleCross(*getParticle(x+1, y), *getParticle(x,y), *getParticle(x, y+1));
			getParticle(x+1, y)->addNormal(normal);
			getParticle(x,y)->addNormal(normal);
			getParticle(x,y+1)->addNormal(normal);

			normal = calculateTriangleCross(*getParticle(x+1, y+1), *getParticle(x+1,y), *getParticle(x, y+1));
			getParticle(x+1,y+1)->addNormal(normal);
			getParticle(x+1,y)->addNormal(normal);
			getParticle(x,y+1)->addNormal(normal);
		}
	}
	
	// Normalize the particle normals so they are unit length
	int num = (int)m_particles.size();
	#pragma omp parallel for
	for (int i=0; i<num; ++i)
	{
		m_particles[i].setNormal((m_particles[i].getNormal().Normalize()));
	}
}

// Used to find normals of a triangle
Vector3 Cloth::calculateTriangleCross(const Particle& p1, const Particle& p2, const Particle& p3) 
{
    Vector3 temp1 = p2.getPosition() - p1.getPosition();
    Vector3 temp2 = p3.getPosition() - p1.getPosition();
    temp2.Normalize();
    Vector3 temp3;
    temp3.Cross(temp1, temp2);
	return temp3;
}

// Gets a particle from the particle array
Particle* Cloth::getParticle(int x, int y){
	return &m_particles[y * m_width + x];
}

// Gets a particle from the particle array
const Particle* Cloth::getProtectedParticle(int x, int y) const{
	return &m_particles[y * m_width + x];
}

// Draws a triangle of the cloth
void Cloth::drawTriangle(const Particle& p1, const Particle& p2, const Particle& p3, const Vector3& color){
	// Since we know the vector3 components will exist beside eachother in memory we can call with the pointer to the x component
	glColor3fv(&color.x);
    
    Vector3 vn1 = p1.getNormal();
    Vector3 v1 = p1.getPosition();
	glNormal3f(vn1.x, vn1.y, vn1.z);
	glVertex3f(v1.x, v1.y, v1.z);
    
    Vector3 vn2 = p2.getNormal();
    Vector3 v2 = p2.getPosition();
	glNormal3f(vn2.x, vn2.y, vn2.z);
	glVertex3f(v2.x, v2.y, v2.z);

    Vector3 vn3 = p3.getNormal();
    Vector3 v3 = p3.getPosition();
	glNormal3f(vn3.x, vn3.y, vn3.z);
	glVertex3f(v3.x, v3.y, v3.z);
}

// Check if the ball is intersecting the cloth
void Cloth::ballCollision(const Vector3 &center, const float radius){

	// Check each particle
	int num = (int)m_particles.size();
	#pragma omp parallel for
	for (int i=0; i<num; ++i)
	{
		Vector3 v = m_particles[i].getPosition() - center;
        float l = v.Mag2();

		// If inside the ball place it on the surface and dampen the velocity for friction
		if (l < radius*radius)
		{
            v.Normalize();
			m_particles[i].setPosition(v * radius + center);
			m_particles[i].mulVelocity(0.5);
		}
	}
}

// Draws the entire cloth
void Cloth::draw() const{
    
	// For each quad draw the two triangles that compose it as they are not likely to be coplanar
	glBegin(GL_TRIANGLES);
	for (int x = 0; x < m_width -1; ++x)
	{
		for (int y =0; y < m_height-1; ++y)
		{
            
			// Used for checker board effect, bitwise xor of boolean expression will work as logical xor
			Vector3 colour = ((x%4 < 2)^(y%4 < 2)) ? Vector3(0.0f,0.0f,0.0f): Vector3(1.0f, 1.0f, 1.0f);
			drawTriangle(*getProtectedParticle(x+1, y), *getProtectedParticle(x,y), *getProtectedParticle(x, y+1), colour);
			drawTriangle(*getProtectedParticle(x+1, y+1), *getProtectedParticle(x+1,y), *getProtectedParticle(x, y+1), colour);
		}
	}
	glEnd();
}

// Step the cloth forward in time by the given duration
void Cloth::update(float duration) {
	int num = (int)m_particles.size();
	#pragma omp parallel for
	for (int i=0; i<num; ++i)
	{
		m_particles[i].integrate(duration);
	}
}

// Calculate the spring forces for clothlike behavior
void Cloth::calculateForces(const Vector3 &wind_dir, const Vector3 &ball_pos, float ball_radius){
    
	// Apply gravity, wind and reset the normals
	int num = (int)m_particles.size();
	#pragma omp parallel for
	for (int i=0; i<num; ++i)
	{
		Particle* p = &m_particles[i];
		// Alter the application of gravity so it is not effected by mass
		p->addForceAccumulated(p->getAcceleration() * (1 / p->getInverseMass())); 
		
		// Apply the wind using the last calculated normals
		Vector3 force =p->getNormal() * (p->getNormal().Dot(wind_dir));
		p->addForceAccumulated(force);

		// Reset the normal, will recalculate after position is adjusted
		p->setNormal(Vector3(0,0,0));
	}

	ballCollision(ball_pos,ball_radius);	// resolve collision with the ball
	calculateClothNormals();				// Calculate new normals

	// Calculate new forces
	for (int x = 0; x < m_width; ++x) {
		for (int y =0; y < m_height; ++y)
		{
			Particle* p = getParticle(x,y);				// Current particle
			float squares = 0;							// A count of the adjacent particles
			Vector3 springForce = Vector3(0,0,0);		// Will contain the forces of all adjacent particles are applying
			float largestStretch = 0;					// Used to cap the length the cloth can stretch

			// Evaluate the Closest 24 positions
			for (int a = -2; a <= 2; ++a) {
				for (int b = -2; b <= 2; ++b)
				{
					// Ensure a particle exist and it isn't the current one
					if ((a != 0 || b != 0) && (x+a >= 0) && (x+a < m_width) && (y+b >= 0) && (y+b < m_height))
					{
						// Get the direction this spring is pulling
						Vector3 springVector = getProtectedParticle(x+a, y+b)->getPosition() - p->getPosition();

						// Find how much force is exerted by this string
						float length = springVector.Mag();
						float normalLength = sqrt((a * m_distanceX) * (a * m_distanceX) + (b * m_distanceY) * (b * m_distanceY)) ;
						float forceScalar = (length - normalLength) / normalLength;

						// Add the force this particle is applying to the other particle forces
						springForce += springVector / length  * forceScalar;
						squares++;

						// Keep track of which spring is exerting the most force
						if (forceScalar > largestStretch) {
							largestStretch = forceScalar;
						}
					} 
				}
			}

			// If a spring is stretched beyond 20% we will begin to minimize the effects of other forces to maintain the cloth shape
			if (largestStretch >= 0.25){
				p->mulForceAccumulated((largestStretch >=0.75)? 0 : (0.75-largestStretch));
			}

			// Apply the force of the cloth on this particle
			p->addForceAccumulated(springForce / squares);
		}
	}

}