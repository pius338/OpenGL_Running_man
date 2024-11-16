#include "cube.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <cmath>

typedef glm::vec4  point4;

const int NumVertices = 36;

point4 points[NumVertices];
std::vector<glm::vec4> cubeNormals;

const point4 Cube::vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

Cube::Cube() {
	points.reserve(NumVertices);
	normals.reserve(NumVertices);
}

Cube::~Cube() {
	points.clear();
	normals.clear();
}

//----------------------------------------------------------------------------

void Cube::quad(int a, int b, int c, int d) {
	glm::vec3 u = glm::vec3(vertices[b]) - glm::vec3(vertices[a]);
	glm::vec3 v = glm::vec3(vertices[c]) - glm::vec3(vertices[a]);
	glm::vec3 normal = glm::normalize(glm::cross(u, v));

	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[a]);
	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[b]);
	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[c]);

	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[a]);
	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[c]);
	normals.push_back(glm::vec4(normal, 0.0)); points.push_back(vertices[d]);
}

//----------------------------------------------------------------------------

void Cube::generateCube()
{
	points.clear();

	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

