// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <vector>
#include "Source.h"


void userintro() {
    /*printf("Left mouse = rotate left faster\n");
    printf("Right mouse = rotate right faster\n");
    printf("Middle mouse = reset\n");
    printf("x, y, z = rotate about x, y, or z axis\n");
    printf("q = quit\n");*/
}


void draw(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glVertex3iv(global.ground[0]);
    glVertex3iv(global.ground[1]);
    glVertex3iv(global.ground[2]);
    glVertex3iv(global.ground[3]);
    glEnd();

    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
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
    glutDisplayFunc(draw);
    glutIdleFunc(draw);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-1*global.orthoX, global.orthoX, -1*global.orthoY, global.orthoY, -1*global.orthoZ, global.orthoZ);

    glMatrixMode(GL_MODELVIEW);
    glRotatef(25.0, 1.0, 0.0, 0.0);//rotate the viewing angle 

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
}
