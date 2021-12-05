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
	float position[3];//x,y,z
	float direction[3];//x,y,z
	float angle[3];//x,y,z
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

	speed = 1;	
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
	int ground[][3];
} glob;

glob global = { {},
				500, 500,
				0.3,
				0.3,
				50,
				{	{100, 0, -100}, {100, 0, 50}, {-100, 0, 50}, {-100, 0, -100},
					{-10, 0, 50}, {-10, 0, 70}, {-100, 0, 70}, {-100, 0, 50},
					{100, 0, 50}, {100, 0, 70}, {10, 0, 70}, {10, 0, 50},
					{100, 0, 70}, {100, 0, 100}, {-100, 0, 100}, {-100, 0, 70}
				}
			};