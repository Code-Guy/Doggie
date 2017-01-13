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

	bool *traversed; // �Ƿ��Ѿ�������

	std::vector<Point> points;
	int waveSpreadLayer; // ������

	std::vector<glm::vec3> *verts; // ģ�͵�����
	std::vector<std::vector<int>> *net; // ģ������
	int *pointSpeed; // ����˶��ٶ�
};

#endif // CLUSTER_H
