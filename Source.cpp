// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>


#define X 0
#define Y 1
#define Z 2

struct glob {
    float angle[3];
    int axis;
};

struct glob global = { {0.0,0.0,0.0},Y };

void userintro() {
    printf("Left mouse = rotate left faster\n");
    printf("Right mouse = rotate right faster\n");
    printf("Middle mouse = reset\n");
    printf("x, y, z = rotate about x, y, or z axis\n");
    printf("q = quit\n");
}

void drawcube(void) {
    int p[][3] = { {10,10,10}, {10,-10,10}, {-10,-10,10}, {-10,10,10},
                   {10,10,-10}, {10,-10,-10}, {-10,-10,-10}, {-10,10,-10} };

    int e[][4] = { {0,3,2,1},{3,7,6,2},{7,4,5,6},{4,0,1,5}, {0,4,7,3},{1,2,6,5} };
    float c[][3] = { {1.0,0,0},{0,1.0,0},{1.0,1.0,1.0},
           {0,0,1.0},{.6,0,.6},{0,.6,.6} };
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glRotatef(global.angle[X], 1.0, 0.0, 0.0);
    glRotatef(global.angle[Y], 0.0, 1.0, 0.0);
    glRotatef(global.angle[Z], 0.0, 0.0, 1.0);

    for (i = 0; i < 6; ++i) {
        glColor3fv(c[i]);
        glBegin(GL_QUADS);
        glVertex3iv(p[e[i][0]]);
        glVertex3iv(p[e[i][1]]);
        glVertex3iv(p[e[i][2]]);
        glVertex3iv(p[e[i][3]]);
        glEnd();
    }
    glutSwapBuffers();
    glFlush();
}

void draw(void) {
    int ground[][3] = { {-30,0,-30}, {30,0,-30}, {30,0,30}, {-30,0,30} };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glVertex3iv(ground[0]);
    glVertex3iv(ground[1]);
    glVertex3iv(ground[2]);
    glVertex3iv(ground[3]);
    glEnd();

    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
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

    if (state == GLUT_DOWN) {
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
    }
}

int main(int argc, char** argv) {

    userintro();
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Glut rotate 2");
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    /*glRotatef(30.0, 0.0, 0.0, 1.0);*/
    glRotatef(10.0, 1.0, 0.0, 0.0);    
    /*glRotatef(50.0, 0.0, 1.0, 0.0);*/
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
}
