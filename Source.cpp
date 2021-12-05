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

int isInHole(float x, float z) {
    if (z > 50 && z < 70 && x > -10 && x < 10) {
        return 1;
    }
    return 0;
}

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

void GlowTrail(void) {
    for (auto& particle : global.particles) // access by reference to avoid copying
    {
        GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat mat_specular[] = { 0.8, 0.6, 0.0, 0.6 };
        GLfloat shiniess[] = { 0.80 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

        glColor3f(0.0, 0.0, 1.0);
        int* normal = (int*)malloc(sizeof(int) * 3);
        normal[0] = 0;
        normal[1] = 1;
        normal[2] = 0;
        glNormal3iv(normal);
        glPointSize(2.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 2; i++) {
            glVertex3f(particle.position[0], particle.position[1], particle.position[2] - 1);
            glVertex3f(particle.position[0], particle.position[1], particle.position[2] - 5);
        }
        glEnd();
    }
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

void drawParticles() {
    for (auto& particle : global.particles) // access by reference to avoid copying
    {
        int numsides;
        float color[][3] = {
                            {1.0,0.0,0.0},
                            {0.0,1.0,0.0},
                            {0.0,0.0,1.0},
                            {0.0,1.0,1.0},
                            {1.0,1.0,0.0},
                            {0.0,1.0,1.0}
        };
        int c = 0;
        float l = 15;
        float x = particle.position[0], y = particle.position[1], z = particle.position[2];
        int poly = particle.shape;
        //float triangle[][3] = { {x, y + (l / 2), z}, {x - (l / 2), y - (l / 2), z}, {x, y - (l / 2), z - (l / 2)} };
        if (poly == 0) {
            float cube[][3] = { {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)},
                                {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)},
                                {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)},
                                {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)},
                                {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)},
                                {x - (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}
            };
            glColor3f(0.9, 0.0, 0.0);
            glBegin(GL_QUADS);
            for (int i = 0; i < 24; i++) {
                /*if (i % 4 == 0) {
                    glColor3fv(color[c]);
                    c++;
                }*/
                glVertex3fv(cube[i]);
            }
            glEnd();
        }
        if (poly == 1) {
            float tetra[][3] = {
                               {x, y + (l / 2), z},
                               {x - (l / 2), y - (l / 2), z + (l / 2)},
                               {x + (l / 2), y - (l / 2), z},
                               {x - (l / 2), y - (l / 2), z - (l / 2)},
            };
            glColor3f(0.0, 0.0, 0.1);
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 4; i++) {
                /*glColor3fv(color[c]);
                c++;*/
                glVertex3fv(tetra[i]);
            }
            glColor3fv(color[0]);
            glVertex3fv(tetra[0]);
            glColor3fv(color[1]);
            glVertex3fv(tetra[1]);
            glEnd();
        }
        if (poly == 2) {
            glColor3f(0.6, 0.6, 0.6);
            glPointSize((float)l);
            glBegin(GL_POINTS);
            glVertex3f(particle.position[0], particle.position[1], particle.position[2]);
            glEnd();
        }
    }
}

//void drawParticles(void) {
    //for (auto& particle : global.particles) // access by reference to avoid copying
    //{
    //    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    //    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    //    GLfloat mat_specular[] = { 0.0, 0.7, 0.9, 1.0 };
    //    GLfloat shiniess[] = { 0.80 };
    //    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //    glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

    //    //glColor3f(0.0, 0.0, 1.0);
    //    float l = 2, x = particle.position[0], y = particle.position[1], z = particle.position[2];
    //    int* normal = (int*)malloc(sizeof(int) * 3);
    //    normal[0] = 0;
    //    normal[1] = 1;
    //    normal[2] = 0;
    //    glNormal3iv(normal);
    //    float cube[][3] = {
    //                        {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)},
    //                        {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)},
    //                        {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)},
    //                        {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)},
    //                        {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)},
    //                        {x - (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}
    //    };
    //    glBegin(GL_QUADS);
    //    for (int i = 0; i < 24; i++) {
    //        glVertex3fv(cube[i]);
    //    }
    //    glEnd();
    //}
//}

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
    displayGround();

    displayCannon();

    //displayParticles();
    drawParticles();

    GlowTrail();

    glutSwapBuffers();
    glFlush();
}

//this creates a new particle and adds it to the list
void fireParticle(void) {
    global.particles.push_back(Particle());
    int polygon = rand() % 3 + 0;
    global.particles.at(global.particles.size() - 1).shape = polygon;
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
    //glOrtho(-1*global.orthoX, global.orthoX, -1*global.orthoY, global.orthoY, -1*global.orthoZ, global.orthoZ);
    gluPerspective(70.0, 1.0, 0.1, -100.0);

    glMatrixMode(GL_MODELVIEW);
    //glRotatef(25.0, 1.0, 0.0, 0.0);//rotate the viewing angle 
    //glRotatef(70.0, 0.0, 1.0, 0.0);//rotate the viewing angle 
    gluLookAt(70, 60.0, 100.0, 
        0, 0, 0, 
        0.0, 1.0, 0.0);
    //gluLookAt(70, 60.0, 100.0, 0, 0, 0, 0.0, 1.0, 0.0);

    glPushMatrix();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glutMainLoop();
}
