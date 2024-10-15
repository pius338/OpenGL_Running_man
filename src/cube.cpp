//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <cmath>
#include <iostream>

glm::mat4 projectMat;
glm::mat4 viewMat;

GLuint pvmMatrixID;

float rotAngle = 0.0f;
float upDown = 0.0f;
float speed = 8.0f;
float max_angle = glm::radians(55.0f);
int isDrawingCar = false;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 1.0, 1.0, 1.0)  // white
};

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d];  Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	projectMat = glm::perspective(glm::radians(10.0f), 1.0f, 0.1f, 100.0f);
	viewMat = glm::lookAt(glm::vec3(10, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

//void drawCar(glm::mat4 carMat)
//{
//	glm::mat4 modelMat, pvmMat;
//	glm::vec3 wheelPos[4];
//
//	wheelPos[0] = glm::vec3(0.3, 0.24, -0.1); // rear right
//	wheelPos[1] = glm::vec3(0.3, -0.24, -0.1); // rear left
//	wheelPos[2] = glm::vec3(-0.3, 0.24, -0.1); // front right
//	wheelPos[3] = glm::vec3(-0.3, -0.24, -0.1); // front left
//
//	// car body
//	modelMat = glm::scale(carMat, glm::vec3(1, 0.6, 0.2));
//	pvmMat = projectMat * viewMat * modelMat;
//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//
//	// car top
//	modelMat = glm::translate(carMat, glm::vec3(0, 0, 0.2));  //P*V*C*T*S*v
//	modelMat = glm::scale(modelMat, glm::vec3(0.5, 0.6, 0.2));
//	pvmMat = projectMat * viewMat * modelMat;
//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//
//	// car wheel
//	for (int i = 0; i < 4; i++)
//	{
//		modelMat = glm::translate(carMat, wheelPos[i]);  //P*V*C*T*S*v
//		modelMat = glm::scale(modelMat, glm::vec3(0.2, 0.1, 0.2));
//		modelMat = glm::rotate(modelMat, -rotAngle*50.0f, glm::vec3(0, 1, 0));
//		pvmMat = projectMat * viewMat * modelMat;
//		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//	}
//}

//----------------------------------------------------------------------------

void drawMan(glm::mat4 manMat)
{
	glm::mat4 modelMat, pvmMat;
	glm::vec3 ArmPos[4], ArmSize[2], LegPos[4], LegSize[2];

	ArmPos[0] = glm::vec3(-1.575, 1.8, 0); //lArm
	ArmPos[1] = glm::vec3(-1.575, -0.3, 0); //lForeArm
	ArmPos[2] = glm::vec3(1.575, 1.8, 0); //rArm
	ArmPos[3] = glm::vec3(1.575, -0.3, 0); //rForeArm

	ArmSize[0] = glm::vec3(0.65, 1.9, 0.65); //Arm
	ArmSize[1] = glm::vec3(0.5, 1.9, 0.5); //ForeArm

	LegPos[0] = glm::vec3(-0.55, -1.35, 0); //lUpperLeg
	LegPos[1] = glm::vec3(-0.55, -4.15, 0); //lLowerLeg
	LegPos[2] = glm::vec3(0.55, -1.35, 0); //rUpperLeg
	LegPos[3] = glm::vec3(0.55, -4.15, 0); //rLowerLeg

	LegSize[0] = glm::vec3(0.8, 2.5, 0.8); //UpperLeg
	LegSize[1] = glm::vec3(0.6, 2.5, 0.6); //LowerLeg

	//body
	modelMat = glm::translate(manMat, glm::vec3(0, 1.5, 0));
	modelMat = glm::scale(modelMat, glm::vec3(2.1, 2.9, 1.2));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//head
	modelMat = glm::translate(manMat, glm::vec3(0, 3.8, 0));
	modelMat = glm::scale(modelMat, glm::vec3(1, 1.3, 1));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//Arm
	for (int i = 0; i < 4; i++) 
	{
		float armAngle = (i < 2) ? rotAngle : -rotAngle;
		modelMat = glm::translate(manMat, glm::vec3(0, 2.5, 0));
		modelMat = glm::rotate(modelMat, armAngle, glm::vec3(1, 0, 0));
		modelMat = glm::translate(modelMat, glm::vec3(0, -2.5, 0));
		if (i % 2)
		{
			modelMat = glm::translate(modelMat, glm::vec3(0, 0.4, 0));
			modelMat = glm::rotate(modelMat, -abs(armAngle) * 2 , glm::vec3(1, 0, 0));
			modelMat = glm::translate(modelMat, glm::vec3(0, -0.4, 0));
		}
		modelMat = glm::translate(modelMat, ArmPos[i]);
		modelMat = glm::scale(modelMat, ArmSize[i%2]);
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	for (int i = 0; i < 4; i++)
	{

		float legAngle = (i < 2) ? -rotAngle * 1.5 : rotAngle * 1.5;
		float lowerLegAngle = legAngle * 0.8;
		modelMat = glm::translate(manMat, glm::vec3(0, -0.4, 0));
		modelMat = glm::rotate(modelMat, legAngle, glm::vec3(1, 0, 0));
		modelMat = glm::translate(modelMat, glm::vec3(0, 0.4, 0));
		if (i % 2)
		{
			modelMat = glm::translate(modelMat, glm::vec3(0, -2.5, 0));
			modelMat = glm::rotate(modelMat, abs(lowerLegAngle), glm::vec3(1, 0, 0));
			modelMat = glm::translate(modelMat, glm::vec3(0, 2.5, 0));
		}
		modelMat = glm::translate(modelMat, LegPos[i]);
		modelMat = glm::scale(modelMat, LegSize[i % 2]);
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

//----------------------------------------------------------------------------

void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//worldMat = glm::rotate(glm::mat4(1.0f), rotAngle, glm::vec3(1.0f, 1.0f, 0.0f));
	worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0,upDown,0));
	worldMat = glm::rotate(worldMat, glm::radians(8.0f), glm::vec3(1, 0, 0));

	if (isDrawingCar)
	{
		drawMan(worldMat);
	}
	else
	{
		pvmMat = projectMat * viewMat * worldMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	float t = abs(currTime - prevTime)/1000.0f;

	rotAngle = max_angle * std::sin(t * speed);
	
	upDown = (max_angle / 3) * std::sin(t * speed * 2.0f);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c': case 'C':
		isDrawingCar = !isDrawingCar;
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Car");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
