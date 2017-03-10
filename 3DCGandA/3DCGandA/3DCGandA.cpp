// 3DCGandA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "trackball.h"
#include "argumentParser.h"
#include <stdio.h>
#include <string.h>

//START READING HERE!!!


//////Predefined global variables

//Use the enum values to define different rendering modes 
//The mode is used by the function display and the mode is 
//chosen during execution with the keys 1-9
enum DisplayModeType { TRIANGLE = 1, FACE = 2, CUBE = 3, ARM = 4, MESH = 5, };
enum NormalsModeType { EQUAL = 1, EVERY_TRIANGLE = 2,  EVERY_VERTEX = 3};

DisplayModeType DisplayMode = TRIANGLE;

unsigned int W_fen = 800;  // screen width
unsigned int H_fen = 800;  // screen height

float LightPos[4] = { 1,1,0.4,1 };
std::vector<float> MeshVertices;
std::vector<unsigned int> MeshTriangles;
std::vector<float> MeshTriangleNormals;
std::vector<float> MeshVertexNormals;
std::vector<int> MeshVertexSumCount;

//Declare your own global variables here:
int myVariableThatServesNoPurpose;
float globalX = 0;

float angle0 = 0;
float angle1 = 0;
float angle2 = 0;
float stepSize = 2;
NormalsModeType NormalsMode = EVERY_VERTEX;

float lightStepSize = 0.1;

void printDisyplayModeType(DisplayModeType dmt) {
	switch (dmt) {
	case TRIANGLE:
		printf("TRIANGLE");
		break;
	case FACE:
		printf("FACE");
		break;
	case CUBE:
		printf("CUBE");
		break;
	case ARM:
		printf("ARM");
		break;
	case MESH:
		printf("MESH");
		break;
	default:

		break;
	}
}


////////// Draw Functions 

//function to draw coordinate axes with a certain length (1 as a default)
void drawCoordSystem(float length = 1)
{
	//draw simply colored axes

	//remember all states of the GPU
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//deactivate the lighting state
	glDisable(GL_LIGHTING);
	//draw axes
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(length, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length);
	glEnd();

	//reset to previous state
	glPopAttrib();
}

/**
* Several drawing functions for you to work on
*/

void drawTriangle()
{
	//a simple example of a drawing function for a triangle
	//1) try changing its color to red
	//2) try changing its vertex positions
	//3) add a second triangle in blue
	//4) add a global variable (initialized at 0), which represents the 
	// x-coordinate of the first vertex of each triangle
	//5) go to the function animate and increment this variable 
	//by a small value - observe the animation.

	glColor3f(1, 0, 0);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(globalX, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);

	glColor3f(0, 0, 1);
	glNormal3f(0, 0, 1);
	glVertex3f(globalX, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glEnd();

}


void drawUnitFace()
{
	//1) draw a unit quad in the x,y plane oriented along the z axis
	//2) make sure the orientation of the vertices is positive (counterclock wise)
	//3) What happens if the order is inversed?

	glColor3f(0, 1, 0);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);
	glEnd();
}

void drawUnitCube()
{
	//1) draw a cube using your function drawUnitFace
	//rely on glTranslate, glRotate, glPushMatrix, and glPopMatrix
	//the latter two influence the model matrix, as seen during the course.
	//glPushMatrix stores the current matrix and puts a copy on 
	//the top of a stack.
	//glPopMatrix pops the top matrix on the stack
	
	glPushMatrix();
	glTranslatef(0.1, 0.1, 0.1);
	glRotatef(angle0, 0, 0, 1);
	drawUnitFace();
	glPopMatrix();
}

void drawArm()
{
	//produce a three-unit arm (upperarm, forearm, hand) making use of your 
	//function drawUnitCube to define each of them
	//1) define 3 global variables that control the angles 
	//between the arm parts
	//and add cases to the keyboard function to control these values

	//2) use these variables to define your arm
	//use glScalef to achieve different arm length
	//use glRotate/glTranslate to correctly place the elements

	//3 optional) make an animated snake out of these boxes 
	//(an arm with 10 joints that moves using the animate function)

	// Upperarm.
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(angle0, 0, 0, 1);
	glPushMatrix();
	glScalef(1, 0.2, 1);
	drawUnitFace();
	glPopMatrix();

	// Forearm.
	glPushMatrix();
	glTranslatef(1, 0, 0);
	glRotatef(angle1, 0, 0, 1);
	glPushMatrix();
	glScalef(1, 0.2, 1);
	drawUnitFace();
	glPopMatrix();

	// Hand.
	glPushMatrix();
	glTranslatef(1, 0, 0);
	glRotatef(angle2, 0, 0, 1);
	glPushMatrix();
	glScalef(2, 0.2, 1);
	drawUnitFace();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawLight()
{
	//1) use glutSolidSphere to draw a sphere at the light's position LightPos
	//use glTranslatef to move it to the right location
	//to make the light source bright, follow the drawCoordSystem function
	//to deactivate the lighting temporarily and draw it in yellow

	//2) make the light position controllable via the keyboard function

	//3) add normal information to all your faces of the previous functions
	//and observe the shading after pressing 'L' to activate the lighting
	//you can use 'l' to turn it off again

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(LightPos[0], LightPos[1], LightPos[2]);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();
}


void drawMesh()
{
	//1) use the mesh data structure;
	//each triangle is defined with 3 consecutive indices in the MeshTriangles table
	//these indices correspond to vertices stored in the MeshVertices table.
	//provide a function that draws these triangles.

	//2) compute the normals of these triangles

	//3) try computing a normal per vertex as the average of the adjacent face normals
	// call glNormal3f with the corresponding values before each vertex
	// What do you observe with respect to the lighting?

	//4) try loading your own model (export it from Blender as a Wavefront obj) and replace the provided mesh file.
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < MeshTriangles.size(); i++){
		int vertexNumber = MeshTriangles[i];
		switch (NormalsMode) {
			case EQUAL:
				break;
			case EVERY_TRIANGLE:
				if (i % 3 == 0) {
					glNormal3f(MeshTriangleNormals[i], MeshTriangleNormals[i + 1], MeshTriangleNormals[i + 2]);
				}
				break;
			case EVERY_VERTEX:
				glNormal3f(MeshVertexNormals[vertexNumber * 3], MeshVertexNormals[vertexNumber * 3 + 1], MeshVertexNormals[vertexNumber * 3 + 2]);
				break;
			default:
				break;
		}

		glVertex3f(MeshVertices[vertexNumber * 3 ], MeshVertices[vertexNumber * 3 + 1], MeshVertices[vertexNumber * 3 + 2]);
	}
	glEnd();
}


void display()
{
	//set the light to the right position
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	drawLight();

	switch (DisplayMode)
	{
	case TRIANGLE:
		drawCoordSystem();
		drawTriangle();
		break;
	case FACE:
		drawCoordSystem();
		drawUnitFace();
		break;
	case CUBE:
		drawCoordSystem();
		drawUnitCube();
		break;
	case ARM:
		drawCoordSystem();
		drawArm();
		break;
	case MESH:
		drawCoordSystem();
		drawMesh();
		break;
	default:

		break;
	}
}


/**
* Animation
*/
void animate()
{
	globalX += 0.0001;
	if (globalX >= 2) {
		globalX = 0;
	}
}


//take keyboard input into account
void keyboard(unsigned char key, int x, int y)
{
	printf("key %d pressed at %d,%d\n", key, x, y);
	fflush(stdout);

	if ((key >= '1') && (key <= '9'))
	{
		DisplayMode = (DisplayModeType)(key - '0');
		printf("display mode set to ");
		printDisyplayModeType(DisplayMode);
		printf("\n");
		return;
	}

	switch (key)
	{
	case 27:     // touche ESC
		exit(0);
	case 'L':
		//turn lighting on
		glEnable(GL_LIGHTING);
		break;
	case 'l':
		//turn lighting off
		glDisable(GL_LIGHTING);
		break;
	case 'q':
		angle0 += stepSize;
		break;
	case 'a':
		angle0 -= stepSize;
		break;
	case 'w':
		angle1 += stepSize;
		break;
	case 's':
		angle1 -= stepSize;
		break;
	case 'e':
		angle2 += stepSize;
		break;
	case 'd':
		angle2 -= stepSize;
		break;
	case 'r':
		LightPos[0] += lightStepSize;
		break;
	case 'f':
		LightPos[0] -= lightStepSize;
		break;
	case 't':
		LightPos[1] += lightStepSize;
		break;
	case 'g':
		LightPos[1] -= lightStepSize;
		break;
	case 'y':
		LightPos[2] += lightStepSize;
		break;
	case 'h':
		LightPos[2] -= lightStepSize;
		break;
	case 'z':
		NormalsMode = EQUAL;
		break;
	case 'x':
		NormalsMode = EVERY_TRIANGLE;
		break;
	case 'c':
		NormalsMode = EVERY_VERTEX;
		break;
	}
}


//Nothing needed below this point
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 
//STOP READING //STOP READING //STOP READING 





void displayInternal(void);
void reshape(int w, int h);
bool loadMesh(const char * filename);
void init()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	//int MatSpec [4] = {1,1,1,1};
	//   glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
	//   glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);


	// Enable Depth test
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//Draw frontfacing polygons as filled
	glPolygonMode(GL_FRONT, GL_FILL);
	//draw backfacing polygons as outlined
	glPolygonMode(GL_BACK, GL_LINE);
	glShadeModel(GL_SMOOTH);
	loadMesh("David.obj");
}


//load mesh
//this code is NOT how you should load a mesh... it is a bit hacky...
void centerAndScaleToUnit(std::vector<float> & vertices)
{
	if (vertices.size() % 3 != 0)
	{
		cout << "ERROR while loading!!!\n";
		return;
	}

	float x = 0;
	float y = 0;
	float z = 0;
	for (unsigned int i = 0; i < vertices.size(); i += 3)
	{
		x += vertices[i];
		y += vertices[i + 1];
		z += vertices[i + 2];
	}


	x /= vertices.size() / 3;
	y /= vertices.size() / 3;
	z /= vertices.size() / 3;

	float maxD = 0;
	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		float dX = (vertices[i] - x);
		float dY = (vertices[i + 1] - y);
		float dZ = (vertices[i + 2] - z);

		float m = sqrt(dX*dX + dY*dY + dZ*dZ);
		if (m > maxD)
			maxD = m;
	}
	float center[] = { x,y,z };
	for (unsigned int i = 0; i < vertices.size(); i += 3)
	{
		vertices[i] = (vertices[i] - x) / maxD;
		vertices[i + 1] = (vertices[i + 1] - y) / maxD;
		vertices[i + 2] = (vertices[i + 2] - z) / maxD;
	}
}

bool loadMesh(const char * filename)
{
	const unsigned int LINE_LEN = 256;
	char s[LINE_LEN];
	FILE * in;
#ifdef WIN32
	errno_t error = fopen_s(&in, filename, "r");
	if (error != 0)
#else
	in = fopen(filename, "r");
	if (!(in))
#endif
		return false;

	//temp stuff
	float x, y, z;
	std::vector<int> vhandles;



	while (in && !feof(in) && fgets(s, LINE_LEN, in))
	{
		// vertex
		if (strncmp(s, "v ", 2) == 0)
		{
			if (sscanf(s, "v %f %f %f", &x, &y, &z))
				MeshVertices.push_back(x);
			MeshVertices.push_back(y);
			MeshVertices.push_back(z);
		}
		// face
		else if (strncmp(s, "f ", 2) == 0)
		{
			int component(0), nV(0);
			bool endOfVertex(false);
			char *p0, *p1(s + 2); //place behind the "f "

			vhandles.clear();

			while (*p1 == ' ') ++p1; // skip white-spaces

			while (p1)
			{
				p0 = p1;

				// overwrite next separator

				// skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
				while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' &&
					*p1 != ' ' && *p1 != '\0')
					++p1;

				// detect end of vertex
				if (*p1 != '/') endOfVertex = true;

				// replace separator by '\0'
				if (*p1 != '\0')
				{
					*p1 = '\0';
					p1++; // point to next token
				}

				// detect end of line and break
				if (*p1 == '\0' || *p1 == '\n')
					p1 = 0;


				// read next vertex component
				if (*p0 != '\0')
				{
					switch (component)
					{
					case 0: // vertex
						vhandles.push_back(atoi(p0) - 1);
						break;

					case 1: // texture coord
							//assert(!vhandles.empty());
							//assert((unsigned int)(atoi(p0)-1) < texcoords.size());
							//_bi.set_texcoord(vhandles.back(), texcoords[atoi(p0)-1]);
						break;

					case 2: // normal
							//assert(!vhandles.empty());
							//assert((unsigned int)(atoi(p0)-1) < normals.size());
							//_bi.set_normal(vhandles.back(), normals[atoi(p0)-1]);
						break;
					}
				}

				++component;

				if (endOfVertex)
				{
					component = 0;
					nV++;
					endOfVertex = false;
				}
			}


			if (vhandles.size()>3)
			{
				//model is not triangulated, so let us do this on the fly...
				//to have a more uniform mesh, we add randomization
				unsigned int k = (false) ? (rand() % vhandles.size()) : 0;
				for (unsigned int i = 0; i<vhandles.size() - 2; ++i)
				{
					MeshTriangles.push_back(vhandles[(k + 0) % vhandles.size()]);
					MeshTriangles.push_back(vhandles[(k + i + 1) % vhandles.size()]);
					MeshTriangles.push_back(vhandles[(k + i + 2) % vhandles.size()]);
				}
			}
			else if (vhandles.size() == 3)
			{
				MeshTriangles.push_back(vhandles[0]);
				MeshTriangles.push_back(vhandles[1]);
				MeshTriangles.push_back(vhandles[2]);
			}
			else
			{
				std::cout << "TriMesh::LOAD: Unexpected number of face vertices (<3). Ignoring face \n";
			}
		}
		memset(&s, 0, LINE_LEN);
	}
	fclose(in);
	centerAndScaleToUnit(MeshVertices);

	MeshVertexNormals.resize(MeshVertices.size(), 0);
	MeshVertexSumCount.resize(MeshVertices.size()/3, 0);
	// Compute triangle normals.
	for (unsigned int i = 0; i < MeshTriangles.size(); i += 3) {
		float p1[] = { MeshVertices[MeshTriangles[i] * 3], MeshVertices[MeshTriangles[i] * 3 + 1], MeshVertices[MeshTriangles[i] * 3 + 2] };
		float p2[] = { MeshVertices[MeshTriangles[i + 1] * 3], MeshVertices[MeshTriangles[i + 1] * 3 + 1], MeshVertices[MeshTriangles[i + 1] * 3 + 2] };
		float p3[] = { MeshVertices[MeshTriangles[i + 2] * 3], MeshVertices[MeshTriangles[i + 2] * 3 + 1], MeshVertices[MeshTriangles[i + 2] * 3 + 2] };
		float u[] = { p2[0] - p1[0], p2[1] - p1[1] , p2[2] - p1[2] };
		float v[] = { p3[0] - p1[0], p3[1] - p1[1] , p3[2] - p1[2] };
		float N[] = { u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0] };
		float length = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
		float normalizedN[] = { N[0] / length, N[1] / length, N[2] / length };
		MeshTriangleNormals.push_back(normalizedN[0]);
		MeshTriangleNormals.push_back(normalizedN[1]);
		MeshTriangleNormals.push_back(normalizedN[2]);

		for (int j = 0; j < 3; j++) {
			MeshVertexSumCount[MeshTriangles[i + j]]++;
			for (int dimension = 0; dimension < 3; dimension++) {
				MeshVertexNormals[MeshTriangles[i + j] * 3 + dimension] += normalizedN[dimension];
			}
		}
	}

	for (int i = 0; i < MeshVertexSumCount.size(); i++) {
		int count = MeshVertexSumCount[i];
		for (int dim = 0; dim < 3; dim++) {
			MeshVertexNormals[i * 3 + dim] = MeshVertexNormals[i * 3 + dim] / count;
		}
	}

	return true;
}




/**
* Programme principal
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	// couches du framebuffer utilisees par l'application
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// position et taille de la fenetre
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(W_fen, H_fen);
	glutCreateWindow(argv[0]);

	init();

	// Initialize viewpoint
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -4);
	tbInitTransform();
	tbHelp();



	// cablage des callback
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(displayInternal);
	glutMouseFunc(tbMouseFunc);    // traqueboule utilise la souris
	glutMotionFunc(tbMotionFunc);  // traqueboule utilise la souris
	glutIdleFunc(animate);

	// lancement de la boucle principale
	glutMainLoop();

	return 0;  // instruction jamais exécutée
}

/**
* Fonctions de gestion opengl à ne pas toucher
*/
// Actions d'affichage
// Ne pas changer
void displayInternal(void)
{
	// Effacer tout
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // la couleur et le z


	glLoadIdentity();  // repere camera

	tbVisuTransform(); // origine et orientation de la scene

	display();

	glutSwapBuffers();
	glutPostRedisplay();
}
// pour changement de taille ou desiconification
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
	gluPerspective(50, (float)w / h, 1, 10);
	glMatrixMode(GL_MODELVIEW);
}

