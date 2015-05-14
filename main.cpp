using namespace std;
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <list>

using namespace std;
#include <vector>

/*
 * rajat arora-1205519
 * This assignment was runned on eclipse for gnome
 * For some weird reason I am missing some files and get /usr/bin/ld: /tmp/ccgXpq30.o: undefined reference to symbol '_ZSt17__throw_bad_allocv@@GLIBCXX_3.4'
* and //usr/lib/x86_64-linux-gnu/libstdc++.so.6: error adding symbols: DSO missing from command line
* when I compile using make.  I think it's because I am using a beta version of gonome
*It works fine in eclipse though
*If you wanna try eclipse. Open eclipse on ubuntu or gonme, new c++ project, click on project properties, then click on c++/build
*then settings and cross g++ linker then libraries
* from there then add GLU,glut,GL
*and click ok
*and then copy my source code and run the file :)
 * Things implemented
 * 1.Particle Lifespan -> random life's
 * 2.Particle Spin
 * 3.Particle Floor collision
 * 4.Friction mode: "f" key
 * 5.Scene rotation: arrow keys
 * 6.Start/stop sim: space bar
 * 7. Reset: "r" key
 * 8. Backface culling,double buffer, perspective projection
 * 9. Lighting
 * 10.Particle cam- "c" key
 * 17. Wind - "w" key
 */


//contains the id=-1 if this is the particle we are following for camera
int id;

//this is for the platform
//taken from profs notes
float verts[8][3] = { { -1, -1, 1 }, { -1, 1, 1 }, { 1, 1, 1 }, { 1, -1, 1 }, {
		-1, -1, -1 }, { -1, 1, -1 }, { 1, 1, -1 }, { 1, -1, -1 } };
float cols[6][3] = { { 1, 0, 0 }, { 0, 1, 1 }, { 1, 1, 0 }, { 0, 1, 0 }, { 0, 0,
		1 }, { 1, 0, 1 } };

//camera stuff
float pos[] = { 0, 1, 0 };
float rot[] = { 0, 0, 0 };
float headRot[] = { 0, 0, 0 };
float camPos[] = { 31, 10, 41 };
float camPos2[] = { 31, 10, 41 };

//angle for ball to spin
float angle = 0.0f;

//our variables for button pressing checking
bool fric = false;
bool pause = false;
bool wind = false;
bool camera = false;

//timer recursively called
void timer(int value) {
	glutTimerFunc(1, timer, 0);
}


//draws a the platform
//taken from profs notes
void drawPolygon(int a, int b, int c, int d, float v[8][3]) {
	glBegin(GL_POLYGON);
	glVertex3fv(v[a]);
	glVertex3fv(v[b]);
	glVertex3fv(v[c]);
	glVertex3fv(v[d]);
	glEnd();
}


//our particle
struct particle {

	float x;
	float y;
	float z;
	double speed;
	double size;
	int color;
	int angle;
	int age; //num of iterations
	double dx, dy, dz;
	int up_down;//checks if particle is going upwards or down
	double g;//gravity
	int id;//id talked about earlier

	int counter;//counter for num of iterations so far of age
} p;

vector<particle> v;

/* cube - takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box
 *  taken from profs notes
 */
void cube(float v[8][3]) {
	glColor3fv(cols[1]);
	drawPolygon(0, 3, 2, 1, v);

	glColor3fv(cols[2]);
	drawPolygon(1, 0, 4, 5, v);

	glColor3fv(cols[3]);
	drawPolygon(5, 1, 2, 6, v);

	glColor3fv(cols[4]);
	drawPolygon(2, 3, 7, 6, v);

	glColor3fv(cols[5]);
	drawPolygon(6, 5, 4, 7, v);

	glColor3fv(cols[0]);
	drawPolygon(4, 0, 3, 7, v);
}

//draw anew point on screen
void newPoint() {
	//default values
	p.x = -10;
	p.y = 10;
	p.z = -10;
	//color is randomized
	p.color = rand() % 6 + 0;
	p.angle = 35;
	int r = rand() % 70 + 0;
	p.age = r; //num of iterations
	//gravity set to default will be randomly reduced later
	p.g = 10;

	//randomize dx,dy
	p.dx = (float) rand() / RAND_MAX; //0.1;
	p.dz = (float) rand() / RAND_MAX;
	//particle initally going down
	p.up_down = 0;
}

//draw our points on screen

void drawPoint() {

	//new random point
	newPoint();
	v.push_back(p);

	//checks weather not camera button pressed
	bool temp = false;
	//iterate through all points
	for (int i = 0; i != (int)v.size(); i++) {


		//if cam pressed, find point, and set temp true, to say we have found point to follow
		if (camera != false) {

			if (v.at(i).id == -1) {

				temp = true;
			}

		}


       //if friction
		if (fric == false) {
			//keep moving points in x,z till end of platform
			if ((v.at(i).x < 13 || v.at(i).z < 13)) {

			 //if no wind
				if (wind == false) {
					//move x,z in pos
					v.at(i).x = v.at(i).x + v.at(i).dx;
					v.at(i).z = v.at(i).z + v.at(i).dz;
				} else {
					//move, x in opp dir if wind
					v.at(i).x = v.at(i).x - v.at(i).dx;
					v.at(i).z = v.at(i).z + v.at(i).dz;
				}

				//make particle go downwards
				if (v.at(i).y >= 0.9 && v.at(i).up_down == 0) {
					v.at(i).y = v.at(i).y - 0.1;

				}
				//if particle fully down, then up_down = 1, and we go upwards
				if (v.at(i).y < 1) {
					v.at(i).up_down = 1;

				}

				//go back up
				if (v.at(i).up_down == 1) {
					v.at(i).y = v.at(i).y + 0.1;
				}
				//when we reach max height, start going back down again
				//and using gravity reduce height by a factor
				if (v.at(i).y >= v.at(i).g) {

					v.at(i).up_down = 0;
					v.at(i).g = v.at(i).g
							- v.at(i).g * (float) rand() / RAND_MAX + 1;

				}
				//if we are past platform
			} else {

				//if particle is still going downwards when we pass platform
				//let it
				if (v.at(i).y > 0.9) {
					v.at(i).y = v.at(i).y - 0.1;
					if (wind == false) {
						v.at(i).x = v.at(i).x + v.at(i).dx;
						v.at(i).z = v.at(i).z + v.at(i).dz;
					} else {
						v.at(i).x = v.at(i).x - v.at(i).dx;
						v.at(i).z = v.at(i).z + v.at(i).dz;
					}

				}
               //if particle is fully down then start age checking
				if (v.at(i).y <= 0.9) {
					if (v.at(i).counter != v.at(i).age) {
						v.at(i).counter++;

					} else {
						//if age is fully complete kill from list
						//and if we are following particle, end the particle tracking
						//and reset cam
						v.erase(v.begin() + i);
						if (temp == true) {
							camera = false;
							camPos[0] = camPos2[0];
							camPos[1] = camPos2[1];
							camPos[2] = camPos2[2];
							temp = false;
						}

					}

				}

			}

			//if the particle is going upwards after passing platform and y not fully 0.9 yet
		} else {

			//move depending on wind or not

			if (wind == false) {
				v.at(i).x = v.at(i).x + v.at(i).dx;
				v.at(i).z = v.at(i).z + v.at(i).dz;
			} else {
				v.at(i).x = v.at(i).x - v.at(i).dx;
				v.at(i).z = v.at(i).z + v.at(i).dz;
			}
			//if particle down let it

			if (v.at(i).y >= 0.9 && v.at(i).up_down == 0) {
				v.at(i).y = v.at(i).y - 0.1;
			}
			//if particle fully down then go back up
			if (v.at(i).y < 1) {
				v.at(i).up_down = 1;

			}
			//go back up
			if (v.at(i).up_down == 1) {
				v.at(i).y = v.at(i).y + 0.1;
			}
		}


		//if we are following the current particle
		//then change camera position to match that particle
		if (temp == true) {
			camPos[0] = v.at(i).x;
			camPos[1] = v.at(i).y;
			camPos[2] = v.at(i).z;
			temp = false;

		}

		//translate and rotate the point and draw it
		glPushMatrix();
		glColor3f(1, 0, 1);
		v.at(i).angle = v.at(i).angle + 35;

		glRotatef(v.at(i).angle, v.at(i).x, v.at(i).y, v.at(i).z);
		glTranslatef(v.at(i).x, v.at(i).y, v.at(i).z);

		glColor3fv(cols[v.at(i).color]);
		glutSolidSphere(0.20, 10, 10);
		glPopMatrix();

	}
}


/* drawBox - takes centre point, width, height and depth of a box,
 *  calculates its corner vertices, and draws it with the cube function
 *  taken from profs notes
 */
void drawBox(float* c, float w, float h, float d) {
	float vertices[8][3] = { { c[0] - w / 2, c[1] - h / 2, c[2] + d / 2 }, {
			c[0] - w / 2, c[1] + h / 2, c[2] + d / 2 }, { c[0] + w / 2, c[1]
			+ h / 2, c[2] + d / 2 },
			{ c[0] + w / 2, c[1] - h / 2, c[2] + d / 2 }, { c[0] - w / 2, c[1]
					- h / 2, c[2] - d / 2 }, { c[0] - w / 2, c[1] + h / 2, c[2]
					- d / 2 }, { c[0] + w / 2, c[1] + h / 2, c[2] - d / 2 }, {
					c[0] + w / 2, c[1] - h / 2, c[2] - d / 2 } };

	cube(vertices);
}


//key board function taken from profs notes

void keyboard(unsigned char key, int x, int y) {

	switch (key) {

	//quit game
	case 'q':
	case 27:
		exit(0);
		break;

		//friction
	case 'f':
		if (fric == false) {
			fric = true;
		} else {
			fric = false;
		}

		break;
		//camera
	case 'c':
		if (camera == false) {

			//store old camera values in camera temp
			camera = true;
			camPos2[0] = camPos[0];
			camPos2[1] = camPos[1];
			camPos2[2] = camPos[2];
			camPos[0] = v.back().x;
			camPos[1] = v.back().y;
			camPos[2] = v.back().z;
           //set current point we are tracking to neg 1
			v.back().id = -1;
			id = -1;

		} else {
			//reset camera to camera temp
			camera = false;
			camPos[0] = camPos2[0];
			camPos[1] = camPos2[1];
			camPos[2] = camPos2[2];
		}

		break;
		//space pause
	case 32:
		if (pause == false) {
			pause = true;
		} else {
			pause = false;
		}

		break;


 //wind
	case 'w':
		if (wind == false) {
			wind = true;
		} else {
			wind = false;
		}

		break;
		//clear
	case 'r':
		v.clear();

		break;

	}

	//if we are paused stop animation
	if (pause == false) {
		glutPostRedisplay();
	}
}


//for special keys
//taken from profs notes
//depeding on key pressed move camera
void special(int key, int x, int y) {
	//printf("%f  %f  %f /n", camPos[0], camPos[1], camPos[2]);
	switch (key) {
	case GLUT_KEY_LEFT:
		camPos[0] -= 0.1;
		break;

	case GLUT_KEY_RIGHT:
		camPos[0] += 0.1;
		break;

	case GLUT_KEY_UP:
		camPos[2] -= 0.1;
		break;

	case GLUT_KEY_DOWN:
		camPos[2] += 0.1;
		break;

	case GLUT_KEY_HOME:
		camPos[1] += 0.1;
		break;

	case GLUT_KEY_END:
		camPos[1] -= 0.1;
		break;

	}
	if (pause == false) {
		glutPostRedisplay();
	}
}

//store position of light
float L1Pos[4] = { 10, 10, 10, 0 };


void init(void) {

	//clear back buffer color
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);


  //start lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);//show color on screen for lighting
	glEnable(GL_CULL_FACE);//cull face

	glLightfv(GL_LIGHT0, GL_POSITION, L1Pos); ////move light to postion
	glLightf(GL_LIGHT0, GL_SHININESS, 30);

	//enable the light
	glEnable(GL_LIGHT0);

	//set projection view taken from profs notes
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, 1, 1, 100);
}

/* display function - GLUT display callback function
 *clear screen, move camera to init position
 *draw points
 *draw Box platform
 */
void display(void) {
	float origin[3] = { 0, 0, 0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0, 1, 0);
	glColor3f(1, 1, 1);
	drawPoint();
	drawBox(origin, 40, 1, 40);

	glPopMatrix();
	glutSwapBuffers();
}

void idle(void) {
	glutTimerFunc(15, timer, 0);
	if (pause == false) {
		glutPostRedisplay();
	}
}

/* Our Main- taken from prof's notes */
int main(int argc, char** argv) {

	glutInit(&argc, argv);		//start up glut

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//double buffer //depth buffer, rgb

	glutIdleFunc(idle);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Particle");	//create the particle window

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	init();

	glutMainLoop();

	return (0);
}
