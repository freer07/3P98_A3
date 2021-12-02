// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <time.h>
#include <vector>
#include "Source.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

#define X 0
#define Y 1
#define Z 2

int isInHole(float x, float z) {
    if (z > 50 && z < 70 && x > -10 && x < 10) {
        return 1;
    }
    return 0;
}

//this is the update function for the frames
void update(int val) {
    if (global.fireMode == 0 && global.particles.size() < 500) {
        fireParticle();
    }

    //interate through the particles and update their position
    for (auto& particle : global.particles) // access by reference to avoid copying
    {
        particle.position[0] = particle.position[0] + particle.direction[0] * particle.speed;
        particle.position[1] = particle.position[1] + particle.direction[1] * particle.speed;
        particle.position[2] = particle.position[2] + particle.direction[2] * particle.speed;
        
        //gravity 
        particle.direction[1] = particle.direction[1] - global.grav;        

        //ground limit (bounce)
        if (particle.position[1] < 0 && !particle.fallenOff) {
            if (isInHole(particle.position[0], particle.position[2])) {
                particle.fallenOff = 1;
            }
            else {
                particle.position[1] = 0;
                particle.direction[1] = particle.direction[1] * -1;
                particle.speed = particle.speed - global.frict;
            }            
        }

        

        //prevent negative speed 
        if (particle.speed < 0) {
            particle.speed = 0;
        }        

        particle.age++;
    }

    //kill particles
    global.particles.erase(
        remove_if(
            global.particles.begin(),
            global.particles.end(),
            [](Particle const& p) { return p.speed <= 0 || p.position[1] < -100; }
        ),
        global.particles.end()
    );

    //show the frame
    glutPostRedisplay();

    //set next update timmer 
    glutTimerFunc(global.frameRate, update, 0);
}

void userintro() {
    printf("---Toggle Fire Modes---\n");
    printf("1 = (default) constant stream\n");
    printf("2 = Full Auto ('F' to fire)\n");
    printf("3 = Semi-Auto ('F' to fire)\n");
    printf("---------------------------\n");
    printf("S = start/stop random speeds\n");
    printf("Left mouse = rotate left faster\n");
    printf("Right mouse = rotate right faster\n");
    printf("Middle mouse = reset\n");
    printf("x, y, z = rotate about x, y, or z axis\n");
    printf("R = reset simulation(including orientation)\n");
    printf("Q = quit");
}

void displayParticles(void) {
    for (auto& particle : global.particles) // access by reference to avoid copying
    {
        glColor3f(0.0, 0.0, 1.0);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glVertex3f(particle.position[0], particle.position[1], particle.position[2]);
        glEnd();
    }
}

void displayGround(void) {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3iv(global.ground[0]);
    glVertex3iv(global.ground[1]);
    glVertex3iv(global.ground[2]);
    glVertex3iv(global.ground[3]);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3iv(global.ground[4]);
    glVertex3iv(global.ground[5]);
    glVertex3iv(global.ground[6]);
    glVertex3iv(global.ground[7]);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3iv(global.ground[8]);
    glVertex3iv(global.ground[9]);
    glVertex3iv(global.ground[10]);
    glVertex3iv(global.ground[11]);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3iv(global.ground[12]);
    glVertex3iv(global.ground[13]);
    glVertex3iv(global.ground[14]);
    glVertex3iv(global.ground[15]);
    glEnd();
}

void displayCannon(void) {
    //cannon 
    float l = 20, x = 0, z = -80;//side length, x and z center coordinates

    int cannon[][3] = { {x - (l / 2), 0, z - (l / 2)}, {x + (l / 2), 0, z - (l / 2)}, {x + (l / 2), 0, z + (l / 2)}, {x - (l / 2), 0, z + (l / 2)},
                        {x - (l / 2), 0, z - (l / 2)}, {x + (l / 2), 0, z - (l / 2)}, {x + (l / 2), l, z - (l / 2)}, {x - (l / 2), l, z - (l / 2)},
                        {x + (l / 2), 0, z - (l / 2)}, {x + (l / 2), 0, z + (l / 2)}, {x + (l / 2), l, z + (l / 2)}, {x + (l / 2), l, z - (l / 2)},
                        {x + (l / 2), 0, z + (l / 2)}, {x - (l / 2), 0, z + (l / 2)}, {x - (l / 2), l, z + (l / 2)}, {x + (l / 2), l, z + (l / 2)},
                        {x - (l / 2), 0, z + (l / 2)}, {x - (l / 2), 0, z - (l / 2)}, {x - (l / 2), l, z - (l / 2)}, {x - (l / 2), l, z + (l / 2)},
                        {x - (l / 2), l, z - (l / 2)}, {x + (l / 2), l, z - (l / 2)}, {x + (l / 2), l, z + (l / 2)}, {x - (l / 2), l, z + (l / 2)}
                    };
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[0]);
    glVertex3iv(cannon[1]);
    glVertex3iv(cannon[2]);
    glVertex3iv(cannon[3]);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[4]);
    glVertex3iv(cannon[5]);
    glVertex3iv(cannon[6]);
    glVertex3iv(cannon[7]);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[8]);
    glVertex3iv(cannon[9]);
    glVertex3iv(cannon[10]);
    glVertex3iv(cannon[11]);
    glEnd();
        
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[12]);
    glVertex3iv(cannon[13]);
    glVertex3iv(cannon[14]);
    glVertex3iv(cannon[15]);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[16]);
    glVertex3iv(cannon[17]);
    glVertex3iv(cannon[18]);
    glVertex3iv(cannon[19]);
    glEnd();

    glColor3f(1.0, 1.0, .0);
    glBegin(GL_QUADS);
    glVertex3iv(cannon[20]);
    glVertex3iv(cannon[21]);
    glVertex3iv(cannon[22]);
    glVertex3iv(cannon[23]);
    glEnd();
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glRotatef(global.angle[0], 1.0, 0.0, 0.0);
    glRotatef(global.angle[1], 0.0, 1.0, 0.0);
    glRotatef(global.angle[2], 0.0, 0.0, 1.0);

    displayGround();

    displayCannon();

    displayParticles();

    glutSwapBuffers();
    glFlush();
}

float randomFloat(float min, float max) {
    float random = (float)rand() / RAND_MAX;
    return min + random * (max - min);
}

//this creates a new particle and adds it to the list
void fireParticle(void) {
    float dx, speed = 1.0;
    dx = randomFloat(-0.2, 0.2); //random (-0.5, 0.5)
    if (global.speedInvoked) {
        speed = randomFloat(1, 5);
    }
    global.particles.push_back(Particle(dx, 1, 1, speed));
    if (global.particles.size() > 500) {
        global.particles.erase(global.particles.begin());//oldest at front newest at back
    }
}

void reset(void) {
    global.particles.clear();
    global.angle[X] = 0.0;
    global.angle[Y] = 0.0;
    global.angle[Z] = 0.0;
    global.axis = 1;
    glPopMatrix();
    glPushMatrix();
}

void keyUp(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'f':
    case 'F':
        global.kPressed = 0;
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case '1':
        global.fireMode = 0;
        break;
    case '2':
        global.fireMode = 1;
        break;
    case '3':
        global.fireMode = 2;
        break;
    case 's':
    case 'S':
    {
        if (global.speedInvoked) {
            global.speedInvoked = 0;
        }
        else {
            global.speedInvoked = 1;
        }
        break;
    }
    case 'f':
    case 'F':
    {
        if (global.fireMode == 1) {
            fireParticle();           
        }
        else if (global.fireMode == 2 && !global.kPressed) {
            global.kPressed = 1;
            fireParticle();
        }
        break;
    }     
    case 'x':
    case 'X':
        global.axis = X;
        break;

    case 'y':
    case 'Y':
        global.axis = Y;
        break;

    case 'z':
    case 'Z':
        global.axis = Z;
        break;
    case 'r':
    case 'R':
        reset();
        break;
    case 0x1B:
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
}

void mouse(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (btn == GLUT_LEFT_BUTTON) {
            global.angle[global.axis] = global.angle[global.axis] + 0.2;
        }
        else if (btn == GLUT_RIGHT_BUTTON) {
            global.angle[global.axis] = global.angle[global.axis] - 0.2;
        }
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    userintro();
    glutInit(&argc, argv);
    glutInitWindowSize(global.screenSizeX, global.screenSizeY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Particle Cannon");
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(display);
    glutTimerFunc(global.frameRate, update, 0);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(70.0, 1.0, 0.1, -100.0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(70, 60.0, 100.0, 0, 0, 0, 0.0, 1.0, 0.0);

    glPushMatrix();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    //TODO: add toggle for face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glutMainLoop();
}
