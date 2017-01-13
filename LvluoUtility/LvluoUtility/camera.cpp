#include "camera.h"

const float MoveZSpeed = 0.2f; // 深度移动速度
const float MoveXYSpeed = 0.008f; // XY方向移动速度

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
	float fovy, float zNear, float zFar) :
	position(position), target(target), up(up),
	fovy(fovy), zNear(zNear), zFar(zFar)
{
	v = glm::lookAt(position, target, up);

	lastFd = -1;
	lastSd = -1;
	displayMode = Normal;
}

Camera::~Camera()
{

}

void Camera::logic(float fd, float sd)
{
	if (fd != lastFd || sd != lastSd)
	{
		lastFd = fd;
		lastSd = sd;

		float re = 1.0f;
		float r = fd / re;
		float f = 1.0f / glm::tan(glm::radians(fovy / 2.0f));

		lv = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			sd * r / 2.0f, 0.0f, 0.0f, 1.0f);
		lp = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-(sd * f) / (fd * aspect * 2.0f), 0.0f, 0.0f, 1.0f);

		rv = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-sd * r / 2.0f, 0.0f, 0.0f, 1.0f);
		rp = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			(sd * f) / (fd * aspect * 2.0f), 0.0f, 0.0f, 1.0f);
	}
}

void Camera::setAspectRatio(float aspect)
{
	this->aspect = aspect;
	p = glm::perspective(fovy, aspect, zNear, zFar);
}

void Camera::setDisplayMode(DisplayMode displayMode)
{
	this->displayMode = displayMode;
}

glm::mat4 Camera::getViewMatrix() const
{
	switch (displayMode)
	{
	case Camera::Normal:
		return v;
	case Camera::LeftEye:
		return lv * v;
	case Camera::RightEye:
		return rv * v;
	default:
		return v;
	}
}

glm::mat4 Camera::getProjectionMatrix() const
{
	switch (displayMode)
	{
	case Camera::Normal:
		return p;
	case Camera::LeftEye:
		return lp * p;
	case Camera::RightEye:
		return rp * p;
	default:
		return p;
	}
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
	switch (displayMode)
	{
	case Camera::Normal:
		return p * v;
	case Camera::LeftEye:
		return lp * p * lv * v;
	case Camera::RightEye:
		return rp * p * rv * v;
	default:
		return p * v;
	}
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

void Camera::moveDZ(int dz)
{
	position.z -= dz * MoveZSpeed;
	v = glm::lookAt(position, target, up);
}

void Camera::moveDXY(int dx, int dy)
{
	position.x -= dx * MoveXYSpeed;
	position.y += dy * MoveXYSpeed;

	target.x -= dx * MoveXYSpeed;
	target.y += dy * MoveXYSpeed;

	v = glm::lookAt(position, target, up);
}