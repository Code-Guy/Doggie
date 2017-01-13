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
	glm::vec3 srcVert; // ������
	glm::vec3 dstVert; // Ŀ���
	int dst; // Ŀ�������

	glm::vec3 position; // λ��
	int layer; // ��������

	float ts; // t * speed
	bool finished; // �Ƿ��˶�����

	int speed; // һ�����ܴ������ٸ���
};

#endif // POINT_H
