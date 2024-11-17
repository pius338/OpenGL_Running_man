#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;

class Cube {
public:
	Cube();
	~Cube();

	void generateCube();

	static const int NumVertices = 36;
	vector<glm::vec4> points;
	vector<glm::vec4> normals;
	vector<glm::vec2> texCoords;

private:
	void quad(int a, int b, int c, int d);

	static const glm::vec4 vertices[8];
};