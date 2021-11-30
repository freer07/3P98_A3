// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <vector>
#include "Source.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

//this is the update function for the frames
void update(int val) {
    vector<int> oldParticles;
    
    int index = 0;
    //interate through the particles and update their position
    for (auto& particle : global.particles) // access by reference to avoid copying
    {
        particle.position[0] = particle.position[0] + particle.direction[0] * particle.speed;
        particle.position[1] = particle.position[1] + particle.direction[1] * particle.speed;
        particle.position[2] = particle.position[2] + particle.direction[2] * particle.speed;
        
        //gravity 
        particle.direction[1] = particle.direction[1] - global.grav;

        //ground limit (bounce)
        if (particle.position[1] < 0) {
            particle.position[1] = 0;
            particle.direction[1] = particle.direction[1] * -1;
            particle.speed = particle.speed - global.frict;
        }

        

        //prevent negative speed 
        if (particle.speed < 0) {
            particle.speed = 0;
        }
        
        if (particle.age > 150 || particle.position[1] < -100) {
            oldParticles.push_back(index);
        }

        particle.age++;
        index++;
    }

    //kill old particles
    for (auto& index : oldParticles) 
    {
        global.particles.erase(global.particles.begin() + index);
    }


    //show the frame
    glutPostRedisplay();

    //set next update timmer 
    glutTimerFunc(global.frameRate, update, 0);
}

void userintro() {
    /*printf("Left mouse = rotate left faster\n");
    printf("Right mouse = rotate right faster\n");
    printf("Middle mouse = reset\n");
    printf("x, y, z = rotate about x, y, or z axis\n");
    printf("q = quit\n");*/
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

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3iv(global.ground[0]);
    glVertex3iv(global.ground[1]);
    glVertex3iv(global.ground[2]);
    glVertex3iv(global.ground[3]);
    glEnd();

    displayCannon();

    displayParticles();

    glutSwapBuffers();
    glFlush();
}

//this creates a new particle and adds it to the list
void fireParticle(void) {
    global.particles.push_back(Particle());
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {

    case 'f':
    case 'F':
        fireParticle();
        break;
    case 0x1B:
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
}

void mouse(int btn, int state, int x, int y) {

    /*
    printf("%3d, %3d, %f %f %f\n", btn, state,
       global.angle[X],global.angle[Y],global.angle[Z]); */

    /*if (state == GLUT_DOWN) {
        if (btn == GLUT_LEFT_BUTTON) {
            global.angle[global.axis] = global.angle[global.axis] + 0.2;
        }
        else if (btn == GLUT_RIGHT_BUTTON) {
            global.angle[global.axis] = global.angle[global.axis] - 0.2;
        }
        else {
            global.angle[X] = 0.0;
            global.angle[Y] = 0.0;
            global.angle[Z] = 0.0;
            glPopMatrix();
            glPushMatrix();
        }
    }*/
}

int main(int argc, char** argv) {
    userintro();
    glutInit(&argc, argv);
    glutInitWindowSize(global.screenSizeX, global.screenSizeY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Particle Cannon");
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(global.frameRate, update, 0);
    //glutIdleFunc(draw);    

    glMatrixMode(GL_PROJECTION);
    glOrtho(-1*global.orthoX, global.orthoX, -1*global.orthoY, global.orthoY, -1*global.orthoZ, global.orthoZ);

    glMatrixMode(GL_MODELVIEW);
    glRotatef(25.0, 1.0, 0.0, 0.0);//rotate the viewing angle 
    glRotatef(70.0, 0.0, 1.0, 0.0);//rotate the viewing angle 

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
}
