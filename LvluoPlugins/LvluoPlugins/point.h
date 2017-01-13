#ifndef POINT_H
#define POINT_H

#include <glm/glm.hpp>

class Point
{
public:
	Point(glm::vec3 srcVert, glm::vec3 dstVert, int dst, int layer, int speed);

	void logic(float dt);
	bool isFinished();

	glm::vec3 getPosition();
	int getDst();
	int getLayer();

private:
	glm::vec3 srcVert; // 出发点
	glm::vec3 dstVert; // 目标点
	int dst; // 目标点索引

	glm::vec3 position; // 位置
	int layer; // 传播层数

	float ts; // t * speed
	bool finished; // 是否运动结束

	int speed; // 一秒钟能传播多少个点
};

#endif // POINT_H
