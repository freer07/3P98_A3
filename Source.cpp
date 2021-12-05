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

int isOffLedge(float x, float z) {
    if ((z > 50 && z < 70 && x > -10 && x < 10) || x > 100 || x < -100 || z > 100 || z < -100) {
        return 1;
    }
    return 0;
}

int* getNormal(int pointA[3], int pointB[3], int pointC[3]) {
    int* normal = (int*)malloc(sizeof(int) * 3);
    int* vecA = (int*)malloc(sizeof(int) * 3);
    int* vecB = (int*)malloc(sizeof(int) * 3);
    vecA[0] = pointB[0] - pointA[0];
    vecA[1] = pointB[1] - pointA[1];
    vecA[2] = pointB[2] - pointA[2];

    vecB[0] = pointC[0] - pointA[0];
    vecB[1] = pointC[1] - pointA[1];
    vecB[2] = pointC[2] - pointA[2];

    normal[0] = vecA[1] * vecB[2] - vecA[2] * vecB[1];
    normal[1] = vecA[2] * vecB[0] - vecA[0] * vecB[2];
    normal[2] = vecA[0] * vecB[1] - vecA[1] * vecB[0];

    return normal;
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
            if (isOffLedge(particle.position[0], particle.position[2])) {
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
    printf("G = Gouraud shaded\n");
    printf("H = (default)Flat shaded\n");
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
        GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat mat_specular[] = { 0.9, 0.0, 0.0, 1.0 };
        GLfloat shiniess[] = { 0.80 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

        glColor3f(0.0, 0.0, 1.0);
        int* normal = (int*)malloc(sizeof(int)*3);
        normal[0] = 0;
        normal[1] = 1;
        normal[2] = 0;
        glNormal3iv(normal);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glVertex3f(particle.position[0], particle.position[1], particle.position[2]);
        glEnd();
    }
}

void drawParticles(void) {
    for (auto& particle : global.particles) // access by reference to avoid copying
    {

        GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat mat_specular[] = { 0.9, 0.0, 0.0, 1.0 };
        GLfloat shiniess[] = { 0.80 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

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
        float l = particle.size;
        float x = particle.position[0], y = particle.position[1], z = particle.position[2];
        int poly = particle.shape;
        //float triangle[][3] = { {x, y + (l / 2), z}, {x - (l / 2), y - (l / 2), z}, {x, y - (l / 2), z - (l / 2)} };
        if (poly == 0) {
            int cube[][3] = { {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)},
                                {x - (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)},
                                {x + (l / 2), y - (l / 2), z - (l / 2)}, {x + (l / 2), y - (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)},
                                {x + (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)},
                                {x - (l / 2), y - (l / 2), z + (l / 2)}, {x - (l / 2), y - (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z - (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)},
                                {x - (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z - (l / 2)}, {x + (l / 2), y + (l / 2), z + (l / 2)}, {x - (l / 2), y + (l / 2), z + (l / 2)}
            };
            glColor3f(1.0, 0.0, 0.0);
            int* normal0 = getNormal(cube[2], cube[1], cube[0]);
            glNormal3iv(normal0);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[0]);
            glVertex3iv(cube[1]);
            glVertex3iv(cube[2]);
            glVertex3iv(cube[3]);
            glEnd();

            glColor3f(1.0, 1.0, 0.0);
            int* normal1 = getNormal(cube[6], cube[5], cube[4]);
            glNormal3iv(normal1);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[4]);
            glVertex3iv(cube[5]);
            glVertex3iv(cube[6]);
            glVertex3iv(cube[7]);
            glEnd();

            glColor3f(0.0, 1.0, 0.0);
            int* normal2 = getNormal(cube[10], cube[9], cube[8]);
            glNormal3iv(normal2);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[8]);
            glVertex3iv(cube[9]);
            glVertex3iv(cube[10]);
            glVertex3iv(cube[11]);
            glEnd();

            glColor3f(0.0, 1.0, 1.0);
            int* normal3 = getNormal(cube[14], cube[13], cube[12]);
            glNormal3iv(normal3);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[12]);
            glVertex3iv(cube[13]);
            glVertex3iv(cube[14]);
            glVertex3iv(cube[15]);
            glEnd();

            glColor3f(0.0, 0.0, 1.0);
            int* normal4 = getNormal(cube[18], cube[17], cube[16]);
            glNormal3iv(normal4);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[16]);
            glVertex3iv(cube[17]);
            glVertex3iv(cube[18]);
            glVertex3iv(cube[19]);
            glEnd();

            glColor3f(1.0, 1.0, 0.0);
            int* normal5 = getNormal(cube[22], cube[21], cube[20]);
            glNormal3iv(normal5);
            glBegin(GL_POLYGON);
            glVertex3iv(cube[20]);
            glVertex3iv(cube[21]);
            glVertex3iv(cube[22]);
            glVertex3iv(cube[23]);
            glEnd();
        }
        if (poly == 1) {
            int tetra[][3] = {
                               {x, y + (l / 2), z},
                               {x - (l / 2), y - (l / 2), z + (l / 2)},
                               {x + (l / 2), y - (l / 2), z},
                               {x - (l / 2), y - (l / 2), z - (l / 2)},
            };
            //glColor3f(0.0, 0.0, 0.1);
            //glBegin(GL_TRIANGLE_STRIP);
            //for (int i = 0; i < 4; i++) {
            //    /*glColor3fv(color[c]);
            //    c++;*/
            //    glVertex3fv(tetra[i]);
            //}
            //glColor3fv(color[0]);
            //glVertex3fv(tetra[0]);
            //glColor3fv(color[1]);
            //glVertex3fv(tetra[1]);
            //glEnd();

            glColor3f(1.0, 0.0, 0.0);
            int* normal0 = getNormal(tetra[2], tetra[3], tetra[0]);
            glNormal3iv(normal0);
            glBegin(GL_POLYGON);
            glVertex3iv(tetra[0]);
            glVertex3iv(tetra[3]);
            glVertex3iv(tetra[2]);
            glEnd();

            glColor3f(1.0, 0.0, 0.0);
            int* normal1 = getNormal(tetra[3], tetra[1], tetra[0]);
            glNormal3iv(normal1);
            glBegin(GL_POLYGON);
            glVertex3iv(tetra[0]);
            glVertex3iv(tetra[1]);
            glVertex3iv(tetra[3]);
            glEnd();

            glColor3f(1.0, 0.0, 0.0);
            int* normal2 = getNormal(tetra[1], tetra[2], tetra[0]);
            glNormal3iv(normal2);
            glBegin(GL_POLYGON);
            glVertex3iv(tetra[0]);
            glVertex3iv(tetra[2]);
            glVertex3iv(tetra[1]);
            glEnd();

            glColor3f(1.0, 0.0, 0.0);
            int* normal3 = getNormal(tetra[2], tetra[3], tetra[1]);
            glNormal3iv(normal3);
            glBegin(GL_POLYGON);
            glVertex3iv(tetra[1]);
            glVertex3iv(tetra[3]);
            glVertex3iv(tetra[2]);
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

void displayGround(void) {
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat shiniess[] = { 0.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

    glColor3f(1.0, 1.0, 1.0);
    int* normal0 = getNormal(global.ground[2], global.ground[1], global.ground[0]);
    glNormal3iv(normal0);
    glBegin(GL_POLYGON);
    glVertex3iv(global.ground[0]);
    glVertex3iv(global.ground[1]);
    glVertex3iv(global.ground[2]);
    glVertex3iv(global.ground[3]);
    glEnd();

    int* normal1 = getNormal(global.ground[6], global.ground[5], global.ground[4]);
    glNormal3iv(normal1);
    glBegin(GL_POLYGON);
    glVertex3iv(global.ground[4]);
    glVertex3iv(global.ground[5]);
    glVertex3iv(global.ground[6]);
    glVertex3iv(global.ground[7]);
    glEnd();

    int* normal2 = getNormal(global.ground[10], global.ground[9], global.ground[8]);
    glNormal3iv(normal2);
    glBegin(GL_POLYGON);
    glVertex3iv(global.ground[8]);
    glVertex3iv(global.ground[9]);
    glVertex3iv(global.ground[10]);
    glVertex3iv(global.ground[11]);
    glEnd();

    int* normal3 = getNormal(global.ground[14], global.ground[13], global.ground[12]);
    glNormal3iv(normal3);
    glBegin(GL_POLYGON);
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
    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.3, 1.0 };
    GLfloat shiniess[] = { 0.30 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shiniess);

    glColor3f(1.0, 0.0, 0.0);
    int* normal0 = getNormal(cannon[2], cannon[1], cannon[0]);
    glNormal3iv(normal0);
    glBegin(GL_POLYGON);
    glVertex3iv(cannon[0]);
    glVertex3iv(cannon[1]);
    glVertex3iv(cannon[2]);
    glVertex3iv(cannon[3]);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);
    int* normal1 = getNormal(cannon[6], cannon[5], cannon[4]);
    glNormal3iv(normal1);
    glBegin(GL_POLYGON);
    glVertex3iv(cannon[4]);
    glVertex3iv(cannon[5]);
    glVertex3iv(cannon[6]);
    glVertex3iv(cannon[7]);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    int* normal2 = getNormal(cannon[10], cannon[9], cannon[8]);
    glNormal3iv(normal2);
    glBegin(GL_POLYGON);
    glVertex3iv(cannon[8]);
    glVertex3iv(cannon[9]);
    glVertex3iv(cannon[10]);
    glVertex3iv(cannon[11]);
    glEnd();
        
    glColor3f(0.0, 1.0, 1.0);
    int* normal3 = getNormal(cannon[14], cannon[13], cannon[12]);
    glNormal3iv(normal3);
    glBegin(GL_POLYGON);
    glVertex3iv(cannon[12]);
    glVertex3iv(cannon[13]);
    glVertex3iv(cannon[14]);
    glVertex3iv(cannon[15]);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    int* normal4 = getNormal(cannon[18], cannon[17], cannon[16]);
    glNormal3iv(normal4);
    glBegin(GL_POLYGON);
    glVertex3iv(cannon[16]);
    glVertex3iv(cannon[17]);
    glVertex3iv(cannon[18]);
    glVertex3iv(cannon[19]);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);
    int* normal5 = getNormal(cannon[22], cannon[21], cannon[20]);
    glNormal3iv(normal5);
    glBegin(GL_POLYGON);
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

    //displayParticles();
    drawParticles();

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
    dx = randomFloat(-0.2, 0.2); 
    if (global.speedInvoked) {
        speed = randomFloat(1, 5);
    }
    int polygon = rand() % 3 + 0;
    int size = rand() % 8 + 2;
    global.particles.push_back(Particle(dx, 1, 1, speed, polygon, size));
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
    case 'h':
    case 'H':
        glShadeModel(GL_FLAT);
        break;
    case 'g':
    case 'G':
        glShadeModel(GL_SMOOTH);
        break;
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

void lightInit() {
    GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 0.5, 0.5, 1.0, 0.0 };
    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat local_view[] = { 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glShadeModel(GL_FLAT);

    glEnable(GL_LIGHTING);   /* turns on lighting */
    glEnable(GL_LIGHT0);     /* turns on light 0  */
    glEnable(GL_NORMALIZE);
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
    lightInit();

    //TODO: add toggle for face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glutMainLoop();
}
