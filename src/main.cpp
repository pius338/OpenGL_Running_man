#include "cube.h"
#include "sphere.h"
#include "initShader.h"
#include "texture.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <cmath>
#include <iostream>
#include <vector>

enum eShadeMode { NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE };

glm::mat4 projectMat;
glm::mat4 viewMat;
glm::mat4 modelMat = glm::mat4(1.0f);

int shadeMode = NO_LIGHT;
int isTexture = false;

float rotAngle = 0.0f;
float upDown = 0.0f;
float speed = 8.0f;
float max_angle = glm::radians(55.0f);

GLuint projectMatrixID;
GLuint viewMatrixID;
GLuint modelMatrixID;
GLuint shadeModeID;
GLuint textureModeID;

GLuint sphereVAO, cubeVAO;
GLuint programID;

Sphere sphere(50, 50);
Cube cube;

//----------------------------------------------------------------------------

// OpenGL initialization
void init() {

	cube.generateCube();

	GLuint buffer[2];
	glGenVertexArrays(1, &sphereVAO);
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(2, buffer);

	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);
	programID = program;

	// Sphere VAO
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

	int sphereVertSize = sizeof(sphere.verts[0]) * sphere.verts.size();
	int sphereNormalSize = sizeof(sphere.normals[0]) * sphere.normals.size();
	int spheretexSize = sizeof(sphere.texCoords[0]) * sphere.texCoords.size();
	glBufferData(GL_ARRAY_BUFFER, sphereVertSize + sphereNormalSize + spheretexSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sphereVertSize, sphere.verts.data());
	glBufferSubData(GL_ARRAY_BUFFER, sphereVertSize, sphereNormalSize, sphere.normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, sphereVertSize + sphereNormalSize, spheretexSize, sphere.texCoords.data());


	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sphereVertSize));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sphereVertSize + sphereNormalSize));

	// Cube VAO

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);

	int cubeVertSize = sizeof(cube.points[0]) * cube.points.size();
	int cubeNormalSize = sizeof(cube.normals[0]) * cube.normals.size();
	int cubetexSize = sizeof(cube.texCoords[0]) * cube.texCoords.size();
	glBufferData(GL_ARRAY_BUFFER, cubeVertSize + cubeNormalSize + cubetexSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, cubeVertSize, cube.points.data());
	glBufferSubData(GL_ARRAY_BUFFER, cubeVertSize, cubeNormalSize, cube.normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, cubeVertSize + cubeNormalSize, cubetexSize, cube.texCoords.data());

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cubeVertSize));

	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(cubeVertSize + cubeNormalSize));

	// Uniform Setup
	projectMatrixID = glGetUniformLocation(program, "mProject");
	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);

	viewMatrixID = glGetUniformLocation(program, "mView");
	viewMat = glm::lookAt(glm::vec3(3, 3, 13), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);

	modelMatrixID = glGetUniformLocation(program, "mModel");

	shadeModeID = glGetUniformLocation(program, "shadeMode");
	glUniform1i(shadeModeID, shadeMode);

	textureModeID = glGetUniformLocation(program, "isTexture");
	glUniform1i(textureModeID, isTexture);

	// Load the texture using any two methods
	GLuint earTexture = loadBMP_custom("ear.bmp");
	GLuint faceTexture = loadBMP_custom("face.bmp");
	GLuint bodyTexture = loadBMP_custom("body.bmp");
	GLuint bagTexture = loadBMP_custom("bag.bmp");
	GLuint foreArmTexture = loadBMP_custom("foreArm.bmp");
	GLuint armTexture = loadBMP_custom("arm.bmp");
	GLuint upperLegTexture = loadBMP_custom("upperLeg.bmp");
	GLuint lowerLegTexture = loadBMP_custom("lowerLeg.bmp");

	GLuint earTextureID = glGetUniformLocation(program, "earTexture");
	GLuint faceTextureID = glGetUniformLocation(program, "faceTexture");
	GLuint bodyTextureID = glGetUniformLocation(program, "bodyTexture");
	GLuint bagTextureID = glGetUniformLocation(program, "bagTexture");
	GLuint foreArmTextureID = glGetUniformLocation(program, "foreArmTexture");
	GLuint armTextureID = glGetUniformLocation(program, "armTexture");
	GLuint upperLegTextureID = glGetUniformLocation(program, "upperLegTexture");
	GLuint lowerLegTextureID = glGetUniformLocation(program, "lowerLegTexture");

	// Bind our texture in Texture Units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, faceTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bodyTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bagTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, foreArmTexture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, armTexture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, upperLegTexture);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, lowerLegTexture);

	glUniform1i(earTextureID, 0);
	glUniform1i(faceTextureID, 1);
	glUniform1i(bodyTextureID, 2);
	glUniform1i(bagTextureID, 3);
	glUniform1i(foreArmTextureID, 4);
	glUniform1i(armTextureID, 5);
	glUniform1i(upperLegTextureID, 6);
	glUniform1i(lowerLegTextureID, 7);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.12, 0.12, 0.12, 1.0);
}

//----------------------------------------------------------------------------

void drawMan(glm::mat4 manMat)
{
	glm::mat4 modelMat;
	glm::vec3 ArmPos[4], ArmSize[2], LegPos[4], LegSize[2];

	ArmPos[0] = glm::vec3(-1.5, 1.5, 0); //lArm
	ArmPos[1] = glm::vec3(-1.5, -0.7, 0); //lForeArm
	ArmPos[2] = glm::vec3(1.5, 1.5, 0); //rArm
	ArmPos[3] = glm::vec3(1.5, -0.7, 0); //rForeArm

	ArmSize[0] = glm::vec3(0.3, 1.3, 0.3); //Arm
	ArmSize[1] = glm::vec3(0.25, 1.3, 0.25); //ForeArm

	LegPos[0] = glm::vec3(-0.85, -1.4, 0); //lUpperLeg
	LegPos[1] = glm::vec3(-0.85, -4.3, 0.4); //lLowerLeg
	LegPos[2] = glm::vec3(0.85, -1.4, 0); //rUpperLeg
	LegPos[3] = glm::vec3(0.85, -4.3, 0.4); //rLowerLeg

	LegSize[0] = glm::vec3(0.4, 1.7, 0.4); //UpperLeg
	LegSize[1] = glm::vec3(0.3, 2, 0.3); //LowerLeg

	glBindVertexArray(cubeVAO);

	//body
	glUniform1i(glGetUniformLocation(programID, "objectType"), 2);
	modelMat = glm::translate(manMat, glm::vec3(0, 1.5, 0));
	modelMat = glm::scale(modelMat, glm::vec3(2.4, 2.8, 1.8));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, cube.NumVertices);

	//bag
	glUniform1i(glGetUniformLocation(programID, "objectType"), 3);
	modelMat = glm::translate(manMat, glm::vec3(0, 1.5, -1.6));
	modelMat = glm::scale(modelMat, glm::vec3(1.8, 2, 1));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, cube.NumVertices);

	//head
	glUniform1i(glGetUniformLocation(programID, "objectType"), 0);
	modelMat = glm::translate(manMat, glm::vec3(0, 3.7, 0));
	modelMat = glm::scale(modelMat, glm::vec3(2, 1.4, 1.5));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, cube.NumVertices);

	glBindVertexArray(sphereVAO);

	//face
	glUniform1i(glGetUniformLocation(programID, "objectType"), 1);
	modelMat = glm::translate(manMat, glm::vec3(0, 3.7, 0.75));
	modelMat = glm::scale(modelMat, glm::vec3(0.8, 0.6, 0.1));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());

	//Ear
	glUniform1i(glGetUniformLocation(programID, "objectType"), 0);
	modelMat = glm::translate(manMat, glm::vec3(-0.7, 4.4, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.4, 0.6));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());

	modelMat = glm::translate(manMat, glm::vec3(0.7, 4.4, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.4, 0.6));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());
	

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
			modelMat = glm::rotate(modelMat, -abs(armAngle) * 2, glm::vec3(1, 0, 0));
			modelMat = glm::translate(modelMat, glm::vec3(0, -0.4, 0));
		}
		modelMat = glm::translate(modelMat, ArmPos[i]);
		modelMat = glm::scale(modelMat, ArmSize[i % 2]);
		if (i % 2) glUniform1i(glGetUniformLocation(programID, "objectType"), 5);
		else glUniform1i(glGetUniformLocation(programID, "objectType"), 4);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());
	}
	//Leg
	for (int i = 0; i < 4; i++)
	{
		float legAngle = (i < 2) ? -rotAngle : rotAngle;
		legAngle = (legAngle * 1.5) - 0.3;
		float lowerLegAngle = legAngle;
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
		if (i % 2) glUniform1i(glGetUniformLocation(programID, "objectType"), 7);
		else glUniform1i(glGetUniformLocation(programID, "objectType"), 6);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());
	}
}

//----------------------------------------------------------------------------

void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID);

	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
	glUniform1i(shadeModeID, shadeMode);

	worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, upDown, 0));
	worldMat = glm::rotate(worldMat, glm::radians(8.0f), glm::vec3(1, 0, 0));

	drawMan(worldMat);
	
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	float t = abs(currTime - prevTime) / 1000.0f;

	rotAngle = max_angle * std::sin(t * speed);

	upDown = (max_angle / 3) * std::sin(t * speed * 2.0f);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l': case 'L':
		shadeMode = (++shadeMode % NUM_LIGHT_MODE);
		glUniform1i(shadeModeID, shadeMode);
		glutPostRedisplay();
		break;
	case 't': case 'T':
		isTexture = !isTexture;
		glUniform1i(textureModeID, isTexture);
		glutPostRedisplay();
		break;
	case '1':
		viewMat = glm::lookAt(glm::vec3(13, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '2':
		viewMat = glm::lookAt(glm::vec3(-1, 7, -12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '3':
		viewMat = glm::lookAt(glm::vec3(3, 3, 13), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case 033:
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

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Cube Man Run");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
