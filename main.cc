#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glut.h>

#define RGB(r, g, b) r/255.0, g/255.0, b/255.0

void animate1();
void animate2();
void scene1();
void scene2();
void display();
void reshape(int width, int height);
void keyboard_handler(unsigned char key, int x, int y);
void usage();
void screendump(const char *filename, short width, short height);

enum { scene_1, scene_2 };
int scene_number;

double x, y, z;
int angle1;
bool rotation;

double scale = 1; 
double angle2;
int sub_angle_stage_1;

int frame = 0;
int scene_numher;

int scene1AnimationDuration = 500;
int scene2AnimationDuration = 200;

int main (int argc, char *argv[]) 
{
    usage();
    /* initialize GLUT, using any commandline parameters passed to the 
       program */
    glutInit(&argc,argv);

    /* setup the size, position, and display mode for new windows */
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    /* create and set up a window */
    glutCreateWindow("LearnOpenGL");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_handler);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 2, 0, 0, 0, 0, 0, 1, 0);

    x = 0;
    y = 0;
    z = 1;
    rotation = false;
    
    /* tell GLUT to wait for events */
    glutMainLoop();
}

void usage()
{
    printf("Usage:\n");
    printf("  q, Q, ESC -- Exit\n");
    printf("  1 -- switch to scene #1\n");
    printf("  2 -- switch to scene #2\n");
    printf("  r, R -- toggle rotation\n");
    printf("  s, S -- start animating current scene\n");
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}

void scene1() 
{
    glPushMatrix();

    glRotatef(angle1, 0, 1, 0);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 1.5, -1);
    glColor3f(0.92, 0.22, 0.87);
    glutWireCone(1, 2.3, 20, 20);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, y, z);
    glColor3f(RGB(23, 8, 184));
    glutWireSphere(1, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void scene2() 
{
    glPushMatrix();

    glRotatef(angle2, 0, 1, 0);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.92, 0.22, 0.87);
    glTranslatef(0, -1, 0);
    glScalef(0.5, 0.5, 0.5);
    glScalef(scale, scale, scale);
    glutWireTorus(0.2, 0.6, 10, 30);
    glPopMatrix();

    glPushMatrix();

    glRotatef(-90, 1, 0, 0);
    glColor3f(RGB(23, 8, 184));
    glTranslatef(0, 1, 0);
    glRotatef(sub_angle_stage_1, 1, 0, 0);
    glTranslatef(0, 0, -1);


    GLUquadricObj *quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);

    gluCylinder(quad, 0.5, 0.5, 2, 30, 20);
    glPopMatrix();

    glPopMatrix();
}

void animate1() 
{
    frame++;
    x += .005;
    y -= .005;
    z -= .005;
    
    if (rotation) {
        angle1 += 1;
        angle1 = angle1 > 360 ? 0 : angle1;
    }

    y = y < -1 ? -1 : y;
    z = z < -1.5 ? -1.5 : z;

    if (z == 2.5) {
        glutIdleFunc(NULL);
    }

    glutPostRedisplay();
}

void animate2() 
{
    static bool stage1_complete = false;

    if (rotation) {
        angle2 += 1;
        angle2 = angle2 > 360 ? 0 : angle2;
    }
    sub_angle_stage_1 += 1;
    sub_angle_stage_1 = sub_angle_stage_1 > 180 ? 180 : sub_angle_stage_1;

    if (sub_angle_stage_1 >= 180)
        stage1_complete = true;

    if (stage1_complete) 
    {
        scale += 0.005;
        scale = scale > 2.0 ? 2.0 : scale;
    }
    glutPostRedisplay();
}

void display () 
{
    /* clear window */
    glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* future matrix manipulations should affect the modelview matrix */
    glMatrixMode(GL_MODELVIEW);

    if (scene_number == scene_1)  {
        scene1();
    }
    else if (scene_number == scene_2) {
        scene2();
    }

    glFlush();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y)
{
    printf("%c %d %d\n", key, x, y);
    if (key == 'q' || key == 'Q') {
        glutDestroyWindow(1);
    }
    else if (key == 0x1b) {
        glutDestroyWindow(1);
    }
    else if (key == '1') {
        scene_number = scene_1;
        glutPostRedisplay();
    } 
    else if (key == '2') {
        scene_number = scene_2;
        glutPostRedisplay();
    } 
    else if (key == 'R' || key == 'r') {
        rotation = !rotation;
    } 
    else if (key == 's' || key == 'S') {
        if (scene_number == scene_1) {
            glutIdleFunc(animate1);
        } else if (scene_number == scene_2) {
            glutIdleFunc(animate2);
        }
    }
    else if (key == 'p' || key == 'P') {
        char filename[512] = { 0 };
        sprintf(filename, "%s_%ld.tga", "screenshot", time(NULL));
        screendump(filename, 800, 600);
    }
}

/*
 * Code by Paul Bourke
 * From: http://www.paulbourke.net/dataformats/tga/
 */
void write_tga_header(FILE *fp, short width, short height)
{
   putc(0,fp);
   putc(0,fp);
   putc(2,fp);                         /* uncompressed RGB */
   putc(0,fp); putc(0,fp);
   putc(0,fp); putc(0,fp);
   putc(0,fp);
   putc(0,fp); putc(0,fp);           /* X origin */
   putc(0,fp); putc(0,fp);           /* y origin */
   putc((width & 0x00FF),fp);
   putc((width & 0xFF00) / 256,fp);
   putc((height & 0x00FF),fp);
   putc((height & 0xFF00) / 256,fp);
   putc(24,fp);                        /* 24 bit bitmap */
   putc(0,fp);
}

void screendump(const char *filename, short width, short height)
{
    unsigned int size = width * height * 3;
    unsigned char *pixels; 
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return;
    }

    write_tga_header(fp, width, height);

    pixels = new unsigned char[size];
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    fwrite(pixels, size, 1, fp);
    fclose(fp);

    delete[] pixels;
}
