#include "wave.h"

Wave::Wave(std::vector<glm::vec3> *verts, std::vector<std::vector<int>> *net, 
	int *pointSpeed, int src, int waveSpreadLayer) : verts(verts), net(net), pointSpeed(pointSpeed), waveSpreadLayer(waveSpreadLayer)
{
	traversed = new bool[verts->size()];
	memset(traversed, 0, verts->size());

	split(src);
}

Wave::~Wave()
{
	delete[] traversed;
}

void Wave::logic(float dt, std::vector<glm::vec3> &positions)
{
	std::vector<std::pair<int, int>> splitPoints;
	for (auto pointIter = points.begin(); pointIter != points.end(); )
	{
		pointIter->logic(dt);
		if (pointIter->isFinished())
		{
			if (pointIter->getLayer() < waveSpreadLayer)
			{
				splitPoints.push_back(std::make_pair<int, int>(pointIter->getDst(), pointIter->getLayer() + 1));
			}
			pointIter = points.erase(pointIter);
		}
		else
		{
			pointIter++;
		}
	}

	for (std::pair<int, int> splitPoint : splitPoints)
	{
		split(splitPoint.first, splitPoint.second);
	}

	for (Point point : points)
	{
		positions.push_back(point.getPosition());
	}
}

bool Wave::isFinished()
{
	return points.empty();
}

void Wave::split(int src, int layer)
{
	for (int dst : net->at(src))
	{
		if (!traversed[dst])
		{
			traversed[dst] = true;
			points.push_back(Point(verts->at(src), verts->at(dst), dst, layer, *pointSpeed));
		}
	}
}