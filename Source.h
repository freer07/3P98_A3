#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <vector>
using namespace std;

class Particle
{
public:
	Particle();
	~Particle();
	int position[3];//x,y,z
	int direction[3];//x,y,z
	int color[3];//r,g,b
	int speed;
	float angle;
	float angleSpeed;
	int size;//used to scale the particle
	int shape;//0 circle, 1 square, etc.
	int age;

private:

};

Particle::Particle()
{
}

Particle::~Particle()
{
}

typedef struct {
	vector<Particle> particles;
	float screenSizeX, screenSizeY;
	float orthoX, orthoY, orthoZ;
	float grav;
	int ground[][3];
} glob;

glob global	= {	{}, 
				500, 500, 
				100, 100, 100, 
				9.8,
				{ {-100,0,-100}, {100,0,-100}, {100,0,100}, {-100,0,100} } 
			};