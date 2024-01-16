#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <freeglut.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <math.h>
GLuint textureID; 



struct Particle {
    float x, y, z;   
    float vx, vy, vz; 
    GLfloat red, green, blue;
    float lifetime_max;
    float lifetime = 0;
    float size;
    float orx, ory, orz;
    float rotation;
    Particle() : x(0.0f), y(0.0f), z(0.0f), vx(0.0f), vy(0.0f), vz(0.0f) {}
};


typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 200.0f };

std::vector<Particle> particles;
int numParticles;
int mouseX = 0;
int mouseY = 0;
int width, height;
//bool depthMaskEnabled = true;

struct Os {
    double x;
    double y;
    double z;
    Os() : x(0.0f), y(0.0f), z(0.0f) {}

    Os(double x_val, double y_val, double z_val) : x(x_val), y(y_val), z(z_val) {}
};


void initTexture() {
    int channels;
    unsigned char *textureData = stbi_load("fire.png", &width, &height, &channels, STBI_rgb_alpha);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    stbi_image_free(textureData);
}
/*
void initParticles() {
    int newParticlesCount = static_cast<int>(rand() % 10 + 10);

    for (int i = 0; i < newParticlesCount; ++i) {
        Particle newParticle;
        newParticle.x = static_cast<float>(mouseX + (rand() % 20 + 5));
        newParticle.y = static_cast<float>(mouseY + (rand() % 20 + 5));
        newParticle.z = static_cast<double>(rand()) / RAND_MAX;;
        newParticle.vx = 0;
        newParticle.vy = -static_cast<float>((rand() % 2 + 1) + (rand() % 3));
        newParticle.vz = 0;
        newParticle.red = static_cast<double>(rand()) / RAND_MAX;
        newParticle.green = static_cast<double>(rand()) / RAND_MAX;
        newParticle.blue = static_cast<double>(rand()) / RAND_MAX;
        newParticle.lifetime_max = 25.0 + static_cast<float>(rand()) / (RAND_MAX / 15.0);
        newParticle.size = static_cast<float>(10 + (rand() % 10));
        newParticle.orx = 0;
        newParticle.ory = 0;
        newParticle.orz = 1;
        newParticle.rotation = 0;
        particles.push_back(newParticle); 
    }
}
*/
void initParticles() {
    int newParticlesCount = static_cast<int>(rand() % 10 + 10);

    for (int i = 0; i < newParticlesCount; ++i) {
        Particle newParticle;
        newParticle.x = static_cast<float>((rand() % 800 - 400));
        newParticle.y = 200;
        newParticle.z = static_cast<float>((rand() % 800 - 400));
        newParticle.vx = 0;
        newParticle.vy = -static_cast<float>((rand() % 2 + 1) + (rand() % 3));
        newParticle.vz = 0;
        //newParticle.red = static_cast<double>(rand()) / RAND_MAX;
        //newParticle.green = static_cast<double>(rand()) / RAND_MAX;
        //newParticle.blue = static_cast<double>(rand()) / RAND_MAX;
        newParticle.lifetime_max = 100.0 + static_cast<float>(rand()) / (RAND_MAX / 15.0);
        newParticle.size = static_cast<float>(10 + (rand() % 10));
        newParticle.orx = 0;
        newParticle.ory = 0;
        newParticle.orz = 1;
        newParticle.rotation = 0;
        particles.push_back(newParticle);  
    }
}

void updateParticles() {

    particles.erase(std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p) { return p.lifetime >= p.lifetime_max; }),
                   particles.end());

    for (auto &particle : particles) {

        particle.x += particle.vx;
        particle.y += particle.vy;
        particle.z += particle.vz;
        particle.lifetime += 0.1;

        Os os(0.0, 0.0, 0.0);
        Os vec(0.0, 0.0, 0.0);
        Os s(0.0, 0.0, 1.0);

        vec.x = s.x - ociste.x;
        vec.y = s.y - ociste.y;
        vec.z = s.z - ociste.z;

        os.x = s.y * vec.z - vec.y * s.z;
        os.y = s.z * vec.x - vec.z * s.x;
        os.z = s.x * vec.y - vec.x * s.y;

        double naz = (pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5))
                    *(pow(pow((double)vec.x, 2.0) + pow((double)vec.y, 2.0) + pow((double)vec.z, 2.0), 0.5));
        double brojnik = s.x*vec.x + s.y*vec.y + s.z*vec.z;
        double phi = acos(brojnik / naz) / (2*3.14159265358979323846) * 360;
        particle.rotation = phi;
        
        particle.orx = os.x;
        particle.ory = os.y;
        particle.orz = os.z;
    }
}

void drawParticles(Particle particle) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix(); // Save the current matrix
    glTranslatef(particle.x, particle.y, particle.z);
    glRotatef(particle.rotation, particle.orx, particle.ory, particle.orz);
    
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, (1.0 - particle.lifetime * 2 / particle.lifetime_max));

    glTexCoord2f(0, 0);
    glVertex3f(-particle.size / 2, -particle.size / 2, particle.z);

    glTexCoord2f(1, 0);
    glVertex3f(particle.size / 2, -particle.size / 2, particle.z);

    glTexCoord2f(1, 1);
    glVertex3f( particle.size / 2, particle.size / 2, particle.z);

    glTexCoord2f(0, 1);
    glVertex3f(-particle.size / 2, particle.size / 2, particle.z);

    glEnd();

    glPopMatrix(); // Restore the previous matrix
    particle.orx = particle.ory = 0;
    particle.orz = 1;
    particle.rotation = 0;
    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateParticles();



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::sort(particles.begin(), particles.end(), [](const Particle& a, const Particle& b) {
        return a.z > b.z;
    });

    for (auto& particle : particles) {
        drawParticles(particle);
    }



    glutSwapBuffers();
    Sleep(10);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);  
    glDepthFunc(GL_LEQUAL);   
}

void spawnParticles(int value) {
    initParticles();
    glutTimerFunc(rand() % 25, spawnParticles, 0); 
}

void mouseMotion(int x, int y) {
    mouseX = x - 400;
    mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y - 300;
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'a': ociste.x = ociste.x + 10.f;
        break;
    case 'd': ociste.x = ociste.x - 10.f;
        break;
    case 'w': ociste.y = ociste.y + 10.f;
        break;
    case 's': ociste.y = ociste.y - 10.f;
        break;
    case 'e': ociste.z = ociste.z + 10.f;
        break;
    case 'q': ociste.z = ociste.z - 10.f;
        break;
    case 'r': ociste.x = 0.0; ociste.y = 0.0;
        break;
    case 27:  exit(0);
        break;
    std::cout << "x: " << ociste.x << ", y: " << ociste.y << ", z: " << ociste.z << std::endl;
    }
    
    reshape(width,height);
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle System");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(myKeyboard);
    initTexture();
    initParticles();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    
    glutTimerFunc(0, spawnParticles, 0);
    Sleep(100);
    glutMainLoop();

    return 0;
}
