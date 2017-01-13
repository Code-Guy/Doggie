#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "lvluoapi.h"

class LVLUO_API Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
		float fovy, float zNear, float zFar);
	~Camera();

	enum DisplayMode
	{
		Normal, LeftEye, RightEye
	};

	void logic(float fd, float sd);

	void setDisplayMode(DisplayMode displayMode);
	void setAspectRatio(float aspect);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewProjectionMatrix() const;

	glm::vec3 getPosition() const;

	void moveDZ(int dz);
	void moveDXY(int dx, int dy);

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	float fovy;
	float aspect;
	float zNear, zFar;
	
	glm::mat4 v, p;
	glm::mat4 lv, lp;
	glm::mat4 rv, rp;

	float lastFd;
	float lastSd;

	DisplayMode displayMode;
};

#endif // CAMERA_H
