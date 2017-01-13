#include "grid.h"
#include "appconfig.h"
#include "resourcemanager.h"
#include "primitivemanager.h"

Grid::Grid()
{
	lastSize = -1;
	lastViewport = glm::ivec4(-1, -1, -1, -1);
}

Grid::~Grid()
{

}

void Grid::logic(glm::ivec4 viewport)
{
	int size = AppConfig::getInstance()->getGridSize();
	if (AppConfig::getInstance()->getGridOn() &&
		lastSize != size || lastViewport != viewport)
	{
		lastSize = size;
		lastViewport = viewport;
		points.clear();

		int panelWidth = viewport.z;
		int panelHeight = viewport.w;

		int wnum = panelWidth / size;
		int hnum = panelHeight / size;
		float wspan = (float)size / panelWidth * 2.0f;
		float hspan = (float)size / panelHeight * 2.0f;

		for (int i = 1; i <= wnum; i++)
		{
			points.push_back(glm::vec3(wspan * i - 1, -1, 0));
			points.push_back(glm::vec3(wspan * i - 1, 1, 0));
		}
		for (int i = 1; i <= hnum; i++)
		{
			points.push_back(glm::vec3(-1, 1 - hspan * i, 0));
			points.push_back(glm::vec3(1, 1 - hspan * i, 0));
		}
	}
}

void Grid::render()
{
	if (AppConfig::getInstance()->getGridOn())
	{
		ResourceManager::getInstance()->getGridShader()->enable();
		ResourceManager::getInstance()->getGridShader()->setColor(AppConfig::getInstance()->getGridColor());

		PrimitiveManager::getInstance()->renderPoints(points, GL_LINES);
	}
}