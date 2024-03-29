#include <stdlib.h>
#include "headers/tinyxml2.h"
#include "headers/Transformation.h"
#include "headers/engine.h"
#include "headers/Point.h"
#include "headers/parser.h"

using namespace std;
using namespace tinyxml2;

void drawOrbits(Transformation *t)
{
    vector<Point*> curvePoints = t->getPointsCurve();
    float cor[4]={0.2f,0.2f,0.2f,1.0f};

    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cor);
    glBegin(GL_LINE_LOOP);
    for(Point *p : curvePoints){
        float normal[3] = { -p->getX(),
            -p->getY(),
            -p->getZ()};
        t->normalize(normal);
        glNormal3fv(normal);
        float x = p->getX();
        float y = p->getY();
        float z = p->getZ();
        glVertex3f(x,y,z);
    }
    glEnd();
    glPopAttrib();
}


void applyTransformation(Transformation *t){
    float CTime = glutGet(GLUT_ELAPSED_TIME);
    if (stop != 1)
        eTime += CTime - cTime;
    cTime = CTime;
    const char* type = t->getType().c_str();
    float x = t->getX();
    float y = t->getY();
    float z = t->getZ();
    float angle = t->getAngle();
    float time = t->getTime();
    if(!strcmp(type,"translate")){
        glTranslatef(x,y,z);
    }
    if(!strcmp(type,"rotate")){
        glRotatef(angle,x,y,z);
    }
    if(!strcmp(type,"scale")){
        glScaled(x,y,z);
    }
    if(!strcmp(type,"rotateTime")){
        float nA = eTime * angle;
        glRotatef(nA,x,y,z);
    }
    if(!strcmp(type,"translateTime")){
        float p[4], deriv[4];
        float dTime = eTime *time;
        t->getGlobalCatmullRomPoint(dTime,p,deriv);
        Transformation* diffuse = NULL;
        Transformation* ambient = NULL;
        Transformation* specular = new Transformation(0, 0, 0);
        Transformation* emission = new Transformation(0, 0, 0);
        Material *m = new Material(diffuse,ambient,specular,emission);
        m->draw();
        drawOrbits(t);
        glTranslatef(p[0],p[1],p[2]);
        if(t->getDeriv()){
            float res[4];
            t->normalize(deriv);
            t->cross(deriv,t->getVetor(),res);
            t->normalize(res);
            t->cross(res,deriv,t->getVetor());
            float matrix[16];
            t->normalize(t->getVetor());
            t->rotMatrix(matrix,deriv,t->getVetor(),res);

            glMultMatrixf(matrix);
        }
    }
}

void drawSystem(Scene *scene, Group *system)
{
    glPushMatrix();

    for (Transformation *t: system->getTransformations()){
        applyTransformation(t);
    }

    vector<Shape*> shapeList = system->getShapes();
    for(vector<Shape*>::iterator shape_it = shapeList.begin(); shape_it != shapeList.end(); ++shape_it)
        (*shape_it)->draw();

    for (Group *g : system->getGroups())
        drawSystem(scene,g);

    glPopMatrix();
}


void MenuAjuda() {
    cout << "#_____________________________ HELP _____________________________#" << endl;
    cout << "| Usage: ./engine {XML FILE}                                     |" << endl;
    cout << "|                 [-h]                                           |" << endl;
    cout << "|   FILE:                                                        |" << endl;
    cout << "| Specify a path to an XML file in which the information about   |" << endl;
    cout << "| the models you wish to create are specified                    |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F7 : Rotate your view up                                     |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F8 : Rotate your view down                                   |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F9 : Rotate your view to the left                            |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F10 : Rotate your view to the right                          |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F1 : Increase image                                          |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F2 : Decrease image                                          |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F6 : Reset zoom                                              |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   FORMAT:                                                      |" << endl;
    cout << "|   F3: Change the figure format into points                     |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F4: Change the figure format into lines                      |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|   F5: Fill up the figure                                       |" << endl;
    cout << "#________________________________________________________________#" << endl;
}

void processMenu(int option)
{
    switch (option)
    {
        case 0:
            exit(0);
        case 1:
            camera->posInitialCamera();
            break;
        case 2:
            stop = 0;
            break;
        case 3:
            stop = 1;
            break;
        case -1:
            glEnable(GL_LIGHTING);
            break;
        case -2:
            glDisable(GL_LIGHTING);
            break;
    }
    glutPostRedisplay();
}

void showMenu()
{
    int moves = glutCreateMenu(processMenu);
    glutAddMenuEntry("On",2);
    glutAddMenuEntry("Off",3);
    glutCreateMenu(processMenu);
    glutAddSubMenu("System movements",moves);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void specialKey (int key, int a, int b)
{
    (void) a, (void) b;
    switch(key){
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
            camera->specialKeyCamera(key);
    }
    glutPostRedisplay();
}

void mousePress(int button, int state, int x, int y)
{
    camera->mousePress(button,state,x,y);
}

void mouseMotion(int x, int y)
{
    camera->mouseMotion(x,y);
}

void fps() {
    int time;
    char name[30];

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        float fps = frame * 1000.0/(time - timebase);
        timebase = time;
        frame = 0;
        sprintf(name,"SOLAR SYSTEM  %.2f FPS",fps);
        glutSetWindowTitle(name);
    }
}

void renderScene(void)
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt (
            camera->getXPosition(), camera->getYPosition(), camera->getZPosition(),
            camera->getOrX(), camera->getOrY(), camera->getOrZ(),
            0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);

    scene->applyLights();

    fps();
    drawSystem(scene, scene->getMainGroup());

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

void init()
{
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    //glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("SOLAR_SYSTEM");

    #ifndef __APPLE__
    if (glewInit() != GLEW_OK)
    {
        cout << "Some problem with GLEW!" << endl;
        return -1;
    }
    #endif

    if (argc < 2) {
        cout << "Invalid input. Use -h if you need some help." << endl;
        return 0;
    }
    else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")) {
        MenuAjuda();
        return 0;
    }

    init();

    scene = loadXMLfile(argv[1]);
    camera = new Camera();
    if(scene == nullptr) return 0;

    // put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutSpecialFunc(specialKey);

    // Mouse
    glutMouseFunc(mousePress);
    glutMotionFunc(mouseMotion);

    showMenu();

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}

