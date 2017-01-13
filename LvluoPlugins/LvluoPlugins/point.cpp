#include "point.h"

Point::Point(glm::vec3 srcVert, glm::vec3 dstVert, int dst, int layer, int speed) :
	srcVert(srcVert), dstVert(dstVert), dst(dst), layer(layer), speed(speed)
{
	position = srcVert;
	ts = 0.0f;
	finished = false;
}

void Point::logic(float dt)
{
	ts += dt * speed;
	position += (dstVert - srcVert) * (dt * speed);
	if (ts > 1.0f)
	{
		position = dstVert;
		finished = true;
	}
}

bool Point::isFinished()
{
	return finished;
}

glm::vec3 Point::getPosition()
{
	return position;
}

int Point::getDst()
{
	return dst;
}

int Point::getLayer()
{
	return layer;
}