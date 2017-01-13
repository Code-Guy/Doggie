#ifndef CLUSTER_H
#define CLUSTER_H

#include "point.h"
#include <vector>

class Wave
{
public:
	Wave(std::vector<glm::vec3> *verts, std::vector<std::vector<int>> *net, 
		int *pointSpeed, int src, int waveSpreadLayer);
	~Wave();

	void logic(float dt, std::vector<glm::vec3> &positions);
	bool isFinished();

private:
	void split(int src, int layer = 0);

	bool *traversed; // 是否已经遍历过

	std::vector<Point> points;
	int waveSpreadLayer; // 最大层数

	std::vector<glm::vec3> *verts; // 模型点坐标
	std::vector<std::vector<int>> *net; // 模型网格
	int *pointSpeed; // 点的运动速度
};

#endif // CLUSTER_H
