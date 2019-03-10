#include <stdlib.h>
#include "tinyxml2.h"
#include "headers/engine.h"
#include "headers/Point.h"
#include "headers/parser.h"

using namespace std;
using namespace tinyxml2;

void drawPrimitives(void) {
    glBegin(GL_TRIANGLES);
    int i=0;
    bool cor = true;
    
    for (auto const& pt : points)  {
        if( i==3 ) {
            cor = !cor;
            i=0;
        }
        
        if(cor) {
            glColor3f(0.2, 02, 1);
            glVertex3f(pt->getX(), pt->getY(), pt->getZ());
        } else {
            glColor3f(0.7, 0.7, 1);
            glVertex3f(pt->getX(), pt->getY(), pt->getZ());
        }
        i++;
    }
    glEnd();
}

void MenuAjuda() {
	cout << "#_____________________________ HELP _____________________________#" << endl;
	cout << "| Usage: ./engine {XML FILE}                                     |" << endl;
	cout << "|                 [-h]                                           |" << endl;
	cout << "|   FILE:                                                        |" << endl;
	cout << "| Specify a path to an XML file in which the information about   |" << endl;
	cout << "| the models you wish to create are specified                    |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ↑ : Rotate your view up                                      |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ↓ : Rotate your view down                                    |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ← : Rotate your view to the left                             |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   → : Rotate your view to the right                            |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   F1 : Increase image                                          |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   F2 : Decrease image                                          |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   FORMAT:                                                      |" << endl;
	cout << "|   F3: Change the figure format into points                     |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   F4: Change the figure format into lines                      |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   F5: Fill up the figure                                       |" << endl;
	cout << "#________________________________________________________________#" << endl;
}

void specialKey (int key, int a, int b)
{
    (void)a;(void)b;
    switch (key)
    {
        case GLUT_KEY_UP:
            if (beta < (M_PI / 2 - step))
                beta += step;
            break;

        case GLUT_KEY_DOWN:
            if (beta > -(M_PI / 2 - step))
                beta -= step;
            break;

        case GLUT_KEY_LEFT:
            alpha -= step;
            break;

        case GLUT_KEY_RIGHT:
            alpha += step;
            break;

        case GLUT_KEY_F1:
            radius -= step;
            break;

        case GLUT_KEY_F2:
            radius += step;
            break;
	case GLUT_KEY_F3:
            line = GL_POINT;
            break;
        case GLUT_KEY_F4:
            line = GL_LINE;
            break;
        case GLUT_KEY_F5:
            line = GL_FILL;
            break;

        default:
            return;
    }
    glutPostRedisplay();
}

void renderScene(void)
{

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha),
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);
    drawPrimitives();


    // End of frame
    glutSwapBuffers();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("SOLAR_SYSTEM");
	if (argc < 2) {
		cout << "Invalid input. Use -h if you need some help." << endl;
		return 0;
	}
	else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")) {
		MenuAjuda();
		return 0;
	}
	else if (loadXMLfile(argv[1], &points) == 0) {
		// put callback registration here
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutIdleFunc(renderScene);
		glutSpecialFunc(specialKey);
	}

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}
