#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN_W  900
#define WIN_H  650

GLuint bgTex = 0;


GLuint loadBMP(const char* file) {
    FILE* f = fopen(file, "rb");
    if (!f) { printf("file not found: %s\n", file); return 0; }

    unsigned char hdr[54];
    fread(hdr, 1, 54, f);

    if (hdr[0]!='B'||hdr[1]!='M') { fclose(f); return 0; }
    if (*(short*)&hdr[28] != 24)   { printf("24bit\n"); fclose(f); return 0; }

    int off = *(int*)&hdr[10]; 
    int w   = *(int*)&hdr[18];
    int h   = *(int*)&hdr[22];
    int ah  = h < 0 ? -h : h;
    int row = (w * 3 + 3) & ~3;

    fseek(f, off, SEEK_SET);
    unsigned char* raw = (unsigned char*)malloc(row * ah);
    fread(raw, 1, row * ah, f);
    fclose(f);

    unsigned char* rgb = (unsigned char*)malloc(w * ah * 3);
    int y, x;
    for (y = 0; y < ah; y++) {
        int sy = h > 0 ? ah-1-y : y;
        for (x = 0; x < w; x++) {
            int s = sy*row + x*3, d = (y*w+x)*3;
            rgb[d]   = raw[s+2];
            rgb[d+1] = raw[s+1];
            rgb[d+2] = raw[s+0];
        }
    }
    free(raw);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, ah, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    free(rgb);

    printf("Texture OK: %s (%dx%d)\n", file, w, ah);
    return id;
}

void drawBackground() {
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bgTex);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0,     0    );
        glTexCoord2f(1, 0); glVertex2f(WIN_W, 0    );
        glTexCoord2f(1, 1); glVertex2f(WIN_W, WIN_H);
        glTexCoord2f(0, 1); glVertex2f(0,     WIN_H);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawBackground();



    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Background Texture");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    bgTex = loadBMP("background.bmp");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
