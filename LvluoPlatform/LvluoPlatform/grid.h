#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include <vector>

class Grid
{
public:
	Grid();
	~Grid();

	void logic(glm::ivec4 viewport);
	void render();

private:
	std::vector<glm::vec3> points;
	int lastSize;
	glm::ivec4 lastViewport;
};

#endif // GRID_H
