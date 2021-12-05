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
	Particle(float dx, float dy, float dz, float spd);
	~Particle();
	float position[3];//x,y,z
	float direction[3];//x,y,z
	float angle[3];//x,y,z
	int shape;
	float speed;
	int age;
	int fallenOff;

	//TODO: Implement these attributes
	//float color[3];//r,g,b	
	//float angle;
	//float angleSpeed;
	//int size;//used to scale the particle
	//int shape;//0 circle, 1 square, etc.	

private:

};

Particle::Particle()
{
	age = 0;
	//default location at 'end' of cannon
	position[0] = 0;//x
	position[1] = 10;//y
	position[2] = -70;//z

	direction[0] = 0;//x
	direction[1] = 1;//y
	direction[2] = 1;//z
	fallenOff = 0;

	speed = 5;	
}

Particle::Particle(float dx, float dy, float dz, float spd)
{
	age = 0;
	//default location at 'end' of cannon
	position[0] = 0;//x
	position[1] = 10;//y
	position[2] = -70;//z

	direction[0] = dx;//x
	direction[1] = dy;//y
	direction[2] = dz;//z
	fallenOff = 0;

	speed = spd;
}

Particle::~Particle()
{
}

typedef struct {
	vector<Particle> particles;
	float screenSizeX, screenSizeY;
	float grav;
	float frict;
	int frameRate;
	int speedInvoked;
	int fireMode;
	int kPressed;
	int axis;
	float angle[3];
	int ground[][3];
} glob;

glob global = { {},
				500, 500,
				0.3,
				0.3,
				50,
				0,
				0,
				0,
				1,
				{0.0, 0.0, 0.0},
				{	{100, 0, -100}, {100, 0, 50}, {-100, 0, 50}, {-100, 0, -100},
					{-10, 0, 50}, {-10, 0, 70}, {-100, 0, 70}, {-100, 0, 50},
					{100, 0, 50}, {100, 0, 70}, {10, 0, 70}, {10, 0, 50},
					{100, 0, 70}, {100, 0, 100}, {-100, 0, 100}, {-100, 0, 70}
				}
			};

void fireParticle(void);